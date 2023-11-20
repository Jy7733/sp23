#include "kvs.h"

int close(kvs_t* kvs)
{
	/* do program */
	node_t* current = kvs->db;
	node_t* next;

	while (current!=NULL) {
		next = current->next;
		free(current->value);
		free(current);
		current=next;
	}

	kvs->db = NULL;
	kvs->items=0;

	return 0;
}
