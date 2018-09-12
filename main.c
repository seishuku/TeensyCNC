// TeensyCNC
// Copyright 2016 Matt Williams
//
// g-code interpreter code is moostly based on RepRap and Marginally Clever's "How to make a Arduino CNC", though I've made a lot of changes.
// USB CDC virtual serial is from Freescale's Processor Expert (I'm not crazy enough to write that from scratch!)
// Everything else is taken from some of my other projects or from scratch.
//
// This all can be improved drastically, but it's a well working CNC example. G-code interpreter needs syntax checking, badly.

// Modifications made by Alun Jones (macros, pen up/down on Z, bootloader entry, job tracing, help, etc!)

#include "MK20D7.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "pwm.h"
#include "motor.h"
#include "cdc.h"

struct {
	char *macro;
	char *replacement;
	char *help;
} macros[] = {
	{ "load",       "M39",     "Load mat in two stages (slow then fast)" },
	{ "unload",     "M40",     "Eject mat" },
	{ "trace",      "M111 S1", "Trace job without cutting" },
	{ "showbbox",   "M111 S3", "Trace bounding box of job" },
	{ "printbbox",  "M111 S2", "Don't move head at all, but print bounding box after job" },
	{ "cut",        "M111",    "Normal cutting mode" },
	{ "bootloader", "M112",    "Enter bootloader" },
	{ "help",       "M115",    "Show help" },
	{ NULL, NULL, NULL }
};

// External system tick counter (driven by ARM systick interrupt)
extern volatile uint32_t SysTick;

// Axis' target steps (absolute) and encoder counts
extern volatile int32_t Target[2];
extern volatile int32_t EncoderPos[2];

// Simple min/max macros
#ifndef min
#define min(a, b) ((a)<(b)?(a):(b))
#endif

#ifndef max
#define max(a, b) ((a)>(b)?(a):(b))
#endif

// Simple USB CDC text printing
#define INFO(x)		cdc_print("* " x "\r\n")
#define RESULT(x)	cdc_print(x "\r\n")

// Cricut Mini steps per inch, this is triggering on both quadrature encoder edges, so it's actually 4X
#define X_STEPS_PER_INCH (2868.6132f)
#define Y_STEPS_PER_INCH (6876.0096f)

// Max steps to the cut area limit on a 8.5 inch wide cutting mat
#define X_MAT_STEPS 24150

// Arc section line length, the smaller the number, the finer the facet on arcs
#define CURVE_SECTION_INCHES 0.005f

// metric to imperial conversion constant
#define MM_TO_INCHES (1.0f/25.4f)

// Unit scaler, 1.0 is no scale (inches), 1/25.4 is metric working units
float scale_to_inches=1.0f;   // Default to inches at start.

// Current, Target, and Delta axis units (working units, inches, MM, etc)
float cu[3]={ 0.0f, 0.0f, 0.0f }; // Current
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
uint8_t abs_mode=1;

// Level 1 => Don't drop cutting head.
// Level 2 => Don't do actual moves.
// Level 3 => Parse up to M2, then sweep bounding box with head up.
// Anything else => Turn debugging off.
enum _DL
{
	DEBUG_NONE=0,
	DEBUG_NOCUT=1,
	DEBUG_NOMOVE=2,
	DEBUG_BBOX_ON_M2=3,
	DEBUG_INVALID=4
} DebugLevel=DEBUG_NONE;

// Incoming serial buffer stuff
#define COMMAND_SIZE 128
char buffer[COMMAND_SIZE];
uint8_t serial_count=0, comment=0;
uint32_t Incoming=0;
uint8_t button_pressed=0;

enum
{
	NO_CANCEL=0,
	BUTTON_CANCEL=1,
	SOFTSTOP_CANCEL=2
} cancelling=NO_CANCEL;

enum
{
	LOAD_UNLOADED=0,
	LOAD_GRABBED=1,
	LOAD_LOADED=2,
} LoadState=LOAD_UNLOADED;

void LoadYAxis(void);
void EndJob(void);
void PollButton(void);

// Some helpful printing functions.
void cdc_print(char *s)
{
	if (CDC1_ApplicationStarted())
	{
		CDC_SendString((CDC_TComData *)s);
		(void)CDC_GetCharsInRxBuf();
	}
}

int8_t bboxstart=0, head_is_down=0;
float bb[4]={ 0, 0, 0, 0};

