#include "kvs.h"

int main()
{
	kvs_t* kvs = open();

	if(!kvs) {
		printf("Failed to open kvs\n");
		return -1;
	}

	// workload execution  
	
	// 1) 	file read 
	// 2) 	if set, insert (key, value) into kvs.
	// 		if get, seach the key in kvs and return the value. 
	//		Return -1 if the key is not found  

	//query.dat 파일 열기
	FILE* file = fopen("query.dat","r");
	if (file == NULL) {
		perror("open fail");
		return -1;   //오류 발생 시 -1 return 
	}

	//answer.dat 파일 열기
	FILE* fp = fopen("answer.dat","w");
	if (file == NULL) {
		perror("open fail");
		return -1;   //오류 발생 시 -1 return 
	}
	
	//qeury.dat 에서 한 줄 씩 읽어오기
	char line[512];
	while (fgets(line,sizeof(line),file)!=NULL) {
		char cmd[5]; //set혹은 get
		char k[100]; //게시물id
		char v[100]; //사용자 id

		sscanf(line,"%[^,],%[^,],%s",cmd,k,v);

		//cmd가 set이면 set함수 호출
		if (strcmp(cmd,"set")==0) {
			set(kvs,k,v);
		}

		else if (strcmp(cmd,"get")==0) {
			char* val = get(kvs,k);
			fprintf(fp,"%s",val);
		}
	}
	printf("kvs items : %d",kvs->items);
	fclose(file);
	fclose(fp);

	close(kvs);
	return 0;
}
