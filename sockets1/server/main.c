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


typedef struct _imu_datapoint{
    char*   device_id;
    time_t  measurement_time;
    char*   sensor_id;
    float   gx;
    float   gy;
    float   gz;
    float   ax;
    float   ay;
    float   az;
    float   mx;
    float   my;
    float   mz;
} imu_datapoint;


int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, n = 0;
    struct sockaddr_in serv_addr;
    struct tm tm;

    char buff[2048];

    imu_datapoint* theData = (imu_datapoint*) malloc(sizeof(imu_datapoint));

    FILE *dataFile;
    char fileBuff[1025];


    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
//    memset(buff, ' ', sizeof(buff));
//    memset(fileBuff, ' ', sizeof(fileBuff));
//    memset(buff, '\n', 1);
//    memset(fileBuff, '\n', 1);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);


    puts("Listening on port 5000\n");

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);



    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);


        while( (n = read(connfd, buff, sizeof(buff)-1)) > 0)
        {
//            fileBuff = buff;
	    int myLen = 0;
            myLen = strlen(buff);
	    fprintf(stdout, "the buffer length is : %d\r\n", myLen);
            strcpy(fileBuff, buff);
            fprintf(stdout, buff);
        }

        dataFile = fopen("dataFile.txt", "a+");
        if(dataFile==NULL)
        {
            puts("ERROR opening file");
            break;
        }
        else
        {
            fseek(dataFile, 0, SEEK_END);

            fprintf(dataFile, fileBuff);
        }

        fclose(dataFile);
        puts("DONE writing measurement to FILE!\n");

        close(connfd);
        sleep(1);
    }



}


