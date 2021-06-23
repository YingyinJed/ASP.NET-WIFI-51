//影隐劫(ZGT)
#ifndef _MAIN_H_
#define _MAIN_H_

#include <reg52.h>
#include <string.h>//引入操作字符串头文件
#include <stdlib.h>//引入随机数相关头文件

#include "BMP.h"
#include "RC522.h"
#include "Delay.h"
#include "UART.h"
#include "MLX90614.h"
#include "I2C.h"
#include "ASCII_HEX.h"
#include "Timer.h"
#include "LCD12864.h"
#include "Beep.h"

sbit SW1 = P3^2;
sbit SW2 = P3^3;

#endif
