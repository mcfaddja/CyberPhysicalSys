//
// Created by Jonathan McFadden on 8/10/18.
//

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    char buffer[128];
    struct sockaddr_in sin;
    struct hostent *host;
    int s = 0;

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }

    host = gethostbyname(argv[1]);

    memcpy(&(sin.sin_addr), host->h_addr,host->h_length);
    sin.sin_family = host->h_addrtype;
    sin.sin_port = htons(5000);

    s = socket(AF_INET, SOCK_STREAM, 0);
    connect(s, (struct sockaddr*)&sin, sizeof(sin));


    while (1)
    {
        fd_set fd;
        FD_ZERO(&fd);
        FD_SET(s, &fd);

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        int ret;

        if (select(s+1, &fd, NULL, NULL, &tv) > 0)
        {
            ret = recv(s, buffer, sizeof(buffer), 0);
            if (ret > 0)
                send(s, buffer, ret, 0);
        }
        else
        {
            ret = send(s, buffer, strlen(buffer), 0);
            if (ret > 0)
                recv(s, buffer, ret, 0);
        }
    }
}