/***************************************************************************
  This is a library for the LSM9DS1 Accelerometer and magnentometer/compass

  Designed specifically to work with the Adafruit LSM9DS1 Breakouts

  These sensors use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include "Adafruit_LSM9DS1.h"


/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/


Adafruit_LSM9DS1::Adafruit_LSM9DS1( int32_t sensorID ) {
    initSPI();
}



Adafruit_LSM9DS1::Adafruit_LSM9DS1(gpio_num_t sclk, gpio_num_t smiso, gpio_num_t smosi, gpio_num_t xgcs, gpio_num_t mcs, int32_t sensorID ) {
  _csm = mcs;
  _csxg = xgcs;
  _mosi = smosi;
  _miso = smiso;
  _clk = sclk;
  _lsm9dso_sensorid_accel = sensorID + 1;
  _lsm9dso_sensorid_mag = sensorID + 2;
  _lsm9dso_sensorid_gyro = sensorID + 3;
  _lsm9dso_sensorid_temp = sensorID + 4;
  _accelSensor = Sensor(this, &Adafruit_LSM9DS1::readAccel, &Adafruit_LSM9DS1::getAccelEvent, &Adafruit_LSM9DS1::getAccelSensor);
  _magSensor   = Sensor(this, &Adafruit_LSM9DS1::readMag,   &Adafruit_LSM9DS1::getMagEvent,   &Adafruit_LSM9DS1::getMagSensor);
  _gyroSensor  = Sensor(this, &Adafruit_LSM9DS1::readGyro,  &Adafruit_LSM9DS1::getGyroEvent,  &Adafruit_LSM9DS1::getGyroSensor);
  _tempSensor  = Sensor(this, &Adafruit_LSM9DS1::readTemp,  &Adafruit_LSM9DS1::getTempEvent,  &Adafruit_LSM9DS1::getTempSensor);
  initSPI();
}

void Adafruit_LSM9DS1::initSPI()
{
    spi_bus_config_t              busCfg = {};
    spi_device_interface_config_t xgDevCfg = {};
    spi_device_interface_config_t mDevCfg = {};
    esp_err_t                     error;

    busCfg.miso_io_num     = _miso;
    busCfg.mosi_io_num     = _mosi;
    busCfg.sclk_io_num     = _clk;
    busCfg.quadwp_io_num   = -1;
    busCfg.quadhd_io_num   = -1;
    busCfg.max_transfer_sz = 0;
    busCfg.flags           = 0;

    xgDevCfg.clock_speed_hz = 10 * 1000 * 1000;
    xgDevCfg.mode           = 0;
    xgDevCfg.spics_io_num   = _csxg;
    xgDevCfg.queue_size     = 1;

    mDevCfg.clock_speed_hz  = 10 * 1000 * 1000;
    mDevCfg.mode            = 0;
    mDevCfg.spics_io_num    = _csm;
    mDevCfg.queue_size      = 1;

    //Initialize the SPI bus
    error = spi_bus_initialize(VSPI_HOST, &busCfg, 1);
    assert(error == ESP_OK);

    //Attach the lsm to the SPI bus
    error = spi_bus_add_device(VSPI_HOST, &xgDevCfg, &xgDevice);
    assert(error == ESP_OK);

    error = spi_bus_add_device(VSPI_HOST, &mDevCfg, &mDevice);
    assert(error == ESP_OK);
}

void Adafruit_LSM9DS1::test()
{
    byte REG1G = read8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG1_G);
    cout << "REG1_G value is " << bitset<8>(REG1G) << endl;

    byte REG5XL = read8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG5_XL);
    cout << "REG5_XL value is " << bitset<8>(REG5XL) << endl;

    byte REG6XL = read8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG6_XL);
    cout << "REG6_XL value is " << bitset<8>(REG6XL) << endl;

    byte REG3M = read8(MAGTYPE, LSM9DS1_REGISTER_CTRL_REG3_M);
    cout << "REG3_M value is " << bitset<8>(REG3M) << endl;
}

bool Adafruit_LSM9DS1::begin()
{
    // soft reset & reboot accel/gyro
    write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG8, 0x05);

    // soft reset & reboot magnetometer
    write8(MAGTYPE, LSM9DS1_REGISTER_CTRL_REG2_M, 0x0C);

    vTaskDelay(10);


    byte id = read8(XGTYPE, LSM9DS1_REGISTER_WHO_AM_I_XG);
    //Serial.print ("XG whoami: 0x"); Serial.println(id, HEX);
    if (id != LSM9DS1_XG_ID)
        return false;

    id = read8(MAGTYPE, LSM9DS1_REGISTER_WHO_AM_I_M);
    //Serial.print ("MAG whoami: 0x"); Serial.println(id, HEX);
    if (id != LSM9DS1_MAG_ID)
        return false;
    

    // enable gyro continuous
    write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG1_G, 0xC0); // on XYZ

    // Enable the accelerometer continous
    write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG5_XL, 0x38); // enable X Y and Z axis
    write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG6_XL, 0xC0); // 1 KHz out data rate, BW set by ODR, 408Hz anti-aliasing

    // enable mag continuous
    write8(MAGTYPE, LSM9DS1_REGISTER_CTRL_REG3_M, 0x00); // continuous mode

    // Set default ranges for the various sensors  
    setupAccel(LSM9DS1_ACCELRANGE_2G);
    setupMag(LSM9DS1_MAGGAIN_4GAUSS);
    setupGyro(LSM9DS1_GYROSCALE_245DPS);

  return true;
}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/
void Adafruit_LSM9DS1::read()
{
  /* Read all the sensors. */
  readAccel();
  readMag();
  readGyro();
  readTemp();
}

