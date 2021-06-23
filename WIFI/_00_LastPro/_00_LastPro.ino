#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <Ticker.h>

//双线程相关
Ticker Ticker1;//创建一个线程执行心跳程序
int UART_Overtime = 0;//用于记录串口接收是否超时

// 建立WiFi客户端对象，对象名称client
WiFiClient client;  
const char * host = "39.101.141.224";//网络服务器地址
const int httpPort = 65535; //http端口65535即网站在服务器的端口映射

//HTTP通信信息
String NameNum="NameNum=";//记录学号信息
String Temperature="&Temperature=";//记录体温信息
int Data_Length;//用于记录HTTP发送的数据长度 
String HTTP_ReqString;//用于接收HTTP接收到的字符串

//串口通信
String ReData;//存储接收到的信息

unsigned char SdData_Red[4] = {0xaa,0x55,0x99,0x99};
unsigned char SdData_Flag;//串口准备好的标志位

unsigned char Data_Head = 0xBB;//校园卡数据头
unsigned char Data_Tial = 0xCC;//校园卡数据尾

unsigned char Temp_Head = 0xdd;//体温数据头
unsigned char Temp_Tial = 0xee;//体温数据尾
unsigned char Temp_SUC[4]={0xaa,0xbb,0x88,0x88};//体温接收成功
unsigned char Temp_FAIL[4]={0xaa,0xbb,0x77,0x77};//体温接收失败

unsigned char SdHTTP_SUC_Red[4] = {0xaa,0x66,0x88,0x88};
unsigned char SdHTTP_FIAL_Red[4] = {0xaa,0x66,0x77,0x77};
void setup() {  
  Serial.begin(9600);//设置9600波特率
  Ticker1.attach(1,_Heartbeat);//让线程1每秒自加1作为脉搏
  WiFi.disconnect();//重连接的时候先清除原本的WiFi连接信息
  connectWiFi();//连接WiFi
  client.connect(host,httpPort);//连接服务器对应端口
}

void loop() {
  loop:
  //掉线重连的检查机制(这个不能用线程执行，线程的能力有限)
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("掉线了正在尝试重新连接。。。");
    connectWiFi();    // 连接wifi
    }
  //串口接收
  unsigned char i;//用于各种for循环
  //*****************以下为从51接收学号信息的代码********************
  //等待51发送准备指令
  UART_Overtime = 100;//启动双线程中的计时功能
  //在接收到串口数据前和超时前等待,等待时间2S
  while(!Serial.available() && UART_Overtime < 102);
  if(UART_Overtime > 102)
  {//如果超时，重新开始
    UART_Overtime = 0;//关闭双线程中的计时功能 
    goto loop;
  }
  while(Serial.available() && UART_Overtime < 102){//当串口在超时前接收到准备信息则进行下一步
    if(Serial.find(SdData_Red[0]))//判断信息头
    if(Serial.find(SdData_Red[1]))     
    if(Serial.find(SdData_Red[2]))
    if(Serial.find(SdData_Red[3]))
      {SdData_Flag = 1;}
    }
  UART_Overtime = 0;//关闭双线程中的计时功能 
  
  //接收到51准备发学号信息的信号
  if(SdData_Flag == 1){
    SdData_Flag = 0;//清除标志位
    Serial.write(SdData_Red,4);//向51发送准备好接收校园卡信息的信号
    UART_Overtime = 100;//启动双线程中的计时功能
    //在接收到串口数据前和超时前等待,等待时间2S
    while(!Serial.available() && UART_Overtime < 102);
    if(UART_Overtime >= 102)
    {//如果超时，重新开始
      UART_Overtime = 0;//关闭双线程中的计时功能 
      goto loop;
    }
    while(Serial.available() && UART_Overtime < 102)
      {//当串口接收到信息时        
        if(Serial.find(Data_Head))//判断信息头
        {ReData = Serial.readStringUntil(Data_Tial);}//读取信息至信息尾       
      }
    UART_Overtime = 0;//关闭双线程中的计时功能   
    if(ReData != "")
      {//接收到的数据不为空
      NameNum += ReData;//记录卡号
      ReData = "";//及时清除缓存区内容      
      Serial.println("");
      Serial.print(NameNum);      
      Serial.println("");
      }
       
     //*****************以下为从51接收体温信息的代码********************          
     //在接收到串口数据前等待 
     UART_Overtime = 100;//启动双线程中的计时功能 
     while(!Serial.available() && UART_Overtime < 110);
     if(UART_Overtime >= 110)
    {//如果超时，重新开始
      UART_Overtime = 0;//关闭双线程中的计时功能 
      goto loop;
    }
    float Temp_Flo;
     while(Serial.available() && UART_Overtime < 110)
     {
        if(Serial.find(Temp_Head))//判断信息头
        {
          char Temp_Hex[4] = {0};//存储来自51的体温信息
          Serial.readBytesUntil(Temp_Tial,Temp_Hex,3);//读到数据尾
          delay(100);//读到数据后延时一段时间
          Temp_Flo = Temp_Float(Temp_Hex);
          char Temp_St[3];  
          sprintf(Temp_St,"%.1f",Temp_Flo);//转化为保留一位小数的字符串
          Temperature += Temp_St;
          Serial.print(Temperature);      
          Serial.println("");
        }
      }
      UART_Overtime = 0;//关闭双线程中的计时功能 
      if(Temp_Flo > 34.5 && Temp_Flo < 45)
      {
        Serial.write(Temp_SUC,4);
      }else
      {
        Serial.write(Temp_FAIL,4);
        goto loop;
      }
      
      

      //*****************以下为利用HTTP请求发送数据到服务器的代码******************** 
      Data_Length = Temperature.length() + NameNum.length();//HTTP发送请求必须知道数据长度
      Serial.print("Data_Length：");
      Serial.print(Data_Length);
      Serial.println("");
      Serial.println("正在添加数据到服务器");
      String HTTP_ReqString = "";//存储来从服务器接收到的数据
      unsigned char HTTP_Count = 0;
      while(HTTP_Count < 10)
      {
         HTTP_ReqString = HttpAddTemp();
         if(HTTP_ReqString == "添加成功")
        {        
          Serial.write(SdHTTP_SUC_Red,4);//发送HTTP添加成功到51
          break;
        }else
        {
          HTTP_Count++;
        }        
      }
      Serial.print("添加结果：");
      Serial.print(HTTP_ReqString);
      Serial.println("");
      delay(500);      
      if(HTTP_Count >= 10)
      {        
        Serial.write(SdHTTP_FIAL_Red,4);//发送HTTP添加失败到51
      }
      Serial.println("");
   }  
  
  //清空接收缓存区
  NameNum="NameNum=";
  Temperature="&Temperature=";
  Data_Length = 0;
}

