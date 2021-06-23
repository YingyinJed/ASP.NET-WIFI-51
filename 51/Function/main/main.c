//影隐劫(ZGT)
/**************************************************************************************

现象：

注意事项：	
STC89C52的资源利用情况
LCD12864占用引脚：
          P1^0   ------>    LCD_RS
          P1^1   ------>    LCD_RW
          P1^2   ------>    LCD_EN
          P1^3   ------>    LCD_PSB
          P0的所有引脚作为数据并行传输口
蜂鸣器占用:P1.4(低电平触发，蜂鸣器为有源蜂鸣器)
LED提示灯占用：
          P1.5   ------> RED(低电平触发)
          P1.6   ------> GREEN(低电平触发)
I2C部分：
          P2^0   ------> I2C的SDA线
          P2^1   ------> I2C的SCL线
          I2C上直接连接着红外测温模块
SPI部分： 
          P2^7   ------>    RST
          P2^2   ------>    NSS
          P2^4   ------>    SCK
          P2^5   ------>    MOSI
          P2^6   ------>    MISO
开关占用：
          P3.2   ------>    SW1，对应外部中断0
          P3.3   ------>    SW2，对应外部中断1
定时器部分：占用了定时器1为串口提供9600波特率          
            占用了定时器0每次溢出计时10ms，用于作为连接超时的标志

串口部分：占用了定时器1并设置波特率为9600
使用了的信息头：Wi-Fi接收校园卡信息前会发送0xaa,0x55,0x99,0x99表示其准备就绪
                51发送校园卡号“0xbb,校园卡号,0xcc”
                51发送体温信息“0xdd,体温数据,0xee”
                温度传输情况，{0xaa,0xbb,0x88,0x88}{0xaa,0xbb,0x77,0x77}88是传输成功，77是传输失败
                接收联网状态，{0xaa,0x44,0x88,0x88}{0xaa,0x44,0x77,0x77}88是联网，77是断网
                HTTP请求结果，{0xaa,0x66,0x88,0x88}{0xaa,0x66,0x77,0x77}88是成功，77是失败
LED和蜂鸣器叫声：
        寻卡成功并读出了卡号       ——响一声500ms
        寻卡成功后wifi响应超时     ——响三声间隔1S 
        可以开始检测体温           ——响一声500ms且绿灯常亮
        读取到体温数据             ——响一声500ms
        测温上传失败               ——响三声间隔1S
        体温异常                   ——响10声间隔1S红灯常亮可以按按键SW2解除
        数据上传成功               ——响两声间隔500ms
        数据上传失败               ——响四声间隔1SLED跟着闪烁后红灯常亮
        
影隐劫(ZGT)                     
***************************************************************************************/
#include "main.h"

/*********************定义全局变量*********************************/
/*------------红外体温监测相关变量------------*/
unsigned char Temp_Time;//定义一个用于测温倒计时
unsigned char Temp_Hex[3];//定义存储温度的数组方便串口发送
float Temp_Flo = 0;//定义浮点型存储温度方便LCD显示
unsigned char Temp_SDFlag = 0;//温度传输标志位，为1时传输成功，为2时传输失败
/*------------RFID相关变量------------*/
unsigned char g_ucTempbuf[20];//用于存储RFID卡的基本信息

unsigned char Password_A[6] = {0xff,0xff,0xff,0xff,0xff,0xff};//存储RFID卡的A密码
unsigned char Data_Read[16];//用于存储从RFID一个块中读出来的信息
unsigned char Data_ASCII[32];//用于将RFID读取到的卡号数据转化为对应的ASCII码
unsigned char ASCII[2];//用于暂时缓存转化好的一个ASCII码值
/*------------定时器0相关变量------------*/
unsigned char Timer0_1S_Flag;//当该标志位每增加1时说明定时器0计数1秒
unsigned char Timer0_1S;//设置一个定时器0计数1S的计数变量

/*------------串口通信相关变量------------*/
//用于串口发送卡号信息
unsigned char UART_Send_STNum[34] = {0xbb,0x00,
                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                     0x00,0xcc};
//用于串口发送体温信息
unsigned char UART_Send_Temp[5] = {0xdd,
                                0x00,0x00,0x00,
                                    0xee};
