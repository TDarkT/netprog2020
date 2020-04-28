#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
   struct hostent* ip = gethostbyname(argv[1]);
   struct in_addr ip_addr = *(struct in_addr*)(ip->h_addr);
   printf("%s",inet_ntoa(ip_addr));   
   return 0; 
}