//双线程中的脉搏和掉线检查
void _Heartbeat(){
  //超时相关，当标志位大于等于100时启动，小于100则关闭
  if(UART_Overtime >= 100)
  {UART_Overtime++;}
}

// 连接WiFi
void connectWiFi(){
  unsigned char WIFISTATE[4] = {0xaa,0x44,0x77,0x77};
  Serial.write(WIFISTATE,4);//记录现在的网络状态并发送到51
  // 建立WiFiManager对象
  WiFiManager wifiManager;
  // 自动连接WiFi。以下语句的参数是连接ESP8266时的WiFi名称和密码
  wifiManager.autoConnect("基于校园卡的门禁系统","12345678");
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println(""); 
  Serial.print("ESP8266 连接到： ");
  Serial.println(WiFi.SSID());              // WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // IP   
  Serial.println("");
  Serial.print("连接成功！");
  delay(1000);  
  unsigned char WIFISTATE_[4] = {0xaa,0x44,0x88,0x88};
  Serial.write(WIFISTATE_,4);//记录现在的网络状态并发送到51                                                                                         
}

//将51中接收到的体温数据转化为浮点型
float Temp_Float(char * Temp_Hex){
  float Temp;
  unsigned int Data;
  Data = Temp_Hex[1];
  Data <<= 8;
  Data |= Temp_Hex[0];
  Temp = Data*0.02-272.15;
  return Temp;
}

//http发送体温信息到服务器
String HttpAddTemp(){
  String Line;
  String httpRequest = String("POST /Home/ESP8266_AddTemp") + " HTTP/1.1\r\n" +//通信方式，使用的方法地址，协议版本
                       "Host: " + host + "\r\n" +//地址
                       "Content-Type:application/x-www-form-urlencoded;charset=UTF-8\r\n"+
                       "Content-Length:"+ String(Data_Length) +"\r\n"+//数据长度
                       "Connection: keep-alive\r\n\r\n"//连接方式长连接
                       +NameNum+Temperature;//数据内容                    
  if(client.connect(host,httpPort)){//尝试连接
    client.print(httpRequest);//如果连接成功就发送请求
    while(!client.available());//等待响应
    while (client.available()){//如果接收到信息，且未超时
      if(client.find("\r\n\r\n")){//找到信息体
         Line = client.readStringUntil('\n');//读到信息尾并存储
        }
      }  
  }
  return Line;
}




