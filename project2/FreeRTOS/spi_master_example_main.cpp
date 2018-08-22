/* SPI Master example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "Adafruit_LSM9DS1.h"


const gpio_num_t LSM9DS1_SCK  = static_cast<gpio_num_t>(18);
const gpio_num_t LSM9DS1_MISO = static_cast<gpio_num_t>(19);
const gpio_num_t LSM9DS1_MOSI = static_cast<gpio_num_t>(23);
const gpio_num_t LSM9DS1_XGCS = static_cast<gpio_num_t>(5);
const gpio_num_t LSM9DS1_MCS  = static_cast<gpio_num_t>(15);

/*
 This code displays some fancy graphics on the 320x240 LCD on an ESP-WROVER_KIT board.
 This example demonstrates the use of both spi_device_transmit as well as
 spi_device_queue_trans/spi_device_get_trans_result and pre-transmit callbacks.

 Some info about the ILI9341/ST7789V: It has an C/D line, which is connected to a GPIO here. It expects this
 line to be low for a command and high for data. We use a pre-transmit callback here to control that
 line: every transaction has as the user-definable argument the needed state of the D/C line and just
 before the transaction is sent, the callback will set this line to the correct state.
*/

extern "C" void app_main()
{
    Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_SCK, LSM9DS1_MISO, LSM9DS1_MOSI, LSM9DS1_XGCS, LSM9DS1_MCS);

    // initialize the lsm
    if (!lsm.begin())
    {
        cout << "Oops ... unable to initialize the LSM9DS1. Check your wiring!" << endl;
        while (1)
            vTaskDelay(10);
    }else {
        cout << "Found LSM9DS1 9DOF" << endl;
    }

    while (1)
    {
        sensors_event_t accel, mag, gyro, temp;

        lsm.getEvent(&accel, &mag, &gyro, &temp);

        cout << "Accel X: " << accel.acceleration.x << ", Y: " 
             << accel.acceleration.y << ", Z: " << accel.acceleration.z << endl;
        cout << "Mag X: " << mag.magnetic.x << ", Y: "
             << mag.magnetic.y << ", Z: " << mag.magnetic.z << endl;
        cout << "Gyro X: " << gyro.gyro.x << ", Y: "
             << gyro.gyro.y << ", Z: " << gyro.gyro.z << endl;
        
        vTaskDelay(10);
    }
}
