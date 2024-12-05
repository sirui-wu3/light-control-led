/*单次转换，非扫描模式ADC*/
#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//开启ADC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//开启GPIOA时钟
	/*配置ADCCLK分频器*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//72MHz进行6分频
	/*配置GPIO*/
	GPIO_InitTypeDef GPIO_InitStructure;//结构体命名
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//设置GPIO模式，模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PA1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化结构体
	/*选择输入通道*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);//ADC1,通道0，序列1，采样时间为55个ADCCLK周期
	/*配置ADC*/
	ADC_InitTypeDef ADC_InitStructure;//结构体命名
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC模式为独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//指定数据为右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发转换选择，不使用外部触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//连续转换模式，disable为单次模式，enable是连续模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//扫描转换模式，disable为非扫描模式(单通道)，enable是扫描模式(多通道)
	ADC_InitStructure.ADC_NbrOfChannel = 1;//通道数目，1-16
	ADC_Init(ADC1, &ADC_InitStructure);//初始化结构体
	/*开启ADC电源*/
	ADC_Cmd(ADC1, ENABLE);
	/*对ADC进行校准*/
	ADC_ResetCalibration(ADC1);//复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);//等待复位校准完成
	ADC_StartCalibration(ADC1);//开始校准
	while (ADC_GetCalibrationStatus(ADC1) == SET);//等待校准完成
}
/*获取ADC结果*/
uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//软件触发转换ADC
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//获取标志位状态，等待转换完成。5.5周期下5.6微秒完成
	return ADC_GetConversionValue(ADC1);//返回ADC值
}
