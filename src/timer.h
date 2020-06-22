#ifndef TIMER_INCLUDED
#define TIMER_INCLUDED

#include "clock.h"
#include "obcache.h"

#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <limits.h>
#include <signal.h>
#include <stdlib.h>

typedef void (*Handler_t)();
typedef struct itimerspec Interval_t;
typedef struct sigevent Event_t;

// currently unused ... 
typedef struct tmr_metrics {
	long count;
	Time_t start;
	Time_t last;
} Timer_metrics_t;

typedef struct tmr_ctxt {
	int signal;
	timer_t	tid ;
	Event_t *evp ;
	Interval_t *interval;
	Handler_t callback;
	Timer_metrics_t *metrics;
	void *user_data ;
} Timer_t;


int				tmr_min(void);
int				tmr_max(void);
void			tmr_callback(int sig);
Event_t		*tmr_evp( int signal );
Timer_t		*tmr_context( int signal, Event_t *evp, timer_t tid, Handler_t callback );
Timer_t		*tmr_get_context(int signal);
void			*tmr_get_userdata(int signal);
long			tmr_create(int signal);
long			tmr_start(int signal, Interval_t *interval, Handler_t callback, void * user_data);
long			tmr_stop(int signal);
long			tmr_signal(int sig, Timer_t *context, Handler_t callback, int restart);
Interval_t 
					*tmr_interval( long sec, long nsec, long isec, long insec );

#define MIN(x,y) (((x) < (y)) ? x : y)
#define MAX(x,y) (((x) > (y)) ? x : y)
#define MAX_TIMERS	10
#define TMR(x) (tmr_min()+x)

#endif
