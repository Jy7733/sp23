#include "kvs.h"
#include <unistd.h>
#include <fcntl.h>


#define BUFFER_SIZE 256

int is_recovery = 0;

//baseline 버전 (fscanf, fprintf 사용 )
void do_snapshot(kvs_t* kvs)
{
    FILE* snpFile = fopen("kvs.img","a");
    node_t* current = kvs->db;
    if (current->key ==NULL){
        return;
    }
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
        if (ret == EOF) break;

        node_t* newNode = (node_t*)malloc(sizeof(node_t));
        strcpy(newNode->key,key);
        newNode->value = strdup(value);
        newNode->next = kvs->db;
        kvs->items++;

        free(value);
        i++;
        
    }
    is_recovery = 1;
    fclose(snpFile);
}

void do_cust_snpshot(kvs_t* kvs) {
    int fd = open("kvs.img",O_WRONLY|O_APPEND|O_CREAT,0644);
    if (fd<0){
        perror("file open error");
        exit(1);
    }

    node_t* current = kvs->db;
    if (current->key == NULL){
        return;
    }
    write(fd,current->key,strlen(current->key));
    write(fd,",",1);
    write(fd,current->value,strlen(current->value));
    write(fd,"\n",1);
    close(fd);
 }
    
void do_cust_recovery(kvs_t* kvs){
    int fd = open("kvs.img",O_RDONLY);
    if (fd<0){
        perror("file open error");
        return;
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
            node_t* newNode = (node_t*)malloc(sizeof(node_t));
            strcpy(newNode->key,k);
            newNode->value = strdup(v);
            newNode->next = kvs->db;
            kvs->items++;

            i=0; 
            continue;
        }
        buffer[i++] = c;

    }
    free(buffer);
    free(v);
    close(fd);
    is_recovery = 1;
}
