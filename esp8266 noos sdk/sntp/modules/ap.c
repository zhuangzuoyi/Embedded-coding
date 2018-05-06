#include "ap.h"
#include "ets_sys.h"
#include "os_type.h"
#include "mem.h"
#include "osapi.h"
#include "user_interface.h"

#include "espconn.h"


void esp8266_app_soft_ap_init(void)
{
	struct softap_config config;  
	
	wifi_softap_get_config(&config);
	  
	os_memset(config.ssid, 0, 32);	
	os_memset(config.password, 0, 64);	
	os_memcpy(config.ssid, "esp8266", 7);  
	os_memcpy(config.password, "12345678", 8);	
	config.authmode = AUTH_OPEN;
	config.ssid_len = 0;// or its actual length  
	config.max_connection = 4; // how many stations can connect to ESP8266 softAP at most.	
	
	wifi_softap_set_config(&config);// Set ESP8266 softap config .	
}

