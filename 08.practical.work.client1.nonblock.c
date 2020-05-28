#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include "package.h"
#include <signal.h>
#define MAX_SIZE 2048

int terminateSignal = 1;
pthread_t t1, t2;
void *dataSend(void* socket) {
	int sk = *(int*) socket;
	char buff[MAX_SIZE];
	while (terminateSignal) {
		printf("Client> ");
		struct PAK package;
		fgets(buff, MAX_SIZE, stdin);
		//if (strncmp("exit", buff, 4) == 0) {
		//	pthread_kill(t2, 1);
		//	pthread_exit(0);
		//}
		memcpy(package.header, "MESS\0", 5);
		package.length = (unsigned short) strlen(buff);
		memcpy(package.data, buff, strlen(buff));
		send(sk, (char *)&package , sizeof(package), 0);
		memset(buff, 0, MAX_SIZE);
		memset(package.data, 0, 1024);
	}
}

void *dataRecv(void* socket) {
	char buff[MAX_SIZE];
	int sk = *(int*) socket;
	while (recv(sk, buff, MAX_SIZE, 0) != 0) {
		struct PAK package = decodePak(buff);
		printf("\rServer> %s", package.data);
		printf("Client> ");fflush(stdout);
		memset(buff, 0 , MAX_SIZE);
		memset(package.data, 0, 1024);
	}
        printf("Connection terminated\n");	
	terminateSignal = 0;
}

int main(int argc, char **argv) {
	struct sockaddr_in ad;
	struct hostent *host;
	
	if (argc == 1) {
		printf("Usage %s <ip>\n",argv[1]);
		exit(1);
	}
	host = gethostbyname(argv[1]);
	if (host == NULL) {
		printf("Error: invalid ip address\n");
		exit(1);
	}

	int serv = socket(AF_INET, SOCK_STREAM, 0);
	memset(&ad, 0, sizeof(ad));
	ad.sin_family = AF_INET;
	ad.sin_addr = *(struct in_addr *)host->h_addr_list[0];
	ad.sin_port = htons(8784);

	if (connect(serv, (struct sockaddr *) &ad, sizeof(ad)) == 0 ) {
		printf("Connection established\n");
		setsockopt(serv, SOL_SOCKET, SO_REUSEADDR, &(int){1}, 4);
		int fl = fcntl(serv, F_GETFL, 0);
		fl |= O_NONBLOCK;
		fcntl(serv, F_SETFL, fl);
		char buff[MAX_SIZE];
		while (1) {
			if (recv(serv, buff, MAX_SIZE, 0) != 0) {
				struct PAK package = decodePak(buff);
				printf("\rServer> %s", package.data);
				memset(buff, 0 , MAX_SIZE);
				memset(package.data, 0, 1024);
			}	printf("Client> ");

			struct PAK package;
			fgets(buff, MAX_SIZE, stdin);
			memcpy(package.header, "MESS\0", 5);
			package.length = (unsigned short) strlen(buff);
			memcpy(package.data, buff, strlen(buff));
			send(serv, (char *)&package , sizeof(package), 0);
			memset(buff, 0, MAX_SIZE);
			memset(package.data, 0, 1024);

		}
	
	} else {
		printf("Connection error\n");
		exit(1);
	}

}