// Simple delay using ARM systick, set up for microseconds (see startup.c)
void DelayUS(uint32_t us)
{
	uint32_t _time=SysTick;

	while((SysTick-_time)<us);
}

// Same delay, only scaled 1000x for milliseconds
void DelayMS(uint32_t ms)
{
	uint32_t _time=SysTick;

	while((SysTick-_time)<(ms*1000))
		PollButton();
}

// Resets CNC state to default
void SetJobDefaults(void)
{
	feedrate=90.0f;
	abs_mode=1;
	scale_to_inches=1.0f;
	DebugLevel=DEBUG_NONE;
}

// Resets Teensy into boot loader mode (for uploading new code)
void EnterBootLoader(void)
{
	MotorDisable();
	INFO("Entering bootloader");

	DelayMS(500);

	__asm__ volatile("bkpt");

	// NOT REACHED!
	while (1);
}

void HeadUp(void)
{
	// In all debug modes, keep the cutter raised.
	head_is_down=0;

	if (DebugLevel==DEBUG_NONE)
	{
		GPIOC_PCOR|=0x0020;
		DelayMS(300); // Delay, limits pen/knife slap/skipping
	}
}

void HeadDown(void)
{
	// In all debug modes, keep the cutter raised.
	head_is_down=1;

	if (DebugLevel==DEBUG_NONE)
	{
		GPIOC_PSOR|=0x0020;
		DelayMS(300);  // Delay, limits pen/knife slap/skipping
	}
}

