/*
 * wifi.c
 *
 *  Created on: Dec 30, 2014
 *      Author: Minh
 */
#include "wifi.h"
#include "user_interface.h"
#include "osapi.h"
#include "espconn.h"
#include "os_type.h"
#include "mem.h"
#include "user_config.h"
#include "config.h"

static ETSTimer WiFiLinker;
WifiCallback wifiCb = NULL;
static uint8_t wifiStatus = STATION_IDLE, lastWifiStatus = STATION_IDLE;
static void ICACHE_FLASH_ATTR sta_check_ip(void *arg)
{
	struct ip_info ipConfig;

	os_timer_disarm(&WiFiLinker);
	wifi_get_ip_info(STATION_IF, &ipConfig);
	wifiStatus = wifi_station_get_connect_status();
	if (wifiStatus == STATION_GOT_IP && ipConfig.ip.addr != 0)
	{

		os_timer_setfn(&WiFiLinker, (os_timer_func_t *)sta_check_ip, NULL);
		os_timer_arm(&WiFiLinker, 2000, 0);
	}
	else
	{
		if(wifi_station_get_connect_status() == STATION_WRONG_PASSWORD)
		{
			os_printf("STATION_WRONG_PASSWORD\r\n");
			wifi_station_connect();
		}
		else if(wifi_station_get_connect_status() == STATION_NO_AP_FOUND)
		{
			os_printf("STATION_NO_AP_FOUND\r\n");
			wifi_station_connect();
		}
		else if(wifi_station_get_connect_status() == STATION_CONNECT_FAIL)
		{

			os_printf("STATION_CONNECT_FAIL\r\n");
			wifi_station_connect();
		}
		else
		{
			os_printf("STATION_IDLE\r\n");
		}

		os_timer_setfn(&WiFiLinker, (os_timer_func_t *)sta_check_ip, NULL);
		os_timer_arm(&WiFiLinker, 500, 0);
	}
	if(wifiStatus != lastWifiStatus){
		lastWifiStatus = wifiStatus;
		if(wifiCb)
			wifiCb(wifiStatus);
	}
}

void ICACHE_FLASH_ATTR sta_init(uint8_t* ssid, uint8_t* pass, WifiCallback cb)
{
	struct station_config stationConf;
	bool is_ok;
	os_printf("WIFI_INIT\r\n");
	is_ok = wifi_set_opmode_current(STATION_MODE);
	os_printf("set sta mode:%d\r\n",is_ok);

	//wifi_station_set_auto_connect(FALSE);
	wifiCb = cb;

	os_memset(&stationConf, 0, sizeof(struct station_config));

	os_sprintf(stationConf.ssid, "%s", ssid);//ssid);
	os_sprintf(stationConf.password, "%s", pass);//pass);
	os_printf("SSID:%s,PASS:%s\r\n",stationConf.ssid,stationConf.password);
	//wifi_station_set_config_current(&stationConf);
	wifi_station_set_config(&stationConf);

	os_timer_disarm(&WiFiLinker);
	os_timer_setfn(&WiFiLinker, (os_timer_func_t *)sta_check_ip, NULL);
	os_timer_arm(&WiFiLinker, 1000, 0);

	//wifi_station_set_auto_connect(TRUE);
	wifi_station_connect();
}


void delete_ap(void)
{
	struct station_config stationConf;
	os_memset(&stationConf, 0, sizeof(struct station_config));
	wifi_station_set_config(&stationConf);
}
