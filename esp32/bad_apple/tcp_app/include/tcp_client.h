
#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__



#ifdef __cplusplus
extern "C" {
#endif

//要连接TCP服务器地址
#define TCP_SERVER_ADRESS "192.168.1.101" 


//统一的端口号，包括TCP客户端或者服务端
#define TCP_PORT 8266

//create a tcp client socket. return ESP_OK:success ESP_FAIL:error
esp_err_t create_tcp_client();

//receive data task
void recv_data(void *pvParameters);

//close all socket
void close_socket();

//get socket error code. return: error code
int get_socket_error_code(int socket);

//show socket error code. return: error code
int show_socket_error_reason(const char* str, int socket);

//check working socket
int check_working_socket();


#ifdef __cplusplus
}
#endif


#endif /*#ifndef __TCP_PERF_H__*/