void PollButton(void)
{
	static uint32_t last_transition=0;
	static uint8_t oldstate=0;
	static uint8_t flagged=0;
	static uint8_t bootflagged=0;

	uint8_t state=!(GPIOD_PDIR&0x0002);

	if (state != oldstate)
	{
		last_transition=SysTick;
		flagged=0;
	}

	// If the button is pressed and we've not yet flagged it and 
	// the state has been stable for 5ms then flag it.
	if((state==1)&&!flagged&&(SysTick-last_transition>5000))
	{
		button_pressed=1;
		flagged=1;
	}

	if((state==1)&&!bootflagged&&(SysTick-last_transition>1500000))
	{
		// If the button is held, enter the bootloader. This
		// acts as an emergency stop on the motors and an emergency
		// method of getting to the bootloader. Never returns...
		bootflagged=1;
		EnterBootLoader();
	}

	oldstate=state;
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
	if (DebugLevel==DEBUG_NOMOVE||DebugLevel==DEBUG_BBOX_ON_M2)
	{
		// In these debug modes, we don't drive the motors.
		if (head_is_down)
		{
			// Keep track of the bounding box.
			if (bboxstart||(tu[0]<bb[0]))
				bb[0]=tu[0];

			if (bboxstart||(tu[0]>bb[2]))
				bb[2]=tu[0];

			if (bboxstart||(tu[1]<bb[1]))
				bb[1]=tu[1];

			if (bboxstart||(tu[1]>bb[3]))
				bb[3]=tu[1];

			bboxstart=0;
		}
		dda_steps=0;
	}
	else
	{
		// Loop until we're out of steps, or it's interrupted by button press
		while(!button_pressed&&dda_steps--)
		{
			Target[dda_daxis]+=dir[dda_daxis];
			dda_over+=ds[!dda_daxis];

			if(dda_over>=ds[dda_daxis])
			{
				dda_over-=ds[dda_daxis];
				Target[!dda_daxis]+=dir[!dda_daxis];
			}

			// Enforce soft limits since there's no safe hard stop.
			if ((Target[0]<0)||(Target[0]>X_MAT_STEPS))
			{
				cancelling=SOFTSTOP_CANCEL;
				break;
			}

			DelayUS(delay);
			PollButton();
		}
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
	float t[3]={ 0.0f, 0.0f, 0.0f }; // Temp coord

	float c[2]={ 0.0f, 0.0f }; // Arc center coord
	int steps, s; // Temps for arc calculation
	float start_angle, end_angle, angle, radius, a[2], b[2];

	// Blank line?
	if (buffer[0] == 0)
	{
		RESULT("ok");
		return;
	}

	// Macro replacement.
	for (int i=0; macros[i].macro != NULL; i++)
	{
		if (strcmp(buffer, macros[i].macro) == 0)
		{
			strcpy(buffer, macros[i].replacement);
			serial_count = strlen(buffer);
			break;
		}
	}

	if(has_command('G')||has_command('X')||has_command('Y')||has_command('Z')||has_command('F'))
	{
		code=(int)search_string('G');

		switch(code)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 30:
				// Prefetch X, Y, Z and Feedrate

				if(abs_mode) // Get specified X/Y/Z coords from command, absolute mode
				{
					if(has_command('X'))
						t[0]=search_string('X') * scale_to_inches;
					else
						t[0]=cu[0];

					if(has_command('Y'))
						t[1]=search_string('Y') * scale_to_inches;
					else
						t[1]=cu[1];

					if(has_command('Z'))
						t[2]=search_string('Z') * scale_to_inches;
					else
						t[2]=cu[2];
				}
				else // Incremental mode
				{
					t[0]=search_string('X') * scale_to_inches + cu[0];
					t[1]=search_string('Y') * scale_to_inches + cu[1];
					t[2]=search_string('Z') * scale_to_inches + cu[2];
				}

				// Have we moved down or up? Zero is the threshold: 
				// below zero is "cutter down".
				if ((t[2] >= 0) && (cu[2] < 0))
					HeadUp();
				else if ((t[2] < 0) && (cu[2] >= 0))
					HeadDown();

				cu[2] = t[2];

				if(has_command('F'))
					feedrate=search_string('F') * scale_to_inches; // Search for feedrate OP
				break;
		}

		switch(code)
		{
			// Rapid or linear feed
			case 0:
			case 1:
				// Set target position
				set_target(t[0], t[1]);

				// Go there, if code = 0 then set rapid feedrate
				dda_move(calculate_feedrate_delay(code?feedrate:250));
				break;

			// CW/CCW arc
			case 2:
			case 3:
				// Get arc X/Y center (I/J)
				c[0]=search_string('I') * scale_to_inches + cu[0];
				c[1]=search_string('J') * scale_to_inches + cu[1];

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
					dda_move(calculate_feedrate_delay(feedrate));
				}
				break;

			// Dwell in seconds
			case 4:
				if (DebugLevel == DEBUG_NONE)
					DelayMS((int)(search_string('P')*1000.0f));
				break;

			// Set inches units
			case 20:
				scale_to_inches = 1;
				break;

			// Set MM units
			case 21:
				scale_to_inches = MM_TO_INCHES;
				break;

			// Go home
			case 28:
				set_target(0.0f, 0.0f);
				dda_move(calculate_feedrate_delay(100.0f));
				break;

			// Go home in two steps
			case 30:
				set_target(t[0], t[1]);
				dda_move(calculate_feedrate_delay(100.0f));
				set_target(0.0f, 0.0f);
				dda_move(calculate_feedrate_delay(100.0f));
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
				RESULT("huh? G");
				break;
		}

		// Done
		switch (cancelling)
		{
			case NO_CANCEL:
				RESULT("ok");
				break;

			case SOFTSTOP_CANCEL:
				INFO("Head went out of bounds!");
				RESULT("cancelled");
				EndJob();
				break;

			case BUTTON_CANCEL:
				RESULT("cancelled");
				break;
		}

		return;
	}

	if(has_command('M'))
	{
		code=search_string('M');

		switch(code)
		{
			// Program end
			case 2:		// End of program
			case 30:	// End of program with reset
				EndJob();
				break;

			// Head down
			case 3:	// Spindle on (CCW)
			case 4:	// Spindle on (CW)
			case 7:	// Mist coolant on (seems to be common for plasma/oxyfuel "torch on")
				HeadDown();
				break;

			// Head up
			case 5:	// Spindle stop
			case 8:	// Flood coolant on (seems to be common for plasma/oxyfuel "torch off")
				HeadUp();
				break;

			// Load the paper
			case 39:
				LoadState=LOAD_UNLOADED;
				LoadYAxis();
				DelayMS(2000);
				LoadYAxis();
				break;

			// Eject the paper.
			case 40:
				LoadState=LOAD_LOADED;
				LoadYAxis();
				break;

			// Debug. 
			case 111:
				if(has_command('S'))
				{
					DebugLevel=search_string('S');

					if (DebugLevel>=DEBUG_INVALID)
						DebugLevel=DEBUG_NONE;
				}
				else
					DebugLevel=DEBUG_NONE;

				switch(DebugLevel)
				{
					case DEBUG_NONE:
						INFO("Job mode: CUT");
						break;

					case DEBUG_NOCUT:
						INFO("Job mode: TRACE (no cut)");
						break;

					case DEBUG_NOMOVE:
						INFO("Job mode: PARSE (no moves, print bounds after M2 command)");
						break;

					case DEBUG_BBOX_ON_M2:
						INFO("Job mode: BBOX (show bounds after M2 command)");
						break;

					default:
						break;
				}

				if (DebugLevel != DEBUG_NONE)
				{
					HeadUp();
					bboxstart=1;
				}

				if ((DebugLevel == DEBUG_NOMOVE) || (DebugLevel == DEBUG_BBOX_ON_M2))
				{
					enum _DL old=DebugLevel;
					DebugLevel=DEBUG_NONE;
					set_target(0.0f, 0.0f);
					dda_move(calculate_feedrate_delay(100.0f));
					DebugLevel=old;
				}
				break;

			// Emergency stop / Enter bootloader
			case 112:
				EnterBootLoader();
				break;

			// Help
			case 115:
				INFO("-- TeensyCNC " __DATE__ " " __TIME__ " --");
				INFO("Available macros:");
				INFO("");
				for (int i=0; macros[i].macro != NULL; i++)
				{
					char buffer[90];
					snprintf(buffer, sizeof(buffer), "*   %-10s - %s\r\n",
					macros[i].macro, macros[i].help);
					cdc_print(buffer);
				}
				INFO("");
				INFO("N.B. macros are lower case to avoid clashing with gcode");
				break;

			// Error
			default:
				RESULT("huh? M");
				break;
		}		

		// Done
		RESULT("ok");

		return;
	}

	// Error, didn't find anything
	RESULT("huh?");
}

