#ifndef __DS1302_H__
#define __DS1302_H__
#include "stm32f10x.h"

#define OUT   0x01
#define IN    0x00

#define DS1302_PORT        GPIOB
#define DS1302_PIN_CLK    GPIO_Pin_1
#define DS1302_PIN_DATA   GPIO_Pin_10
#define DS1302_PIN_RES    GPIO_Pin_11
#define DS1302_PORT   GPIOB

#define CLK_Reset_0  GPIOB->BRR=DS1302_PIN_CLK
#define CLK_Set_1    GPIOB->BSRR=DS1302_PIN_CLK

#define IO_Reset_0   GPIOB->BRR=DS1302_PIN_DATA
#define IO_Set_1     GPIOB->BSRR=DS1302_PIN_DATA

#define RES_Reset_0  GPIOB->BRR=DS1302_PIN_RES
#define RES_Set_1    GPIOB->BSRR=DS1302_PIN_RES

#define IO_Read   GPIO_ReadInputDataBit(GPIOB,DS1302_PIN_DATA)
/*************************/
#define Time_24_Hour    0x00    //24小时制
#define Time_Start     0x00    //开始走时
////DS1302寄存器操作指令定义时间地址
#define ds1302_sec_addr   0x80       //秒
#define ds1302_min_addr   0x82       //分
#define ds1302_hour_addr  0x84       //时
#define ds1302_day_addr   0x86       //日
#define ds1302_month_addr 0x88       //月
#define ds1302_year_addr  0x8c       //年

#define ds1302_year   0x8D            //年
#define ds1302_month  0x89            //月
#define ds1302_day    0x87            //日
#define ds1302_hour   0x85            //时
#define ds1302_min    0x83            //分
#define ds1302_sec    0x81            //秒

/********************/
void DS1302_GPIOInit(void);
void DS1302_IO_GPIO(unsigned char FLAG);//配置IO的方向
void DS1302_delay(u8 dd);
void DS1302_Write(unsigned char add,unsigned char dat);
unsigned char DS1302_Read(unsigned char add);
void DS1302_SetTime(unsigned char *ad);
void DS1302_OFF(void);
void DS1302_ON(void);
void DS1302_init(void);

#endif
