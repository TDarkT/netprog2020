#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
   if (argc == 1)
   {
   	printf("Usage: %s <url>\n", argv[0]);
	exit(1);
   }
   struct hostent* ip = gethostbyname(argv[1]);
   if (ip == NULL)
   {
   	printf("Error resolve url\n");
        exit(1);	
   }
   struct in_addr ip_addr = *(struct in_addr*)(ip->h_addr);
   printf("%s: %s\n",argv[1],inet_ntoa(ip_addr));   
   return 0; 
}
