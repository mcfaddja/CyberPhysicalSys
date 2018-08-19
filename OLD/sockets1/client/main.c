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

    theData->device_id = "SCHOOL_ESP32_A";
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
    int buffSize = 2048;
    char buff[buffSize];

    char miniBuff[512];
    struct sockaddr_in serv_addr;

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }

    memset(buff, '\n', 1);
//    memset(buff, '0',sizeof(buff));
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


    strcat(buff, "{\r\n");

    snprintf(miniBuff, sizeof(miniBuff), "\t\"Device ID\" : \"%s\"\r\n", theData->device_id);
    strcat(buff, miniBuff);

    time_t ticks = theData->measurement_time;
    snprintf(miniBuff, sizeof(miniBuff), "\t\"Time\" : \"%.24s\"\r\n", ctime(&ticks));
    strcat(buff, miniBuff);

    snprintf(miniBuff, sizeof(miniBuff), "\t\"Sensor ID\" : \"%s\"\r\n", theData->sensor_id);
    strcat(buff, miniBuff);

    snprintf(miniBuff, sizeof(miniBuff), "\t\"gx\" : %f10\r\n", theData->gx);
    strcat(buff, miniBuff);

    snprintf(miniBuff, sizeof(miniBuff), "\t\"gy\" : %f10\r\n", theData->gy);
    strcat(buff, miniBuff);

    snprintf(miniBuff, sizeof(miniBuff), "\t\"gz\" : %f10\r\n", theData->gz);
    strcat(buff, miniBuff);

    snprintf(miniBuff, sizeof(miniBuff), "\t\"ax\" : %f10\r\n", theData->ax);
    strcat(buff, miniBuff);

    snprintf(miniBuff, sizeof(miniBuff), "\t\"ay\" : %f10\r\n", theData->ay);
    strcat(buff, miniBuff);

    snprintf(miniBuff, sizeof(miniBuff), "\t\"az\" : %f10\r\n", theData->az);
    strcat(buff, miniBuff);

    snprintf(miniBuff, sizeof(miniBuff), "\t\"mx\" : %f10\r\n", theData->mx);
    strcat(buff, miniBuff);

    snprintf(miniBuff, sizeof(miniBuff), "\t\"my\" : %f10\r\n", theData->my);
    strcat(buff, miniBuff);

    snprintf(miniBuff, sizeof(miniBuff), "\t\"mz\" : %f10\r\n", theData->mz);
    strcat(buff, miniBuff);

    snprintf(miniBuff, sizeof(miniBuff), "}\n");
    strcat(buff, miniBuff);

//    fprintf(stdout, buff);
//    fprintf(stdout, buff);
    int myLen = 0;
    myLen = strlen(buff);
    fprintf(stdout, "the length is %d\r\n", myLen);
    write(sockfd, buff, strlen(buff));


    if(n < 0)
    {
        printf("\n Read error \n");
    }

    return 0;
}