void Adafruit_LSM9DS1::readAccel() {
  // Read the accelerometer
  byte buffer[6];
  readBuffer(XGTYPE, 
       LSM9DS1_REGISTER_OUT_X_L_XL, 
       6, buffer);
  
  uint8_t xlo = buffer[0];
  int16_t xhi = buffer[1];
  uint8_t ylo = buffer[2];
  int16_t yhi = buffer[3];
  uint8_t zlo = buffer[4];
  int16_t zhi = buffer[5];
  
  // Shift values to create properly formed integer (low byte first)
  xhi <<= 8; xhi |= xlo;
  yhi <<= 8; yhi |= ylo;
  zhi <<= 8; zhi |= zlo;
  accelData.x = xhi;
  accelData.y = yhi;
  accelData.z = zhi;

  cout << "In readAccel, X value is " << xhi << ", Y value is " << yhi << ", and Z value is " << zhi << endl;
}

void Adafruit_LSM9DS1::readMag() {
  // Read the magnetometer
  byte buffer[6];
  readBuffer(MAGTYPE, 
       LSM9DS1_REGISTER_OUT_X_L_M, 
       6, buffer);
  
  uint8_t xlo = buffer[0];
  int16_t xhi = buffer[1];
  uint8_t ylo = buffer[2];
  int16_t yhi = buffer[3];
  uint8_t zlo = buffer[4];
  int16_t zhi = buffer[5];
  
  // Shift values to create properly formed integer (low byte first)
  xhi <<= 8; xhi |= xlo;
  yhi <<= 8; yhi |= ylo;
  zhi <<= 8; zhi |= zlo;
  magData.x = xhi;
  magData.y = yhi;
  magData.z = zhi;

  cout << "In readMag, X value is " << xhi << ", Y value is " << yhi << ", and Z value is " << zhi << endl;
}

void Adafruit_LSM9DS1::readGyro() {
  // Read gyro
  byte buffer[6];
  readBuffer(XGTYPE, 
       LSM9DS1_REGISTER_OUT_X_L_G, 
       6, buffer);
  
  uint8_t xlo = buffer[0];
  int16_t xhi = buffer[1];
  uint8_t ylo = buffer[2];
  int16_t yhi = buffer[3];
  uint8_t zlo = buffer[4];
  int16_t zhi = buffer[5];
  
  // Shift values to create properly formed integer (low byte first)
  xhi <<= 8; xhi |= xlo;
  yhi <<= 8; yhi |= ylo;
  zhi <<= 8; zhi |= zlo;
  
  gyroData.x = xhi;
  gyroData.y = yhi;
  gyroData.z = zhi;

  cout << "In readGyro, X value is " << xhi << ", Y value is " << yhi << ", and Z value is " << zhi << endl << endl;
}