unsigned char code SdData_Red[4] = {0xaa,0x55,0x99,0x99};

//接收信息头相关
unsigned char UART_BUF[4];//串口信息头数据缓存区                              
unsigned char UART_Receive_Red = 0;//接收准备信号,信息头{0xaa,0x55,0x99,0x99}
unsigned char UART_Receive_CON = 0;//接收联网状态，{0xaa,0x44,0x88,0x88}88是联网标志位置1
                                             //{0xaa,0x44,0x77,0x77}77是断网标志位置0
unsigned char UART_HTTP_FLAG = 0;//HTTP响应标志位，当HTTP未响应为0，响应成功为1，响应失败为2
/*------------LCD12864显示相关变量------------*/  
unsigned char LCD_TipWord_1[16];//作为LCD显示一行的缓存区
unsigned char LCD_Flag = 0;//定义一个标志位防止程序不停刷新LCD
void main()
{
    unsigned char status,i;//status用于存储RFID每一步的结果状态，i和j用于各种for循环
    
    lcd12864_parallel_init();//初始化LCD12864   
    PcdInit();//初始化RC522
    Uart_init();//串口初始化
    Timer0_Init();//初始化定时器0
    lcd12864_Clean();//清屏
    lcd12864_parallel_pos(1,1);//LCD显示定位
    lcd12864_parallel_string("初始化完成！");//打印提示语句初始化完成
    Delay_Ms_(1000);//延时一秒只为提示语句能被看见
    lcd12864_Clean();//清屏
	while(1)//进入程序大循环
    {
        while(1)
        {
            if(LCD_Flag == 0)//说明卡已拿走，刷新LCD
            {
                LCD_Flag = 1;//改变显示标志位防止重复刷新
                LCD12864_Display(0,0,0,"寻卡。。");//LCD上打印提示语
                if(UART_Receive_CON == 0)
                {LCD12864_Display(1,3,0,"网络已中断");}
                else
                {LCD12864_Display(1,3,0,"网络已连接");}
            }
            /************************RFID读取卡内信息的过程************************/            
            //集成寻卡、防冲撞、选定卡片和验证卡密
            status = RC522_FindCard(g_ucTempbuf,PICC_AUTHENT1A,0,Password_A);
            if(status == MI_OK)//如果上面的验证都通过了
            {                
                //读取块0
                status = PcdRead(0,Data_Read);
                if (status != MI_OK)//判断读取是否出错
                {continue;}
                
                LCD_Flag = 1;//寻卡的验证都通过了故LCD还是不显示寻卡中
                for(i = 0;i < 16;i++)
                {                           
                    HexToChar(Data_Read[i],ASCII);//将数据的每一个值转化为ASCII码                    
                    Data_ASCII[2*i] = ASCII[0];
                    Data_ASCII[(( (i+1) *2) - 1)] = ASCII[1];//将转换好的ASCII码存储在缓存区                                         
                }
                for(i = 0;i < 32;i++)//将卡号信息放在串口发送缓存区
                {UART_Send_STNum[i+1] = Data_ASCII[i];}   
                UART_SendString(4,SdData_Red);//告诉WiFi51已经准备好了                
            }
            else//由于出错了就不用执行下面的步骤了直接重来
            {continue;}
                              
            /************************为校园卡数据添加信息头和结束符后通过串口发送************************/            
            Timer0_1S = 0;
            Timer0_1S_Flag = 0;
            Delay_Ms_(10);//操作定时器前，将其标志位置0后稍微延时后再开启定时器能减少其出错概率
            TR0 = 1;//开启定时器1
            while(UART_Receive_Red != 0x99 && Timer0_1S_Flag < 5);//只有WiFi发送准备好的信号才发送串口数据限时1S
            TR0 = 0;//使用完后关闭定时器           
            Timer0_Init();
            
            if(UART_Receive_Red == 0x99)//判断是不是接收到准备好的信号才跳出了循环
            {
                UART_Receive_Red = 0;//清除串口接收标志位
                LCD12864_Display(1,0,0,"寻卡成功");
                LCD12864_Display(1,1,0,"WiFi响应成功");
                UART_SendString(34,UART_Send_STNum);//通过串口发送学号信息到WiFi
            }
            else//说明超时了，下面的就不用执行了，重新寻卡
            {
                UART_Receive_Red = 0;
                LCD12864_Display(1,0,0,"寻卡。。");
                LCD12864_Display(1,1,0,"WiFi响应超时");
                Beep_Start(3,1000);//让蜂鸣器响3声间隔1S  
                LCD_Flag = 0;//程序执行完毕刷新LCD改变标志位
                continue;
            }            
            LED_GREEN = 0;//提示用户可以开始测体温
            Beep_Start(1,500);//让蜂鸣器响500ms
            
            /************************以下为红外体温监测的代码************************/
            LCD12864_Display(1,2,0,"靠近测温:");  
            
            Timer0_1S = 0;
            Timer0_1S_Flag = 0; 
            Delay_Ms_(10);//操作定时器前，将其标志位置0后稍微延时后再开启定时器能减少其出错概率
            TR0 = 1;//开启定时器1
            while(Timer0_1S_Flag < 9)
            {                
                //获取红外温度传感器的温度                    
                MemRead(Temp_Hex);
                Temp_Flo = Temp_Float(Temp_Hex);
                if(Temp_Flo > Temp_MIN)
                {//检测到有热源靠近后预热再测一次
                    Delay_Ms_(50);//延时50ms给传感器预热一下
                    MemRead(Temp_Hex);
                    Temp_Flo = Temp_Float(Temp_Hex); 
                    break;//跳出循环
                }
                Temp_Time = 0x39 - Timer0_1S_Flag;                
                sprintf(LCD_TipWord_1,"靠近测温:%c",Temp_Time);                
                LCD12864_Display(1,2,0,LCD_TipWord_1);//打印提示语
            }
            TR0 = 0;//使用完后关闭定时器            
            Timer0_Init();
            LED_GREEN = 1;//提示用户测温结束
            Beep_Start(1,500);//让蜂鸣器响500ms
            if(Temp_Flo > Temp_MIN)
            {//只有这样才是读到体温了
                sprintf(LCD_TipWord_1,"测得体温：%0.1f",Temp_Flo);                
                LCD12864_Display(1,2,0,LCD_TipWord_1);//打印提示语
            }else
            {//没有读到体温就重来
                sprintf(LCD_TipWord_1,"测温失败重刷卡",Temp_Flo);
                LCD12864_Display(1,2,0,LCD_TipWord_1);//打印提示语              
                Delay_Ms_(2000);//延时2S让提示语能被看清
                LCD_Flag = 0;//程序执行完毕刷新LCD改变标志位 
                Temp_Flo = 0;//及时清除上一次读取到的体温信息
                continue;
            }    
            //将体温数据缓存到串口发送体温缓存区
            for(i = 1;i < 4;i++)
            {UART_Send_Temp[i] = Temp_Hex[i-1];}
            UART_SendString(5,UART_Send_Temp);//串口发送体温数据
            
            //****体温数据传输监测****
            LCD12864_Display(1,0,0,"测温上传。。");
            Timer0_1S = 0;
            Timer0_1S_Flag = 0; 
            Delay_Ms_(10);//操作定时器前，将其标志位置0后稍微延时后再开启定时器能减少其出错概率
            TR0 = 1;//开启定时器1
            while(Temp_SDFlag == 0 && Timer0_1S_Flag < 2);
            TR0 = 0;//使用完后关闭定时器            
            Timer0_Init();
            if(Temp_SDFlag == 1)
            {
                LCD12864_Display(1,0,0,"测温上传成功");
                Temp_SDFlag = 0;//清除标志位
            }else
            {      
                LCD12864_Display(1,0,0,"测温上传失败");
                Beep_Start(3,1000);                                 
                Temp_SDFlag = 0;//清除标志位
                LCD_Flag = 0;//程序执行完毕刷新LCD改变标志位
                continue;
            }
            
            //****体温异常监测****
            if(Temp_Flo > Temp_MAX)
            {
                LCD12864_Display(1,1,0,"高温。。。。");//打印提示语
                Timer0_1S = 0;
                Timer0_1S_Flag = 0; 
                LED_RED = 0;
                Delay_Ms_(10);//操作定时器前，将其标志位置0后稍微延时后再开启定时器能减少其出错概率
                TR0 = 1;//开启定时器1
                while(SW2 == 1 && Timer0_1S_Flag < 5)
                {
                    Beep_Start(1,1000);                    
                }
                LED_RED = 1;
                TR0 = 0;//使用完后关闭定时器            
                Timer0_Init();
                
            }else
            {
                LCD12864_Display(1,1,0,"添加中。。。");//打印提示语
            }
            Temp_Flo = 0;//及时清除上一次读取到的体温信息
            
            /************************以下为接收WiFi响应的代码************************/             
            while(UART_HTTP_FLAG == 0);  
            if(UART_HTTP_FLAG == 1)
            {
                LCD12864_Display(1,1,0,"添加。。成功");//打印提示语
            }else if(UART_HTTP_FLAG == 2)
            {
                LCD12864_Display(1,1,0,"添加。。失败");//打印提示语
            }
            UART_HTTP_FLAG = 0;//清除标志位
            Delay_Ms_(2000);//延时2秒为了让测温者能看见测温结果
            LCD_Flag = 0;//程序执行完毕刷新LCD改变标志位             
        }
    }
}

