/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define PIN_A	5
#define	PIN_B	16
#define PIN_C	17

void blink_task(void *pvParameter)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(PIN_A);
    gpio_pad_select_gpio(PIN_B);
    gpio_pad_select_gpio(PIN_C);

    /* Set the GPIO as a push/pull output */
    gpio_set_direction(PIN_A, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_B, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_C, GPIO_MODE_OUTPUT);
    while(1) {
        /* Blink off (output low) */
        gpio_set_level(PIN_A, 1);
        vTaskDelay(250 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(PIN_B, 1);
	gpio_set_level(PIN_A, 0);
        vTaskDelay(250 / portTICK_PERIOD_MS);
	gpio_set_level(PIN_C, 1);
        gpio_set_level(PIN_B, 0);
        vTaskDelay(250 / portTICK_PERIOD_MS);
	gpio_set_level(PIN_C, 0);
    }
}

void app_main()
{
    xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}
