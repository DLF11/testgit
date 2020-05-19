#include "led.h" 
#include "delay.h"

extern uint8_t   G_led_download;	



void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE |RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOE��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_12;      //E13,14  OUT4����ڣ�PE6�����ʹ�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                             //��ͨ���ģʽ																								
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                           //�������																									
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  		            //100MHz																							
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                           //����																												
	GPIO_Init(GPIOE, &GPIO_InitStructure);               		          //��ʼ��GPIO																											

	GPIO_ResetBits(GPIOE,  GPIO_Pin_6);	                                //
	GPIO_SetBits(GPIOB,  GPIO_Pin_4);		

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;      //E13,14  OUT4����ڣ�PE6�����ʹ�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                             //��ͨ���ģʽ																								
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                           //�������																									
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  		            //100MHz																							
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                           //����																												
	GPIO_Init(GPIOB, &GPIO_InitStructure);               		          //��ʼ��GPIO																											

	GPIO_SetBits(GPIOB,  GPIO_Pin_4);		

	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_11;            		           //A11    VCCʹ�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          			          //��ͨ���ģʽ																								
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         			         //�������																									
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    			        //100MHz																							
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         			       //����																												
	GPIO_Init(GPIOA, &GPIO_InitStructure);             				      //��ʼ��GPIO																											
	GPIO_SetBits(GPIOA,  GPIO_Pin_11);	          				     //	
	
	
	
}








