#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define MAXLINE 1024

int main(int argc, char** argv) 
{
	int clientfd, port;
	struct sockaddr_in serveraddr;

	char *host,buf[MAXLINE];

	host = argv[1];
	port = atoi(argv[2]);

	if ((clientfd = socket(AF_INET,SOCK_STREAM,0))<0) {
		perror("socket");
		exit(1);
	}
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons((unsigned short)port);
	serveraddr.sin_addr.s_addr = inet_addr(host);

	if(connect(clientfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		perror("connect");
		exit(1);
	}

	FILE* input_file = fopen("cluster004_web.trc","r");
	FILE* output_file = fopen("answer.dat","w");

	int line_size = 1024;
	char* line = malloc(line_size * sizeof(char));

	int v_size = 512;
	char* v = malloc(v_size * sizeof(char));

	char cmd[5];
	char k[300];


	while(fgets(line,line_size,input_file)!=NULL) {
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
			write(clientfd,line,strlen(line));

			int response;
			read(clientfd,&response,sizeof(response));
			fprintf(output_file,"%d\n",response);
		}
		else if(strcmp(cmd,"get")==0) {
			write(clientfd,line,strlen(line));

			char response[v_size];
			int n = read(clientfd,response,v_size);
			response[n] = '\0';
			fprintf(output_file,"%s\n",response);
		}
	}

	free(line);
	free(v);
	fclose(input_file);
	fclose(output_file);

	close(clientfd);
	return 0;
}

