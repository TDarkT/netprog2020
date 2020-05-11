#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <string.h>
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
		while(1) {
			char buffer[512];
			printf("Client> ");
			fflush(stdin);
			scanf("%s",buffer);
			send(serv, buffer, sizeof(buffer), 0);
			memset(buffer, 0, 512);
			if (recv(serv, buffer, 512, 0) == 0) {
				printf("Connection terminated");
				exit(1);
			}
			printf("Server> %s\n",buffer);			
			
		}
	} else {
		printf("Connection error\n");
		exit(1);
	}

}
