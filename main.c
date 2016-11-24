// TeensyCNC
// Copyright 2016 Matt Williams
//
// g-code interpreter code is moostly based on RepRap and Marginally Clever's "How to make a Arduino CNC", though I've made a lot of changes.
// USB CDC virtual serial is from Freescale's Processor Expert (I'm not crazy enough to write that from scratch!)
// Everything else is taken from some of my other projects or from scratch.
//
// This all can be improved drastically, but it's a well working CNC example. G-code interpreter needs syntax checking, badly.

#include "mk20d7.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pwm.h"
#include "motor.h"
#include "cdc.h"

extern volatile uint32_t SysTick;

extern volatile int32_t Target[2];

#ifndef min
#define min(a, b) ((a)<(b)?(a):(b))
#endif

#ifndef max
#define max(a, b) ((a)>(b)?(a):(b))
#endif

// Cricut Mini steps per inch, this is triggering on both quadrature encoder edges, so it's actually 4X
#define X_STEPS_PER_INCH (2868.6132f)
#define Y_STEPS_PER_INCH (6876.0096f)

// Arc section line length, the smaller the number, the finer the facet on arcs
#define CURVE_SECTION_INCHES 0.005f

// Current, Target, and Delta axis units (working units, inches, MM, etc)
float cu[2]={ 0.0f, 0.0f }; // Current
float tu[2]={ 0.0f, 0.0f }; // Target
float du[2]={ 0.0f, 0.0f }; // Delta

// Current, Target, and Delta axis steps (machine units, steps)
int32_t cs[2]={ 0, 0 }; // Current
int32_t ts[2]={ 0, 0 }; // Target
int32_t ds[2]={ 0, 0 }; // Delta

// Axis direction flags
int8_t dir[2]={ 1, 1 };

// Feedrate, default is 90in/min
float feedrate=90.0f;

// Motion mode, 0 = incremental; 1 = absolute
uint8_t abs_mode=0;

// Incoming serial buffer stuff
#define COMMAND_SIZE 128
char buffer[COMMAND_SIZE];
uint8_t serial_count=0, comment=0;
uint32_t Incoming=0;

// Simple delay using ARM systick, set up for microseconds (see startup.c)
void DelayUS(uint32_t us)
{
	uint32_t _time=SysTick;

	while((SysTick-_time)<us);
}

// Calculates axis deltas, used any time target units change
void calculate_deltas(void)
{
	du[0]=fabs(tu[0]-cu[0]);
	du[1]=fabs(tu[1]-cu[1]);

	cs[0]=(cu[0]*X_STEPS_PER_INCH);
	cs[1]=(cu[1]*Y_STEPS_PER_INCH);

	ts[0]=(tu[0]*X_STEPS_PER_INCH);
	ts[1]=(tu[1]*Y_STEPS_PER_INCH);

	ds[0]=abs(ts[0]-cs[0]);
	ds[1]=abs(ts[1]-cs[1]);

	dir[0]=(ts[0]-cs[0])>0?1:-1;
	dir[1]=(ts[1]-cs[1])>0?-1:1;
}

// Calculate the length of the line to move and how long it will take to get there
// Taken from RepRap?
uint32_t calculate_feedrate_delay(float feedrate)
{
	float distance=sqrt(du[0]*du[0]+du[1]*du[1]);
	int32_t total=0;

	if(ds[0]>ds[1])
		total=ds[0];
	else
		total=ds[1];

	return ((distance*60000000.0f)/feedrate)/total;	
}

// DDA line move code, optimized from "How to make a Arduino CNC", though it's pretty generic IMO

// Number of steps to make the move
int32_t dda_steps, dda_over;
// Which axis is dominant
uint8_t dda_daxis=0;

void set_target(float x, float y)
{
	// Set the target units
	tu[0]=x;
	tu[1]=y;

	// Recalculate deltas
	calculate_deltas();

	// Don't bother setting up the DDA if we're already there
	if((ts[0]==cs[0])&&(ts[1]==cs[1]))
		return;
	else
	{
		// Set up total steps from dominant axis and set a flag for which one
		if(ds[0]>ds[1]) // X over Y
		{
			dda_daxis=0;
			dda_steps=ds[0];
			dda_over=ds[0]/2;
		}
		else // Y over X
		{
			dda_daxis=1;
			dda_steps=ds[1];
			dda_over=ds[1]/2;
		}
	}
}

