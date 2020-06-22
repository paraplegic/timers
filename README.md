# libEvent

A small c library to implement timers using signals
for oneshot or repeated callbacks

## obcache.c

Maintains a singleton cache of objects pointed to 
by a static pointer local to the library.  Basically
a hack to avoid global data structures.

### Cache_t		object_cache();

Return a pointer to the static cache.

### void			obj_cache_create( int size, int obj_size );

Instantiate the static cache as an array of pointers to struct (or whatever) 
This call is idempotent, and will only modify the pointer once. 

### Obj_t 		obj_free(Obj_t obj);

Free an object created by calloc().

### Obj_t 		obj_noop(Obj_t obj);

A callback to walk the cache which does NOTHING.

### int 			obj_cache_destroy();

Destroys the cache using the obj_free() on the pointers cached.
Note that if more complicated structures are allocated in the cache
then they should have their own destructor to avoid memory leaks, but
this is beyond the scope of the current implementation.

### int 			obj_cache_walk(Iterator_t);

Iterates over the cache passing each pointer into a function which has a
fingerprint defined by Iterator_t (typedef void (*Iterator_t)())

### int 			obj_cache_size();

Walks the cache and counts the number of instantiated objects.

### void 			obj_put( int ix, Obj_t obj );

Add an object pointer to the cache at position ix.

### Obj_t 		obj_get( int ix );

Get the object pointer from the cache at position ix.

### void obj_append( Obj_t obj );

An extraordinarily inefficient mechanism to append an object pointer
at the end of the cache.  DON'T use this method.

## clock.c

## some handy macros:
- nano_uSecs( x ) ((long) ((double)x/1000))
- nano_mSecs( x ) ((long) ((double)x/1000000))
- sec_uSecs( x ) ((long) ((double)x*1000000))
- sec_mSecs( x ) ((long) ((double)x*1000))
- usec_Nano( x ) ((long) ((double)x*1000))
- msec_Nano( x ) ((long) ((double)x*1000000))

### Time_t *clk_real_time(Time_t *t);
### Time_t *clk_future_time( long sec, long nsec );
### time_t clk_epoch_time(time_t *tloc);
### char *clk_iso_time(struct tm *tptr);
### char *clk_human_zulu(time_t *tloc);
### char *clk_human_local(time_t *tloc);



## timer.c

### int				tmr_min(void);
### int				tmr_max(void);
### void			tmr_callback(int sig);
### Event_t		*tmr_evp( int signal );
### Timer_t		*tmr_context( int signal, Event_t *evp, timer_t tid, Handler_t callback );
### Timer_t		*tmr_get_context(int signal);
### void			*tmr_get_userdata(int signal);
### long			tmr_create(int signal);
### long			tmr_start(int signal, Interval_t *interval, Handler_t callback, void * user_data);
### long			tmr_stop(int signal);
### long			tmr_signal(int sig, Timer_t *context, Handler_t callback, int restart);
### Interval_t *tmr_interval( long sec, long nsec, long isec, long insec );

### TMR(x) (tmr_min()+x)
