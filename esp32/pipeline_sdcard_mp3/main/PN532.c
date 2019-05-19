#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "audio_element.h"
#include "audio_event_iface.h"
#include "i2s_stream.h"
#include "mp3_decoder.h"
#include "esp_peripherals.h"
#include "periph_sdcard.h"
#include "ringbuf.h"
#include "board.h"
#include "driver/uart.h"
#include "PN532.h"



static const char *TAG = "PN532:";



#define ECHO_TEST_TXD  (GPIO_NUM_22)
#define ECHO_TEST_RXD  (GPIO_NUM_19)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE (1024)


void pn532_uart_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);
}

void pn532_wake_normal(void)
{
    uint8_t data[30];
    uint8_t cmd[]={0x55,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x03,0xFD,0xD4,0x14,0x01,0x17,0x00};
    uart_write_bytes(UART_NUM_1,(const char *)cmd, sizeof(cmd));
    uart_read_bytes(UART_NUM_1, data, BUF_SIZE, 500 / portTICK_RATE_MS);
}


#define CARD_MAX  7
uint8_t card_id [] [4] =
{
    {0xd2,0x34,0x3f,0x96},
    {0xaf,0x59,0x29,0x04},
    {0x3c,0x4f,0x51,0x04},
    {0x22,0xe1,0x72,0x1b},
    {0x52,0x9d,0x1c,0x47},
    {0xa0,0xfe,0x9b,0xa5},
    {0xb0,0x55,0x35,0xba}
};

int pn532_req(void)
{
    
    uint8_t data[30];
    uint8_t i=0,checksum=0;
    uint8_t cmd[]={0x00,0x00,0xff,0x04,0xfc,0xd4,0x4a,0x02,0x00,0xe0,0x00};
    uart_write_bytes(UART_NUM_1,(const char *)cmd, sizeof(cmd));
    uint8_t len = uart_read_bytes(UART_NUM_1, data, BUF_SIZE, 500 / portTICK_RATE_MS);
    
    if(len > 11 && data[0] == 0x00 && data[1] == 0x00 && data[2] == 0xff && data[3] == 0x00  && data[4] == 0xff && data[5] == 0x00 && data[6] == 0x00 && data[7] == 0x00 && data[8] == 0xff)
    {
        checksum = data[9] + data[10];
        if((checksum == 0x00) && ((data[9] + 13) == len))
        {
            
            for(i=11;i<len-1;i++)
                checksum +=data[i];
            if(checksum ==0)
            {
                //id start 19
                printf("Card UID is:%2x,%2x,%2x,%2x\r\n",data[19],data[20],data[21],data[22]);
                for(i=0;i<CARD_MAX;i++)
                {
                    if(data[19] == card_id[i][0] && data[20] == card_id[i][1] && data[21] == card_id[i][2] && data[22] == card_id[i][3] )
                        return i;
                }
            }else
            {
                printf("Checksum faile\r\n");
            }
            
        }
    }
    return -1;
}