// Simple hard-stop axis homing function.
// It's a bit of a hack, but works.
void HomeXAxis(void)
{
	int32_t prevcount=0, prevtime=0;

	// Make sure the head is up.
	HeadUp();

	// Disable motor drive PID loop
	MotorDisable();

	// Store current X encoder position
	prevcount=EncoderPos[0];
	// Store current tick
	prevtime=SysTick;

	// Drive the X motor home with enough torque to move it at a good pace, but not so much that it can't be stopped by the hard-stop.
	MotorCtrlX(-40000);

	// Let it move for a few ticks to generate some delta
	DelayMS(10);

	while(1)
	{
		// Velocity of motion over 1mS (1000uS)
		if((SysTick-prevtime)>1000)
		{
			// Calculate the delta position from the last mS
			int32_t dC=abs(EncoderPos[0]-prevcount);

			// If the velocity drops below 1 step/mS, we've hit the hard-stop and drop out of the loop
			if(dC<1)
				break;

			// Otherwise, update the previous position/time and continue on
			prevcount=EncoderPos[0];
			prevtime=SysTick;
		}
	}

	// Stop the motor and let it settle
	MotorCtrlX(0);
	DelayMS(100);

	// Zero out encoder and step positions
	EncoderPos[0]=0;
	EncoderPos[1]=0;
	Target[0]=0;
	Target[1]=0;

	// Zero out the CNC position
	set_position(0.0f, 0.0f);

	// Let it settle again and reenable the PID loop
	DelayMS(100);
	MotorEnable();

	// We're home!
}

void LoadYAxis(void)
{
	HomeXAxis();

	switch (LoadState)
	{
		// Just load it in a little bit to allow alignment.
		case LOAD_UNLOADED:
			INFO("Short loading");
			set_position(0.0f, 0.0f);
			set_target(0.0f, -1.0f);
			dda_move(calculate_feedrate_delay(50.0f));
			set_position(0.0f, 0.0f);
			LoadState=LOAD_GRABBED;
			break;

		// Align it with the bottom left of the paper.
		case LOAD_GRABBED:
			INFO("Full loading");
			set_position(0.0f, 0.0f);
			set_target(0.0f, -12.875f);
			dda_move(calculate_feedrate_delay(250.0f));
			set_position(0.0f, 0.0f);
			LoadState=LOAD_LOADED;
			break;

		// Load it out, run out enough to eject it no matter where.
		case LOAD_LOADED:
			INFO("Unloading");
			set_position(0.0f, 0.0f);
			set_target(0.0f, 14.0f);
			dda_move(calculate_feedrate_delay(250.0f));
			set_position(0.0f, 0.0f);
			LoadState=LOAD_UNLOADED;
			break;

		default:
			LoadState=LOAD_UNLOADED;
			break;
	}

	INFO("Done");
}

