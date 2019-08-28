#include "MK20D10.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

extern int errno;
extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

register char *stack_ptr asm("sp");

char *__env[1]={ 0 };
char **environ=__env;

void _init(void)
{
}

void initialise_monitor_handles()
{
}

int _getpid(void)
{
	return 1;
}

int _kill(int pid, int sig)
{
	errno=EINVAL;
	return -1;
}

void _exit(int status)
{
	_kill(status, -1);
	while(1);
}

int _read(int file, char *ptr, int len)
{
	int DataIdx;

	for(DataIdx=0;DataIdx<len;DataIdx++)
		*ptr++=__io_getchar();

	return len;
}

int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for(DataIdx=0;DataIdx<len;DataIdx++)
		__io_putchar(*ptr++);

	return len;
}

caddr_t _sbrk(int incr)
{
	extern char end asm("end");
	static char *heap_end;
	char *prev_heap_end;

	if(heap_end==0)
		heap_end=&end;

	prev_heap_end=heap_end;

	if(heap_end+incr>stack_ptr)
	{
		errno=ENOMEM;
		return (caddr_t)-1;
	}

	heap_end+=incr;

	return (caddr_t)prev_heap_end;
}

int _close(int file)
{
	return -1;
}


int _fstat(int file, struct stat *st)
{
	st->st_mode=S_IFCHR;

	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _open(char *path, int flags, ...)
{
	return -1;
}

int _wait(int *status)
{
	errno=ECHILD;

	return -1;
}

int _unlink(char *name)
{
	errno=ENOENT;

	return -1;
}

int _times(struct tms *buf)
{
	return -1;
}

int _stat(char *file, struct stat *st)
{
	st->st_mode=S_IFCHR;

	return 0;
}

int _link(char *old, char *new)
{
	errno=EMLINK;

	return -1;
}

int _fork(void)
{
	errno=EAGAIN;

	return -1;
}

int _execve(char *name, char **argv, char **env)
{
	errno=ENOMEM;

	return -1;
}

volatile extern uint32_t Tick;

// Ripped from Teensyduino
uint32_t micros(void)
{
	uint32_t count, current, istatus;

	__asm__ volatile ("CPSID i");
	current=SysTick->VAL;
	count=Tick;
	istatus=SCB->ICSR;
	__asm__ volatile ("CPSIE i");

	if((istatus&SCB_ICSR_PENDSTSET_Msk)&&current>50)
		count++;

	current=((72000000/1000)-1)-current;

	return count*1000+current/(72000000/1000000);
}

// Ripped from Teensyduino
void DelayMS(uint32_t ms)
{
	uint32_t start=micros();

	if(ms>0)
	{
		while(1)
		{
			while((micros()-start)>=1000)
			{
				ms--;

				if(ms==0)
					return;

				start+=1000;
			}
		}
	}
}

// Ripped from Teensyduino
void DelayUS(uint32_t usec)
{
	uint32_t n=usec*24; // 72MHz

	if(n==0)
		return;

	__asm__ volatile(
		"L_%=_delayMicroseconds:\n"
		"subs   %0, #1\n"
		"bne    L_%=_delayMicroseconds\n"
		: "+r" (n) : );
}
