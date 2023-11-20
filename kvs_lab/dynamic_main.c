// #include "kvs.h"

int main()
{
	// kvs_t* kvs = open();

	// if(!kvs) {
	// 	printf("Failed to open kvs\n");
	// 	return -1;
	// }

	// // workload execution  
	
	// // 1) 	file read 
	// // 2) 	if set, insert (key, value) into kvs.
	// // 		if get, seach the key in kvs and return the value. 
	// //		Return -1 if the key is not found  


	// close(kvs);
    void *handle;
    
    handle = dlopen("./libkvs.so",RTLD_LAZY); //라이브러리 주소 받아오기
    if (!handle) {
        fprintf(stderr,"%s\n",dlerror());
        exit(1);
    }

    //라이브러리 내의 함수 불러오기
    kvs_t* (*open)() = dlsym(handle,"open");
    int(*close)(kvs_t*) = dlsym(handle,"close");
    int(*set)(kvs_t*,const char*,const char*) = dlsym(handle,"set");
    char* (*get)(kvs_t*,const char*) = dlsym(handle,"get");

    //open 함수 호출
    kvs_t* kvs = open();

    //query.dat파일 열기 
	FILE* file = fopen("query.dat","r");
	if (file == NULL) {
		perror("open fail");
		return -1;   //오류 발생 시 -1 return 
	}

    //answer.dat파일 열기
	FILE* fp = fopen("answer.dat","w");
	if (file == NULL) {
		perror("open fail");
		return -1;   //오류 발생 시 -1 return 
	}

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


    fclose(file);
    fclose(fp);


    close(kvs);
	dlcose(handle); //라이브러리 handle close
	return 0;
}