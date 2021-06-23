//Ӱ����(ZGE)https://blog.csdn.net/pawn331/article/details/94977844?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522161841611616780261959982%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=161841611616780261959982&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-1-94977844.pc_search_result_before_js&utm_term=LCD12864%E5%B8%A6%E4%B8%AD%E6%96%87%E5%AD%97%E5%BA%93
#include "LCD12864.h"
/*-------------------- �����ǵײ�ʱ���� --------------------*/
/*********************************************************************
* �� �� ��       : lcd12864_parallel_busy
* ��������		 : ���LCDæ״̬��lcd12864_parallel_busyΪ1ʱ��æ���ȴ���lcd12864_parallel_busyΪ0ʱ,�У���дָ�������ݡ�
* �����б�       : ��
* �������    	 : ���1ʱΪæ����֮Ϊ��
*********************************************************************/
bit lcd12864_parallel_busy() {
    bit result;
    LCD_RS = 0; //ѡ������ģʽ
    LCD_RW = 1; //ѡ���ȡ
    LCD_EN = 1; //����ʹ��
    _nop_();_nop_();_nop_();_nop_();//������ʱ
    result = (bit)(P0 & 0x80);//�������ŵ����λ�洢æ��״̬
    LCD_EN = 0; //�رղ���ʹ��
    return(result);
}

/*********************************************************************
* �� �� ��       : lcd12864_parallel_write_cmd
* ��������		 : ����LCD12864�Ͳ�ʱ����LCD�ڲ�д��8λ������
* �����б�       : cmd - ��д���8λ������
* �������    	 : ��
*********************************************************************/
void lcd12864_parallel_write_cmd(unsigned char cmd) {
    while(lcd12864_parallel_busy());//��LCD��ʱ�Ų���

    LCD_RS = 0; //ѡ������ģʽ
    LCD_RW = 0; //ѡ���ȡ
    LCD_EN = 0; //�ȹر�ʹ�ܣ�������д���ʹ��
    _nop_();
    _nop_();
    P0 = cmd; //������ӳ�䵽P0�˿�
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    LCD_EN = 1; //��ʹ���øո�ӳ�������д��LCD
    _nop_();_nop_();_nop_();_nop_();//�ȴ�ʱ��������д������ݼ�¼Ϊ��Ч����
    LCD_EN = 0; //�رղ���ʹ��
}

/*********************************************************************
* �� �� ��       : lcd12864_parallel_write_data
* ��������		 : ����LCD12864�Ͳ�ʱ����LCD�ڲ�д��8λ����
* �����б�       : dat - ��д���8λ������
* �������    	 : ��
*********************************************************************/
void lcd12864_parallel_write_data(unsigned char dat) {
    while(lcd12864_parallel_busy());

    LCD_RS = 1; //ѡ������ģʽ
    LCD_RW = 0; //ѡ��д��ģʽ
    LCD_EN = 0; //����Ĳ�����д������ʱһ��
    P0 = dat;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    LCD_EN = 1;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    LCD_EN = 0;
}

/*-------------------- �����ǵײ�ʱ�����������Ǹ߲�ʱ���� --------------------*/
/*********************************************************************
* �� �� ��       : lcd12864_parallel_init()
* ��������		 : ����LCD12864�Ͳ�ʱ����г�ʼ������
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
void lcd12864_parallel_init() {
    LCD_PSB = 1;         //���ڷ�ʽ
    lcd12864_parallel_write_cmd(0x34);      //����ָ����� ����λ 0011������8λ���еķ�ʽ�������� ����λ��������ָ�
    Delay_Ms_(5);
    lcd12864_parallel_write_cmd(0x30);      //�������ֽ�����ָ���Ϊ�˻���ָ�����
    Delay_Ms_(5);
    lcd12864_parallel_write_cmd(0x0C);      //��ʾ�����ع��
    Delay_Ms_(5);
    lcd12864_parallel_write_cmd(0x01);      //���LCD����ʾ����
    Delay_Ms_(5);
}

/*********************************************************************
* �� �� ��       : lcd12864_parallel_pos
* ��������		 : LCD��ʾ������
* �����б�       : X,Y��x - �������꣬��Χ��0-7
*				   y - �������꣬��Χ��0-3
* �������    	 : ��
*********************************************************************/
void lcd12864_parallel_pos(unsigned char X, unsigned char Y) {
    unsigned char  pos;

    if(X == 0) {//��һ�еĵ�ַ��80H
        X = 0x80;
    } else if(X == 1) {//�ڶ��еĵ�ַ��90H
        X = 0x90;
    } else if(X == 2) {//�����еĵ�ַ��88H
        X = 0x88;
    } else if(X == 3) {//�����еĵ�ַ��98H
        X = 0x98;
    }

    pos = X + Y;
    lcd12864_parallel_write_cmd(pos);//��ʾ��ַ
}

