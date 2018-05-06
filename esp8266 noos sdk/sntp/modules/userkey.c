#include "ets_sys.h"
#include "driver/uart.h"
#include "osapi.h"
#include "wifi.h"
#include "config.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"
#include "ap.h"
#include "Tcp_sesrver.h"
#include "Userkey.h"
#include "driver/key.h"
#include "gpio.h"
#include "wifi.h"



#define key_pin_name  PERIPHS_IO_MUX_MTMS_U
#define key_pin_func  FUNC_GPIO14
#define key_pin_num   14


void key_down5s_func(void)
{      
	delete_ap();
	os_printf("reset ap\r\n");
}  




void user_key_init(void)
{
    LOCAL struct keys_param key;  
    LOCAL struct single_key_param *psingle_key[1];  
	
    psingle_key[0] =  key_init_single(GPIO_ID_PIN(key_pin_num),key_pin_name,key_pin_func,key_down5s_func,NULL);   
    key.key_num = 1;  
    key.single_key = &psingle_key[0];  
    key_init(&key);  
   PIN_PULLUP_EN(key_pin_name); 

}


