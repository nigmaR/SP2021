#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define BUF_SIZE 256

int main(int argc, char ** argv)
{
     int sock, newsock, port;
     char buf[BUF_SIZE];
     struct sockaddr_in serv_addr, cli_addr;
     sock = socket(AF_INET, SOCK_STREAM, 0);
     if (socket < 0)
     {
       printf("socket() failed: %d\n", errno);
       return EXIT_FAILURE;
     }
     memset((char *) &serv_addr, 0, sizeof(serv_addr));
     port = 15000;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(port);
     if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
     {
       printf("bind() failed: %d\n", errno);
       return EXIT_FAILURE;
     }
     listen(sock, 1);
     uint clen = sizeof(cli_addr);
     newsock = accept(sock, (struct sockaddr *) &cli_addr, &clen);
     if (newsock < 0) 
     {
       printf("accept() failed: %d\n", errno);
       return EXIT_FAILURE;
     }
     memset(buf, 0, BUF_SIZE);
     read(newsock, buf, BUF_SIZE-1);
     buf[BUF_SIZE] = 0;
     printf("Received MSG: %s\n", buf);
     char ansv[BUF_SIZE] = {'\0'};
     strcpy (ansv, "Another time!!");
     write(newsock, ansv, strlen(ansv));
     close(newsock);
     close(sock);
}
