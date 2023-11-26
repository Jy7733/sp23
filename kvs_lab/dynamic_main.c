#include "kvs.h"
#include <dlfcn.h>

int main()
{
    void* handle = dlopen("./libkvs.so",RTLD_LAZY); //라이브러리 주소 받아오기
    
    if (!handle) {
        fprintf(stderr,"%s\n",dlerror());
        exit(1);
    }

    //라이브러리 내의 함수 불러오기
    kvs_t* (*open_func)() = dlsym(handle,"open");
    int (*close_func)(kvs_t*) = dlsym(handle,"close");
    int (*set)(kvs_t*,const char*,const char*) = dlsym(handle,"set");
    char *(*get)(kvs_t*,const char*) = dlsym(handle,"get");
    
    if (!open_func || !close_func || !set || !get) {
        fprintf(stderr, "Failed : %s\n", dlerror());
        dlclose(handle);
        return 1;
    }


//     //open 함수 호출
    kvs_t* kvs = open_func();
    if (!kvs) {
        fprintf(stderr, "Failed to initialize kvs\n");
        dlclose(handle);
        exit(1);
    }

//     //query.dat파일 열기 
	FILE* file = fopen("query.dat","r");
	if (file == NULL) {
		perror("open fail");
        fclose(file);
		return -1;   //오류 발생 시 -1 return 
	}

//     //answer.dat파일 열기
	FILE* fp = fopen("answer.dat","w");
	if (file == NULL) {
		perror("open fail");
        fclose(fp);
		return -1;   //오류 발생 시 -1 return 
	}

	char line[512];
	while (fgets(line,sizeof(line),file)!=NULL) {
		char cmd[5]; //set혹은 get
		char k[100]; //게시물id
		char v[100]; //사용자 id

		sscanf(line,"%[^,],%[^,],%s",cmd,k,v);
		if (strcmp(cmd,"set")==0) {
			set(kvs,k,v);
		}

		else if (strcmp(cmd,"get")==0) {
            char* val = get(kvs,k);
            fprintf(fp,"%s\n",val);
		}
	}
    fclose(file);
    fclose(fp);


    close_func(kvs);
    if (dlclose(handle) < 0){
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

	return 0;
}