//Ӱ����(ZGT)
/**************************************************************************************

����

ע�����	
STC89C52����Դ�������
LCD12864ռ�����ţ�
          P1^0   ------>    LCD_RS
          P1^1   ------>    LCD_RW
          P1^2   ------>    LCD_EN
          P1^3   ------>    LCD_PSB
          P0������������Ϊ���ݲ��д����
������ռ��:P1.4(�͵�ƽ������������Ϊ��Դ������)
LED��ʾ��ռ�ã�
          P1.5   ------> RED(�͵�ƽ����)
          P1.6   ------> GREEN(�͵�ƽ����)
I2C���֣�
          P2^0   ------> I2C��SDA��
          P2^1   ------> I2C��SCL��
          I2C��ֱ�������ź������ģ��
SPI���֣� 
          P2^7   ------>    RST
          P2^2   ------>    NSS
          P2^4   ------>    SCK
          P2^5   ------>    MOSI
          P2^6   ------>    MISO
����ռ�ã�
          P3.2   ------>    SW1����Ӧ�ⲿ�ж�0
          P3.3   ------>    SW2����Ӧ�ⲿ�ж�1
��ʱ�����֣�ռ���˶�ʱ��1Ϊ�����ṩ9600������          
            ռ���˶�ʱ��0ÿ�������ʱ10ms��������Ϊ���ӳ�ʱ�ı�־

���ڲ��֣�ռ���˶�ʱ��1�����ò�����Ϊ9600
ʹ���˵���Ϣͷ��Wi-Fi����У԰����Ϣǰ�ᷢ��0xaa,0x55,0x99,0x99��ʾ��׼������
                51����У԰���š�0xbb,У԰����,0xcc��
                51����������Ϣ��0xdd,��������,0xee��
                �¶ȴ��������{0xaa,0xbb,0x88,0x88}{0xaa,0xbb,0x77,0x77}88�Ǵ���ɹ���77�Ǵ���ʧ��
                ��������״̬��{0xaa,0x44,0x88,0x88}{0xaa,0x44,0x77,0x77}88��������77�Ƕ���
                HTTP��������{0xaa,0x66,0x88,0x88}{0xaa,0x66,0x77,0x77}88�ǳɹ���77��ʧ��
LED�ͷ�����������
        Ѱ���ɹ��������˿���       ������һ��500ms
        Ѱ���ɹ���wifi��Ӧ��ʱ     �������������1S 
        ���Կ�ʼ�������           ������һ��500ms���̵Ƴ���
        ��ȡ����������             ������һ��500ms
        �����ϴ�ʧ��               �������������1S
        �����쳣                   ������10�����1S��Ƴ������԰�����SW2���
        �����ϴ��ɹ�               �������������500ms
        �����ϴ�ʧ��               �������������1SLED������˸���Ƴ���
        
Ӱ����(ZGT)                     
***************************************************************************************/
#include "main.h"

/*********************����ȫ�ֱ���*********************************/
/*------------�������¼����ر���------------*/
unsigned char Temp_Time;//����һ�����ڲ��µ���ʱ
unsigned char Temp_Hex[3];//����洢�¶ȵ����鷽�㴮�ڷ���
float Temp_Flo = 0;//���帡���ʹ洢�¶ȷ���LCD��ʾ
unsigned char Temp_SDFlag = 0;//�¶ȴ����־λ��Ϊ1ʱ����ɹ���Ϊ2ʱ����ʧ��
/*------------RFID��ر���------------*/
unsigned char g_ucTempbuf[20];//���ڴ洢RFID���Ļ�����Ϣ

unsigned char Password_A[6] = {0xff,0xff,0xff,0xff,0xff,0xff};//�洢RFID����A����
unsigned char Data_Read[16];//���ڴ洢��RFIDһ�����ж���������Ϣ
unsigned char Data_ASCII[32];//���ڽ�RFID��ȡ���Ŀ�������ת��Ϊ��Ӧ��ASCII��
unsigned char ASCII[2];//������ʱ����ת���õ�һ��ASCII��ֵ
/*------------��ʱ��0��ر���------------*/
unsigned char Timer0_1S_Flag;//���ñ�־λÿ����1ʱ˵����ʱ��0����1��
unsigned char Timer0_1S;//����һ����ʱ��0����1S�ļ�������

/*------------����ͨ����ر���------------*/
//���ڴ��ڷ��Ϳ�����Ϣ
unsigned char UART_Send_STNum[34] = {0xbb,0x00,
                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                     0x00,0xcc};
