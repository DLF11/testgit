#include "power.h" 
#include "key.h"
#include "led.h"
#include "lcd.h"
#include "data.h"
#include "adc.h"


extern uint16_t   adcx;                        //AD采集电源点平值
u8 j=0;


void powe(void)
{
	
	
	CHARG_EN=0;	
	if(DEC==1)	      									    //开机检测充电线是否插入为高，及进入充电检测
	{													   //
		CHARG_EN=1;		                                  //使能充电开关
		if((BAT==1)&&(adcx>=0xFBA))						 //再次检测充电芯片，chrg引脚确认高电平为充电完成，
		{												// 						
			LCD_ShowString(50,0,210,24,16,"#$'&<>?@        "); 
			j=1;
		}								 			  //
		else										 //chrg引脚确认低电平为充电中，
		{		
			LCD_ShowString(50,0,210,24,16," #$'&{}......    "); 
			j=1;
		}
	}else
	{
		
		LCD_ShowString(50,0,210,24,16,"                "); 		
		j=0;	
		CHARG_EN=0;
	}
	
	
}



