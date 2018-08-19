//
// Created by Jonathan McFadden on 8/10/18.
//

//#include "../shared/error.h"
//#include "../shared/comm.h"
//#include "../shared/commands.h"
//#include "hashmap.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
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
    char buffer[128];
    struct sockaddr_in sin;
    int s = 0;

    memset(&sin, '0', sizeof(sin));
    metset(buffer, '0', sizeof(buffer));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
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

