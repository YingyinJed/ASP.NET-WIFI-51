//影隐劫(ZGT)
#ifndef _BEEP_H_
#define _BEEP_H_

#include <reg52.h>

#include "Delay.h"

#define RED     0
#define GREEN   1

sbit LED_RED = P1^5;
sbit LED_GREEN = P1^6;
sbit BEEP = P1^4;

void Beep_Start(unsigned int RingNum,unsigned char RingTime);//让蜂鸣器根据需求启动
//void Beep_LED_Start(unsigned int RingNum,unsigned char RingTime);//让蜂鸣器和LED根据需求启动
//void LED_Start(unsigned char LED_TYPE,unsigned int RingNum,unsigned char RingTime);//让LED根据需求启动
#endif