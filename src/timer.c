#include "timer.h"

int tmr_min(void){
	return SIGRTMIN;
}

int tmr_max(void){
	return SIGRTMAX;
}

Interval_t *tmr_interval( long sec, long nsec, long isec, long insec ){
	Interval_t *rv ;

	rv = calloc(1,sizeof(Interval_t));
	if( rv != NULL ){
		rv ->it_value.tv_sec = sec;
		rv ->it_value.tv_nsec = nsec;
		rv ->it_interval.tv_sec = isec;
		rv ->it_interval.tv_nsec = insec;
	}
	return rv;
}

Event_t *tmr_evp( int signal ){
	Event_t *rv ;

	rv = calloc( 1, sizeof( Event_t ));
	if( rv != NULL ){
		rv ->sigev_notify = 0;
		rv ->sigev_signo = signal;
	}
	return rv ;
}

Timer_t *tmr_context( int signal, Event_t *evp, timer_t tid, Handler_t callback ){
	Timer_t *rv = calloc(1,sizeof(Timer_t));
	if( rv != NULL ){
		rv ->evp = evp;
		rv ->tid = tid;
		rv ->signal = signal;
		rv ->interval = (Interval_t *) NULL ;
		rv ->callback = callback;
	}
	return rv;
}

long tmr_create(int signal){
	long rv ;
	timer_t tid ;
	Timer_t *ctxt;
	Cache_t cache; 
	Event_t *evp;

	if( signal > tmr_max()){
		perror("invalid signal specified");
		return -1;
	}

	obj_cache_create( SIGRTMAX, sizeof( Timer_t *));
	evp = tmr_evp( signal );
	rv = timer_create( CLOCK_REALTIME, tmr_evp(signal), &tid );
	if( rv != 0 ){
		perror("timer_create()");
	}
	obj_put( signal,tmr_context(signal, evp, tid, (Handler_t) NULL));
	return rv;
}

long tmr_start(int signal, Interval_t *interval, Handler_t callback, void * user_data){
	long rv ;
	Timer_t *ctxt ;
	Interval_t old ;

	ctxt = tmr_get_context(signal);
	if( ctxt == NULL ){
		printf("no context found in object cache.\n");
		exit(1);
	}
	ctxt ->interval = interval;
	ctxt ->callback = callback;
	ctxt ->user_data = user_data;
	rv = tmr_signal(signal, ctxt, callback, 1);

	rv = timer_settime( ctxt->tid, 0, ctxt->interval, &old );
	if( rv != 0 ){
		perror("timer_settime()");
	}
	return rv;
}

Timer_t *tmr_get_context(int signal){
	return obj_get(signal);
}

void *tmr_get_userdata(int signal){
	Timer_t *ctxt = tmr_get_context(signal);
	return ctxt->user_data;
}

long tmr_stop(int signal){
	Timer_t 		*ctxt; 
	Interval_t  *interval;

	interval = tmr_interval(0,0,0,0);
	ctxt = tmr_get_context(signal);
	if( ctxt->interval ){
		free(ctxt->interval);
		ctxt->interval = interval ;
	}
	return tmr_start(signal, ctxt->interval, ctxt->callback, ctxt->user_data );
}

void tmr_callback(int sig){
	Timer_t *ctxt;

	ctxt = tmr_get_context(sig);
	if( ctxt->callback )
		ctxt->callback(sig);
}

long tmr_signal(int sig, Timer_t *context, Handler_t callback, int restart){
	long err ;
	struct sigaction action ;

	sigemptyset( &action.sa_mask );
	action.sa_handler = (Handler_t) tmr_callback;
	if(restart != 0){
		action.sa_flags = SA_RESTART;
	}

	err = sigaction( sig, &action, NULL );
	if( err != 0 ){
	}
	return err ;
}

void tmr_clear(){
	timer_t tid ;
	Timer_t *ctxt ;
	Cache_t *cache = get_cache();

	for(int i=1;i++;i<cache->argc){
		ctxt = obj_get(i); 
		if( ctxt != NULL ){
			tid = ctxt->tid ;
			timer_delete( tid );
			if( ctxt->user_data != NULL ){
				ctxt->evp = obj_free(ctxt->evp);
				ctxt->user_data = obj_free(ctxt->user_data);
			}
		}
	} 
	obj_cache_destroy(); 
}

#ifdef TEST
#include <stdio.h>
#include <unistd.h>
#define TMR(x) (tmr_min()+x)
void callbk( int sig ){
	Timer_t *context ;
	context = tmr_get_context(sig);
	printf( "--- caught signal %d (%s) ---\n", sig, (char *) context->user_data ) ;
	
}

char *allocate(char *s){
	char *rv;
	int len = strlen(s);
	rv = (char *) calloc( 1, len+1);
	memcpy(rv,s,len);
	return rv;
}

int main(){
	printf( "::: TMR create: (%d) %ld :::\n", TMR(1), tmr_create(TMR(1)));
	printf( "::: TMR create: (%d) %ld :::\n", TMR(2), tmr_create(TMR(2)));
	printf( "::: TMR create: (%d) %ld :::\n", TMR(3), tmr_create(TMR(3)));
	printf( "::: TMR create: (%d) %ld :::\n", TMR(4), tmr_create(TMR(4)));
	tmr_start(TMR(1), tmr_interval( 0,msec_Nano(500),0,msec_Nano(500) ), callbk, "hi mom");
	tmr_start(TMR(2), tmr_interval( 7,0,7,0 ), callbk, allocate("slower timer"));
	tmr_start(TMR(3), tmr_interval( 10,0,0,0 ), callbk, allocate("one shot timer"));
	tmr_start(TMR(4), tmr_interval( 60,0,60,0 ), callbk, allocate("one minute timer"));

	int ix = 0;
	while(1){
		ix++;
		pause();
		if( ix == 30){
			printf( "Tired of %d\n", TMR(1));
			tmr_stop(TMR(1));
		}
		if( ix > 40 ){
			printf( "Tired of %d\n", TMR(2));
			tmr_stop(TMR(2));
		}
		if( ix > 41 ){
			printf( "Enough of this ... %d\n", TMR(4));
			tmr_stop(TMR(4));
			break ;
		}
	}
	tmr_clear();

}

#endif
