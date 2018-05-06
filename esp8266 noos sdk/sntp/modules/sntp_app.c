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

#include "sntp_app.h"


os_timer_t sntp_read_timer;  
char time_temp[30];

void ICACHE_FLASH_ATTR   sntp_read_timer_callback(void *arg)  
{  
		uint32_t time = sntp_get_current_timestamp();  
		ets_sprintf(time_temp,"%s",sntp_get_real_time(time));
		os_printf("date:%s",time_temp);	
} 






void ICACHE_FLASH_ATTR  user_sntp_init(void)  
{  
    sntp_setservername(0,"0.cn.pool.ntp.org");  
    sntp_setservername(1,"1.cn.pool.ntp.org");  
    sntp_setservername(2,"2.cn.pool.ntp.org");  
    sntp_init();  
  
    os_timer_disarm(&sntp_read_timer);  
    os_timer_setfn(&sntp_read_timer, sntp_read_timer_callback , NULL);  
    os_timer_arm(&sntp_read_timer,1000,1);  

	
} 



