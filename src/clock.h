#ifndef CLOCK_DEFINED
#define CLOCK_DEFINED

#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define nano_uSecs( x ) ((long) ((double)x/1000))
#define nano_mSecs( x ) ((long) ((double)x/1000000))
#define sec_uSecs( x ) ((long) ((double)x*1000000))
#define sec_mSecs( x ) ((long) ((double)x*1000))
#define usec_Nano( x ) ((long) ((double)x*1000))
#define msec_Nano( x ) ((long) ((double)x*1000000))
#define DTSTR_SIZE	30

typedef struct timespec Time_t;
typedef struct itimerspec Interval_t;

Time_t *clk_real_time(Time_t *t);
Time_t *clk_future_time( long sec, long nsec );
time_t clk_epoch_time(time_t *tloc);
char *clk_iso_time(struct tm *tptr);
char *clk_human_zulu(time_t *tloc);
char *clk_human_local(time_t *tloc);

#endif
