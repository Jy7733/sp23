#include "kvs.h"

char* get(kvs_t* kvs, const char* key)
{
	/* do program here */
	char* value = (char*)malloc(sizeof(char)*100);

	if(!value){
		printf("Failed to malloc\n");
		return NULL;
	}

	strcpy(value,"-1\n");
	// strcpy(value, "deadbeaf");
	node_t* current = kvs->db;
	while(current!=NULL) {
		if (strcmp(key, current->key)==0) {
			strcpy(value,current->value);
			strcat(value,"\n");
		}
		current = current->next;
	}

	return value;
}
