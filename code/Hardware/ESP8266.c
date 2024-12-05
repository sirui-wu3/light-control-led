#include "stm32f10x.h"                  // Device header
#include "Serial.h"
#include "Delay.h"
//ESP8266 WIFI发送AT指令
//pbuf：AT指令，字符串格式，如："AT"
void ESP8266_SendCmd(unsigned char *pbuf)
{
	while(*pbuf!='\0') //遇到空格跳出循环	
	{
		Serial_SendByte(*pbuf);
		Delay_us(50);
		pbuf++;	
	}
	Delay_us(50);
	Serial_SendByte('\r');//回车
	Delay_us(50);
	Serial_SendByte('\n');//换行
	Delay_ms(10);
}

//ESP8266-WIFI模块工作模式初始化
void ESP8266_Init(void)
{
	Serial_Init();//串口初始化
	Delay_ms(100);
	ESP8266_SendCmd("AT+CWMODE=2");//设置路由器模式 1 staTIon模式 2 AP点 路由器模式 3 station+AP混合模式
	Delay_ms(100);
	ESP8266_SendCmd("AT+CWSAP=\"ESP123\",\"123456\",11,0"); //设置WIFI热点名及密码
	Delay_ms(100);
	ESP8266_SendCmd("AT+CIPAP=\"192.168.4.1\"");
	Delay_ms(100);
//	ESP8266_SendCmd("AT+RST");//重新启动wifi模块
//	delay(2000);
	ESP8266_SendCmd("AT+CIPMUX=1");	//开启多连接模式，允许多个各客户端接入
	Delay_ms(100);
	ESP8266_SendCmd("AT+CIPSERVER=1,8080");	//启动TCP/IP 端口为8080 实现基于网络控制	
	Delay_ms(100);
} 

//ESP8266 WIFI发送数据到APP
//pbuf：数据
void ESP8266_SendData(unsigned char *pbuf)
{
	ESP8266_SendCmd("AT+CIPSEND=0,5");
	while(*pbuf!='\0') //遇到空格跳出循环	
	{
		Serial_SendByte(*pbuf);
		Delay_us(50);
		pbuf++;	
	}
	Serial_SendByte('\n');//换行	
	Delay_ms(10);
}