/*********************************************************************
* �� �� ��       : lcd12864_parallel_string(unsigned char *str)
* ��������		 : ����lcd12864_parallel_pos��λ��ʼ��ʾ�ַ�
* �����б�       : str - ָ�����ʾ���ַ�����ָ��
* �������    	 : ��
*********************************************************************/
void lcd12864_parallel_string(unsigned char *str) {
    while(*str > 0) {
        lcd12864_parallel_write_data(*str);
        str++;
        Delay_Ms_(1);
    }
}
/*********************************************************************
* �� �� ��       : lcd12864_Clean(void)
* ��������		 : ����
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
void lcd12864_Clean(void)
{
    lcd12864_parallel_write_cmd(0x01);      //���LCD����ʾ����
    Delay_Ms_(5);
}
/*********************************************************************
* �� �� ��       : lcd12864_Drawing(unsigned char *data)
* ��������		 : ����lcd12864_parallel_pos��λ��ʼ��ʾ�ַ�
* �����б�       : str - ָ�����ʾ���ַ�����ָ��
* �������    	 : ��
*********************************************************************/
//void lcd12864_Drawing(unsigned char *Bmp)
//{
//    unsigned int line,row;
//    lcd12864_parallel_write_cmd(0x34);
//    lcd12864_parallel_write_cmd(0x34);
//    for(line = 0;line < 32;line++)
//    {
//        lcd12864_parallel_write_cmd(0x80+line);
//        lcd12864_parallel_write_cmd(0x80);
//        for(row = 0;row < 16;row++)
//        {lcd12864_parallel_write_data(*Bmp ++);}
//    }
//    for(line = 0;line < 32;line++)
//    {
//        lcd12864_parallel_write_cmd(0x80+line);
//        lcd12864_parallel_write_cmd(0x88);
//        for(row = 0;row < 16;row++)
//        {lcd12864_parallel_write_data(*Bmp ++);}
//    }
//    lcd12864_parallel_write_cmd(0x36);
//    lcd12864_parallel_write_cmd(0x30);
//}
/*********************************************************************
* �� �� ��       : LCD12864_DisplayOneLine(unsigned char position,unsigned char *p)
* ��������		 : ��ָ��λ����ʾ10���ַ���Ҫ������ʾ������Ϣ
* �����б�       : position��λ��Χ��0x80��0x88��0x90��0x98����ͨ���ӷ�΢��λ�ã�*p��Ҫ����Ŀ�������Ϣ
* �������    	 : ��
*********************************************************************/
//void LCD12864_DisplayOneLine(unsigned char position,unsigned char *p)
//{
//	 unsigned char i;
//	 lcd12864_parallel_write_cmd(position);//д��Ҫ��ʾ���ֵ��е��׵�ַ
//	 Delay_Ms_(150);

//	 for(i = 0;i<10;i++)		//����ִ��д�����
//	 {
//	 	 lcd12864_parallel_write_data(*p);
//		 p++;
//	 }
//}
/*********************************************************************
* �� �� ��       : void LCD12864_Display(unsigned char CleanFlag,unsigned char x,unsigned char y,unsigned char * DisplayWord)
* ��������		 : Ϊ���ø���̵�������LCD��ʾ
* �����б�       : CleanFlagΪ0ʱ����������Ϊ0ʱ���壻x��y�����꣬DisplayWord��Ҫ��ʾ�����
* �������    	 : ��
*********************************************************************/
void LCD12864_Display(unsigned char CleanFlag,unsigned char x,unsigned char y,unsigned char * DisplayWord)
{
    if(CleanFlag == 0)
    {lcd12864_Clean();}//����    
    lcd12864_parallel_pos(x,y);//LCD��ʾ��λ
    lcd12864_parallel_string(DisplayWord);//��ӡ��ʾ�������Ѱ��
}



