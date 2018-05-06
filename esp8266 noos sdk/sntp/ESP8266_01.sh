#!/bin/bash

echo "gen_misc.sh version 20150511"
echo ""



echo ""

touch user/user_main.c

echo ""
echo "start..."
echo ""
make clean
#make COMPILE=gcc BOOT=$boot APP=$app SPI_SPEED=$spi_speed SPI_MODE=$spi_mode SPI_SIZE_MAP=$spi_size_map
make COMPILE=gcc BOOT=none APP=0 SPI_SPEED=40 SPI_MODE=DIO SPI_SIZE_MAP=2
