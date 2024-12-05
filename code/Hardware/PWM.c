#include "stm32f10x.h"                  // Device header

void PWM_Init(void)  //pwm初始化函数
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //TIM2，通用定时器，属于APB1的外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA时钟
	/*配置重映射*//*把PA0的PWM功能移到PA15上*/
  //	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //开启AFIO时钟
  //	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); //引脚重映射设置，部分重映射模式1（PA0->PA15,PA1->PB3）
  //	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //将PA15,PB3,PB4变为普通端口
	/*配置GPIO*/
	GPIO_InitTypeDef GPIO_InitStructure;  //结构体命名
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //设置GPIO模式，复用推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//PA0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化结构体
	/*选择时基单元*/ 
	TIM_InternalClockConfig(TIM2);  //内部时钟
	/*配置时基单元*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; //结构体命名
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频为1分频，即不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数器模式，向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		//ARR（自动重装值）,0-65535，决定定时时间的参数（定时频率=72MHZ/(PSC+1)/(ARR+1)）
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;	//PSC（预分频）,0-65535，决定定时时间的参数   //1KHz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器（高级定时器才有）
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);  //初始化结构体
	/*初始化输出比较单元*/
	TIM_OCInitTypeDef TIM_OCInitStructure;  //结构体命名
	TIM_OCStructInit(&TIM_OCInitStructure);  //给结构体赋初始值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //设置输出比较的模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //设置输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;		//0-FFFF,设置CCR（捕获比较寄存器）
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //选择1通道，初始化输出比较单元
	/*启动定时器*/
	TIM_Cmd(TIM2, ENABLE);  //启动定时器
}

void PWM_SetCompare1(uint16_t Compare)  //更改CCR的值
{
	TIM_SetCompare1(TIM2, Compare);  //设置CCR的值
}

/*主程序调用函数*/
/*PWM_SetCompare1(100);*/
