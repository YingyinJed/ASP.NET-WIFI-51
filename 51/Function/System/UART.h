//Ӱ����(ZGT)
#ifndef _UART_H_
#define _UART_H_

#include <reg52.h>
#include <stdio.h>//����ʹ��Printf����
void Uart_init(void);//��ʼ����������
void UART_SendString(unsigned char DataLength,unsigned char *TXStr);//���ڷ����ַ�������
bit UART_ReceiveString(unsigned char DataLength,unsigned char * RecStr);//�жϽ����ַ�������

#endif
