/* tcp_perf Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include <sys/socket.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "tcp_client.h"
#include "ssd1306_spi.h"

#define TAG "TCP_CLIENT:"


/* FreeRTOS event group to signal when we are connected to wifi */


/*socket*/
static struct sockaddr_in server_addr;
static int connect_socket = 0;



//receive data
int img_width=0,img_height=0;
void recv_data(void *pvParameters)
{
    int len = 0;
    char databuff[1024];

    while (1)
    {
        memset(databuff, 0x00, sizeof(databuff));
        len = recv(connect_socket, databuff, sizeof(databuff), 0);
        if (len > 0)
        {
            if(databuff[0] == 0xaa && len == 3)
            {
                img_width  = databuff[1];
                img_height = databuff[2];
                send(connect_socket, "GET_SIZE:OK", sizeof("GET_SIZE:OK"), 0);
            }else if(databuff[0] == 0xab)
            {
               show_img(&databuff[1],img_width,img_height);
                send(connect_socket, "GET_DAT:OK", sizeof("GET_DAT:OK"), 0);
            }
            
        }
        else
        {
            show_socket_error_reason("recv_data", connect_socket);
        }
    }

    close_socket();

    vTaskDelete(NULL);
}


esp_err_t create_tcp_client()
{

    ESP_LOGI(TAG, "will connect gateway ssid : %s port:%d\n",
             TCP_SERVER_ADRESS, TCP_PORT);

    connect_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (connect_socket < 0)
    {
        show_socket_error_reason("create client", connect_socket);
        return ESP_FAIL;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TCP_PORT);
    server_addr.sin_addr.s_addr = inet_addr(TCP_SERVER_ADRESS);
    ESP_LOGI(TAG, "connectting server...");
    if (connect(connect_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        show_socket_error_reason("client connect", connect_socket);
        ESP_LOGE(TAG, "connect failed!");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "connect success!");
    return ESP_OK;
}



int get_socket_error_code(int socket)
{
    int result;
    u32_t optlen = sizeof(int);
    int err = getsockopt(socket, SOL_SOCKET, SO_ERROR, &result, &optlen);
    if (err == -1)
    {
        ESP_LOGE(TAG, "getsockopt failed:%s", strerror(err));
        return -1;
    }
    return result;
}

int show_socket_error_reason(const char *str, int socket)
{
    int err = get_socket_error_code(socket);

    if (err != 0)
    {
        ESP_LOGW(TAG, "%s socket error %d %s", str, err, strerror(err));
    }

    return err;
}

int check_working_socket()
{
    int ret;

    ESP_LOGD(TAG, "check connect_socket");
    ret = get_socket_error_code(connect_socket);
    if (ret != 0)
    {
        ESP_LOGW(TAG, "connect socket error %d %s", ret, strerror(ret));
    }
    if (ret != 0)
    {
        return ret;
    }
    return 0;
}

void close_socket()
{
    close(connect_socket);
}
