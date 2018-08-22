#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h> // not used in this demo but required!
#define LSM9DS1_XGCS 5
#define LSM9DS1_MCS 0

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_XGCS, LSM9DS1_MCS);

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
  // Prints mag in formation "x,y,z" gauss
  Serial.print(m.magnetic.x); Serial.print(","); Serial.print(m.magnetic.y); Serial.print(","); Serial.print(m.magnetic.z);
  Serial.println();
  // Prints gyro in format "x,y,z" dps
  Serial.print(g.gyro.x); Serial.print(","); Serial.print(g.gyro.y); Serial.print(","); Serial.print(g.gyro.z);
  Serial.println();
  delay(200);
}
