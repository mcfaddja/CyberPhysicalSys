//
// Created by Jonathan McFadden on 8/10/18.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
//#include <pthread.h>
//#include <stdint.h>

//#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, n = 0;
    struct sockaddr_in serv_addr;

    char buff[1025];
    time_t ticks;


    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buff, '0', sizeof(buff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);


    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);


    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        while( (n = read(connfd, buff, sizeof(buff)-1)) > 0)
        {
            buff[n] = 0;
            fputs(buff, stdout);
        }

	    puts("DONE receiving measurement!\n");

        close(connfd);
        sleep(1);
    }
}

