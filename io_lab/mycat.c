#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFF_SIZE 16

int main(int argc, char* argv[])
{
	int ifd, ofd;
	char* buff;
	ssize_t nreads, nwrites;

	if(argc < 2) {
		fprintf(stdout, "No arguments\n");
		return 0;
	} else if(argc == 2) {
		// output to stdout file 
		ifd = open(argv[1], O_RDONLY);
		if(ifd < 0) {
         fprintf(stderr, "Failed to open input file\n");
         return 0;
      	}

		buff = (char*)malloc(sizeof(char) * BUFF_SIZE);

		while ((nreads = read(ifd, buff, BUFF_SIZE)) > 0)
        {
			write(STDOUT_FILENO, buff,nreads);
        }

	} else if(argc == 4 && !strcmp(argv[2], "into")) {
		// output to redirection file 
		ifd = open(argv[1], O_RDONLY);
        if (ifd < 0) {
            perror("Error opening input file");
            exit(EXIT_FAILURE);
        }
        ofd = open(argv[3],O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (ofd < 0) {
			fprintf(stderr, "Failed to open output file\n");
         	return 0;
        }
        buff = (char*)malloc(sizeof(char) * BUFF_SIZE);
        while((nreads = read(ifd, buff, BUFF_SIZE)) > 0) {
        	write(ofd, buff, nreads);	
      	}	

	} else {
		fprintf(stdout, "Wrong arguments\n");
		return 0;
	}

	// Open input file
	ifd = open(argv[1],O_RDONLY);

	// Allocate buffer 
	buff = (char*)malloc(sizeof(char) * BUFF_SIZE);
	if(!(buff = (char*) malloc (sizeof(char) * BUFF_SIZE))){
		fprintf(stdout, "Failed to allocate memory\n");
		exit(-1);
	}

	// Copy files 

	
	// Close files 
	free(buff);
	close(ifd);
    close(ofd);


}
