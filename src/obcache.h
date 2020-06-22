#ifndef CACHE_INCLUDED
#define CACHE_INCLUDED

#include <stdio.h>
#include <stdlib.h>

typedef void * Obj_t;
typedef Obj_t (*Iterator_t)( Obj_t );
typedef struct cache {
	int argc;
	void **argv;
} Cache_t;

void			*object_cache();
void			obj_cache_create( int size, int obj_size );
Cache_t		*get_cache();
Obj_t 		obj_free(Obj_t obj);
Obj_t 		obj_noop(Obj_t obj);
int 			obj_cache_destroy();
int 			obj_cache_walk(Iterator_t);
int 			obj_cache_size();
void 			obj_put( int ix, Obj_t obj );
Obj_t 		obj_get( int ix );

#endif
