#ifndef __SSD1306_SPI_H__
#define __SSD1306_SPI_H__




#include "driver/gpio.h"
#include "disp_spi.h"
#include "stdint.h"

void ssd1306_init(void);
void show_img(char *img_data,int width,int height);

#endif

