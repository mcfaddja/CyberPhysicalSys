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
#include <time.h>

#define DEVICE_ID "SCHOOL_ESP32_A"


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
    imu_datapoint* theData = (imu_datapoint*) malloc(sizeof(imu_datapoint));

    theData->device_id = DEVICE_ID;
    theData->measurement_time = time(NULL);
    theData->sensor_id = "1";
    theData->gx = (float) 0.0;
    theData->gy = (float) 0.0;
    theData->gz = (float) 0.0;
    theData->ax = (float) 0.0;
    theData->ay = (float) 0.0;
    theData->az = (float) 1.0;
    theData->mx = (float) 0.0;
    theData->my = (float) 0.0;
    theData->mz = (float) 0.0;


    int sockfd = 0, n = 0;
    int buffSize = 1024;
    char buff[1024];
    struct sockaddr_in serv_addr;

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }

    memset(buff, '0',sizeof(buff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }


    snprintf(buff, sizeof(buff), "%s\r\n", theData->device_id);
    write(sockfd, buff, strlen(buff));

    time_t ticks = theData->measurement_time;
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    write(sockfd, buff, strlen(buff));

    snprintf(buff, sizeof(buff), "%s\r\n", theData->sensor_id);
    write(sockfd, buff, strlen(buff));

    snprintf(buff, sizeof(buff), "gx: %f\r\n", theData->gx);
    write(sockfd, buff, strlen(buff));

    snprintf(buff, sizeof(buff), "gy: %f\r\n", theData->gy);
    write(sockfd, buff, strlen(buff));

    snprintf(buff, sizeof(buff), "gz: %f\r\n", theData->gz);
    write(sockfd, buff, strlen(buff));

    snprintf(buff, sizeof(buff), "ax: %f\r\n", theData->ax);
    write(sockfd, buff, strlen(buff));

    snprintf(buff, sizeof(buff), "ay: %f\r\n", theData->ay);
    write(sockfd, buff, strlen(buff));

    snprintf(buff, sizeof(buff), "az: %f\r\n", theData->az);
    write(sockfd, buff, strlen(buff));

    snprintf(buff, sizeof(buff), "mx: %f\r\n", theData->mx);
    write(sockfd, buff, strlen(buff));

    snprintf(buff, sizeof(buff), "my: %f\r\n", theData->my);
    write(sockfd, buff, strlen(buff));

    snprintf(buff, sizeof(buff), "mz: %f\r\n", theData->mz);
    write(sockfd, buff, strlen(buff));



//    while ( (n = read(sockfd, recvBuff, siøzeof(recvBuff)-1)) > 0)
//    {
//        recvBuff[n] = 0;
//        if(fputs(recvBuff, stdout) == EOF)
//        {
//            printf("\n Error : Fputs error\n");
//        }
//    }
//
    if(n < 0)
    {
        printf("\n Read error \n");
    }

    return 0;
}