void EndJob(void)
{
	enum _DL olddebug=DebugLevel;
	DebugLevel=DEBUG_NONE;

	HeadUp();

	if(cancelling==NO_CANCEL)
	{
		if(olddebug==DEBUG_BBOX_ON_M2)
		{
			cu[0]=cu[1]=0.0f;

			set_target(bb[0], bb[1]);
			dda_move(calculate_feedrate_delay(250.0f));
			DelayMS(1000);
			set_target(bb[2], bb[1]);
			dda_move(calculate_feedrate_delay(100.0f));
			set_target(bb[2], bb[3]);
			dda_move(calculate_feedrate_delay(100.0f));
			set_target(bb[0], bb[3]);
			dda_move(calculate_feedrate_delay(100.0f));
			set_target(bb[0], bb[1]);
			dda_move(calculate_feedrate_delay(100.0f));
			DelayMS(1000);

			set_target(0.0f, 0.0f);
			dda_move(calculate_feedrate_delay(250.0f));
		}

		if (olddebug!=DEBUG_NONE)
		{
			// Floating point snprintf hangs, so let's work with integers.
			// For mm measurements, that's fine. Less so for inches!
			char buffer[90];
			snprintf(buffer, sizeof(buffer), "* Bounding box was %d,%d %d,%d %s\r\n",
				(int)(bb[0]/scale_to_inches+0.5f), 
				(int)(bb[1]/scale_to_inches+0.5f),
				(int)(bb[2]/scale_to_inches+0.5f), 
				(int)(bb[3]/scale_to_inches+0.5f), 
				(scale_to_inches==1?"inches":"mm"));
			cdc_print(buffer);
		}
	}

/*	Alun Jones (full reset)
	SetJobDefaults();
	HomeXAxis(); */

	// Return home at end of job, hopefully future will allow home position to be retained after load/unloads (for easier repeat jobs)
	set_target(0.0f, 0.0f);
	dda_move(calculate_feedrate_delay(100.0f));
}

int main(void)
{
	uint32_t lastactive = 0;

	// Head up/down solenoid
	// Teensy pin 13 (PTC5 output, also Teensy's onboard LED)
	PORTC_PCR5=PORT_PCR_MUX(1);
	GPIOC_PDDR|=0x0020;
	GPIOC_PCOR|=0x0020;

	// Load button
	// Teensy pin 14 (PTD1 input)
	PORTD_PCR1=((PORTD_PCR1&~(PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x06)))|(PORT_PCR_MUX(0x01)));
	GPIOD_PDDR&=~0x0002;

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

	// Home the X axis
	HomeXAxis();

	// Setup defaults
	SetJobDefaults();

	while(1)
	{
		uint32_t idle=(SysTick-lastactive)>250000;

		if (idle&&(cancelling!=NO_CANCEL))
			cancelling=NO_CANCEL;

		PollButton();

		if(button_pressed)
		{
			INFO("Button was pressed");
			button_pressed=0;

			if (idle)
				LoadYAxis();
			else
			{
				INFO("Cancelling");
				cancelling=BUTTON_CANCEL;
				EndJob();
				RESULT("cancelled");
				lastactive=SysTick;
			}
		}

		Incoming=CDC_GetCharsInRxBuf();

		if(Incoming>0)
		{
			while(Incoming--)
			{
				CDC_TComData tmp;

				CDC_RecvChar(&tmp);

				// Handle comments
				if ((tmp == '(') || (tmp == '%') || (tmp == ';'))
					comment=1;

				if(!comment)
					buffer[serial_count++]=(char)tmp;
				else if ((tmp == '\r') || (tmp == '\n'))
				{
					buffer[serial_count++]=(char)tmp;
					comment=0;
				}
				else if (tmp == ')')
					comment=0;
			}
			if (cancelling!=NO_CANCEL)
			{
				RESULT("cancelled");

				for(int i=0;i<COMMAND_SIZE;i++)
					buffer[i]=0;

				serial_count=0;
			}

			lastactive=SysTick;
		}

		// CR/LF ends lines and starts command processing
		if((cancelling==NO_CANCEL)&&((buffer[serial_count-1]=='\n')||(buffer[serial_count-1]=='\r')))
		{
			buffer[serial_count]=0;

			while((serial_count>0)&&(isspace((int)buffer[serial_count-1])))
				buffer[--serial_count]=0;

			process_string();

			// After processing, clear the buffer
			for(int i=0;i<COMMAND_SIZE;i++)
				buffer[i]=0;

			serial_count=0;
			lastactive=SysTick;
		}
	}
}