void Adafruit_LSM9DS1::readTemp() {
  // Read temp sensor
  byte buffer[2];
  readBuffer(XGTYPE, 
       LSM9DS1_REGISTER_TEMP_OUT_L, 
       2, buffer);
  uint8_t xlo = buffer[0];
  int16_t xhi = buffer[1];

  xhi <<= 8; xhi |= xlo;
  
  // Shift values to create properly formed integer (low byte first)
  temperature = xhi;
}

void Adafruit_LSM9DS1::setupAccel ( lsm9ds1AccelRange_t range )
{
  uint8_t reg = read8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG6_XL);
  reg &= ~(0b00011000);
  reg |= range;
  //Serial.println("set range: ");
  write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG6_XL, reg );
  
  switch (range)
  {
    case LSM9DS1_ACCELRANGE_2G:
      _accel_mg_lsb = LSM9DS1_ACCEL_MG_LSB_2G;
      break;
    case LSM9DS1_ACCELRANGE_4G:
      _accel_mg_lsb = LSM9DS1_ACCEL_MG_LSB_4G;
      break;
    case LSM9DS1_ACCELRANGE_8G:
      _accel_mg_lsb = LSM9DS1_ACCEL_MG_LSB_8G;
      break;    
    case LSM9DS1_ACCELRANGE_16G:
      _accel_mg_lsb =LSM9DS1_ACCEL_MG_LSB_16G;
      break;
  }
}

void Adafruit_LSM9DS1::setupMag ( lsm9ds1MagGain_t gain )
{
  uint8_t reg = read8(MAGTYPE, LSM9DS1_REGISTER_CTRL_REG2_M);
  reg &= ~(0b01100000);
  reg |= gain;
  write8(MAGTYPE, LSM9DS1_REGISTER_CTRL_REG2_M, reg );

  switch(gain)
  {
    case LSM9DS1_MAGGAIN_4GAUSS:
      _mag_mgauss_lsb = LSM9DS1_MAG_MGAUSS_4GAUSS;
      break;
    case LSM9DS1_MAGGAIN_8GAUSS:
      _mag_mgauss_lsb = LSM9DS1_MAG_MGAUSS_8GAUSS;
      break;
    case LSM9DS1_MAGGAIN_12GAUSS:
      _mag_mgauss_lsb = LSM9DS1_MAG_MGAUSS_12GAUSS;
      break;
    case LSM9DS1_MAGGAIN_16GAUSS:
      _mag_mgauss_lsb = LSM9DS1_MAG_MGAUSS_16GAUSS;
      break;
  }
}

void Adafruit_LSM9DS1::setupGyro ( lsm9ds1GyroScale_t scale )
{
  uint8_t reg = read8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG1_G);
  reg &= ~(0b00011000);
  reg |= scale;
  write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG1_G, reg );

  switch(scale)
  {
    case LSM9DS1_GYROSCALE_245DPS:
      _gyro_dps_digit = LSM9DS1_GYRO_DPS_DIGIT_245DPS;
      break;
    case LSM9DS1_GYROSCALE_500DPS:
      _gyro_dps_digit = LSM9DS1_GYRO_DPS_DIGIT_500DPS;
      break;
    case LSM9DS1_GYROSCALE_2000DPS:
      _gyro_dps_digit = LSM9DS1_GYRO_DPS_DIGIT_2000DPS;
      break;
  }
}


/***************************************************************************
 UNIFIED SENSOR FUNCTIONS
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Gets the most recent accel sensor event
*/
/**************************************************************************/
bool Adafruit_LSM9DS1::getEvent(sensors_event_t *accelEvent,
                                sensors_event_t *magEvent,
                                sensors_event_t *gyroEvent,
                                sensors_event_t *tempEvent )
{
  /* Grab new sensor reading and timestamp. */
  read();
  uint32_t timestamp = static_cast<uint32_t>(xTaskGetTickCount());

  /* Update appropriate sensor events. */
  if (accelEvent) getAccelEvent(accelEvent, timestamp);
  if (magEvent)   getMagEvent(magEvent, timestamp);
  if (gyroEvent)  getGyroEvent(gyroEvent, timestamp);
  if (tempEvent)  getTempEvent(tempEvent, timestamp);
  
  return true;
}

