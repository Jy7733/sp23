#include "kvs.h"

kvs_t* kvs_open()
{
	kvs_t* kvs = (kvs_t*) malloc (sizeof(kvs_t));

	if(kvs){
		kvs->items = 0;
		kvs->db=NULL;
	}
	
	// do_recovery(kvs);
	do_cust_recovery(kvs);

	printf("kvs_item : %d\n", kvs->items);
	return kvs;
}
