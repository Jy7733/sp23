#include "kvs.h"

int set(kvs_t* kvs, const char* key, const char* value)
{
	// printf("set: %s, %s\n", key, value);

	/* do program here */
	node_t* newNode = (node_t*)malloc(sizeof(node_t));
	if (!newNode) {
		fprintf(stderr,"set failed\n");
		return -1;
	}

	strcpy(newNode->key,key);
	newNode->value = strdup(value);

	if (!newNode->value) {
		fprintf(stderr,"set failed\n");
		return -1;
	}

	newNode->next = kvs->db;
	kvs->db = newNode;
	kvs->items++;


	return 0;
}