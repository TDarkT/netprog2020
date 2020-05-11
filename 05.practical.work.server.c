#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
    	while(1) {
		char buffer[512];
		if (recv(cli, buffer, 512, 0) == 0) {
			printf("Connection terminated");
			exit(1);
		}
		printf("Client> %s\n", buffer);
		printf("Server> ");
		memset(buffer, 0 ,512);
		scanf("%s", buffer);
	 	send(cli, buffer, sizeof(buffer), 0);	
		
	}
    }	

    // disconnect
    close(cli);
}

