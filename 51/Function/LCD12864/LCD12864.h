//影隐劫(ZGT)
#ifndef _LCD12864_H_
#define _LCD12864_H_

#include <reg52.h>
#include <intrins.h>
#include <string.h>//引入操作字符串头文件
#include <stdlib.h>//引入随机数相关头文件

#include "delay.h"//这里用到了自定义的延时函数

/*12864端口定义*/
#define LCD_data  P0             //数据口
sbit LCD_RS  =  P1^0;          //寄存器选择输入
sbit LCD_RW  =  P1^1;          //液晶读/写控制
sbit LCD_EN  =  P1^2;          //液晶使能控制
sbit LCD_PSB =  P1^3;          //串/并方式控制

#define LCD12864_DATA_PORT	 P0			// LCD12864的8位数据端口

#define LCD_Line1 0x80  //LCD显示的第一行
#define LCD_Line2 0x90  //LCD显示的第二行
#define LCD_Line3 0x88  //LCD显示的第三行
#define LCD_Line4 0x98  //LCD显示的第四行

void lcd12864_parallel_init();//初始化带中文字库的LCD12864
void lcd12864_parallel_pos(unsigned char X, unsigned char Y);//显示定位
void lcd12864_parallel_string(unsigned char *str);//通过上面的函数定位后显示字符串
void lcd12864_Clean(void);//清屏
//void lcd12864_Drawing(unsigned char *Bmp);//显示图片
//void LCD12864_DisplayOneLine(unsigned char position,unsigned char *p);//包括定位的显示函数，主要用来显示卡号信息
void LCD12864_Display(unsigned char CleanFlag,unsigned char x,unsigned char y,unsigned char * DisplayWord);//为了更简短的清屏并显示语句做的部分合并
#endif
