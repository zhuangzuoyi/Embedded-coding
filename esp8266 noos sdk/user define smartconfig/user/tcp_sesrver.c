#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "mem.h"
#include "espconn.h"


static struct espconn *pTcpServer;


char SSID[15]= "";
char PASS[15]= "";


void wifiConnect_Cb(uint8_t status)
{
	os_printf("Connected to AP\r\n");
	//user_devicefind_init();
}


void Tcp_Server_Recv(void *arg, char *pdata, unsigned short len)
{
   uint8_t i=0;
   os_printf("received:\"%s\",%d\r\n",pdata,len);
   if(pdata[0] == 'S' && pdata[1] == 'S' && pdata[2] == 'I' && pdata[3] == 'D' && pdata[4] ==':')
   	{
   		for(i=0;i<len-3;i++)
			SSID[i] = pdata[5+i];
		os_printf("SSID:\"%s\"\r\n",SSID);
   	}else if(pdata[0] == 'P' && pdata[1] == 'A' && pdata[2] == 'S' && pdata[3] == 'S' && pdata[4] ==':')
	{
   		for(i=0;i<len-3;i++)
			PASS[i] = pdata[5+i];
		os_printf("PASS:\"%s\"\r\n",PASS);
	}else if(pdata[0] == 'S' && pdata[1] == 'T' && pdata[2] == 'A')
	{
		os_printf("change to sta mode\r\n");
		sta_init(SSID, PASS, wifiConnect_Cb);
	}
}

void Tcp_Server_Listen(void *arg)
{
    struct espconn *pespconn = (struct espconn *)arg;
    espconn_regist_recvcb(pespconn, Tcp_Server_Recv);                           //注册接收监听函数
    os_printf("connected callback\r\n");
}


void tcp_server_init(void)
{
        espconn_tcp_set_max_con(1);                                         //设置TCP连接的最大多少
        pTcpServer = (struct espconn *)os_zalloc(sizeof(struct espconn));
        pTcpServer->type = ESPCONN_TCP;                                     //TCP服务
        pTcpServer->state = ESPCONN_NONE;                                   //状态
        pTcpServer->proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
        pTcpServer->proto.tcp->local_port = 8888;                           //断开号
        espconn_regist_connectcb(pTcpServer, Tcp_Server_Listen);
        espconn_accept(pTcpServer);
        espconn_regist_time(pTcpServer, 180, 0);                            //设置超时断开时间 单位s
}



