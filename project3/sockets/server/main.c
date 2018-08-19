#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); }


int main(int argc, char *argv[])
{
    time_t timer;
    char buffer[26];
    struct tm* tm_info;

    int port = 5000;

    int listenfd = 0, connfd = 0, n = 0;
    struct sockaddr_in serv_addr;
    char buff[BUFFER_SIZE];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);


    puts("Listening on port 5000\n");

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);


    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    puts(buffer);


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


    return 0;
}