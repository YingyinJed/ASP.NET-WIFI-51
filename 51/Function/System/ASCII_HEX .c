//影隐劫(ZGT)
#include "ASCII_HEX.h"
/*******************************************************************************
* 函数名         :HexToChar(unsigned char bChar)
* 函数功能     	 :将16进制数转化为对应的ASCII码
* 输入           :需要转化的16进制数,缓存转化好的ASCII码
* 输出           :对应的ASCII码
*******************************************************************************/
void HexToChar(unsigned char bChar,unsigned char * ASCII)
{
    ASCII[1] = bChar & 0x0F;//低四位
    ASCII[0]  = bChar | 0x0F;//高四位
    ASCII[0] = ASCII[0] >> 4;
           
    if(0x00 <= ASCII[0] && ASCII[0] < 0x0A )
    {
        ASCII[0] = ASCII[0] + 0x30;
    }//十以内是数字故加48转为数字
    else if(0x0A <= ASCII[0] && ASCII[0] < 0X10)
    {
        ASCII[0] = ASCII[0] + 0x37;
    }//十以后十字母故加81
    
    if(0X00 <= ASCII[1] && ASCII[1] < 0X0A )
    {
        ASCII[1] = ASCII[1] + 0x30;
    }//十以内是数字故加48转为数字
    else if(0X0A <= ASCII[1] && ASCII[1] < 0X10)
    {
        ASCII[1] = ASCII[1] + 0x37;
    }//十以后十字母故加81
}
