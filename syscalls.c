#include "MIMXRT1062.h"
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

extern unsigned long _heap_start;
extern unsigned long _heap_end;

char *__brkval=(char *)&_heap_start;

void *_sbrk(int incr)
{
	char *prev=__brkval;
	if(incr!=0)
	{
		if(prev+incr>(char *)&_heap_end)
		{
			errno=ENOMEM;
			return (void *)-1;
		}
		__brkval=prev+incr;
	}
	return prev;
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

// Taken from Teensyduino
extern uint32_t systick_millis_count;
extern uint32_t systick_cycle_count;
extern uint32_t scale_cpu_cycles_to_microseconds;
uint32_t systick_safe_read=0;

uint32_t micros(void)
{
	uint32_t smc, scc;

	do
	{
		__LDREXW(&systick_safe_read);
		smc=systick_millis_count;
		scc=systick_cycle_count;
	} while(__STREXW(1, &systick_safe_read));

	uint32_t cyccnt=DWT_BASE;

	asm volatile("" : : : "memory");

	uint32_t ccdelta=cyccnt-scc;
	uint32_t frac=((uint64_t)ccdelta*scale_cpu_cycles_to_microseconds)>>32;

	if(frac>1000)
		frac=1000;

	uint32_t usec=1000*smc+frac;

	return usec;
}
