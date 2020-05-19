#include "adc.h"
#include "delay.h"	
#include "lcd.h"
////////////////////////
/////////////////////////////////////////////////////////// 	

uint16_t   adcx;                        //AD�ɼ���Դ��ƽֵ
u16 i=0;	
extern uint8_t   sys_ready;
//��ʼ��ADC															   
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��

  //�ȳ�ʼ��ADC1ͨ��5 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PA1 ͨ��1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
 
	ADC_Cmd(ADC1, ENABLE);//����ADת����	

}				  
//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
////����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
	 

void VBAT(void)
{
	i++;
	adcx=Get_Adc_Average(ADC_Channel_1,3);//��ȡͨ��1��ת��ֵ��3��ȡƽ��
	
	
	if((i>=30)||(sys_ready==0))
	{
		if(adcx>=0xFB0)
		{
			LCD_ShowString(0,0,210,16,16,"~99%    "); 	//��ѹ����4.2
		}	
		else if((adcx<0xFB0)&&(adcx>=0xF00))
		{
//			LCD_ShowString(143,0,210,16,16,"   "); 
			LCD_ShowString(0,0,210,16,16,"~98%    "); 	//��ѹ��3.95~4.00֮��
		}
		else if((adcx<0xF00)&&(adcx>=0xEBE))
		{
//			LCD_ShowString(143,0,210,16,16,"   "); 
			LCD_ShowString(0,0,210,16,16,"~90%    "); 	//��ѹ��3.90~3.85֮��
		}
		else if((adcx<0xEBE)&&(adcx>=0xDFD))
		{
//			LCD_ShowString(143,0,210,16,16,"   "); 
			LCD_ShowString(0,0,210,16,16,"~80%    "); 	//��ѹ��3.75~3.65֮��
		}
		else if(adcx<0xDFD)
		{
//			LCD_ShowString(143,0,210,16,16,"   "); 
			LCD_ShowString(0,0,210,16,16,"~00%    "); 	//��ѹ��3.60һ����Ϊ�޵�
		}			
		i=0;
	}
}







