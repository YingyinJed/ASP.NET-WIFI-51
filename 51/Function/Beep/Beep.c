//Ӱ����(ZGT)
#include "Beep.h"
/*******************************************************************************
* ������         :Beep_Start(unsigned char RingNum,unsigned char RingTime)
* ��������     	 :�÷�����������������
* ����           :RingNum��Ҫ��Ĵ�����ÿ����Ҫ���ʱ��
* ���           :��
*******************************************************************************/
void Beep_Start(unsigned int RingNum,unsigned char RingTime)
{
    unsigned char i;
    for(i = 0;i < RingNum;i++)
    {
        BEEP = 0;Delay_Ms_(RingTime);
        BEEP = 1;;Delay_Ms_(RingTime);
    }
}
/*******************************************************************************
* ������         :Beep_Start(unsigned char RingNum,unsigned char RingTime)
* ��������     	 :�÷�����������������
* ����           :LED_TYPE��Ҫ��ʾ��LED��RingNum��Ҫ��Ĵ�����RingTimeÿ����Ҫ���ʱ��
* ���           :��
*******************************************************************************/
//void LED_Start(unsigned char LED_TYPE,unsigned int RingNum,unsigned char RingTime)
//{
//    unsigned char i;
//    if(LED_TYPE == RED)
//    {
//        for(i = 0;i < RingNum;i++)
//        {
//            LED_RED = 0;Delay_Ms_(RingTime);
//            LED_RED = 1;;Delay_Ms_(RingTime);
//        }
//    }
//    if(LED_TYPE == GREEN)
//    {
//        for(i = 0;i < RingNum;i++)
//        {
//            LED_GREEN = 0;Delay_Ms_(RingTime);
//            LED_GREEN = 1;;Delay_Ms_(RingTime);
//        }
//    }    
//}
/*******************************************************************************
* ������         :Beep_LED_Start(unsigned char RingNum,unsigned char RingTime)
* ��������     	 :�÷�������LED������������
* ����           :RingNum��Ҫ��Ĵ�����ÿ����Ҫ���ʱ��
* ���           :��
*******************************************************************************/
//void Beep_LED_Start(unsigned int RingNum,unsigned char RingTime)
//{
//    unsigned char i;
//    for(i = 0;i < RingNum;i++)
//    {
//        BEEP = 0;LED_RED = 0;
//        Delay_Ms_(RingTime);
//        LED_RED = 1;BEEP = 1;
//        Delay_Ms_(RingTime);
//    }
//}