//Ӱ����(ZGT)
#ifndef _MLX90614_H_
#define _MLX90614_H_

#include "main.h"//�����������ԵĹ��ܾͲ��ض�Ϊ����ϵͳͷ�ļ���

#define Temp_MIN 34.5//��������Сֵ
#define Temp_MAX 37.5//���������ֵ

unsigned char MemRead(unsigned char * Data_Temp);//����I2C��ȡ�������´��������¶�����
float Temp_Float(unsigned char * Temp_Hex);

#endif
