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
    bind(ss, (struct sockaddr *)&ad, ad_length);

    // then listen
    listen(ss, 0); 
    cli = accept(ss, (struct sockaddr *)&ad, &ad_length);
    if (cli == -1) {
    	printf("Oops! Something wrong");
	exit(1);
    } else {
	printf("Client Connected");
	}
    }
    // disconnect
    close(cli);
}

