#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Delay.h"

char USART1_flag;
char USART1_data[20];
int USART1_i=0;
int USART1_Ready=0;//数据接收完成标志
	
void Serial_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//开启USART时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//开启GPIO时钟
	/*配置GPIO*/
	GPIO_InitTypeDef GPIO_InitStructure;//结构体命名
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO模式，复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化结构体
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO模式，上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;////配置速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化结构体
	/*配置USART*/
	USART_InitTypeDef USART_InitStructure;//结构体命名
	USART_InitStructure.USART_BaudRate = 9600;//设置波特率为9600
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制，不使用流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//串口模式为TX和RX，即发送和接收
	USART_InitStructure.USART_Parity = USART_Parity_No;//校验位，不校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位，1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长，8位或九位，校验时选九位
	USART_Init(USART1, &USART_InitStructure);//初始化结构体//查询方式配置结束
	/*开启串口中断*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	/*配置NVIC(串口的中断模式)*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//分组
	/*初始化NVIC*/
	NVIC_InitTypeDef NVIC_InitStructure;//结构体命名
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//中断优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//响应优先级
	NVIC_Init(&NVIC_InitStructure);//初始化结构体
	/*开启USART电源*/
	USART_Cmd(USART1, ENABLE);
}

/*发送字节*/
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);//发送数据
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//获取标志位，等待数据发送完成
}

/*发送数组*///HEX模式
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

/*发送字符串*///文本模式
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

/*次方函数*///x的y次方
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

/*发送数字*/
void Serial_SendNumber(uint32_t Number, uint8_t Length)//个十百位等拆分开，以十进制拆分开，转换为字符依次发
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

/*printf封装函数*/
void Serial_Printf(char *format, ...)//接受格式化字符串，接受可变参数列表
{
	char String[100];//定义字符串
	va_list arg;//定义一个参数列表变量
	va_start(arg, format);//从format开始接收参数表，放在arg内
	vsprintf(String, format, arg);//打印位置，格式化字符串，参数表
	va_end(arg);//释放参数表
	Serial_SendString(String);//把String发送出去
}

/*串口中断函数*/
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//如果RXNE置一
	{
		USART1_flag = USART_ReceiveData(USART1);//读数据
		if(USART1_flag=='+')//检测'+'
		{
			USART1_Ready=1;
		}
		else if(USART1_flag=='\n')
		{
			USART1_Ready=0;
			USART1_i=0;	
		}
		if(USART1_Ready==1)
		{
			USART1_data[USART1_i]=USART1_flag;
			USART1_i++;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除标记位
	}
}
