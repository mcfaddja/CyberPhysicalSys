#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h> 
#include "BluetoothSerial.h"

#define LSM9DS1_XGCS 5
#define LSM9DS1_MCS 0

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_XGCS, LSM9DS1_MCS);
BluetoothSerial SerialBT;

#define DECLINATION 15.35

void setupSensor()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);

  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  
  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
}

void setup()
{
  Serial.begin(115200);

  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  Serial.println("LSM9DS1 data read demo");

  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  
  Serial.println("Found LSM9DS1 9DOF");
  // helper to just set the default scaling we want, see above!
  setupSensor();
}
void loop()
{
  lsm.read(); /* ask it to read in the data */
  /* Get a new sensor event */
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);
  // Prints accel in the format "x,y,z" m/s^2
  Serial.print(a.acceleration.x); Serial.print(","); Serial.print(a.acceleration.y); Serial.print(","); Serial.print(a.acceleration.z);
  Serial.println();
  char aXbuff[10]; char aYbuff[10]; char aZbuff[10];
  snprintf(aXbuff, sizeof(aXbuff), "%f", a.acceleration.x);
  snprintf(aYbuff, sizeof(aYbuff), "%f", a.acceleration.y);
  snprintf(aZbuff, sizeof(aZbuff), "%f", a.acceleration.z);
  SerialBT.print(aXbuff); SerialBT.write(','); SerialBT.print(aYbuff); SerialBT.write(','); SerialBT.print(aZbuff); SerialBT.write(',');
  // Prints mag in formation "x,y,z" gauss
  char mXbuff[10]; char mYbuff[10]; char mZbuff[10]; 
  snprintf(mXbuff, sizeof(mXbuff), "%f", m.magnetic.x);
  snprintf(mYbuff, sizeof(mYbuff), "%f", m.magnetic.y);
  snprintf(mZbuff, sizeof(mZbuff), "%f", m.magnetic.z);
  SerialBT.print(mXbuff); SerialBT.write(','); SerialBT.print(mYbuff); SerialBT.write(','); SerialBT.print(mZbuff); SerialBT.write(',');
  Serial.print(m.magnetic.x); Serial.print(","); Serial.print(m.magnetic.y); Serial.print(","); Serial.print(m.magnetic.z);
  Serial.println();
  // Prints gyro in format "x,y,z" dps
  char gXbuff[10]; char gYbuff[10]; char gZbuff[10]; 
  snprintf(gXbuff, sizeof(gXbuff), "%f", g.gyro.x);
  snprintf(gYbuff, sizeof(gYbuff), "%f", g.gyro.y);
  snprintf(gZbuff, sizeof(gZbuff), "%f", g.gyro.z);
  SerialBT.print(gXbuff); SerialBT.write(','); SerialBT.print(gYbuff); SerialBT.write(','); SerialBT.print(gZbuff); SerialBT.write(',');
  Serial.print(g.gyro.x); Serial.print(","); Serial.print(g.gyro.y); Serial.print(","); Serial.print(g.gyro.z);
  Serial.println();
  delay(200);
}
