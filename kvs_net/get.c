#include "kvs.h"

char* get(kvs_t* kvs, const char* key)
{
	// /* do program here */
	char* value = (char*)malloc(sizeof(char)*100);

	if(!value){
		printf("Failed to malloc\n");
		return NULL;
	}

	strcpy(value,"-1");
	node_t* current = kvs->db;
	while(current!=NULL) {
		if (strcmp(key, current->key)==0) {
			value = (char*)malloc(strlen(current->value)+1);
			strcpy(value,current->value);
			break;
		}
		current = current->next;
	}

	return value;
}
