//影隐劫(ZGT)
#include "Beep.h"
/*******************************************************************************
* 函数名         :Beep_Start(unsigned char RingNum,unsigned char RingTime)
* 函数功能     	 :让蜂鸣器根据需求启动
* 输入           :RingNum需要响的次数，每次需要响的时间
* 输出           :无
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
* 函数名         :Beep_Start(unsigned char RingNum,unsigned char RingTime)
* 函数功能     	 :让蜂鸣器根据需求启动
* 输入           :LED_TYPE需要显示的LED，RingNum需要响的次数，RingTime每次需要响的时间
* 输出           :无
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
* 函数名         :Beep_LED_Start(unsigned char RingNum,unsigned char RingTime)
* 函数功能     	 :让蜂鸣器和LED根据需求启动
* 输入           :RingNum需要响的次数，每次需要响的时间
* 输出           :无
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