//���ڴ��ڷ���������Ϣ
unsigned char UART_Send_Temp[5] = {0xdd,
                                0x00,0x00,0x00,
                                    0xee};
unsigned char code SdData_Red[4] = {0xaa,0x55,0x99,0x99};

//������Ϣͷ���
unsigned char UART_BUF[4];//������Ϣͷ���ݻ�����                              
unsigned char UART_Receive_Red = 0;//����׼���ź�,��Ϣͷ{0xaa,0x55,0x99,0x99}
unsigned char UART_Receive_CON = 0;//��������״̬��{0xaa,0x44,0x88,0x88}88��������־λ��1
                                             //{0xaa,0x44,0x77,0x77}77�Ƕ�����־λ��0
unsigned char UART_HTTP_FLAG = 0;//HTTP��Ӧ��־λ����HTTPδ��ӦΪ0����Ӧ�ɹ�Ϊ1����Ӧʧ��Ϊ2
/*------------LCD12864��ʾ��ر���------------*/  
unsigned char LCD_TipWord_1[16];//��ΪLCD��ʾһ�еĻ�����
unsigned char LCD_Flag = 0;//����һ����־λ��ֹ����ͣˢ��LCD
void main()
{
    unsigned char status,i;//status���ڴ洢RFIDÿһ���Ľ��״̬��i��j���ڸ���forѭ��
    
    lcd12864_parallel_init();//��ʼ��LCD12864   
    PcdInit();//��ʼ��RC522
    Uart_init();//���ڳ�ʼ��
    Timer0_Init();//��ʼ����ʱ��0
    lcd12864_Clean();//����
    lcd12864_parallel_pos(1,1);//LCD��ʾ��λ
    lcd12864_parallel_string("��ʼ����ɣ�");//��ӡ��ʾ����ʼ�����
    Delay_Ms_(1000);//��ʱһ��ֻΪ��ʾ����ܱ�����
    lcd12864_Clean();//����
	while(1)//��������ѭ��
    {
        while(1)
        {
            if(LCD_Flag == 0)//˵���������ߣ�ˢ��LCD
            {
                LCD_Flag = 1;//�ı���ʾ��־λ��ֹ�ظ�ˢ��
                LCD12864_Display(0,0,0,"Ѱ������");//LCD�ϴ�ӡ��ʾ��
                if(UART_Receive_CON == 0)
                {LCD12864_Display(1,3,0,"�������ж�");}
                else
                {LCD12864_Display(1,3,0,"����������");}
            }
            /************************RFID��ȡ������Ϣ�Ĺ���************************/            
            //����Ѱ��������ײ��ѡ����Ƭ����֤����
            status = RC522_FindCard(g_ucTempbuf,PICC_AUTHENT1A,0,Password_A);
            if(status == MI_OK)//����������֤��ͨ����
            {                
                //��ȡ��0
                status = PcdRead(0,Data_Read);
                if (status != MI_OK)//�ж϶�ȡ�Ƿ����
                {continue;}
                
                LCD_Flag = 1;//Ѱ������֤��ͨ���˹�LCD���ǲ���ʾѰ����
                for(i = 0;i < 16;i++)
                {                           
                    HexToChar(Data_Read[i],ASCII);//�����ݵ�ÿһ��ֵת��ΪASCII��                    
                    Data_ASCII[2*i] = ASCII[0];
                    Data_ASCII[(( (i+1) *2) - 1)] = ASCII[1];//��ת���õ�ASCII��洢�ڻ�����                                         
                }
                for(i = 0;i < 32;i++)//��������Ϣ���ڴ��ڷ��ͻ�����
                {UART_Send_STNum[i+1] = Data_ASCII[i];}   
                UART_SendString(4,SdData_Red);//����WiFi51�Ѿ�׼������                
            }
            else//���ڳ����˾Ͳ���ִ������Ĳ�����ֱ������
            {continue;}
                              
            /************************ΪУ԰�����������Ϣͷ�ͽ�������ͨ�����ڷ���************************/            
            Timer0_1S = 0;
            Timer0_1S_Flag = 0;
            Delay_Ms_(10);//������ʱ��ǰ�������־λ��0����΢��ʱ���ٿ�����ʱ���ܼ�����������
            TR0 = 1;//������ʱ��1
            while(UART_Receive_Red != 0x99 && Timer0_1S_Flag < 5);//ֻ��WiFi����׼���õ��źŲŷ��ʹ���������ʱ1S
            TR0 = 0;//ʹ�����رն�ʱ��           
            Timer0_Init();
            
            if(UART_Receive_Red == 0x99)//�ж��ǲ��ǽ��յ�׼���õ��źŲ�������ѭ��
            {
                UART_Receive_Red = 0;//������ڽ��ձ�־λ
                LCD12864_Display(1,0,0,"Ѱ���ɹ�");
                LCD12864_Display(1,1,0,"WiFi��Ӧ�ɹ�");
                UART_SendString(34,UART_Send_STNum);//ͨ�����ڷ���ѧ����Ϣ��WiFi
            }
            else//˵����ʱ�ˣ�����ľͲ���ִ���ˣ�����Ѱ��
            {
                UART_Receive_Red = 0;
                LCD12864_Display(1,0,0,"Ѱ������");
                LCD12864_Display(1,1,0,"WiFi��Ӧ��ʱ");
                Beep_Start(3,1000);//�÷�������3�����1S  
                LCD_Flag = 0;//����ִ�����ˢ��LCD�ı��־λ
                continue;
            }            
            LED_GREEN = 0;//��ʾ�û����Կ�ʼ������
            Beep_Start(1,500);//�÷�������500ms
            
            /************************����Ϊ�������¼��Ĵ���************************/
            LCD12864_Display(1,2,0,"��������:");  
            
            Timer0_1S = 0;
            Timer0_1S_Flag = 0; 
            Delay_Ms_(10);//������ʱ��ǰ�������־λ��0����΢��ʱ���ٿ�����ʱ���ܼ�����������
            TR0 = 1;//������ʱ��1
            while(Timer0_1S_Flag < 9)
            {                
                //��ȡ�����¶ȴ��������¶�                    
                MemRead(Temp_Hex);
                Temp_Flo = Temp_Float(Temp_Hex);
                if(Temp_Flo > Temp_MIN)
                {//��⵽����Դ������Ԥ���ٲ�һ��
                    Delay_Ms_(50);//��ʱ50ms��������Ԥ��һ��
                    MemRead(Temp_Hex);
                    Temp_Flo = Temp_Float(Temp_Hex); 
                    break;//����ѭ��
                }
                Temp_Time = 0x39 - Timer0_1S_Flag;                
                sprintf(LCD_TipWord_1,"��������:%c",Temp_Time);                
                LCD12864_Display(1,2,0,LCD_TipWord_1);//��ӡ��ʾ��
            }
            TR0 = 0;//ʹ�����رն�ʱ��            
            Timer0_Init();
            LED_GREEN = 1;//��ʾ�û����½���
            Beep_Start(1,500);//�÷�������500ms
            if(Temp_Flo > Temp_MIN)
            {//ֻ���������Ƕ���������
                sprintf(LCD_TipWord_1,"������£�%0.1f",Temp_Flo);                
                LCD12864_Display(1,2,0,LCD_TipWord_1);//��ӡ��ʾ��
            }else
            {//û�ж������¾�����
                sprintf(LCD_TipWord_1,"����ʧ����ˢ��",Temp_Flo);
                LCD12864_Display(1,2,0,LCD_TipWord_1);//��ӡ��ʾ��              
                Delay_Ms_(2000);//��ʱ2S����ʾ���ܱ�����
                LCD_Flag = 0;//����ִ�����ˢ��LCD�ı��־λ 
                Temp_Flo = 0;//��ʱ�����һ�ζ�ȡ����������Ϣ
                continue;
            }    
            //���������ݻ��浽���ڷ������»�����
            for(i = 1;i < 4;i++)
            {UART_Send_Temp[i] = Temp_Hex[i-1];}
            UART_SendString(5,UART_Send_Temp);//���ڷ�����������
            
            //****�������ݴ�����****
            LCD12864_Display(1,0,0,"�����ϴ�����");
            Timer0_1S = 0;
            Timer0_1S_Flag = 0; 
            Delay_Ms_(10);//������ʱ��ǰ�������־λ��0����΢��ʱ���ٿ�����ʱ���ܼ�����������
            TR0 = 1;//������ʱ��1
            while(Temp_SDFlag == 0 && Timer0_1S_Flag < 2);
            TR0 = 0;//ʹ�����رն�ʱ��            
            Timer0_Init();
            if(Temp_SDFlag == 1)
            {
                LCD12864_Display(1,0,0,"�����ϴ��ɹ�");
                Temp_SDFlag = 0;//�����־λ
            }else
            {      
                LCD12864_Display(1,0,0,"�����ϴ�ʧ��");
                Beep_Start(3,1000);                                 
                Temp_SDFlag = 0;//�����־λ
                LCD_Flag = 0;//����ִ�����ˢ��LCD�ı��־λ
                continue;
            }
            
            //****�����쳣���****
            if(Temp_Flo > Temp_MAX)
            {
                LCD12864_Display(1,1,0,"���¡�������");//��ӡ��ʾ��
                Timer0_1S = 0;
                Timer0_1S_Flag = 0; 
                LED_RED = 0;
                Delay_Ms_(10);//������ʱ��ǰ�������־λ��0����΢��ʱ���ٿ�����ʱ���ܼ�����������
                TR0 = 1;//������ʱ��1
                while(SW2 == 1 && Timer0_1S_Flag < 5)
                {
                    Beep_Start(1,1000);                    
                }
                LED_RED = 1;
                TR0 = 0;//ʹ�����رն�ʱ��            
                Timer0_Init();
                
            }else
            {
                LCD12864_Display(1,1,0,"����С�����");//��ӡ��ʾ��
            }
            Temp_Flo = 0;//��ʱ�����һ�ζ�ȡ����������Ϣ
            
            /************************����Ϊ����WiFi��Ӧ�Ĵ���************************/             
            while(UART_HTTP_FLAG == 0);  
            if(UART_HTTP_FLAG == 1)
            {
                LCD12864_Display(1,1,0,"��ӡ����ɹ�");//��ӡ��ʾ��
            }else if(UART_HTTP_FLAG == 2)
            {
                LCD12864_Display(1,1,0,"��ӡ���ʧ��");//��ӡ��ʾ��
            }
            UART_HTTP_FLAG = 0;//�����־λ
            Delay_Ms_(2000);//��ʱ2��Ϊ���ò������ܿ������½��
            LCD_Flag = 0;//����ִ�����ˢ��LCD�ı��־λ             
        }
    }
}

