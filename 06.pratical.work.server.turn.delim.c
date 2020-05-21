#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "package.h"
#define MAX_SIZE 2048

int signal = 1;
void* dataRecv(void* socket) {
	int sk = *(int*) socket;
	char buff[MAX_SIZE];
	while (recv(sk, buff, MAX_SIZE, 0)!= 0) {
		struct PAK package = decodePak(buff);
		printf("\rClient> %s", package.data);
		printf("Server> ");fflush(stdout);
		memset(buff, 0, MAX_SIZE);
		memset(package.data, 0 ,1024);	
	}
	signal = 0;

}

void* dataSend(void* socket) {
	char buff[MAX_SIZE];
	int sk = *(int*) socket;
	struct PAK package;
	while (signal) {
		printf("Server> ");
		fgets(buff, MAX_SIZE, stdin);
		memcpy(package.header, "MESS\0", 5);
		package.length = (unsigned short)strlen(buff);
		memcpy(package.data, buff, strlen(buff));
		send(sk, (char *) &package, sizeof(package), 0);
		memset(buff, 0, MAX_SIZE);
	}
}

int main()
{
    int ss, cli, pid;
    struct sockaddr_in ad;
    char s[100];
    socklen_t ad_length = sizeof(ad);

    // create the socket
    ss = socket(AF_INET, SOCK_STREAM, 0);

    // bind the socket to port 8784
    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY;
    ad.sin_port = htons(8784);
    if (bind(ss, (struct sockaddr *)&ad, ad_length) < 0) {
    	printf("Can't bind");
	exit(1);
    }

    // then listen
    if (listen(ss, 0) == -1){
    	printf("Can't listen on port 8784");
	exit(1);
    }

    cli = accept(ss, (struct sockaddr *)&ad, &ad_length);
    if (cli == -1) {
    	printf("Oops! Something wrong");
	exit(1);
    } else { 
	printf("Client Connected\n");
   	pthread_t t1, t2;
	pthread_create(&t1, NULL, &dataSend,&cli);
	pthread_create(&t2, NULL, &dataRecv,&cli);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
    }	

    // disconnect
    close(cli);
}