/*串口中断函数主要写了一个接收的服务，接收信息长度为5，信息头是为0xaa，0x55，信息体在[2]，[3]是校验位*/
void UART_NVIC() interrupt 4
{        
    if(UART_ReceiveString(4,UART_BUF))//函数内含超时计时
    {
        //******以下是接收到WiFi串口准备完成的信息头*******
        if(UART_BUF[0] == 0xaa && UART_BUF[1] == 0x44 && UART_BUF[2] == 0x77 && UART_BUF[3] == UART_BUF[2])
        {
            UART_Receive_CON = 0;
            LCD12864_Display(1,3,0,"网络已中断");
            memset(UART_BUF,0,sizeof(UART_BUF));//清空缓存区
        }
        else if(UART_BUF[0] == 0xaa && UART_BUF[1] == 0x44 && UART_BUF[2] == 0x88 && UART_BUF[3] == UART_BUF[2])
        {
            UART_Receive_CON = 1;
            LCD12864_Display(1,3,0,"网络已连接");
            memset(UART_BUF,0,sizeof(UART_BUF));//清空缓存区
        }
            
        //******以下是接收到WiFi串口准备完成的信息头*******
        else if(UART_BUF[0] == 0xaa && UART_BUF[1] == 0x55 && UART_BUF[2] == 0x99 && UART_BUF[3] == UART_BUF[2])
        {            
            UART_Receive_Red = UART_BUF[2];
            memset(UART_BUF,0,sizeof(UART_BUF));//清空缓存区
        }
        
        //******以下是接收到体温传输无误的信息头*******
        else if(UART_BUF[0] == 0xaa && UART_BUF[1] == 0xbb && UART_BUF[2] == 0x88 && UART_BUF[3] == UART_BUF[2])
        {
            Temp_SDFlag = 1;           
        }
        else if(UART_BUF[0] == 0xaa && UART_BUF[1] == 0xbb && UART_BUF[2] == 0x77 && UART_BUF[3] == UART_BUF[2])
        {
            Temp_SDFlag = 2;            
        }
        
        //******以下是接收到WiFi_HTTP返回的信息头*******
        else if(UART_BUF[0] == 0xaa && UART_BUF[1] == 0x66 && UART_BUF[2] == 0x88 && UART_BUF[3] == UART_BUF[2])
        {
            UART_HTTP_FLAG = 1;           
        }
        else if(UART_BUF[0] == 0xaa && UART_BUF[1] == 0x66 && UART_BUF[2] == 0x77 && UART_BUF[3] == UART_BUF[2])
        {
            UART_HTTP_FLAG = 2;            
        }
    }
}
/*定时器0中断服务函数*/
void Timer0_NVIC() interrupt 1
{    
    TH0 = 0XDB;
    TL0 = 0XFF;//还原初值
    if(Timer0_1S < 100)//定时器0每10ms溢出一次故计100次即1s
    {Timer0_1S++;}
    else
    {
        Timer0_1S = 0;//清除计数
        Timer0_1S_Flag++;//标志位加1
    }
}