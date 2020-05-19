#include "key.h"
#include "delay.h"
#include "data.h"
#include "lcd.h"
#include "led.h"

extern uint8_t   G_led_download;	
extern uint8_t   run;	
extern uint8_t   flag_Ka;
extern uint16_t   time;						//待机标志


void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟
																																				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 ; 											//B0-IN1A,B1-IN1B
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;														//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;												//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;														//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);															//
																																					
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;	//C0-IN3A,C1-IN3B,C2-IN2A,C3-IN2B,C4-IN4A,C5-IN4B
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;														//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;												//100M
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP;														//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);																		//初始化GPIOC0,2,4
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_0| GPIO_Pin_11;	 //E1-L,E2-R,E3-S,E4-F,E0-BAT,E11-DEC
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;														//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;												//100M
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP;														//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);
} 



void key_L(void)
{
	 if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0)
	{
		delay_ms(20);
		 if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0)
		{
			G_led_download=0;
			run=0;
			LCD_DisplayJiuGongGe();
			out_init();		//输出状态初始化
			UI_init();
		}
	}
	 if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==0)
	{
		OUT1NO();
		OUT2NO();
		OUT3NO();
		OUT4NO();		
		delay_ms(200);
		OUT1ON();
		OUT2ON();
		OUT3ON();
		OUT4ON();		
		time=0;
	}
	 else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0)
	{
		OUT1NO();
		OUT2NO();
		OUT3NO();
		OUT4NO();		
		delay_ms(200);
		OUT1OFF();
		OUT2OFF();
		OUT3OFF();
		OUT4OFF();	
		time=0;		
	}


		
//	uint8_t  i;
//	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)==0)
//	{
//		i++;
//		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==0)
//		{
//			if(i==1)
//			{
//				OUT1ON();
//			}
//			else if(i==2)
//			{
//				OUT2ON();
//			}
//			else if(i==3)
//			{
//				OUT3ON();
//			}
//			else if(i==4)
//			{
//			OUT4A=1;
//			OUT4B=0;			
//			}
//			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==0);
//		}
//		else
//		{
//			OUT1ON();
//			OUT2ON();
//			OUT3ON();
//			OUT4A=1;
//			OUT4B=0;			
//		}
//		
//		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0)
//		{
//			if(i==1)
//			{
//				OUT1ON();
//			}
//			else if(i==2)
//			{
//				OUT2ON();
//			}
//			else if(i==3)
//			{
//				OUT3ON();
//			}
//			else if(i==4)
//			{
//			OUT4A=1;
//			OUT4B=0;			
//			}
//			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0);
//		}
//		else
//		{
//			OUT1ON();
//			OUT2ON();
//			OUT3ON();
//			OUT4A=1;
//			OUT4B=0;			
//		}
//		if((i==5)||(run!=0)||(G_led_download!=0)||(flag_Ka!=0))
//		{
//			i=0;
//		}
//		if(i==1)
//		{
//			LCD_ShowString(10,90,200,16,16,"OUT1 ON");
//		}
//		else if(i==2)
//		{
//			LCD_ShowString(10,90,200,16,16,"OUT2 ON");
//		}
//		else if(i==3)
//		{
//			LCD_ShowString(10,90,200,16,16,"OUT3 ON");
//		}
//		else if(i==4)
//		{
//			LCD_ShowString(10,90,200,16,16,"OUT4 ON");
//		}
//		
//		
//		while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)==0);
//	}
}


	









