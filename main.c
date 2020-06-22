#include <stdio.h>

#include "timer.h"

char *SERVICE=NULL;
void logger(int signal){
	char *ts = clk_human_local(0);
	void *ud = tmr_get_userdata(signal);
	fprintf(stderr, "%s %s: %s\n", ts, SERVICE, (char *) ud );
}

void segfaultme(int signal){
	unsigned long *bad_ptr = (unsigned long *) 0x3;
	logger(signal);
	*bad_ptr = 0xDEADBEEF;
}


int main( int argc, char **argv ){

	SERVICE = argv[0];

	tmr_create(TMR(1));
	tmr_create(TMR(2));
	tmr_create(TMR(3));
	tmr_create(TMR(4));

	tmr_start(TMR(1),tmr_interval(2,0,2,0), logger, "2 second timer");
	tmr_start(TMR(2),tmr_interval(5,0,5,0), logger, "5 second timer");
	tmr_start(TMR(3),tmr_interval(20,0,0,0), logger, "20 second one-shot");
	tmr_start(TMR(4),tmr_interval(30,0,0,0), segfaultme, "scheduled segfault at 30 seconds.");

	while(1)
		pause();
}
