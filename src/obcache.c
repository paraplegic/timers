#include "obcache.h"

// There can be only one ...
void *object_cache(){
	static Cache_t *heap = (Cache_t *) NULL;
	return &heap;
}

Cache_t *get_cache(){
	Cache_t **cache = object_cache();
	return *cache;
}

void obj_cache_create( int size, int obj_size ){
	Cache_t **heap = object_cache();
	Cache_t *lcl ; 
	if( *heap == NULL ){
			*heap = (Cache_t *) calloc( 1, sizeof(Cache_t));
			lcl = *heap ;
			lcl->argc = size ;
			lcl->argv = (void *) calloc( size, obj_size);
			lcl = lcl ;
	}
}

Obj_t obj_noop(Obj_t obj){
	return obj;
}

Obj_t obj_free(Obj_t obj){
	if( obj != NULL ){
		free(obj);
	}
	return (Obj_t) NULL;
}

int obj_cache_walk( Iterator_t iter ){
	int ix ;
	Obj_t obj;
	Cache_t *heap = get_cache();
	for( ix=0; ix <heap->argc; ix++ ){
			obj = obj_get(ix);
			if( iter != NULL ) obj = iter(obj);
	}
	return ix;
}

int obj_cache_destroy(){
	int ix = -1;
	Cache_t **heap = object_cache();
	Cache_t *cache = get_cache();
	if( cache != NULL ){
		ix = obj_cache_walk( (Iterator_t) obj_free );
		cache->argc = 0;
		cache->argv = obj_free(cache->argv);
		cache = obj_free(cache);
		*heap = NULL ;
	}
	return ix ;
}

int obj_cache_size(){
	Cache_t *cache = get_cache();
	if( cache != NULL ){
		return cache->argc ;
	}
	return -1 ;
}

void obj_put( int ix, Obj_t obj ){
	Cache_t *cache = get_cache();
	if( cache != NULL ){
		cache->argv[ix] = obj;
	}
}

// severely NON-LINEAR ... don't use this
void obj_append( Obj_t obj ){
	obj_put(obj_cache_size(),obj);
}

Obj_t obj_get( int ix ){
	Cache_t *cache = get_cache();
	if( cache != NULL && cache->argv != NULL ){
		return cache->argv[ix];
	}
	return NULL;
}

#ifdef TEST
#include <assert.h>
int main( int argc, char **argv ){
	int *myobject;
	int *tstobject;;
	int max = 3 * 1024000;
	int ix = 0;

	obj_cache_create( max, sizeof( Obj_t ));

	for(ix = 0; ix < max; ix++ ){
		myobject = calloc( 1, sizeof(int));
		*myobject = ix ;
		// this goes NON-LINEAR in quick order
		// test at your peril
		// obj_append( myobject );
		obj_put(ix,myobject);
		tstobject = obj_get(ix);
		assert(myobject == tstobject);
		assert(ix == (int) *tstobject);
	}
	printf( "Put %d objects into cache.\n", ix );
	assert(ix==max);

	printf( "Cache contains %d objects.\n", ix = obj_cache_size());
	assert(ix==max);

	printf( "Object cache pointer is %p.\n",get_cache());

	for(ix = 0; ix < max; ix++ ){
		myobject = obj_get(ix); 
		assert(ix == *myobject);
	}
	printf( "Pulled %d objects from cache.\n", ix );
	assert(ix==max);

	printf( "Object cache pointer is %p.\n",get_cache());

	ix = obj_cache_destroy(); 
	printf( "Cache destroyed %d objects.\n", ix);
	assert(ix==max);

	printf( "Object cache pointer is %p.\n",get_cache());
	printf( "Cache contains %d objects.\n", ix = obj_cache_size());
	assert(ix==-1);

	printf( "Cache destroyed %d objects.\n", ix = obj_cache_destroy());
	assert(ix==-1);

	printf( "Object cache pointer is %p.\n",get_cache());
	
	for(ix = 0; ix < max; ix++ ){
		myobject = obj_get(ix); 
		if( myobject != NULL ){
			printf( "%d => %p\n", ix, myobject ) ;
		}
		assert(myobject == NULL);
	}
	printf( "ALL TESTS PASSED.\n");
	exit(0);
}
#endif
