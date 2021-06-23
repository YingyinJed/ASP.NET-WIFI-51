//Ӱ����(ZGT)
#ifndef _LCD12864_H_
#define _LCD12864_H_

#include <reg52.h>
#include <intrins.h>
#include <string.h>//��������ַ���ͷ�ļ�
#include <stdlib.h>//������������ͷ�ļ�

#include "delay.h"//�����õ����Զ������ʱ����

/*12864�˿ڶ���*/
#define LCD_data  P0             //���ݿ�
sbit LCD_RS  =  P1^0;          //�Ĵ���ѡ������
sbit LCD_RW  =  P1^1;          //Һ����/д����
sbit LCD_EN  =  P1^2;          //Һ��ʹ�ܿ���
sbit LCD_PSB =  P1^3;          //��/����ʽ����

#define LCD12864_DATA_PORT	 P0			// LCD12864��8λ���ݶ˿�

#define LCD_Line1 0x80  //LCD��ʾ�ĵ�һ��
#define LCD_Line2 0x90  //LCD��ʾ�ĵڶ���
#define LCD_Line3 0x88  //LCD��ʾ�ĵ�����
#define LCD_Line4 0x98  //LCD��ʾ�ĵ�����

void lcd12864_parallel_init();//��ʼ���������ֿ��LCD12864
void lcd12864_parallel_pos(unsigned char X, unsigned char Y);//��ʾ��λ
void lcd12864_parallel_string(unsigned char *str);//ͨ������ĺ�����λ����ʾ�ַ���
void lcd12864_Clean(void);//����
//void lcd12864_Drawing(unsigned char *Bmp);//��ʾͼƬ
//void LCD12864_DisplayOneLine(unsigned char position,unsigned char *p);//������λ����ʾ��������Ҫ������ʾ������Ϣ
void LCD12864_Display(unsigned char CleanFlag,unsigned char x,unsigned char y,unsigned char * DisplayWord);//Ϊ�˸���̵���������ʾ������Ĳ��ֺϲ�
#endif