// Sets the position (home)
void set_position(float x, float y)
{
	cu[0]=x;
	cu[1]=y;

	calculate_deltas();
}

// Run the actual move, stripped bare, hopefully makes it faster
void dda_move(uint32_t delay)
{
	// Loop until we're out of steps
	while(dda_steps--)
	{
		Target[dda_daxis]+=dir[dda_daxis];
		dda_over+=ds[!dda_daxis];

		if(dda_over>=ds[dda_daxis])
		{
			dda_over-=ds[dda_daxis];
			Target[!dda_daxis]+=dir[!dda_daxis];
		}

		DelayUS(delay);
	}

	// Arrived at target
	cu[0]=tu[0];
	cu[1]=tu[1];

	// Recalculate deltas
	calculate_deltas();
}

// Pull the number that follows the letter 'code'
float search_string(char code)
{
	char *ptr=buffer;

	while(ptr&&*ptr&&ptr<buffer+serial_count)
	{
		if(*ptr==code)
			return atof(ptr+1);

		ptr=strchr(ptr, ' ')+1;
	}

	return 0.0f;
}

// Checks if the 'key' code is in the buffer
uint8_t has_command(char key)
{
	int i;

	for(i=0;i<serial_count;i++)
	{
		if(buffer[i]==key)
			return 1;
	}
	
	return 0;
}

