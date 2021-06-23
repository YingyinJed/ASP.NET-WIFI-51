//影隐劫(ZGE)https://blog.csdn.net/pawn331/article/details/94977844?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522161841611616780261959982%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=161841611616780261959982&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-1-94977844.pc_search_result_before_js&utm_term=LCD12864%E5%B8%A6%E4%B8%AD%E6%96%87%E5%AD%97%E5%BA%93
#include "LCD12864.h"
/*-------------------- 下面是底层时序函数 --------------------*/
/*********************************************************************
* 函 数 名       : lcd12864_parallel_busy
* 函数功能		 : 检查LCD忙状态，lcd12864_parallel_busy为1时，忙，等待。lcd12864_parallel_busy为0时,闲，可写指令与数据。
* 参数列表       : 无
* 函数输出    	 : 输出1时为忙，反之为闲
*********************************************************************/
bit lcd12864_parallel_busy() {
    bit result;
    LCD_RS = 0; //选择命令模式
    LCD_RW = 1; //选择读取
    LCD_EN = 1; //操作使能
    _nop_();_nop_();_nop_();_nop_();//短暂延时
    result = (bit)(P0 & 0x80);//数据引脚的最高位存储忙闲状态
    LCD_EN = 0; //关闭操作使能
    return(result);
}

/*********************************************************************
* 函 数 名       : lcd12864_parallel_write_cmd
* 函数功能		 : 按照LCD12864低层时序向LCD内部写入8位命令字
* 参数列表       : cmd - 待写入的8位命令字
* 函数输出    	 : 无
*********************************************************************/
void lcd12864_parallel_write_cmd(unsigned char cmd) {
    while(lcd12864_parallel_busy());//等LCD闲时才操作

    LCD_RS = 0; //选择命令模式
    LCD_RW = 0; //选择读取
    LCD_EN = 0; //先关闭使能，等数据写完才使能
    _nop_();
    _nop_();
    P0 = cmd; //将数据映射到P0端口
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    LCD_EN = 1; //打开使能让刚刚映射的数据写入LCD
    _nop_();_nop_();_nop_();_nop_();//等待时序让上面写入的数据记录为有效数据
    LCD_EN = 0; //关闭操作使能
}

/*********************************************************************
* 函 数 名       : lcd12864_parallel_write_data
* 函数功能		 : 按照LCD12864低层时序向LCD内部写入8位数据
* 参数列表       : dat - 待写入的8位命令字
* 函数输出    	 : 无
*********************************************************************/
void lcd12864_parallel_write_data(unsigned char dat) {
    while(lcd12864_parallel_busy());

    LCD_RS = 1; //选择数据模式
    LCD_RW = 0; //选择写入模式
    LCD_EN = 0; //下面的操作和写入命令时一样
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

/*-------------------- 上面是底层时序函数，下面是高层时序函数 --------------------*/
/*********************************************************************
* 函 数 名       : lcd12864_parallel_init()
* 函数功能		 : 按照LCD12864低层时序进行初始化序列
* 参数列表       : 无
* 函数输出    	 : 无
*********************************************************************/
void lcd12864_parallel_init() {
    LCD_PSB = 1;         //并口方式
    lcd12864_parallel_write_cmd(0x34);      //扩充指令操作 高四位 0011代表按照8位并行的方式传输数据 后四位代表扩充指令集
    Delay_Ms_(5);
    lcd12864_parallel_write_cmd(0x30);      //在这里又将扩充指令改为了基本指令操作
    Delay_Ms_(5);
    lcd12864_parallel_write_cmd(0x0C);      //显示开，关光标
    Delay_Ms_(5);
    lcd12864_parallel_write_cmd(0x01);      //清除LCD的显示内容
    Delay_Ms_(5);
}

/*********************************************************************
* 函 数 名       : lcd12864_parallel_pos
* 函数功能		 : LCD显示的坐标
* 参数列表       : X,Y轴x - 横向坐标，范围是0-7
*				   y - 纵向坐标，范围是0-3
* 函数输出    	 : 无
*********************************************************************/
void lcd12864_parallel_pos(unsigned char X, unsigned char Y) {
    unsigned char  pos;

    if(X == 0) {//第一行的地址是80H
        X = 0x80;
    } else if(X == 1) {//第二行的地址是90H
        X = 0x90;
    } else if(X == 2) {//第三行的地址是88H
        X = 0x88;
    } else if(X == 3) {//第四行的地址是98H
        X = 0x98;
    }

    pos = X + Y;
    lcd12864_parallel_write_cmd(pos);//显示地址
}

/*********************************************************************
* 函 数 名       : lcd12864_parallel_string(unsigned char *str)
* 函数功能		 : 经过lcd12864_parallel_pos定位后开始显示字符
* 参数列表       : str - 指向待显示的字符串的指针
* 函数输出    	 : 无
*********************************************************************/
void lcd12864_parallel_string(unsigned char *str) {
    while(*str > 0) {
        lcd12864_parallel_write_data(*str);
        str++;
        Delay_Ms_(1);
    }
}
/*********************************************************************
* 函 数 名       : lcd12864_Clean(void)
* 函数功能		 : 清屏
* 参数列表       : 无
* 函数输出    	 : 无
*********************************************************************/
void lcd12864_Clean(void)
{
    lcd12864_parallel_write_cmd(0x01);      //清除LCD的显示内容
    Delay_Ms_(5);
}
/*********************************************************************
* 函 数 名       : lcd12864_Drawing(unsigned char *data)
* 函数功能		 : 经过lcd12864_parallel_pos定位后开始显示字符
* 参数列表       : str - 指向待显示的字符串的指针
* 函数输出    	 : 无
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
* 函 数 名       : LCD12864_DisplayOneLine(unsigned char position,unsigned char *p)
* 函数功能		 : 在指定位置显示10个字符主要用于显示卡号信息
* 参数列表       : position定位范围（0x80、0x88、0x90、0x98可以通过加法微调位置）*p需要传输的卡号新信息
* 函数输出    	 : 无
*********************************************************************/
//void LCD12864_DisplayOneLine(unsigned char position,unsigned char *p)
//{
//	 unsigned char i;
//	 lcd12864_parallel_write_cmd(position);//写入要显示文字的行的首地址
//	 Delay_Ms_(150);

//	 for(i = 0;i<10;i++)		//依次执行写入操作
//	 {
//	 	 lcd12864_parallel_write_data(*p);
//		 p++;
//	 }
//}
/*********************************************************************
* 函 数 名       : void LCD12864_Display(unsigned char CleanFlag,unsigned char x,unsigned char y,unsigned char * DisplayWord)
* 函数功能		 : 为了用更简短的语句完成LCD显示
* 参数列表       : CleanFlag为0时会清屏，不为0时不清；x，y表坐标，DisplayWord需要显示的语句
* 函数输出    	 : 无
*********************************************************************/
void LCD12864_Display(unsigned char CleanFlag,unsigned char x,unsigned char y,unsigned char * DisplayWord)
{
    if(CleanFlag == 0)
    {lcd12864_Clean();}//清屏    
    lcd12864_parallel_pos(x,y);//LCD显示定位
    lcd12864_parallel_string(DisplayWord);//打印提示语句正在寻卡
}



