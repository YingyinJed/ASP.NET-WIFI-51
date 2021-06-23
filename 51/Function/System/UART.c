//影隐劫（ZGT）
#include "UART.h"
/*******************************************************************************
* 函 数 名       :UsartInit()
* 函数功能       :设置串口
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/
void Uart_init(void)
{
 	TMOD |= 0x20;  	//T1工作模式2  8位自动重装
	TH1 = 0xfd;
	TL1 = 0xfd; 	//比特率9600
	TR1 = 1;		//启动T1定时器
	SM0 = 0;
	SM1 = 1; 		//串口工作方式1 10位异步
	REN = 1;		//串口允许接收
	EA  = 1;		//开总中断
	ES  = 1;		//串口中断打开
}
/*******************************************************************************
* 函 数 名       :UART_SendString()
* 函数功能       :串口发送字符串
* 输    入       :DataLength发送的字节长度,TXStr输入需要发送的字符串地址
* 输    出       :无
*******************************************************************************/
void UART_SendString(unsigned char DataLength,unsigned char *TXStr)  
{                
    ES=0;//由于发送的是字符串了就先关闭中断
    while(DataLength != 0) 
    {                      
        SBUF=*TXStr;
        while(!TI);//串口发完就会跳出该循环
        TI=0;//清除标志位
        TXStr++;//发送下一个字符
        DataLength--;
    }
    ES=1; //操作完成后打开
}


/*******************************************************************************
* 函 数 名       :UART_SendString()
* 函数功能       :串口发送字符串
* 输    入       :DataLength发送的字节长度,RecStr接收字符串的缓存区
* 输    出       :无
*******************************************************************************/
bit UART_ReceiveString(unsigned char DataLength,unsigned char * RecStr)    
{
    unsigned char * buf_string = RecStr;
    char num=0;
    unsigned char count=0;
    
    loop:    
    *buf_string=SBUF;//接收数据
    count=0;
    RI=0;    
    if(num < DataLength-1)  //根据数据包长度接收
    {
        num++;
        buf_string++;    
        while(!RI)
        {
            count++;
            if(count>130)return 0;//接收数据等待延迟，等待时间太久会导致CPU运算闲置，太短会出现"数据包被分割",默认count=130
        }
        goto loop;
    }
    return 1;
}
