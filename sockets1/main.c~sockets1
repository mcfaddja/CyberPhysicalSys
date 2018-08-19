//
// Created by Jonathan McFadden on 8/18/18.
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


int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, n = 0;
    struct sockaddr_in serv_addr;
    struct tm tm;

    char buff[2048];

//    FILE *dataFile;
//    char fileBuff[1025];


    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);


    puts("Listening on port 5000\n");

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);


    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);

        while ((n = read(connfd, buff, sizeof(buff) - 1)) > 0)
        {
            fprintf(stdout, buff);
        }

        puts("\n connection done!!! \n \n");

        close(connfd);
        sleep(1);

    }
}