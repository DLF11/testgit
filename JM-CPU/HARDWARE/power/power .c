#include "power.h" 
#include "key.h"
#include "led.h"
#include "lcd.h"
#include "data.h"
#include "adc.h"


extern uint16_t   adcx;                        //AD�ɼ���Դ��ƽֵ
u8 j=0;


void powe(void)
{
	
	
	CHARG_EN=0;	
	if(DEC==1)	      									    //������������Ƿ����Ϊ�ߣ�����������
	{													   //
		CHARG_EN=1;		                                  //ʹ�ܳ�翪��
		if((BAT==1)&&(adcx>=0xFBA))						 //�ٴμ����оƬ��chrg����ȷ�ϸߵ�ƽΪ�����ɣ�
		{												// 						
			LCD_ShowString(50,0,210,24,16,"#$'&<>?@        "); 
			j=1;
		}								 			  //
		else										 //chrg����ȷ�ϵ͵�ƽΪ����У�
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



