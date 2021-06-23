//影隐劫(ZGT)
#include "MLX90614.h"
/*******************************************************************************
* 函 数 名       :MemRead()
* 函数功能       :读取红外体温传感器的温度数据
* 输    入       : 无
* 输    出       : 读取到的体温数据是浮点型
*******************************************************************************/
unsigned char MemRead(unsigned char * Data_Temp)
{
    //读AT24C02的字节    
	I2C_Start();
	I2C_Send_Byte(0x00);		//写入需要读的器件地址
	if(!Test_ACK())					//等待应答
	{	return (0);	}
	I2C_Send_Byte(0x07);			//从该地址开始读	
    if(!Test_ACK())					//等待应答
	{	return (0);	}    
	I2C_Start();
	I2C_Send_Byte(0x01);	//写入需要读的器件地址，这里+1是因为I2C的最后一位控制读写，为0是写为1是读
	Master_ACK(0);
	Data_Temp[0] = I2C_Read_Byte();//DataL
	Master_ACK(0);
    Data_Temp[1] = I2C_Read_Byte();//DataH
	Master_ACK(1);
    Data_Temp[2] = I2C_Read_Byte();//Pecreg
	I2C_Stop();    
	return (1);
}
float Temp_Float(unsigned char * Temp_Hex)
{
    float Temp;
    int Data;
    Data = Temp_Hex[1];
    Data <<= 8;
    Data |= Temp_Hex[0];
    Temp = Data*0.02-272.15;
    return Temp;
}