/**************************************************************************/
/*!
    @brief  Gets the sensor_t data
*/
/**************************************************************************/
void Adafruit_LSM9DS1::getSensor(sensor_t *accel, sensor_t *mag,
                                 sensor_t *gyro, sensor_t *temp )
{
  /* Update appropriate sensor metadata. */
  if (accel) getAccelSensor(accel);
  if (mag)   getMagSensor(mag);
  if (gyro)  getGyroSensor(gyro);
  if (temp)  getTempSensor(temp);
}

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/
void Adafruit_LSM9DS1::write8(boolean type, byte reg, byte value)
{
  digitalWrite(type, reg, value);
}

byte Adafruit_LSM9DS1::read8(boolean type, byte reg)
{
    uint8_t value;

    readBuffer(type, reg, 1, &value);

    return value;
}

byte Adafruit_LSM9DS1::readBuffer(boolean type, byte reg, byte len, uint8_t *buffer)
{
    digitalRead(type, reg, len, buffer);

  return len;
}

esp_err_t Adafruit_LSM9DS1::digitalRead(boolean type, uint8_t reg, byte len, uint8_t *buffer) {
    spi_device_handle_t dev = {};
    spi_transaction_t   t   = {};
    esp_err_t           err = ESP_OK;

    byte cmd      = reg;
    char *dmaData = nullptr;

    cmd |= 0x80;

    if (type == MAGTYPE)
    {
        dev = mDevice;
        cmd |= 0x40;
    }else {
        dev = xgDevice;
    }
    
    t.flags     = SPI_TRANS_USE_RXDATA;
    t.length    = (len + 1) * 8;
    t.rxlength  = len * 8;
    t.user      = NULL;
    t.tx_buffer = &cmd;

    if (len > 3)
    {
        dmaData = static_cast<char*>(heap_caps_malloc(len + 1, MALLOC_CAP_DMA));
        memset(dmaData, 0, len + 1);

        t.flags     = 0;
        t.rx_buffer = dmaData;
    }

    err = spi_device_transmit(dev, &t);

    if (err != ESP_OK)
    {
        printf("SPI RX error: %d\n", err);
        goto cleanup;
    }else {
        memcpy(buffer, (len > 3 ? ((byte*)t.rx_buffer + 1) : (t.rx_data + 1)), len);
    }
    
    cleanup:
    if (dmaData)
        heap_caps_free(dmaData);

    return err;
}

esp_err_t Adafruit_LSM9DS1::digitalWrite(boolean type, uint8_t reg, byte value) {
    spi_device_handle_t dev = {};
    spi_transaction_t   t   = {};
    esp_err_t           err = ESP_OK;

    byte cmd = reg;
    cmd &= 0x7F;
    
    if (type == MAGTYPE)
        dev = mDevice;
    else
        dev = xgDevice;

    t.flags     = SPI_TRANS_USE_TXDATA;
    t.length    = sizeof(word) * 8;
    t.user      = NULL;
    t.tx_data[0] = cmd;
    t.tx_data[1] = value;
    t.tx_data[2] = 0;
    t.tx_data[3] = 0;

    err = spi_device_transmit(dev, &t);

    return err;
}

void Adafruit_LSM9DS1::getAccelEvent(sensors_event_t* event, uint32_t timestamp) {
  memset(event, 0, sizeof(sensors_event_t));
  event->version   = sizeof(sensors_event_t);
  event->sensor_id = _lsm9dso_sensorid_accel;
  event->type      = SENSOR_TYPE_ACCELEROMETER;
  event->timestamp = timestamp;
  event->acceleration.x = accelData.x * _accel_mg_lsb;
  event->acceleration.x /= 1000;
  event->acceleration.x *= SENSORS_GRAVITY_STANDARD;
  event->acceleration.y = accelData.y * _accel_mg_lsb;
  event->acceleration.y /= 1000;
  event->acceleration.y *= SENSORS_GRAVITY_STANDARD;
  event->acceleration.z = accelData.z * _accel_mg_lsb;
  event->acceleration.z /= 1000;
  event->acceleration.z *= SENSORS_GRAVITY_STANDARD;
}

