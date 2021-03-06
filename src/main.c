#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

#define BUF_SIZ	40
#define ABS(x) ((x<0)? -1*x: x)

/*
	::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	this is a small crash server, which logs to stdout some timer data, and then schedules
	a segmentation fault in order to crash and create a core file.  It has no other purpose
	and should not be used for anything, unless you actually know what you're doing. 
	::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
*/

char *SERVICE=NULL;

int randval(int hi, int lo){
	int rv = rand() % (hi+1 - lo) + lo;
	return ABS(rv) ;
}

void log_put( char *msg ){
	char *ts = clk_human_local(0);

	fprintf(stdout, "%s %s: %s\n", ts, SERVICE, msg );
	fflush(stdout);
	free(ts);

}

void logger(int signal){
	void *ud = tmr_get_userdata(signal);
	log_put((char *) ud);
}

void segfaultme(int signal){
	unsigned long *bad_ptr = (unsigned long *) 0x3;
	logger(signal);
	*bad_ptr = 0xDEADBEEF;
}

int main( int argc, char **argv ){
	int crash_schedule; 
	char buf[BUF_SIZ];
	SERVICE = argv[0];

	int hi = 0 ; 
	if (argc > 1) 
		hi = atol(argv[1]);

	int lo = 0 ; 
	if (argc > 2)
		lo = atol(argv[2]);

	if (hi > 0){
		if (lo == 0) lo = (int) (hi*0.8);
	}

	srand(clk_epoch_time(0));
	crash_schedule = randval(hi,lo);
	snprintf(buf, BUF_SIZ, "crash scheduled in %d seconds.",crash_schedule);
	log_put( buf );

	tmr_create(TMR(1));
	tmr_create(TMR(2));
	tmr_create(TMR(3));
	tmr_create(TMR(4));

	// tmr_start(TMR(1),tmr_interval(2,0,2,0), logger, "2 second timer");
	tmr_start(TMR(2),tmr_interval(5,0,5,0), logger, "5 second timer");
	tmr_start(TMR(3),tmr_interval(20,0,0,0), logger, "20 second one-shot");
	tmr_start(TMR(4),tmr_interval(crash_schedule,0,0,0), segfaultme, "scheduled segfault.");

	while(1)
		pause();
}
