#include "led.h" 
#include "delay.h"

extern uint8_t   G_led_download;	



void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE |RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOE，GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_12;      //E13,14  OUT4输出口，PE6充电检测使能脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                             //普通输出模式																								
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                           //推挽输出																									
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  		            //100MHz																							
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                           //上拉																												
	GPIO_Init(GPIOE, &GPIO_InitStructure);               		          //初始化GPIO																											

	GPIO_ResetBits(GPIOE,  GPIO_Pin_6);	                                //
	GPIO_SetBits(GPIOB,  GPIO_Pin_4);		

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;      //E13,14  OUT4输出口，PE6充电检测使能脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                             //普通输出模式																								
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                           //推挽输出																									
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  		            //100MHz																							
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                           //上拉																												
	GPIO_Init(GPIOB, &GPIO_InitStructure);               		          //初始化GPIO																											

	GPIO_SetBits(GPIOB,  GPIO_Pin_4);		

	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_11;            		           //A11    VCC使能脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          			          //普通输出模式																								
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         			         //推挽输出																									
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    			        //100MHz																							
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         			       //上拉																												
	GPIO_Init(GPIOA, &GPIO_InitStructure);             				      //初始化GPIO																											
	GPIO_SetBits(GPIOA,  GPIO_Pin_11);	          				     //	
	
	
	
}








