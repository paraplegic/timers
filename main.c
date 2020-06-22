#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

#define BUF_SIZ	40
#define ABS(x) ((x<0)? -1*x: x)

int randval(int hi, int lo){
	int rv = rand() % (hi+1 - lo) + lo;
	return ABS(rv) ;
}

char *SERVICE=NULL;
void log_put( char *msg ){
	char *ts = clk_human_local(0);
	fprintf(stderr, "%s %s: %s\n", ts, SERVICE, msg );
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

	srand(clk_epoch_time(0));
	crash_schedule = randval(32,10);
	snprintf(buf, BUF_SIZ, "crash scheduled in %d seconds.",crash_schedule);
	log_put( buf );

	tmr_create(TMR(1));
	tmr_create(TMR(2));
	tmr_create(TMR(3));
	tmr_create(TMR(4));

	tmr_start(TMR(1),tmr_interval(2,0,2,0), logger, "2 second timer");
	tmr_start(TMR(2),tmr_interval(5,0,5,0), logger, "5 second timer");
	tmr_start(TMR(3),tmr_interval(20,0,0,0), logger, "20 second one-shot");
	tmr_start(TMR(4),tmr_interval(crash_schedule,0,0,0), segfaultme, "scheduled segfault.");

	while(1)
		pause();
}
