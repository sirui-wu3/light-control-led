#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "AD.h"
#include "ds18b20.h"
#include "DS1302.h"//时钟
#include "PWM.h"
#include "Serial.h"
#include "ESP8266.h"
extern char USART1_data[20];//串口数据存储数组
extern unsigned char SET_time[6];//初始化时间参数
int hour,min,sec;//时分秒
short temperature; 
u8 KeyNum,ModeNum,ChooseFlag,LedFlag,BrightNum;
u8 set1=22,set2=30;
u16 looptimes,AD;
int main(void)
{
	while(DS18B20_Init())	//DS18B20初始化	
	{
 		Delay_ms(100);
	}	
	OLED_Init();
	AD_Init();
	Key_Init();			// 1234567890123456
	OLED_ShowString(1, 1, "Mode:AUTO  ");
	OLED_ShowString(2, 1, "L:000 B:0");
	OLED_ShowString(3, 1, "T:+00.0");
	OLED_ShowString(4, 1, "00:00:00 22:30 ");
	DS1302_init();//DS1302初始化
	PWM_Init();
	ESP8266_Init();
	while (1)
	{
		if(1)
		{
			if(USART1_data[9]=='a')
			{
				ChooseFlag++;
				ChooseFlag%=6;
				if(ChooseFlag==0)
				{
					OLED_ShowChar(4, 3, ':');
					OLED_ShowChar(4, 6, ':');
					OLED_ShowChar(4, 9, ' ');
					OLED_ShowChar(4, 12, ':');
					OLED_ShowChar(4, 15, ' ');
				}
				if(ChooseFlag==1)
				{
					OLED_ShowChar(4, 3, '<');
					OLED_ShowChar(4, 6, ':');
					OLED_ShowChar(4, 9, ' ');
					OLED_ShowChar(4, 12, ':');
					OLED_ShowChar(4, 15, ' ');
				}
				if(ChooseFlag==2)
				{
					OLED_ShowChar(4, 3, ':');
					OLED_ShowChar(4, 6, '<');
					OLED_ShowChar(4, 9, ' ');
					OLED_ShowChar(4, 12, ':');
					OLED_ShowChar(4, 15, ' ');
				}
				if(ChooseFlag==3)
				{
					OLED_ShowChar(4, 3, ':');
					OLED_ShowChar(4, 6, ':');
					OLED_ShowChar(4, 9, '<');
					OLED_ShowChar(4, 12, ':');
					OLED_ShowChar(4, 15, ' ');
				}
				if(ChooseFlag==4)
				{
					OLED_ShowChar(4, 3, ':');
					OLED_ShowChar(4, 6, ':');
					OLED_ShowChar(4, 9, ' ');
					OLED_ShowChar(4, 12, '<');
					OLED_ShowChar(4, 15, ' ');
				}
				if(ChooseFlag==5)
				{
					OLED_ShowChar(4, 3, ':');
					OLED_ShowChar(4, 6, ':');
					OLED_ShowChar(4, 9, ' ');
					OLED_ShowChar(4, 12, ':');
					OLED_ShowChar(4, 15, '<');
				}
				USART1_data[9]='z';
			}
			else if(USART1_data[9]=='b')
			{
				if(ChooseFlag==1)
				{
					SET_time[3]=DS1302_Read(ds1302_hour);
					SET_time[4]=DS1302_Read(ds1302_min);
					SET_time[5]=DS1302_Read(ds1302_sec);
					SET_time[3]++;
					if(SET_time[3]>23){SET_time[3]=0;}//时越界判断
					DS1302_SetTime(SET_time);//设置时间
				}
				if(ChooseFlag==2)
				{
					SET_time[3]=DS1302_Read(ds1302_hour);
					SET_time[4]=DS1302_Read(ds1302_min);
					SET_time[5]=DS1302_Read(ds1302_sec);
					SET_time[4]++;
					if(SET_time[4]>59){SET_time[4]=0;}//分越界判断
					DS1302_SetTime(SET_time);
				}
				if(ChooseFlag==3)
				{
					SET_time[3]=DS1302_Read(ds1302_hour);
					SET_time[4]=DS1302_Read(ds1302_min);
					SET_time[5]=DS1302_Read(ds1302_sec);
					SET_time[5]++;
					if(SET_time[5]>59){SET_time[5]=0;}//秒越界判断
					DS1302_SetTime(SET_time);
				}
				if(ChooseFlag==4)
				{
					set1++;
					if(set1>23){set1=0;}
					OLED_ShowNum(4,10,set1,2);
				}
				if(ChooseFlag==5)
				{
					set2++;
					if(set2>59){set2=0;}
					OLED_ShowNum(4,13,set2,2);
				}
				USART1_data[9]='z';
			}
			else if(USART1_data[9]=='c')
			{
				if(ChooseFlag==1)
				{
					SET_time[3]=DS1302_Read(ds1302_hour);
					SET_time[4]=DS1302_Read(ds1302_min);
					SET_time[5]=DS1302_Read(ds1302_sec);
					SET_time[3]--;
					if(SET_time[3]>23){SET_time[3]=23;}//时越界判断
					DS1302_SetTime(SET_time);
				}
				if(ChooseFlag==2)
				{
					SET_time[3]=DS1302_Read(ds1302_hour);
					SET_time[4]=DS1302_Read(ds1302_min);
					SET_time[5]=DS1302_Read(ds1302_sec);
					SET_time[4]--;
					if(SET_time[4]>59){SET_time[4]=59;}//分越界判断
					DS1302_SetTime(SET_time);
				}
				if(ChooseFlag==3)
				{
					SET_time[3]=DS1302_Read(ds1302_hour);
					SET_time[4]=DS1302_Read(ds1302_min);
					SET_time[5]=DS1302_Read(ds1302_sec);
					SET_time[5]--;
					if(SET_time[5]>59){SET_time[5]=59;}//秒越界判断
					DS1302_SetTime(SET_time);
				}
				if(ChooseFlag==4)
				{
					set1--;
					if(set1>23){set1=23;}
					OLED_ShowNum(4,10,set1,2);
				}
				if(ChooseFlag==5)
				{
					set2--;
					if(set2>59){set2=59;}
					OLED_ShowNum(4,13,set2,2);
				}
				USART1_data[9]='z';
			}
			else if(USART1_data[9]=='d')
			{
				if(ModeNum==1&&LedFlag==0)
				{
					BrightNum++;
					BrightNum%=6;
					PWM_SetCompare1(BrightNum*20);
					OLED_ShowNum(2, 9, BrightNum, 1);
				}
				USART1_data[9]='z';
			}
			else if(USART1_data[9]=='e')
			{
				ModeNum=0;
				OLED_ShowString(1, 1, "Mode:AUTO  ");
				USART1_data[9]='z';
			}
			else if(USART1_data[9]=='f')
			{
				ModeNum=1;
				OLED_ShowString(1, 1, "Mode:MANUAL");
				USART1_data[9]='z';
			}
			else if(USART1_data[9]=='g')
			{
				ModeNum=2;
				OLED_ShowString(1, 1, "Mode:READ  ");
				if(LedFlag==0)
				{
					BrightNum=4;
					PWM_SetCompare1(BrightNum*20);
					OLED_ShowNum(2, 9, BrightNum, 1);
				}
				USART1_data[9]='z';
			}
			else if(USART1_data[9]=='h')
			{
				ModeNum=3;
				OLED_ShowString(1, 1, "Mode:MOVIE ");
				if(LedFlag==0)
				{
					BrightNum=1;
					PWM_SetCompare1(BrightNum*20);
					OLED_ShowNum(2, 9, BrightNum, 1);
				}
				USART1_data[9]='z';
			}
		}
		KeyNum=Key();
		if(KeyNum)
		{
			if(KeyNum==1)
			{
				ChooseFlag++;
				ChooseFlag%=6;
				if(ChooseFlag==0)
				{
					OLED_ShowChar(4, 3, ':');
					OLED_ShowChar(4, 6, ':');
					OLED_ShowChar(4, 9, ' ');
					OLED_ShowChar(4, 12, ':');
					OLED_ShowChar(4, 15, ' ');
				}
				if(ChooseFlag==1)
				{
					OLED_ShowChar(4, 3, '<');
					OLED_ShowChar(4, 6, ':');
					OLED_ShowChar(4, 9, ' ');
					OLED_ShowChar(4, 12, ':');
					OLED_ShowChar(4, 15, ' ');
				}
				if(ChooseFlag==2)
				{
					OLED_ShowChar(4, 3, ':');
					OLED_ShowChar(4, 6, '<');
					OLED_ShowChar(4, 9, ' ');
					OLED_ShowChar(4, 12, ':');
					OLED_ShowChar(4, 15, ' ');
				}
				if(ChooseFlag==3)
				{
					OLED_ShowChar(4, 3, ':');
					OLED_ShowChar(4, 6, ':');
					OLED_ShowChar(4, 9, '<');
					OLED_ShowChar(4, 12, ':');
					OLED_ShowChar(4, 15, ' ');
				}
				if(ChooseFlag==4)
				{
					OLED_ShowChar(4, 3, ':');
					OLED_ShowChar(4, 6, ':');
					OLED_ShowChar(4, 9, ' ');
					OLED_ShowChar(4, 12, '<');
					OLED_ShowChar(4, 15, ' ');
				}
				if(ChooseFlag==5)
				{
					OLED_ShowChar(4, 3, ':');
					OLED_ShowChar(4, 6, ':');
					OLED_ShowChar(4, 9, ' ');
					OLED_ShowChar(4, 12, ':');
					OLED_ShowChar(4, 15, '<');
				}
			}
			if(KeyNum==2)
			{
				if(ChooseFlag==1)
				{
					SET_time[3]=DS1302_Read(ds1302_hour);
					SET_time[4]=DS1302_Read(ds1302_min);
					SET_time[5]=DS1302_Read(ds1302_sec);
					SET_time[3]++;
					if(SET_time[3]>23){SET_time[3]=0;}//时越界判断
					DS1302_SetTime(SET_time);//设置时间
				}
				if(ChooseFlag==2)
				{
					SET_time[3]=DS1302_Read(ds1302_hour);
					SET_time[4]=DS1302_Read(ds1302_min);
					SET_time[5]=DS1302_Read(ds1302_sec);
					SET_time[4]++;
					if(SET_time[4]>59){SET_time[4]=0;}//分越界判断
					DS1302_SetTime(SET_time);
				}
				if(ChooseFlag==3)
				{
					SET_time[3]=DS1302_Read(ds1302_hour);
					SET_time[4]=DS1302_Read(ds1302_min);
					SET_time[5]=DS1302_Read(ds1302_sec);
					SET_time[5]++;
					if(SET_time[5]>59){SET_time[5]=0;}//秒越界判断
					DS1302_SetTime(SET_time);
				}
				if(ChooseFlag==4)
				{
					set1++;
					if(set1>23){set1=0;}
					OLED_ShowNum(4,10,set1,2);
				}
				if(ChooseFlag==5)
				{
					set2++;
					if(set2>59){set2=0;}
					OLED_ShowNum(4,13,set2,2);
				}
			}
			if(KeyNum==3)
			{
				if(ChooseFlag==1)
				{
					SET_time[3]=DS1302_Read(ds1302_hour);
					SET_time[4]=DS1302_Read(ds1302_min);
					SET_time[5]=DS1302_Read(ds1302_sec);
					SET_time[3]--;
					if(SET_time[3]>23){SET_time[3]=23;}//时越界判断
					DS1302_SetTime(SET_time);
				}
				if(ChooseFlag==2)
				{
					SET_time[3]=DS1302_Read(ds1302_hour);
					SET_time[4]=DS1302_Read(ds1302_min);
					SET_time[5]=DS1302_Read(ds1302_sec);
					SET_time[4]--;
					if(SET_time[4]>59){SET_time[4]=59;}//分越界判断
					DS1302_SetTime(SET_time);
				}
				if(ChooseFlag==3)
				{
					SET_time[3]=DS1302_Read(ds1302_hour);
					SET_time[4]=DS1302_Read(ds1302_min);
					SET_time[5]=DS1302_Read(ds1302_sec);
					SET_time[5]--;
					if(SET_time[5]>59){SET_time[5]=59;}//秒越界判断
					DS1302_SetTime(SET_time);
				}
				if(ChooseFlag==4)
				{
					set1--;
					if(set1>23){set1=23;}
					OLED_ShowNum(4,10,set1,2);
				}
				if(ChooseFlag==5)
				{
					set2--;
					if(set2>59){set2=59;}
					OLED_ShowNum(4,13,set2,2);
				}
			}
			if(KeyNum==4)
			{
				if(ModeNum==1&&LedFlag==0)
				{
					BrightNum++;
					BrightNum%=6;
					PWM_SetCompare1(BrightNum*20);
					OLED_ShowNum(2, 9, BrightNum, 1);
				}
			}
			if(KeyNum==5)//自动
			{
				ModeNum=0;
				OLED_ShowString(1, 1, "Mode:AUTO  ");
			}
			if(KeyNum==6)//手动
			{
				ModeNum=1;
				OLED_ShowString(1, 1, "Mode:MANUAL");
			}
			if(KeyNum==7)//阅读
			{
				ModeNum=2;
				OLED_ShowString(1, 1, "Mode:READ  ");
				if(LedFlag==0)
				{
					BrightNum=4;
					PWM_SetCompare1(BrightNum*20);
					OLED_ShowNum(2, 9, BrightNum, 1);
				}
			}
			if(KeyNum==8)//电影
			{
				ModeNum=3;
				OLED_ShowString(1, 1, "Mode:MOVIE ");
				if(LedFlag==0)
				{
					BrightNum=1;
					PWM_SetCompare1(BrightNum*20);
					OLED_ShowNum(2, 9, BrightNum, 1);
				}
			}
		}
		Delay_ms(1);
		looptimes++;
		if(looptimes%100==0)
		{
			
		}
		if(looptimes%250==0)
		{
			hour =DS1302_Read(ds1302_hour);//读取时
			min =DS1302_Read(ds1302_min);//读取分
			sec =DS1302_Read(ds1302_sec);//读取秒
			OLED_ShowNum(4,1,hour,2);//显示时
			OLED_ShowNum(4,4,min,2);//显示分
			OLED_ShowNum(4,7,sec,2);//显示秒
			if(hour==set1&&min==set2)
			{
				LedFlag=1;
				BrightNum=0;
				PWM_SetCompare1(BrightNum*20);
				OLED_ShowNum(2, 9, BrightNum, 1);
			}
		}
		if(looptimes%500==0)
		{
			AD = AD_GetValue()/10;//100-360
			if(AD<100){AD=100;}
			if(AD>360){AD=360;}
			AD-=100;//0-260
			AD/=2.6;//0-100
			OLED_ShowNum(2, 3, AD, 3);
			if(ModeNum==0&&LedFlag==0)
			{
				if(AD<=10){BrightNum=5;}
				if(AD>10&&AD<=30){BrightNum=4;}
				if(AD>30&&AD<=50){BrightNum=3;}
				if(AD>50&&AD<=70){BrightNum=2;}
				if(AD>70&&AD<=90){BrightNum=1;}
				if(AD>90){BrightNum=0;}
				PWM_SetCompare1(BrightNum*20);
				OLED_ShowNum(2, 9, BrightNum, 1);
			}
		}
		if(looptimes%1000==0)
		{
			temperature=DS18B20_Get_Temp();//获取温度
			if(temperature<0)
			{
				OLED_ShowChar(3, 3, '-');
				temperature=-temperature;					//转为正数
				OLED_ShowNum(3, 4, temperature/10, 2);
				OLED_ShowNum(3, 7, temperature%10, 1);
				temperature=-temperature;
			}
			else
			{
				OLED_ShowChar(3, 3, '+');
				OLED_ShowNum(3, 4, temperature/10, 2);
				OLED_ShowNum(3, 7, temperature%10, 1);
			}
		}
		if(looptimes%3000==0)
		{
			looptimes=0;
			ESP8266_SendCmd("AT+CIPSEND=0,15");
			Serial_Printf("L:%d",AD);
			Serial_Printf("\r\n");//7
			if(temperature<0)
			{
				temperature=-temperature;
				Serial_Printf("T:-%d.%d",temperature/10,temperature%10);
				temperature=-temperature;
			}
			else{Serial_Printf("T:+%d.%d",temperature/10,temperature%10);}//7
			Serial_Printf("         ");
		}
	}
}
