#include "clock.h"

Time_t *clk_real_time(Time_t *t){
	Time_t *rv;

	rv = t; 
	if( rv == NULL ){
		rv = calloc( 1, sizeof(Time_t));
	}

	if( rv != NULL ){
		clock_gettime(CLOCK_REALTIME, rv);
	}
	return rv ;
}

Time_t *clk_future_time( long sec, long nsec ){
	Time_t *rv;

	rv = clk_real_time(0);
	if( rv != NULL ){
		rv ->tv_sec += sec ;
		rv ->tv_nsec += nsec ; 
	}
	return rv ;
}

time_t clk_epoch_time(time_t *tloc){
	time_t *rv ;
	rv = tloc;
	if( rv == NULL ){
		rv = calloc(1,sizeof(time_t));
	}
	return time(rv);
}


char *clk_iso_time(struct tm *tptr){
	char *rv = calloc(1,DTSTR_SIZE);

	strftime( rv, (size_t) DTSTR_SIZE, "%F %T %z", tptr );
	return rv ;
}

char *clk_human_zulu(time_t *tloc){
	time_t gmt;
	struct tm tptr;
	char *rv = calloc(1,DTSTR_SIZE);

	memset((char *) &tptr, 0, sizeof(tptr));

	if( tloc != NULL )
		gmt = *tloc;
	else
		gmt = time(0);

	rv = clk_iso_time(gmtime_r(&gmt, &tptr));
	return rv ;
}

char *clk_human_local(time_t *tloc){
	time_t lcltime;
	struct tm tptr;
	char *rv = calloc(1,DTSTR_SIZE);

	memset((char *) &tptr, 0, sizeof(tptr));

	if( tloc != NULL )
		lcltime = *tloc;
	else
		lcltime = time(0);

	rv = clk_iso_time(localtime_r(&lcltime, &tptr));
	return rv ;
}



#ifdef TEST
#include <stdio.h>
int main()
{
	Time_t *t;
	Interval_t *iv;
	time_t clk; 
	char *cclk;

	t = clk_real_time(0);
	printf("realtime returns %ld seconds and %ld nanoseconds\n", t->tv_sec, t->tv_nsec);

	clk = clk_epoch_time(0);
	printf("epoch_time() returns: %ld local ('%s')\n", clk, cclk = clk_human_local(0));
	printf("epoch_time() returns: %ld zulu ('%s')\n", clk, cclk = clk_human_zulu(0));
	printf("time returns %ld micro seconds\n", nano_uSecs(t->tv_nsec));
	printf("time returns %ld milli seconds\n", nano_mSecs(t->tv_nsec));
	printf("secs = %ld milli seconds\n", sec_mSecs(t->tv_sec));
	printf("time = %ld micro seconds\n", sec_uSecs(t->tv_sec));
	free(t);

	t = clk_future_time( 10, 10 );
	printf("future returns: %ld seconds and %ld nanoseconds\n", t->tv_sec, t->tv_nsec);
	free(t);
}
#endif