// Process the current command
void process_string(void)
{
	int code; // Current command code
	float t[2]={ 0.0f, 0.0f }; // Temp coord
	uint32_t feedrate_micros; // Feedrate in microseconds

	float c[2]={ 0.0f, 0.0f }; // Arc center coord
	int steps, s; // Temps for arc calculation
	float start_angle, end_angle, angle, radius, a[2], b[2];

	if(has_command('G')||has_command('X')||has_command('Y')||has_command('F'))
	{
		code=(int)search_string('G');

		switch(code)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 30:
				// Prefetch X, Y, and Feedrate

				if(abs_mode) // Get specified X/Y coords from command, absoulte mode
				{
					if(has_command('X'))
						t[0]=search_string('X');
					else
						t[0]=cu[0];

					if(has_command('Y'))
						t[1]=search_string('Y');
					else
						t[1]=cu[1];
				}
				else // Incremental mode
				{
					t[0]=search_string('X')+cu[0];
					t[1]=search_string('Y')+cu[1];
				}

				if(has_command('F'))
					feedrate=search_string('F'); // Search for feedrate OP
				break;
		}

		switch(code)
		{
			// Rapid or linear feed
			case 0:
			case 1:
				// Set target position
				set_target(t[0], t[1]);

				if(code==0) // Special case for rapid move
					feedrate_micros=calculate_feedrate_delay(250);
				else
					feedrate_micros=calculate_feedrate_delay(feedrate);

				// Go there
				dda_move(feedrate_micros);
				break;

			// CW/CCW arc
			case 2:
			case 3:
				// Get arc X/Y center (I/J)
				c[0]=search_string('I')+cu[0];
				c[1]=search_string('J')+cu[1];

				// Calculate the cross product
				a[0]=(cu[0]-c[0]);
				a[1]=(cu[1]-c[1]);
				b[0]=(t[0]-c[0]);
				b[1]=(t[1]-c[1]);

				// CW or CCW
				if(code==2)
				{
					// Find the starting and ending angle between the two points
					start_angle=atan2(b[1], b[0]);
					end_angle=atan2(a[1], a[0]);
				}
				else
				{
					start_angle=atan2(a[1], a[0]);
					end_angle=atan2(b[1], b[0]);
				}

				// Ensure we're still in range for radians
				if(end_angle<=start_angle)
					end_angle+=2.0f*3.1415926f;

				// Calculate the total angle and radius of the arc
				angle=end_angle-start_angle;
				radius=sqrt(a[0]*a[0]+a[1]*a[1]);

				// Calculate the number of steps, CURVE_SECTION_INCHES = lenth of line segment that forms the circle
				steps=(int)ceil(max(angle*2.4f, (radius*angle)/CURVE_SECTION_INCHES));

				// Run it out
				for(s=1;s<=steps;s++)
				{
					float sf=(float)((code==3)?s:steps-s)/steps; // Step fraction, invert 's' for CCW arcs
					float theta=start_angle+angle*sf;

					set_target(c[0]+cos(theta)*radius, c[1]+sin(theta)*radius);
					feedrate_micros=calculate_feedrate_delay(feedrate);
					dda_move(feedrate_micros);
				}
				break;

			// Dwell in seconds
			case 4:
				DelayMS((int)(search_string('P')*1000.0f));
				break;

			// Set inches units (doesn't do anything)
			case 20:
				break;

			// Set MM units (not supported ATM)
			case 21:
				break;

			// Go home
			case 28:
				set_target(0.0f, 0.0f);
				dda_move(100);
				break;

			// Go home in two steps
			case 30:
				set_target(t[0], t[1]);
				dda_move(100);
				set_target(0.0f, 0.0f);
				dda_move(100);
				break;

			// Set absolute mode
			case 90:
				abs_mode=1;
				break;

			// Set incremental mode
			case 91:
				abs_mode=0;
				break;

			// Set current position as home
			case 92:
				set_position(0.0f, 0.0f);
				break;

			// Error
			default:
				CDC_SendString((CDC_TComData *)"huh? G\r\n");
				break;
		}

		// Done
		CDC_SendString((CDC_TComData *)"ok\r\n");
		return;
	}

	if(has_command('M'))
	{
		code=search_string('M');

		switch(code)
		{
			// Program end
			case 2:
				GPIOC_PCOR|=0x0020;
				break;

			// Head down
			case 7:
				GPIOC_PSOR|=0x0020;
				DelayMS(300);  // Delay, limits pen/knife slap/skipping
				break;

			// Head up
			case 8:
				GPIOC_PCOR|=0x0020;
				DelayMS(300); // Delay, limits pen/knife slap/skipping
				break;

			// Error
			default:
				CDC_SendString((CDC_TComData *)"huh? M\r\n");
				break;
		}		

		// Done
		CDC_SendString((CDC_TComData *)"ok\r\n");

		return;
	}

	// Error, didn't find anything
	CDC_SendString((CDC_TComData *)"huh?\r\n");
}

int main(void)
{
	// Onboard LED output pin (head up/down solenoid)
	PORTC_PCR5=PORT_PCR_MUX(1);
	GPIOC_PDDR|=0x0020;
	GPIOC_PCOR|=0x0020;

	// Initialize X/Y motor PWM channels, set 0 duty (FFFFh = 0%, 0 = 100%)
	PWM_Init();
	PWM_SetRatio(0x00, 0xFFFF);
	PWM_SetRatio(0x01, 0xFFFF);
	PWM_SetRatio(0x05, 0xFFFF);
	PWM_SetRatio(0x06, 0xFFFF);

	// Initialize motor PID control and encoder interrupts
	Motor_Init();

	// Initialize USB CDC virtual serial device
	CDC_Init();

	while(1)
	{
		Incoming=CDC_GetCharsInRxBuf();

		if(Incoming>0)
		{
			while(Incoming--)
			{
				CDC_TComData tmp;

				CDC_RecvChar(&tmp);

				// Handle comments
				if(tmp=='(')
					comment=1;

				if(!comment)
					buffer[serial_count++]=(char)tmp;

				if(tmp==')')
					comment=0;
			}
		}

		// CR/LF ends lines and starts command processing
		if((buffer[serial_count-1]=='\n')||(buffer[serial_count-1]=='\r'))
		{
			uint8_t i;

			buffer[serial_count]=0;
			process_string();

			// After processing, clear the buffer
			for(i=0;i<COMMAND_SIZE;i++)
				buffer[i]=0;

			serial_count=0;
		}
	}
}
