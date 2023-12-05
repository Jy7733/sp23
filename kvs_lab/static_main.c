#include "kvs.h"


int main()
{
	kvs_t* kvs = kvs_open();

	if(!kvs) {
		printf("Failed to open kvs\n");
		return -1;
	}

	FILE* file = fopen("cluster004.trc","r");

	int line_size = 1024;
	char* line = malloc(line_size * sizeof(char));

	int v_size = 512;
	char* v = malloc(v_size * sizeof(char));

	char cmd[5];
	char k[300];


	while(fgets(line,line_size,file)!=NULL) {
		//line의 크기가 부족하다면, realloc으로 재할당
		if(strlen(line) >= line_size -1){
			line_size *=2;
			line = realloc(line,line_size * sizeof(char));
		}
		//v의 크기가 부족하다면,realloc으로 재할당 
		if(strlen(line) > v_size) {
			v_size = strlen(line) +1;
			v = realloc(v,v_size * sizeof(char));
		}

		sscanf(line,"%[^,],%[^,],%[^\n]",cmd,k,v);

		if(strcmp(cmd,"set")==0) {
			set(kvs,k,v);
			 //snapshot 
			// do_snapshot(kvs);
			do_cust_snpshot(kvs);
		}
		else if(strcmp(cmd,"get")==0) {
			get(kvs,k);
		}
	}
	printf("kvs items : %d\n",kvs->items);

	free(line);
	free(v);
	fclose(file);
	kvs_close(kvs);
	return 0;
}
