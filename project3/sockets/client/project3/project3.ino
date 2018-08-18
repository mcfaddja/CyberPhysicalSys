
#include <WiFi.h>
#include <WiFiMulti.h>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>


WiFiMulti WiFiMulti;
LSM9DS1 imu;


#define LSM9DS1_M_CS  0
#define LSM9DS1_AG_CS 5

#define PRINT_CALCULATED
#define PRINT_SPEED 250

#define DECLINATION -8.58


typedef struct _imu_datapoint {
  char*   device_id;
  time_t  measurement_time;
  char*   sensor_id;
  float   dgx;
  float   dgy;
  float   dgz;
  float   dax;
  float   day;
  float   daz;
  float   dmx;
  float   dmy;
  float   dmz;
  float   cpitch;
  float   croll;
  float   cheading;
} imu_datapoint;

uint64_t chipid;
char idStr1[32];
char idStr2[32];
char idStr[64];



void setup()
{
  Serial.begin(115200);
  delay(10);





  WiFiMulti.addAP("McFaddenN", "$h()()tING4P()()p$");

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);


  Serial.println("Staring up IMU... ");

  imu.settings.device.commInterface = IMU_MODE_SPI;
  imu.settings.device.mAddress = LSM9DS1_M_CS;
  imu.settings.device.agAddress = LSM9DS1_AG_CS;

  if (!imu.begin())
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                   "work for an out of the box LSM9DS1 " \
                   "Breakout, but may need to be modified " \
                   "if the board jumpers are.");
    while (1)
      ;
  }

  Serial.println("IMU is up!!!");


  Serial.println("Beginning loop and data feed to server");

  delay(500);
}

void loop()
{
  Serial.println("Initializing data storage... ");

  int buffSize = 640;
  int miniBuffSize = 64;
  char buff[buffSize];
  char miniBuff[miniBuffSize];

  memset(buff, '\0', sizeof(buff));
  memset(miniBuff, '\0', sizeof(miniBuff));

  imu_datapoint* theData = (imu_datapoint*) malloc(sizeof(imu_datapoint));

  chipid = ESP.getEfuseMac();
  sprintf(idStr1, "%04X", (uint16_t)(chipid >> 32));
  sprintf(idStr2, "%08X", (uint32_t)chipid);
  memcpy(idStr, idStr1, strlen(idStr1) + 1);
  strcat(idStr, idStr2);
  //  Serial.printf("%s", idStr);
  //  Serial.println(" ");
  theData->device_id = idStr;

  theData->sensor_id = "1";

  Serial.println("Data storage successfully initialized!!!");


  

  Serial.println("Connected to server!!!");


  Serial.println("Getting IMU data...");

  Serial.println("\t getting gyro data...");
  imu.readGyro();
  theData->dgx = imu.calcGyro(imu.gx);
  theData->dgy = imu.calcGyro(imu.gy);
  theData->dgz = imu.calcGyro(imu.gz);

  Serial.println(theData->dgx, 2);
  Serial.println(imu.calcGyro(imu.gx), 2);
  Serial.println(theData->dgy, 2);
  Serial.println(imu.calcGyro(imu.gy), 2);
  Serial.println(theData->dgz, 2);
  Serial.println(imu.calcGyro(imu.gz), 2);

  Serial.println("\t getting acceleration data...");
  imu.readAccel();
  theData->dax = imu.calcAccel(imu.ax);
  theData->day = imu.calcAccel(imu.ay);
  theData->daz = imu.calcAccel(imu.az);

  Serial.println(theData->dax, 2);
  Serial.println(theData->day, 2);
  Serial.println(theData->daz, 2);

  Serial.println("\t getting magnetometer data...");
  imu.readMag();
  theData->dmx = imu.calcMag(imu.mx);
  theData->dmy = imu.calcMag(imu.my);
  theData->dmz = imu.calcMag(imu.mz);

  Serial.println(theData->dmx, 2);
  Serial.println(theData->dmy, 2);
  Serial.println(theData->dmz, 2);

  Serial.println("\t got the data!!!");


  //  float tmpHeading;
  //  if (theData->dmy == 0)
  //    tmpHeading = (theData->dmx < 0) ? PI : 0;
  //  else
  //    tmpHeading = atan2(theData->dmx, theData->dmy);
  //
  //  tmpHeading -= DECLINATION * PI / 180;
  //
  //  if (tmpHeading > PI) tmpHeading -= (2 * PI);
  //  else if (tmpHeading < -PI) tmpHeading += (2 * PI);
  //  else if (tmpHeading < 0) tmpHeading += 2 * PI;
  //
  //  tmpHeading *= 180.0 / PI;
  //
  //  theData->cheading = tmpHeading;
  //  theData->cpitch = atan2(-(theData->dax), sqrt((theData->day) * (theData->day) + (theData->daz) * (theData->daz))) * 180.0/PI;
  //  theData->croll = atan2(theData->day, theData->daz) * 180.0/PI;

//  theData->cheading = calcHeading(-(theData->dmx), -(theData->dmy));
//  theData->croll = calcRoll(theData->day, theData->daz);
//  theData->cpitch = calcPitch(theData->dax, theData->day, theData->daz);
//
//  Serial.print("heading: ");
//  Serial.print(theData->cheading, 2);
//  Serial.print(", \t pitch: ");
//  Serial.print(theData->cpitch, 2);
//  Serial.print(", \t roll: ");
//  Serial.print(theData->croll, 2);
//  Serial.println(" ");
//  printAttitude(theData->dax, theData->day, theData->daz, -theData->dmx, -theData->dmy, theData->dmz);


  memset(buff, '\n', 1);
  strcat(buff, "{\r\n");

  snprintf(miniBuff, sizeof(miniBuff), "\t\"Device ID\" : \"%s\"\r\n", theData->device_id);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "\t\"Sensor ID\" : \"%s\"\r\n", theData->sensor_id);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "\t\"gx\" : %f8\r\n", theData->dgx);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "\t\"gy\" : %f8\r\n", theData->dgy);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "\t\"gz\" : %f8\r\n", theData->dgz);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "\t\"ax\" : %f8\r\n", theData->dax);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "\t\"ay\" : %f8\r\n", theData->day);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "\t\"az\" : %f8\r\n", theData->daz);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "\t\"mx\" : %f8\r\n", theData->dmx);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "\t\"my\" : %f8\r\n", theData->dmy);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "\t\"mz\" : %f8\r\n", theData->dmz);
  strcat(buff, miniBuff);

