/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "osapi.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "driver/gpio16.h"
#include "driver/key.h"

#include "gpio.h"


#define LED_BIT   BIT5

#define key_id GPIO_ID_PIN(4)
#define key_func FUNC_GPIO4
#define key_pin_name  PERIPHS_IO_MUX_GPIO4_U

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


void led_init(void)
{
	// Initialize the GPIO subsystem.
	   gpio_init();
	   //Set LED GPIO to output mode
	   PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
	   //Set LED GPIO low  output
	   gpio_output_set(0, LED_BIT, LED_BIT, 0);
}


void key_down50ms_func(void)  
{  
    os_printf("key down 50ms!\n");  
    if (GPIO_REG_READ(GPIO_OUT_ADDRESS) & LED_BIT)
    {
        //Set GPIO1 to LOW
        gpio_output_set(0, LED_BIT, LED_BIT, 0);
    }
    else
    {
        //Set GPIO1 to HIGH
        gpio_output_set(LED_BIT, 0, LED_BIT, 0);
    }

}  
  
void key_down5s_func(void)  
{  
    os_printf("key down 5s!\n");  
}  


  


void user_key_init(void)
{
		LOCAL struct keys_param key;  
		LOCAL struct single_key_param *psingle_key;	
		psingle_key =  key_init_single(GPIO_ID_PIN(4),PERIPHS_IO_MUX_GPIO4_U,FUNC_GPIO4,key_down5s_func,key_down50ms_func);  
 
		key.key_num = 1;  
		key.single_key = &psingle_key;  
		key_init(&key);  
		PIN_PULLUP_EN(key_pin_name);
}

void ICACHE_FLASH_ATTR
user_init(void)
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_printf("  \r\n");
	os_printf("Hello ESP8266 GPIO\r\n");
	led_init();
	user_key_init();
}