void Adafruit_LSM9DS1::getMagEvent(sensors_event_t* event, uint32_t timestamp) {
  memset(event, 0, sizeof(sensors_event_t));
  event->version   = sizeof(sensors_event_t);
  event->sensor_id = _lsm9dso_sensorid_mag;
  event->type      = SENSOR_TYPE_MAGNETIC_FIELD;
  event->timestamp = timestamp;
  event->magnetic.x = magData.x * _mag_mgauss_lsb;
  event->magnetic.x /= 1000;
  event->magnetic.y = magData.y * _mag_mgauss_lsb;
  event->magnetic.y /= 1000;
  event->magnetic.z = magData.z * _mag_mgauss_lsb;
  event->magnetic.z /= 1000;
}

void Adafruit_LSM9DS1::getGyroEvent(sensors_event_t* event, uint32_t timestamp) {
  memset(event, 0, sizeof(sensors_event_t));
  event->version   = sizeof(sensors_event_t);
  event->sensor_id = _lsm9dso_sensorid_accel;
  event->type      = SENSOR_TYPE_GYROSCOPE;
  event->timestamp = timestamp;
  event->gyro.x = gyroData.x * _gyro_dps_digit;
  event->gyro.y = gyroData.y * _gyro_dps_digit;
  event->gyro.z = gyroData.z * _gyro_dps_digit;
}

void Adafruit_LSM9DS1::getTempEvent(sensors_event_t* event, uint32_t timestamp) {
  memset(event, 0, sizeof(sensors_event_t));
  event->version   = sizeof(sensors_event_t);
  event->sensor_id = _lsm9dso_sensorid_temp;
  event->type      = SENSOR_TYPE_AMBIENT_TEMPERATURE;
  event->timestamp = timestamp;
  event->temperature = 21.0 + (float)temperature/8;
}

void Adafruit_LSM9DS1::getAccelSensor(sensor_t* sensor) {
  memset(sensor, 0, sizeof(sensor_t));
  strncpy (sensor->name, "LSM9DS1_A", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _lsm9dso_sensorid_accel;
  sensor->type        = SENSOR_TYPE_ACCELEROMETER;
  sensor->min_delay   = 0;
  sensor->max_value   = 0.0;  // ToDo
  sensor->min_value   = 0.0;  // ToDo
  sensor->resolution  = 0.0;  // ToDo
}

void Adafruit_LSM9DS1::getMagSensor(sensor_t* sensor) {
  memset(sensor, 0, sizeof(sensor_t));
  strncpy (sensor->name, "LSM9DS1_M", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _lsm9dso_sensorid_mag;
  sensor->type        = SENSOR_TYPE_MAGNETIC_FIELD;
  sensor->min_delay   = 0;
  sensor->max_value   = 0.0;  // ToDo
  sensor->min_value   = 0.0;  // ToDo
  sensor->resolution  = 0.0;  // ToDo
}

void Adafruit_LSM9DS1::getGyroSensor(sensor_t* sensor) {
  memset(sensor, 0, sizeof(sensor_t));
  strncpy (sensor->name, "LSM9DS1_G", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _lsm9dso_sensorid_gyro;
  sensor->type        = SENSOR_TYPE_GYROSCOPE;
  sensor->min_delay   = 0;
  sensor->max_value   = 0.0;  // ToDo
  sensor->min_value   = 0.0;  // ToDo
  sensor->resolution  = 0.0;  // ToDo
}

void Adafruit_LSM9DS1::getTempSensor(sensor_t* sensor) {
  memset(sensor, 0, sizeof(sensor_t));
  strncpy (sensor->name, "LSM9DS1_T", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _lsm9dso_sensorid_temp;
  sensor->type        = SENSOR_TYPE_AMBIENT_TEMPERATURE;
  sensor->min_delay   = 0;
  sensor->max_value   = 0.0;  // ToDo
  sensor->min_value   = 0.0;  // ToDo
  sensor->resolution  = 0.0;  // ToDo
}