//  snprintf(miniBuff, sizeof(miniBuff), "\t\"Pitch\" : %f8\r\n", theData->cpitch);
//  strcat(buff, miniBuff);
//
//  snprintf(miniBuff, sizeof(miniBuff), "\t\"Roll\" : %f8\r\n", theData->croll);
//  strcat(buff, miniBuff);
//
//  snprintf(miniBuff, sizeof(miniBuff), "\t\"Heading\" : %f8\r\n", theData->cheading);
//  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "}\r\n");
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "\r\n");
  strcat(buff, miniBuff);

  Serial.printf("%s", buff);
  Serial.printf("%d", strlen(buff));


  Serial.println("Starting connection to server... ");

  const uint16_t port = 5000;
  const char * host = "192.168.1.39";

  Serial.print("connecting to ");
  Serial.println(host);

  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }

  client.println(buff);
  client.println("\n connection DONE!!! \n \n");

  client.stop();
  

  delay(1001);
}

float calcHeading(float mx, float my) {
  float heading;

  if (my == 0)
    heading = (mx < 0) ? PI : 0;
  else
    heading = atan2(mx, my);

  heading -= DECLINATION * PI / 180;

  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;

  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;

  return heading;
}

float calcRoll(float ay, float az) {
  float roll = atan2(ay, az);
  roll  *= 180.0 / PI;

  return roll;
}

float calcPitch(float ax, float ay, float az) {
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));
  pitch *= 180.0 / PI;

  return pitch;
}


void printAttitude(float ax, float ay, float az, float mx, float my, float mz)
{
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));

  float heading;
  if (my == 0)
    heading = (mx < 0) ? PI : 0;
  else
    heading = atan2(mx, my);

  heading -= DECLINATION * PI / 180;

  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;

  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  pitch *= 180.0 / PI;
  roll  *= 180.0 / PI;

  Serial.print("Pitch, Roll: ");
  Serial.print(pitch, 2);
  Serial.print(", ");
  Serial.println(roll, 2);
  Serial.print("Heading: "); Serial.println(heading, 2);
}

















