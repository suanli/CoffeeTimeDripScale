#!/bin/bash

:<<!
******NOTICE******
MUST set SDK_PATH & BIN_PATH firstly!!!
example:
export SDK_PATH=~/esp_iot_sdk_freertos
export BIN_PATH=~/esp8266_bin
!

export SDK_PATH=/Volumes/Project/ESP8266/CoffeeTimeDripScale/
export BIN_PATH=/Volumes/Project/ESP8266/CoffeeTimeDripScale/bin/

echo "gen_misc.sh version 2017/03/13"
echo ""
# new boot
boot=new 
# 0=eagle.flash.bin+eagle.irom0text.bin, 1=user1.bin, 2=user2.bin
app=1
# choose spi speed(0=20MHz, 1=26.7MHz, 2=40MHz, 3=80MHz)"
spi_speed=26.7
# choose spi mode(0=QIO, 1=QOUT, 2=DIO, 3=DOUT)"
spi_mode=DIO

# choose spi size and map"
# 0= 512KB( 256KB+ 256KB)"
# 2=1024KB( 512KB+ 512KB)"
# 3=2048KB( 512KB+ 512KB)"
# 4=4096KB( 512KB+ 512KB)"
# 5=2048KB(1024KB+1024KB)"
# 6=4096KB(1024KB+1024KB)"
spi_size_map=6

echo "start..."
echo ""

make clean

make BOOT=$boot APP=$app SPI_SPEED=$spi_speed SPI_MODE=$spi_mode SPI_SIZE_MAP=$spi_size_map
