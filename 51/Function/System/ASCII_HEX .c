//Ӱ����(ZGT)
#include "ASCII_HEX.h"
/*******************************************************************************
* ������         :HexToChar(unsigned char bChar)
* ��������     	 :��16������ת��Ϊ��Ӧ��ASCII��
* ����           :��Ҫת����16������,����ת���õ�ASCII��
* ���           :��Ӧ��ASCII��
*******************************************************************************/
void HexToChar(unsigned char bChar,unsigned char * ASCII)
{
    ASCII[1] = bChar & 0x0F;//����λ
    ASCII[0]  = bChar | 0x0F;//����λ
    ASCII[0] = ASCII[0] >> 4;
           
    if(0x00 <= ASCII[0] && ASCII[0] < 0x0A )
    {
        ASCII[0] = ASCII[0] + 0x30;
    }//ʮ���������ֹʼ�48תΪ����
    else if(0x0A <= ASCII[0] && ASCII[0] < 0X10)
    {
        ASCII[0] = ASCII[0] + 0x37;
    }//ʮ�Ժ�ʮ��ĸ�ʼ�81
    
    if(0X00 <= ASCII[1] && ASCII[1] < 0X0A )
    {
        ASCII[1] = ASCII[1] + 0x30;
    }//ʮ���������ֹʼ�48תΪ����
    else if(0X0A <= ASCII[1] && ASCII[1] < 0X10)
    {
        ASCII[1] = ASCII[1] + 0x37;
    }//ʮ�Ժ�ʮ��ĸ�ʼ�81
}