/*�����жϺ�����Ҫд��һ�����յķ��񣬽�����Ϣ����Ϊ5����Ϣͷ��Ϊ0xaa��0x55����Ϣ����[2]��[3]��У��λ*/
void UART_NVIC() interrupt 4
{        
    if(UART_ReceiveString(4,UART_BUF))//�����ں���ʱ��ʱ
    {
        //******�����ǽ��յ�WiFi����׼����ɵ���Ϣͷ*******
        if(UART_BUF[0] == 0xaa && UART_BUF[1] == 0x44 && UART_BUF[2] == 0x77 && UART_BUF[3] == UART_BUF[2])
        {
            UART_Receive_CON = 0;
            LCD12864_Display(1,3,0,"�������ж�");
            memset(UART_BUF,0,sizeof(UART_BUF));//��ջ�����
        }
        else if(UART_BUF[0] == 0xaa && UART_BUF[1] == 0x44 && UART_BUF[2] == 0x88 && UART_BUF[3] == UART_BUF[2])
        {
            UART_Receive_CON = 1;
            LCD12864_Display(1,3,0,"����������");
            memset(UART_BUF,0,sizeof(UART_BUF));//��ջ�����
        }
            
        //******�����ǽ��յ�WiFi����׼����ɵ���Ϣͷ*******
        else if(UART_BUF[0] == 0xaa && UART_BUF[1] == 0x55 && UART_BUF[2] == 0x99 && UART_BUF[3] == UART_BUF[2])
        {            
            UART_Receive_Red = UART_BUF[2];
            memset(UART_BUF,0,sizeof(UART_BUF));//��ջ�����
        }
        
        //******�����ǽ��յ����´����������Ϣͷ*******
        else if(UART_BUF[0] == 0xaa && UART_BUF[1] == 0xbb && UART_BUF[2] == 0x88 && UART_BUF[3] == UART_BUF[2])
        {
            Temp_SDFlag = 1;           
        }
        else if(UART_BUF[0] == 0xaa && UART_BUF[1] == 0xbb && UART_BUF[2] == 0x77 && UART_BUF[3] == UART_BUF[2])
        {
            Temp_SDFlag = 2;            
        }
        
        //******�����ǽ��յ�WiFi_HTTP���ص���Ϣͷ*******
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
/*��ʱ��0�жϷ�����*/
void Timer0_NVIC() interrupt 1
{    
    TH0 = 0XDB;
    TL0 = 0XFF;//��ԭ��ֵ
    if(Timer0_1S < 100)//��ʱ��0ÿ10ms���һ�ιʼ�100�μ�1s
    {Timer0_1S++;}
    else
    {
        Timer0_1S = 0;//�������
        Timer0_1S_Flag++;//��־λ��1
    }
}