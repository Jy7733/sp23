#include "kvs.h"
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 256

int recovery = 0;

//baseline 버전 (fscanf, fprintf 사용 )
void do_snapshot(kvs_t* kvs)
{
    FILE* snpFile = fopen("kvs.img","a");
    node_t* current = kvs->db;
    fprintf(snpFile,"%s,%s\n",current->key,current->value);
    fclose(snpFile);
}

void do_recovery(kvs_t *kvs) {
    FILE *snpFile = fopen("kvs.img","r"); 
    if (snpFile == NULL) {
        perror("kvs.img open failed\n");
        return;
    }
    char key[300];
    char *value = NULL;
    size_t value_size = 0;
    int ret;
    int i = 0;

    while (1) {
        ret = fscanf(snpFile,"%[^,],",key);
        if (ret == EOF) break;

        long curr_pos = ftell(snpFile);
        fseek(snpFile,0,SEEK_END);
        long end_pos = ftell(snpFile);
        fseek(snpFile,curr_pos,SEEK_SET);
        value_size = end_pos - curr_pos;

        value = (char *)malloc(value_size + 1);
        ret = fscanf(snpFile,"%[^\n]",value);
        // printf("key : %s\n value : %s\n",key,value);
        if (ret == EOF) break;

        set(kvs,key,value);
        free(value);
        i++;
    }

    fclose(snpFile);
}

void do_cust_snpshot(kvs_t* kvs) {
    int fd = open("./kvs.img",O_WRONLY|O_CREAT,0644);
    if (fd<0){
        perror("file open error");
        exit(1);
    }

    node_t* current = kvs->db;
    // // printf("value len : %ld\n",strlen((char*)current->value));
    char buffer[512];
    int len = 0;
    int written = 0;

    // while(current!=NULL) {
    len += sprintf(buffer + len, "%s,",current->key);
    // printf("len : %d\n",len);
    char* value_part = current->value;
    while (strlen(value_part) >0) {
    //     //buffer+len 위치 부터 buffersize - len -1 까지 길이 저장 
        int part_len = sprintf(buffer+len,"%.*s",BUFFER_SIZE - len - 1,value_part);
        value_part += part_len;
        len += part_len;

        if (len >=BUFFER_SIZE-1 || strlen(value_part)==0) {
            written = write(fd,buffer,len);
            // printf("written : %d\n",written);
            if (written!=len) {
                perror("wirte error 111");
                exit(1);
            }
            len = 0;
            }
        }
    written = write(fd,"\n",1);
    if (written!=1){
        perror("write error 222");
    }
    close(fd); 
 }
    
void do_cust_recovery(kvs_t* kvs){
    int fd = open("kvs.img",O_RDONLY|O_CREAT,0644);
    if (fd<0){
        perror("file open error");
        exit(1);
    }

    int buffer_size = 1024;
    char* buffer = malloc(buffer_size*sizeof(char));

    int v_size = 512;
    char* v = malloc(v_size*sizeof(char));

    char k[300];
    char c;
    int i = 0;

    while (read(fd,&c,1)>0) {
        if (c == '\n') {
            buffer[i] = '\0';

            if(strlen(buffer) >= buffer_size-1) {
                buffer_size *= 2;
                buffer = realloc(buffer, buffer_size * sizeof(char));
            }

            if(strlen(buffer) > v_size ) {
                v_size = strlen(buffer) + 1;
                v = realloc(v, v_size * sizeof(char));
            }

            sscanf(buffer,"%[^,],%[^\n]",k,v);
            set(kvs,k,v);
            // printf("key : %s\n, value : %s\n",k,v);
            i=0; 
            continue;
        }
        buffer[i++] = c;

    }
    free(buffer);
    free(v);

    close(fd);
}