//影隐劫(ZGT)
#ifndef _UART_H_
#define _UART_H_

#include <reg52.h>
#include <stdio.h>//用于使用Printf函数
void Uart_init(void);//初始化串口设置
void UART_SendString(unsigned char DataLength,unsigned char *TXStr);//串口发送字符串程序
bit UART_ReceiveString(unsigned char DataLength,unsigned char * RecStr);//中断接收字符串程序

#endif
