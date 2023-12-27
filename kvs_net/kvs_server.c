#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include "kvs.h"

#define LISTENQ 1024
#define MAXLINE 1024

int main(int argc, char **argv) 
{
	int listenfd, connfd, port, clientlen;
	struct sockaddr_in serveraddr, clientaddr;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0))<0) {
		perror("socket");
		exit(-1);
	}

	port = atoi(argv[1]);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);

	if (bind(listenfd, (struct sockaddr*)&serveraddr,sizeof(serveraddr)) == -1) {
		perror("Failed to bind");
		return -1;
	}

	if(listen(listenfd,LISTENQ) <0)
		return -1;

	printf("Waiting for request ... \n");

	clientlen = sizeof(clientaddr);
	if ((connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientlen))<0) {
		perror("accept");
		exit(-1);
	}

	char* caddrp = inet_ntoa(clientaddr.sin_addr);
	printf("Connected to %s\n",caddrp);

	kvs_t* kvs = kvs_open();

	int line_size = 1024;
	char* line = malloc(line_size*sizeof(char));

	int v_size = 512;
	char* v = malloc(v_size*sizeof(char));

	char cmd[5];
	char k[300];

	int n;
	while ((n=read(connfd,line,line_size))>0) {
		line[n] = '\0';

		if (strlen(line) >= line_size -1) {
			line_size *= 2;
			line = realloc(line,line_size*sizeof(char));
		}
		if (strlen(line) > v_size) {
			v_size = strlen(line) + 1;
			v = realloc(v,v_size*sizeof(char));
		}

		sscanf(line,"%[^,],%[^,],%[^\n]",cmd,k,v);

		if(strcmp(cmd,"set")==0) {
			set(kvs,k,v);
			int value_size = strlen(v);
			write(connfd,&value_size,sizeof(value_size));
		}
		else if(strcmp(cmd,"get")==0) {
			char* value = get(kvs,k);
			write(connfd,value,strlen(value));
		}
	}
	return 0;
}