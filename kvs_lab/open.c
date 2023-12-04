#include "kvs.h"


kvs_t* kvs_open()
{
	kvs_t* kvs = (kvs_t*) malloc (sizeof(kvs_t));

	if(kvs){
		kvs->items = 0;
		kvs->db=NULL;
	}
	
	is_recovery = 1;
	do_recovery(kvs);
	is_recovery = 0;

	printf("kvs_item : %d\n", kvs->items);
	return kvs;
}
