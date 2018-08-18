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

    int cnt = 0;

    char buff[1025];
//    time_t ticks;

    imu_datapoint* theData = (imu_datapoint*) malloc(sizeof(imu_datapoint));

    FILE *dataFile;
    char fileBuff[1025];

//
//    if(dataFile==NLU)
//    {
//        puts("ERROR opening file");
//        return 0;
//    }
//    else
//    {
//        puts("File open successful, configuring network stuff");

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buff, '0', sizeof(buff));
    memset(fileBuff, '0', sizeof(fileBuff));

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
            buff[n] = 0;
//            fputs(buff, stdout);
            fprintf(stdout, buff);
            fprintf(stdout, "\n%i\n", n);

            switch(cnt) {
                case 0:
                    theData->device_id = buff;
                    fprintf(stdout, "This is the buffer: \n %s", buff);
//                    fprintf(stdout, "This is the device id: \n %s". theData->device_id);
                    break;
                case 1:

//                    const char *time_details = buff;
//                    struct tm tm;
                    strptime(buff, "%a %m %d %H:%M:%S %Y", &tm);
                    theData->measurement_time = mktime(&tm);
                    fprintf(stdout, "This is the buffer: \n %s", buff);
//                    time_t ticks1 = theData->measurement_time;
//                    fprintf(stdout, "This is the time: \n %.24s". ctime(&ticks1));
                    break;
                case 2:
                    theData->sensor_id = buff;
                    fprintf(stdout, "This is the buffer: \n %s", buff);
//                    fprintf(stdout, "This is the sensor id: \n %s", theData->sensor_id);
                    break;
                case 3:
                    theData->gx = atof(buff);
                    fprintf(stdout, "This is the buffer: \n %s", buff);
                    break;
                case 4:
                    theData->gy = atof(buff);
                    fprintf(stdout, "This is the buffer: \n %s", buff);
                    break;
                case 5:
                    theData->gz = atof(buff);
                    fprintf(stdout, "This is the buffer: \n %s", buff);
                    break;
                case 6:
                    theData->ax = atof(buff);
                    fprintf(stdout, "This is the buffer: \n %s", buff);
                    break;
                case 7:
                    theData->ay = atof(buff);
                    fprintf(stdout, "This is the buffer: \n %s", buff);
                    break;
                case 8:
                    theData->az = atof(buff);
                    fprintf(stdout, "This is the buffer: \n %s", buff);
                    break;
                case 9:
                    theData->mx = atof(buff);
                    fprintf(stdout, "This is the buffer: \n %s", buff);
                    break;
                case 10:
                    theData->my = atof(buff);
                    fprintf(stdout, "This is the buffer: \n %s", buff);
                    break;
                case 11:
                    theData->mz = atof(buff);
                    fprintf(stdout, "This is the buffer: \n %s", buff);
                    break;
            }

            puts("DONE receiving measurement!\n");

            fprintf(stdout, "\n%i\n", cnt);
            cnt++;
            if(cnt==12) {
                cnt = 0;
            }

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

            snprintf(fileBuff, sizeof(fileBuff), "{\n");
            fprintf(dataFile, fileBuff);

            snprintf(fileBuff, sizeof(fileBuff), "\t\"Device ID\" : \"%s\"\r\n", theData->device_id);
            fprintf(dataFile, fileBuff);

            time_t ticks2 = theData->measurement_time;
            snprintf(fileBuff, sizeof(fileBuff), "\t\"Time\" : \"%.24s\"\r\n", ctime(&ticks2));
            fprintf(dataFile, fileBuff);

            snprintf(fileBuff, sizeof(fileBuff), "\t\"Sensor ID\" : \"%s\r\"\n", theData->sensor_id);
            fprintf(dataFile, fileBuff);

            snprintf(fileBuff, sizeof(fileBuff), "\t\"gx\" : %f\r\n", theData->gx);
            fprintf(dataFile, fileBuff);

            snprintf(fileBuff, sizeof(fileBuff), "\t\"gy\" : %f\r\n", theData->gy);
            fprintf(dataFile, fileBuff);

            snprintf(fileBuff, sizeof(fileBuff), "\t\"gz\" : %f\r\n", theData->gz);
            fprintf(dataFile, fileBuff);

            snprintf(fileBuff, sizeof(fileBuff), "\t\"ax\" : %f\r\n", theData->ax);
            fprintf(dataFile, fileBuff);

            snprintf(fileBuff, sizeof(fileBuff), "\t\"ay\" : %f\r\n", theData->ay);
            fprintf(dataFile, fileBuff);

            snprintf(fileBuff, sizeof(fileBuff), "\t\"az\" : %f\r\n", theData->az);
            fprintf(dataFile, fileBuff);

            snprintf(fileBuff, sizeof(fileBuff), "\t\"mx\" : %f\r\n", theData->mx);
            fprintf(dataFile, fileBuff);

            snprintf(fileBuff, sizeof(fileBuff), "\t\"my\" : %f\r\n", theData->my);
            fprintf(dataFile, fileBuff);

            snprintf(fileBuff, sizeof(fileBuff), "\t\"mz\" : %f\r\n", theData->mz);
            fprintf(dataFile, fileBuff);

            snprintf(fileBuff, sizeof(fileBuff), "}\n");
            fprintf(dataFile, fileBuff);
        }

        fclose(dataFile);
        puts("DONE writing measurement to FILE!\n");

        close(connfd);
        sleep(1);
    }



}

