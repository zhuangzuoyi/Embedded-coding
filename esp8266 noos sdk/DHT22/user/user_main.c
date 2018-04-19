/* main.c -- MQTT client example
*
* Copyright (c) 2014-2015, Tuan PM <tuanpm at live dot com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of Redis nor the names of its contributors may be used
* to endorse or promote products derived from this software without
* specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/
#include "ets_sys.h"
#include "driver/uart.h"
#include "osapi.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"
#include "dht22.h"




#define DHT_NUMBER_OF_SENSORS 1

uint8_t pin_num[GPIO_PIN_NUM] = {16, 5, 4, 0,
								  2,  14,  12, 13,
								  15,  3,  1, 9,
								  10};



/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
 *******************************************************************************/
uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}


DHT_Sensor sensor;
#define DHT_NUMBER_OF_SENSORS 1
DHT_Sensor sensors[DHT_NUMBER_OF_SENSORS];
LOCAL os_timer_t dht22_timer;
#define DELAY 2000 /* milliseconds */

LOCAL void ICACHE_FLASH_ATTR dht22_cb(void *arg)
{
	static uint8_t i;
	DHT_Sensor_Data data;
	uint8_t pin;
	os_timer_disarm(&dht22_timer);

	// Two DHT22 sensors
	for (i = 0; i < DHT_NUMBER_OF_SENSORS; i++)
	{
	    pin = pin_num[sensors[i].pin];
		if (DHTRead(&sensors[i], &data))
		{ 
		    char buff[20];
		    os_printf("GPIO%d\r\n", pin);
		    os_printf("Temperature: %s *C\r\n", DHTFloat2String(buff, data.temperature));
		    os_printf("Humidity: %s %%\r\n", DHTFloat2String(buff, data.humidity));
		    os_printf("--------------------\r\n");
		} else {
		    os_printf("Failed to read temperature and humidity sensor on GPIO%d\n", pin);
		}
	}

	os_timer_arm(&dht22_timer, DELAY, 1);
}


void user_init(void)
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_delay_us(60000);
	os_printf("DHT staring\r\n");
	os_printf("Chip ID:%d\r\n",system_get_chip_id());


	sensors[0].pin = 3;
	sensors[0].type = DHT22;
	if (DHTInit(&sensors[0]))
	    os_printf("DHT22 #0 init on GPIO%d\r\n", pin_num[sensors[0].pin]);
	else
	    os_printf("Error init DHT22 #0 on GPIO%d\r\n", pin_num[sensors[0].pin]);

  

	os_printf("--------------------\r\n");
	os_timer_disarm(&dht22_timer);
	os_timer_setfn(&dht22_timer, (os_timer_func_t *)dht22_cb, (void *)0);
	os_timer_arm(&dht22_timer, DELAY, 1);

}
