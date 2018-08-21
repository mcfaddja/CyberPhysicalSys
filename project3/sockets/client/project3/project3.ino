
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


typedef struct imu_datapoint {
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
};

uint64_t chipid;
char idStr1[32];
char idStr2[32];
char idStr[64];



void setup()
{
  Serial.begin(115200);
  delay(10);

  connectWiFi();
  
  connectIMU();

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

  struct imu_datapoint theData;// = (imu_datapoint*) malloc(sizeof(imu_datapoint));
  chipid = ESP.getEfuseMac();
  sprintf(idStr1, "%04X", (uint16_t)(chipid >> 32));
  sprintf(idStr2, "%08X", (uint32_t)chipid);
  memcpy(idStr, idStr1, strlen(idStr1) + 1);
  strcat(idStr, idStr2);
  theData.device_id = idStr;

  theData.sensor_id = "1";
//  getChipID(theData);
  
  Serial.println("Data storage successfully initialized!!!");


  Serial.println("Getting IMU data...");
  getGyroData(theData);
  getAccelData(theData);
  getMagData(theData);
  Serial.println("got ALL the data!!!");


//  memset(buff, '\n', 1);
//  strcat(buff, "{\r\n");

  snprintf(miniBuff, sizeof(miniBuff), "%s\r\n", theData.device_id);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "%s\r\n", theData.sensor_id);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "%f\r\n", theData.dgx);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "%f\r\n", theData.dgy);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "%f\r\n", theData.dgz);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "%f\r\n", theData.dax);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "%f\r\n", theData.day);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "%f\r\n", theData.daz);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "%f\r\n", theData.dmx);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "%f\r\n", theData.dmy);
  strcat(buff, miniBuff);

  snprintf(miniBuff, sizeof(miniBuff), "%f\r\n", theData.dmz);
  strcat(buff, miniBuff);

//  snprintf(miniBuff, sizeof(miniBuff), "}\r\n");
//  strcat(buff, miniBuff);
//
//  snprintf(miniBuff, sizeof(miniBuff), "\r\n");
//  strcat(buff, miniBuff);

  Serial.printf("%s\n", buff);
  Serial.printf("%d\n\n", strlen(buff));


  Serial.println("Starting connection to server... ");

  const uint16_t port = 5000;
  const char * host = "192.168.1.40";

  Serial.print("connecting to ");
  Serial.println(host);

  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(2500);
    return;
  }

  Serial.println("Connected to server!!!");
  Serial.println("sending data...");

  client.println(buff);
  Serial.println("data sent, closing connection...");
  
//  client.println("\n transmission DONE!!! \n \n");
  client.stop();
  Serial.println("connection closed!!!");


  delay(250);
}


void connectWiFi() {
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
}


void connectIMU() {
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
}

void getChipID(struct imu_datapoint &theData) {
  chipid = ESP.getEfuseMac();
  sprintf(idStr1, "%04X", (uint16_t)(chipid >> 32));
  sprintf(idStr2, "%08X", (uint32_t)chipid);
  memcpy(idStr, idStr1, strlen(idStr1) + 1);
  strcat(idStr, idStr2);
  theData.device_id = idStr;

  theData.sensor_id = "1";
}

void getGyroData(struct imu_datapoint &theData) {
  Serial.println("\t getting gyro data...");
  imu.readGyro();
  theData.dgx = imu.calcGyro(imu.gx);
  theData.dgy = imu.calcGyro(imu.gy);
  theData.dgz = imu.calcGyro(imu.gz);

  Serial.println(theData.dgx, 2);
  Serial.println(imu.calcGyro(imu.gx), 2);
  Serial.println(theData.dgy, 2);
  Serial.println(imu.calcGyro(imu.gy), 2);
  Serial.println(theData.dgz, 2);
  Serial.println(imu.calcGyro(imu.gz), 2);
}

void getAccelData(struct imu_datapoint &theData) {
  Serial.println("\t getting acceleration data...");
  imu.readAccel();
  theData.dax = imu.calcAccel(imu.ax);
  theData.day = imu.calcAccel(imu.ay);
  theData.daz = imu.calcAccel(imu.az);

  Serial.println(theData.dax, 2);
  Serial.println(theData.day, 2);
  Serial.println(theData.daz, 2);
}

void getMagData(struct imu_datapoint &theData) {
  Serial.println("\t getting magnetometer data...");
  imu.readMag();
  theData.dmx = imu.calcMag(imu.mx);
  theData.dmy = imu.calcMag(imu.my);
  theData.dmz = imu.calcMag(imu.mz);

  Serial.println(theData.dmx, 2);
  Serial.println(theData.dmy, 2);
  Serial.println(theData.dmz, 2);
}

