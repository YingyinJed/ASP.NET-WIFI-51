//影隐劫(ZGT)
#ifndef _MLX90614_H_
#define _MLX90614_H_

#include "main.h"//由于是依赖性的功能就不特定为它引系统头文件了

#define Temp_MIN 34.5//定义检测最小值
#define Temp_MAX 37.5//定义检测最大值

unsigned char MemRead(unsigned char * Data_Temp);//利用I2C读取红外体温传感器的温度数据
float Temp_Float(unsigned char * Temp_Hex);

#endif
