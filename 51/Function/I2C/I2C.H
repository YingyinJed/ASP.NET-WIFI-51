//影隐劫（ZGT）
#ifndef _I2C_H
#define _I2C_H

#include <reg52.h>
#include <intrins.h>

#define AT24C02_ADDR 0xa0			//定义实验板中AT24C02地址即EEPROM外部内存的地址
#define PCF8591_ADDR 0x90			//定义实验板中PCF8591地址即模数转换的地址
#define DACOUT_EN	 0x40			//DAC输出使能

#ifndef Main_Fosc
#define Main_Fosc	 11059200UL		//定义主时钟的HZ							
#endif

sbit SDA = P2^0;	//这一个数据线和一个时钟线根据实际电路设置
sbit SCL = P2^1;	//实验板中采用的是P2^0和P2^1

//void Delay_5us(void);										//延时5us用于I2C协议中的各种延时
//void I2C_Init(void);										//I2C初始化
void I2C_Start(void);										//I2C起始信号
void I2C_Stop(void);										//I2C终止信号
void Master_ACK(bit i);										//主机应答信号
bit	 Test_ACK(void);										//返回从机应答信号
void I2C_Send_Byte(unsigned char byte);						//主机通过I2C发送一个字节
unsigned char I2C_Read_Byte(void);							//主机通过I2C读取一个字节
//bit I2C_TransmitData(unsigned char Slave_ADDR,ADDR,DATA);	//发送一个字节数据到特定的地址(这个解释使用于AT24C02，PCF8591要另说)
//unsigned char I2C_ReceiveData(unsigned char Slave_ADDR,ADDR);//主机接收一个特定从设备地址的数据(同上)
//int I2C_ReceiveData1(unsigned char Slave_ADDR,ADDR);		//和上面的函数一样不过它是用整型存储数据
/*-----------------------------------------------------------------
最后两个子函数的用法比较特殊具体要参考对应的需要对话的I2C设备的手册
例如原//的注释是AT24C02的注释而PCF8591有所不同
	I2C_TransmitData(PCF8591_ADDR,DACOUT_EN,100);//在PCF8591中是第二个参数是模拟量输出使能，其它解释与AT24C02一样
	I2C_ReceiveData(PCF8591_ADDR,ADDR);			 //在PCF8591中第二个参数是控制字节，其它与AT24C02一样
*/
#endif
