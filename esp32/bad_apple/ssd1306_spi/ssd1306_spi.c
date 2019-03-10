#include <sys/time.h>
#include "disp_spi.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"



uint8_t SSD1306_GRAM[128][8];	

#define PIN_NUM_DC   21
#define PIN_NUM_RST  19

 void ssd1306_write_cmd(const uint8_t cmd)
{
    gpio_set_level(PIN_NUM_DC, 0);
    disp_spi_send(&cmd,1);
}


void ssd1306_write_data(const uint8_t *data, int len)
{
    gpio_set_level(PIN_NUM_DC, 1);
    disp_spi_send(data,len);
}


void ssd1306_refresh_gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		ssd1306_write_cmd (0xb0+i);   
		ssd1306_write_cmd (0x00);      
		ssd1306_write_cmd (0x10);      
		for(n=0;n<128;n++)
            ssd1306_write_data(&SSD1306_GRAM[n][i],1); 
	}   
}
void ssd1306_fill(void)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)
        for(n=0;n<128;n++)
            SSD1306_GRAM[n][i]=0Xff;  
	ssd1306_refresh_gram();
}

void show_img(char *img_data,int width,int height)
{
    uint8_t i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 128; j++)
           SSD1306_GRAM[j][i] =0x00;
    }

    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            SSD1306_GRAM[j][i] = img_data[i*width+j];
        }
    }
    ssd1306_refresh_gram();
}

void ssd1306_init(void)
{
    //Initialize non-SPI GPIOs
    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);
    // gpio_set_direction(PIN_NUM_BCKL, GPIO_MODE_OUTPUT);

    //Reset the display
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(100 / portTICK_RATE_MS);
    gpio_set_level(PIN_NUM_RST, 0);
    vTaskDelay(100 / portTICK_RATE_MS);
    gpio_set_level(PIN_NUM_RST, 1);
    vTaskDelay(100 / portTICK_RATE_MS);


	ssd1306_write_cmd(0xAE); 
	ssd1306_write_cmd(0xD5); 
	ssd1306_write_cmd(80);  
	ssd1306_write_cmd(0xA8);
	ssd1306_write_cmd(0X3F);
	ssd1306_write_cmd(0xD3); 
	ssd1306_write_cmd(0X00);

	ssd1306_write_cmd(0x40); 
													    
	ssd1306_write_cmd(0x8D); 
	ssd1306_write_cmd(0x14); 
	ssd1306_write_cmd(0x20); 
	ssd1306_write_cmd(0x02); 
	ssd1306_write_cmd(0xA1); 
	ssd1306_write_cmd(0xC0);
	ssd1306_write_cmd(0xDA); 
	ssd1306_write_cmd(0x12); 
		 
	ssd1306_write_cmd(0x81); 
	ssd1306_write_cmd(0xEF);
	ssd1306_write_cmd(0xD9); 
	ssd1306_write_cmd(0xf1); 
	ssd1306_write_cmd(0xDB); 
	ssd1306_write_cmd(0x30); 

	ssd1306_write_cmd(0xA4); 
	ssd1306_write_cmd(0xA6); 
	ssd1306_write_cmd(0xAF); 

    ssd1306_fill();
}