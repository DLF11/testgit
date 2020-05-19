#include "data.h"
#include "usart.h"	
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart2.h" 
#include "lcd.h"
#include "iwdg.h"




extern uint32_t  usart1_buffer_len;	 //串口1接收数据长度
extern uint32_t  usart1_buffer_data[60];
extern uint8_t   usart1_ka;
extern uint8_t   rc;
extern uint8_t   sys_ready;
extern uint8_t   	G_ka_num;
extern uint8_t   G_cond;										//条件通过标志
extern uint8_t   	G_ka_num;
extern uint8_t     run;	
extern uint8_t  audioplay;
extern uint8_t   G_led_download;	

uint16_t H1=600;			//组合卡变量
uint16_t H0=0,H2=0,H3=0,H4=0,H5=0,H6=0,H7=0;			//组合卡变量
uint16_t S1, S2, S3, S4;			//随机变量

int8_t k4=0,k5=0;
int8_t   j1=0,j2=0,j3=0,j4=0,j5=0,j6=0,j7=0,j8=0,j9=0,j10=0;	//0x0A为动作卡，0x09为条件卡
int8_t   i1=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0,i8=0,i9=0;	//i为逻辑开始个数，j为逻辑结束个数
int8_t   cond1=1,cond2=1,cond3=1,cond4=1,cond5=1,cond6=1,cond7=1,cond8=1,cond9=1;

void OUT1NO(void)
{
	LCD_ShowString(0,80,100,10,16," OUT1 ");
	LCD_ShowString(0,95,100,10,16,"......");
	TIM_SetCompare3(TIM4,500);
	TIM_SetCompare4(TIM4,500);
}
void OUT1ON(void)
{
	LCD_ShowString(0,80,100,10,16," OUT1 ");
	LCD_ShowString(0,95,100,10,16,"[]/|{}");
	TIM_SetCompare3(TIM4,0);
	TIM_SetCompare4(TIM4,500);
}

void OUT1OFF(void)
{
	LCD_ShowString(0,80,100,10,16," OUT1 ");
	LCD_ShowString(0,95,100,10,16,"[]/|{}");
	TIM_SetCompare3(TIM4,500);
	TIM_SetCompare4(TIM4,0);
}


void OUT2NO(void)
{
	LCD_ShowString(0,115,100,16,16," OUT2 ");
	LCD_ShowString(0,130,100,16,16,"......");
	TIM_SetCompare1(TIM3,500);	
	TIM_SetCompare2(TIM3,500);
}
void OUT2ON(void)
{
	LCD_ShowString(0,115,100,16,16," OUT2 ");
	LCD_ShowString(0,130,100,16,16,"[]/|{}");
	TIM_SetCompare1(TIM3,0);	
	TIM_SetCompare2(TIM3,500);
}
void OUT2OFF(void)
{
	LCD_ShowString(0,115,100,16,16," OUT2 ");
	LCD_ShowString(0,130,100,16,16,"[]/|{}");
	TIM_SetCompare1(TIM3,500);
	TIM_SetCompare2(TIM3,0);
}


void OUT3NO(void)
{
	LCD_ShowString(0,150,100,16,16," OUT3 ");
	LCD_ShowString(0,165,100,16,16,"......");
	TIM_SetCompare1(TIM4,500);
	TIM_SetCompare2(TIM4,500);
}
void OUT3ON(void)
{
	LCD_ShowString(0,150,100,16,16," OUT3 ");
	LCD_ShowString(0,165,100,16,16,"[]/|{}");
	TIM_SetCompare1(TIM4,0);
	TIM_SetCompare2(TIM4,500);
}
void OUT3OFF(void)
{
	LCD_ShowString(0,150,100,16,16," OUT3 ");
	LCD_ShowString(0,165,100,16,16,"[]/|{}");
	TIM_SetCompare1(TIM4,500);
	TIM_SetCompare2(TIM4,0);
}

void OUT4NO(void)
{
	LCD_ShowString(0,185,100,16,16," OUT4 ");
	LCD_ShowString(0,200,100,16,16,"......");	
	TIM_SetCompare3(TIM1,500);
	TIM_SetCompare4(TIM1,500);
}
void OUT4ON(void)
{	
	LCD_ShowString(0,185,100,16,16," OUT4 ");
	LCD_ShowString(0,200,100,16,16,"[]/|{}");
	TIM_SetCompare3(TIM1,0);
	TIM_SetCompare4(TIM1,500);
}
void OUT4OFF(void)
{
	LCD_ShowString(0,185,100,16,16," OUT4 ");
	LCD_ShowString(0,200,100,16,16,"[]/|{}");
	TIM_SetCompare3(TIM1,500);
	TIM_SetCompare4(TIM1,0);
}



void UI_IN1(void)
{
	LCD_ShowString(130,80,100,16,16,"  IN1 ");
	LCD_ShowString(130,95,100,16,16,"[](){}");	
}
void UI_IN2(void)
{
    LCD_ShowString(130,115,100,16,16,"  IN2 ");
	LCD_ShowString(130,130,100,16,16,"[](){}");
}

void UI_IN3(void)
{
		LCD_ShowString(130,150,100,16,16,"  IN3 ");
		LCD_ShowString(130,165,100,16,16,"[](){}");
}
void UI_IN4(void)
{
		LCD_ShowString(130,185,100,10,16,"  IN4 ");
		LCD_ShowString(130,200,100,10,16,"[](){}");
}
void UI_IN(void)
{
	if(IN1_A==OFF)
	{
		LCD_ShowString(130,80,100,16,16,"  IN1 ");
		LCD_ShowString(130,95,100,16,16,"......");		
	}
	if(IN2_A==OFF)
	{
		LCD_ShowString(130,115,100,16,16,"  IN2 ");
		LCD_ShowString(130,130,100,16,16,"......");
	}
	if(IN3_A==OFF)
	{
		LCD_ShowString(130,150,100,16,16,"  IN3 ");
		LCD_ShowString(130,165,100,16,16,"......");	
	}
	if(IN4_A==OFF)
	{
		LCD_ShowString(130,185,100,10,16,"  IN4 ");
		LCD_ShowString(130,200,100,10,16,"......");	
	}	
				
}



void ka_A(uint8_t k)
{
	 switch(usart1_buffer_data[k]+1)
	 {
		case 1:
			OUT1ON();
		break;
 
		case 2:
			OUT2ON();
		break;
 
		case 3:
			OUT3ON();
		break;
		
		case 4:
			OUT4ON();
			
		break;
		
		case 5:
			OUT1NO();
			
		break;
		
		case 6:
			OUT2NO();
			
		break;
		
		case 7:
			OUT3NO();
			
		break;
		
		case 8:
			OUT4NO();
			
		break;
		
		case 9:
			delay_ms(500);
		break;
		
		case 10:
			delay_ms(1000);
		break;
		
		case 11:
			delay_ms(200);
		break;
		
		case 12:
			delay_ms(5000);
		break;
		
		case 13:
			LCD_ShowString(0,80,100,10,16," OUT1 ");
	        LCD_ShowString(0,95,100,10,16,"[]/|{}");
			LCD_ShowString(0,115,100,16,16," OUT2 ");
			LCD_ShowString(0,130,100,16,16,"[]/|{}");
			OUT1ON();
			OUT2OFF();
		break;
 
		case 14:
			LCD_ShowString(0,80,100,10,16," OUT1 ");
	        LCD_ShowString(0,95,100,10,16,"[]/|{}");
			LCD_ShowString(0,115,100,16,16," OUT2 ");
			LCD_ShowString(0,130,100,16,16,"[]/|{}");
			OUT1OFF();	

			OUT2ON();	
		break;
	 
		case 15:
			LCD_ShowString(0,80,100,10,16," OUT1 ");
	        LCD_ShowString(0,95,100,10,16,"[]/|{}");
			LCD_ShowString(0,115,100,16,16," OUT2 ");
			LCD_ShowString(0,130,100,16,16,"[]/|{}");
			OUT1ON();
			TIM_SetCompare1(TIM3,500);	
			TIM_SetCompare2(TIM3,300);
		break;
	 
		case 16:
			LCD_ShowString(0,80,100,10,16," OUT1 ");
	        LCD_ShowString(0,95,100,10,16,"[]/|{}");
			LCD_ShowString(0,115,100,16,16," OUT2 ");
			LCD_ShowString(0,130,100,16,16,"[]/|{}");
			TIM_SetCompare3(TIM4,300);
			TIM_SetCompare4(TIM4,500);
			OUT2OFF();	
		break;
			
		case 17:
			OUT1NO();
			OUT2NO();
			
		break;	  
		case 18:
//cond1=1;cond2=1;cond3=1;cond4=1;cond5=1;cond6=1;cond7=1;cond8=1;cond9=1;
//	if(usart1_buffer_data[4]==0x0A)				//第一张卡为动作卡
//	{	
//		//一个新逻辑
//		if((i1!=0)&&(i2==0))
//		{
//			//1个逻辑
//			if((j1!=0)&&(j2==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//			}
//		}
//		
//		//两个新逻辑
//		if((i2!=0)&&(i3==0))
//		{
//			//1个逻辑
//			if((j1!=0)&&(j2==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//			}
//		
//			//2个逻辑
//			if((j2!=0)&&(j3==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//			}		
//		}
//		
//		//三个新逻辑
//		if((i3!=0)&&(i4==0))
//		{
//			//2个逻辑
//			if((j2!=0)&&(j3==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//			}		
//			
//			
//			//3个逻辑
//			if((j3!=0)&&(j4==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i3;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//			}		
//		}
//		
//		//四个新逻辑
//		if((i4!=0)&&(i5==0))
//		{
//			//3个逻辑
//			if((j3!=0)&&(j4==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i3;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//			}	
//			
//			
//			//4个逻辑
//			if((j4!=0)&&(j5==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i3;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i4;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//			}	
//		}	
//		//五个新逻辑
//		if((i5!=0)&&(i6==0))
//		{
//			//4个逻辑
//			if((j4!=0)&&(j5==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i3;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i4;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//			}	
//			
//			
//			//5个逻辑
//			if((j5!=0)&&(j6==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i3;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i4;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i5;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//			}			
//		}
//		
//		//六个新逻辑
//		if((i6!=0)&&(i7==0))
//		{
//			//5个逻辑
//			if((j5!=0)&&(j6==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i3;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i4;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i5;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//			}
//			
//			//6个逻辑
//			if((j6!=0)&&(j7==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i3;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i4;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i5;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//				for(k5=i6;k5<j6;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond6=G_cond&cond6;
//				}
//			}
//		}
//		
//		//七个新逻辑
//		if((i7!=0)&&(i8==0))
//		{
//			//6个逻辑
//			if((j6!=0)&&(j7==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i3;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i4;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i5;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//				for(k5=i6;k5<j6;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond6=G_cond&cond6;
//				}
//			}

//			
//			//7个逻辑
//			if((j7!=0)&&(j8==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i3;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i4;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i5;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//				for(k5=i6;k5<j6;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond6=G_cond&cond6;
//				}
//				for(k5=i7;k5<j7;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond7=G_cond&cond7;
//				}
//			}
//		}		
//		
//		//八个新逻辑
//		if((i8!=0)&&(i9==0))
//		{
//			//7个逻辑
//			if((j7!=0)&&(j8==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i3;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i4;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i5;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//				for(k5=i6;k5<j6;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond6=G_cond&cond6;
//				}
//				for(k5=i7;k5<j7;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond7=G_cond&cond7;
//				}
//			}
//			
//			//8个逻辑
//			if((j8!=0)&&(j9==0))
//			{
//				for(k5=i1;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i2;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i3;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i4;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i5;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//				for(k5=i6;k5<j6;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond6=G_cond&cond6;
//				}
//				for(k5=i7;k5<j7;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond7=G_cond&cond7;
//				}
//				for(k5=i8;k5<j8;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond8=G_cond&cond8;
//				}				
//			}
//		}		
//		
//	}	
//	else if(usart1_buffer_data[4]==0x09)		//第一张卡位条件卡
//	{
//		//无新逻辑
//		if(i1==0)
//		{
//			//0个逻辑，不执行
//			//1个逻辑
//			if((j1!=0)&&(j2==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//			}
//		}
//		//一个新逻辑
//		if((i1!=0)&&(i2==0))
//		{
//			//1个逻辑
//			if((j1!=0)&&(j2==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//			}

//			//2个逻辑
//			if((j2!=0)&&(j3==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//			}				
//		}
//			
//		//两个新逻辑
//		if((i2!=0)&&(i3==0))
//		{
//			//2个逻辑
//			if((j2!=0)&&(j3==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//			}		
//			
//			//3个逻辑
//			if((j3!=0)&&(j4==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//			}		
//			
//		}
//		
//		//三个新逻辑
//		if((i3!=0)&&(i4==0))
//		{
//			//3个逻辑
//			if((j3!=0)&&(j4==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//			}		
//			
//			//4个逻辑
//			if((j4!=0)&&(j5==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i3;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//			}		
//			
//		}
//		
//		//四个新逻辑
//		if((i4!=0)&&(i5==0))
//		{
//			//4个逻辑
//			if((j4!=0)&&(j5==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i3;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//			}		
//			
//			//5个逻辑
//			if((j5!=0)&&(j6==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i3;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i4;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//			}		
//		}
//		
//		//五个新逻辑
//		if((i5!=0)&&(i6==0))
//		{
//			//5个逻辑
//			if((j5!=0)&&(j6==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i3;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i4;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//			}			
//			
//			//6个逻辑
//			if((j6!=0)&&(j7==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i3;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i4;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//				for(k5=i5;k5<j6;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond6=G_cond&cond6;
//				}
//			}
//		}
//		
//		//六个新逻辑
//		if((i6!=0)&&(i7==0))
//		{
//			//6个逻辑
//			if((j6!=0)&&(j7==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i3;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i4;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//				for(k5=i5;k5<j6;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond6=G_cond&cond6;
//				}
//			}
//			//7个逻辑
//			if((j7!=0)&&(j8==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i3;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i4;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//				for(k5=i5;k5<j6;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond6=G_cond&cond6;
//				}
//				for(k5=i6;k5<j7;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond7=G_cond&cond7;
//				}
//			}
//		}

//		//七个新逻辑
//		if((i7!=0)&&(i8==0))
//		{
//			//7个逻辑
//			if((j7!=0)&&(j8==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i3;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i4;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//				for(k5=i5;k5<j6;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond6=G_cond&cond6;
//				}
//				for(k5=i6;k5<j7;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond7=G_cond&cond7;
//				}
//			}
//			//8个逻辑
//			if((j8!=0)&&(j9==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i3;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i4;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//				for(k5=i5;k5<j6;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond6=G_cond&cond6;
//				}
//				for(k5=i6;k5<j7;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond7=G_cond&cond7;
//				}
//				for(k5=i8;k5<j9;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond8=G_cond&cond8;
//				}
//			}
//		}		
//		//八个新逻辑
//		if((i8!=0)&&(i9==0))
//		{
//			//8个逻辑
//			if((j8!=0)&&(j9==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i3;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i4;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//				for(k5=i5;k5<j6;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond6=G_cond&cond6;
//				}
//				for(k5=i6;k5<j7;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond7=G_cond&cond7;
//				}
//				for(k5=i7;k5<j8;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond8=G_cond&cond8;
//				}
//			}
//			//9个逻辑
//			if((j9!=0)&&(j10==0))
//			{
//				for(k5=0;k5<j1;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond1=G_cond&cond1;
//				}
//				for(k5=i1;k5<j2;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond2=G_cond&cond2;
//				}
//				for(k5=i2;k5<j3;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond3=G_cond&cond3;
//				}
//				for(k5=i3;k5<j4;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond4=G_cond&cond4;
//				}
//				for(k5=i4;k5<j5;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond5=G_cond&cond5;
//				}
//				for(k5=i5;k5<j6;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond6=G_cond&cond6;
//				}
//				for(k5=i6;k5<j7;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond7=G_cond&cond7;
//				}
//				for(k5=i7;k5<j8;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond8=G_cond&cond8;
//				}
//				for(k5=i8;k5<j9;k5++)
//				{
//					ka_9((k5+1)*2+3);
//					cond9=G_cond&cond9;
//				}
//			}
//		}		
//	}
//	if((cond1==1)&&(cond2==1)&&(cond3==1)&&(cond4==1)&&(cond5==1)&&(cond6==1)&&(cond7==1)&&(cond8==1)&&(cond9==1))	
//	{
//	
//	}
//	else
//	{
		if((G_led_download==0)&&(run==1))
		{
			if(usart1_buffer_data[4]==0x0A)
			{
				if(k4==(i1-1))
				{
					k4=0-1;
				}
				else if(k4==(i2-1))
				{
					k4=j1-1;
				}
				else if(k4==(i3-1))
				{
					k4=j2-1;				
				}
				else if(k4==(i4-1))
				{
					k4=j3-1;				
				}
				else if(k4==(i5-1))
				{
					k4=j4-1;			
				}
				else if(k4==(i6-1))
				{
					k4=j5-1;				
				}
				else if(k4==(i7-1))
				{
					k4=j6-1;				
				}
				else if(k4==(i8-1))
				{
					k4=j7-1;				
				}
				else if(k4==(i9-1))
				{
					k4=j8-1;				
				}
				else if(k4==(G_ka_num-1))
				{
					if(j1==0)
					{
						k4=0-1;					
					}
					else if(j10!=0)
					{
						k4=j10-1;					
					}					
					else if(j9!=0)
					{
						k4=j9-1;						
					}
					else if(j8!=0)
					{
						k4=j8-1;						
					}
					else if(j7!=0)
					{
						k4=j7-1;					
					}
					else if(j6!=0)
					{
						k4=j6-1;						
					}
					else if(j5!=0)
					{
						k4=j5-1;						
					}
					else if(j4!=0)
					{
						k4=j4-1;						
					}
					else if(j3!=0)
					{
						k4=j3-1;						
					}
					else if(j2!=0)
					{
						k4=j2-1;						
					}
					else if(j1!=0)
					{
						k4=j1-1;						
					}
				}
			}
			else
			{
				if(k4==(i1-1))
				{
					k4=j1-1;
				}
				else if(k4==(i2-1))
				{
					k4=j2-1;
				}
				else if(k4==(i3-1))
				{
					k4=j3-1;				
				}
				else if(k4==(i4-1))
				{
					k4=j4-1;				
				}
				else if(k4==(i5-1))
				{
					k4=j5-1;			
				}
				else if(k4==(i6-1))
				{
					k4=j6-1;				
				}
				else if(k4==(i7-1))
				{
					k4=j7-1;				
				}
				else if(k4==(i8-1))
				{
					k4=j8-1;				
				}
				else if(k4==(i9-1))
				{
					k4=j9-1;				
				}
				else if(k4==(G_ka_num-1))
				{
					if(j10!=0)
					{
						k4=j10-1;					
					}					
					else if(j9!=0)
					{
						k4=j9-1;						
					}
					else if(j8!=0)
					{
						k4=j8-1;						
					}
					else if(j7!=0)
					{
						k4=j7-1;					
					}
					else if(j6!=0)
					{
						k4=j6-1;						
					}
					else if(j5!=0)
					{
						k4=j5-1;						
					}
					else if(j4!=0)
					{
						k4=j4-1;						
					}
					else if(j3!=0)
					{
						k4=j3-1;						
					}
					else if(j2!=0)
					{
						k4=j2-1;						
					}
					else if(j1!=0)
					{
						k4=j1-1;						
					}
				}			
			}		
		}	
//	}
		break;			
		default:
		    OUT1NO();
			OUT2NO();
			OUT3NO();
			OUT4NO();
			run=0;
		break;
	}
}	
void ka_9(uint8_t g)
 {
	switch(usart1_buffer_data[g]+89)
	{
		case 90:
			if(IN1_A==ON)
			{
				UI_IN1();
				delay_ms(20);
				if(IN1_A==ON)
				{
					G_cond=1;
				}
			}else G_cond=0;
			break;
		case 91:
			if(IN2_A==0)
			{
				UI_IN2();
				delay_ms(20);
				if(IN2_A==0)
				{
					G_cond=1;
				}
			}else G_cond=0;
			break;
		case 92:
			if(IN3_A==ON)
			{
				UI_IN3();
				delay_ms(20);
				if(IN3_A==ON)
				{
					G_cond=1;
				}
			}else G_cond=0;
			break;
		case 100:					
			if(IN3_A==OFF)
			{
				delay_ms(20);
				if(IN3_A==OFF)
				{
					G_cond=1;
				}
			}else G_cond=0;
			break;
		case 93:
			if(IN1_A==OFF&&(IN2_A==OFF)&&(IN3_A==OFF))
			{
				
				delay_ms(20);

			    G_cond=1;
				
			}else G_cond=0;
			break;
		case 94:
			if(rc==UP)
			{
				delay_ms(20);
				if(rc==UP)
				{
					G_cond=1;
				}
			}else G_cond=0;
			break;
		case 95:
			if(rc==DOWN)
			{
				delay_ms(20);
				if(rc==DOWN)
				{
					G_cond=1;
				}
			}else G_cond=0;
			break;
		case 96:
			if(rc==LEFT)
			{
				delay_ms(20);
				if(rc==LEFT)
				{
					G_cond=1;
				}
			}else G_cond=0;
			break;
		case 97:	
			if(rc==RIGHT)
			{
				delay_ms(20);
				if(rc==RIGHT)
				{
					G_cond=1;
				}
			}else G_cond=0;
			break;
		case 98:
			if(rc==R)
			{
				delay_ms(20);
				if(rc==R)
				{
					G_cond=1;
				}
			}else G_cond=0;
			break;
		case 99:
			if(rc==S)
			{
				delay_ms(20);
				if(rc==S)
				{
					G_cond=1;
				}
			}else G_cond=0;
			break;

		default:
				G_cond=0;
		break;
	}
	
}



void ka_H(uint8_t h)
{
	 if(H0==1)
	 {
		 H2=0;
		 H3=0;
		 H4=0;
		 H5=0;
		 H6=0;
		 H7=0;
	 }
	 S1++;
	 switch(h)
	 {
		case 1:               
		  OUT1ON();
		  OUT2ON();
		
		  delay_ms(500);
		  OUT1NO();
		  OUT2NO();		  
		  delay_ms(400);		
		break;
			
		case 2:
		  OUT1ON();
		  OUT2NO();
		  delay_ms(500);
		  OUT1NO();
		  OUT2ON();
		  delay_ms(400);		
		break;
		
		case 3:
		  OUT1ON();
		  OUT2NO();
		  delay_ms(H1);
		  OUT1NO();
		  OUT2ON();
		  delay_ms(H1-100);
		  H1=H1-50;	
		  if(H1<150) {H1=600;}
		break;
		 
		case 4:
			H2++;
		  if(H2<6)
		  {
			  OUT1ON();
			  OUT2NO();
			  delay_ms(500);
			  OUT1NO();
			  OUT2ON();
			  delay_ms(400);		  
		  }
		  else if((H2>=6)&&(H2<11))
		  {
			  OUT1ON();
			  OUT2ON();
			  delay_ms(500);
			  OUT1NO();
			  OUT2NO();		  
			  delay_ms(400);			  
		  }
		  else if((H2>=11)&&(H2<16))
		  {
			  OUT1NO();
			  OUT2ON();
			  delay_ms(500);
			  OUT1ON();
			  OUT2NO();
			  delay_ms(400);			  
		  }
		  else if((H2>=16)&&(H2<21))
		  {
			  OUT1ON();
			  OUT2ON();
			  delay_ms(500);	
			  H2=0;
		  }
		break;
		  
		case 6:
			OUT1ON();
			OUT2OFF();
			delay_ms(1000);
			OUT1NO();
			OUT2NO();
			run=0;
		break;
		
		case 7:
			OUT1OFF();
			OUT2ON();
			delay_ms(1000);
			OUT1NO();
			OUT2NO();
			run=0;
		break;
		
		case 8:
			OUT1ON();
			OUT2OFF();
		break;

		case 9:
			OUT1ON();
			OUT2OFF();
			delay_ms(2000);
			OUT1NO();
			OUT2NO();	
			delay_ms(200);		
			OUT1OFF();
			OUT2ON();
			delay_ms(1400);		
			OUT1NO();
			OUT2NO();	
			delay_ms(200);	
		break;
		
		case 14:
			OUT1ON();
			
			delay_ms(2000);
			OUT1OFF();
			
			delay_ms(1600);
		break;
		
		case 21:
			OUT1ON();
			OUT2ON();
			OUT3NO();		
			delay_ms(3000);
			OUT1OFF();
			OUT3ON();
			OUT2NO();		
			delay_ms(2600);
		break;
		
		case 22:
			OUT1OFF();
			OUT2ON();
			delay_ms(20000);
            run=0;

		break;
			
		case 23:
			delay_ms(5000);
			OUT1ON();
			OUT2ON();
			IWDG_Feed();//喂狗
			delay_ms(2000);
			OUT1OFF();
			delay_ms(2000);
			IWDG_Feed();//喂狗
			run=0;			
		break;
		
		case 24:
			while(run)
			{
				OUT1ON();
				delay_ms(6000);
				IWDG_Feed();//喂狗
				OUT1OFF();
				delay_ms(6000);
				IWDG_Feed();//喂狗
				OUT1ON();
				delay_ms(6000);
				IWDG_Feed();//喂狗
				OUT1OFF();
				delay_ms(6000);
				IWDG_Feed();//喂狗
				OUT1ON();
				delay_ms(6000);
				IWDG_Feed();//喂狗
				OUT1OFF();
				OUT2ON();
			}
		break;
		
		case 25:
			while(run)
			{
				OUT1ON();
				delay_ms(6000);
				OUT1OFF();
				IWDG_Feed();//喂狗
				delay_ms(6000);
				OUT1ON();
				IWDG_Feed();//喂狗
				delay_ms(6000);
				OUT1OFF();
				IWDG_Feed();//喂狗
				delay_ms(6000);
				OUT1ON();
				IWDG_Feed();//喂狗
				delay_ms(6000);
				OUT1OFF();
				IWDG_Feed();//喂狗
				delay_ms(6000);
				OUT1ON();
				IWDG_Feed();//喂狗
				delay_ms(4000);
				IWDG_Feed();//喂狗
				OUT1OFF();
				OUT2ON();
			}			
		break;
		
		case 26:
			if(IN1_A==ON)
			{
				UI_IN1();
				OUT1ON();
			}
			else if(IN2_A==ON)
			{
				UI_IN2();
				OUT2ON();
			}
			else if(IN3_A==ON)
			{
				UI_IN3();
				OUT1ON();
				OUT2ON();
			}
			else if((IN1_A!=ON)&&(IN2_A!=ON)&&(IN3_A!=ON))
			{
				
				OUT1NO();
				OUT2NO();
			}
			
		break;
		
		case 29:
			while(run)
			{
				IWDG_Feed();//喂狗
				S2=S1%10;
				if(S2==0)
				{
					OUT1ON();
					H3=1;
				}
				else if(S2==1)
				{
					OUT2ON();
					H4=1;
				}
				else if(S2==2)
				{
					OUT3ON();
					H5=1;
				}	
				else if(S2==3)
				{
					OUT1ON();
					OUT2ON();
					H3=1;
					H4=1;
				}	
				else if(S2==4)
				{
					OUT1ON();
					OUT3ON();
					H3=1;
					H5=1;
				}	
				else if(S2==5)
				{
					OUT2ON();
					OUT3ON();
					H4=1;
					H5=1;
				}				
				while(run)
				{
					IWDG_Feed();//喂狗
					S1++;
					if(IN1_A==ON)
					{
						UI_IN1();
						OUT1NO();
						H3=0;
					}
					else if(IN2_A==ON)
					{
						UI_IN2();
						OUT2NO();
						H4=0;
					}
					else if(IN3_A==ON)
					{
						UI_IN3();
						OUT3NO();
						H5=0;
					}	
					else if((H3==0)&&(H4==0)&&(H5==0))
					{
							
						delay_ms(100);
						break;
					}
					
				}
			}
		break;
		
		case 30:
			while(run)
			{
				IWDG_Feed();//喂狗
				if((IN1_A==ON)&&(H2==0))
				{
					delay_ms(50);
					if((IN1_A==ON)&&(H2==0))
					{
						UI_IN1();
						H2=1;
						OUT1ON();
						OUT2NO();
						OUT3NO();
					}
				}
				else if((IN1_A==ON)&&(H2==1))
				{
					delay_ms(50);
					if((IN1_A==ON)&&(H2==1))
					{
						UI_IN1();
						H2=2;
						OUT2ON();
						OUT1NO();
						OUT3NO();
					}
				}
				else if((IN1_A==ON)&&(H2==2))
				{
					delay_ms(50);
					if((IN1_A==ON)&&(H2==2))
					{	
						UI_IN1();
						H2=0;
						OUT3ON();
						OUT1NO();
						OUT2NO();	
					}
				}	
				while(IN1_A==ON);
		
			}

					
		break;
		
		case 31:
			if(IN1_A==ON)
			{
				UI_IN1();
				OUT1ON();
				OUT3ON();
				OUT2ON();
			}
			else  
			{
			
				OUT1NO();
				OUT2NO();
				OUT3NO();
			}
		break;
		
		case 32:
			H2=0;
			while(run)
			{
				IWDG_Feed();//喂狗			//防止大延时造成误复位
				if((IN1_A==ON)&&(H2==2))
				{
					delay_ms(50);
					if((IN1_A==ON)&&(H2==2))
					{
						UI_IN1();
						H2=0;
						LCD_ShowString(0,80,100,10,16," OUT1 ");
	                    LCD_ShowString(0,95,100,10,16,"[]/|{}");
						TIM_SetCompare3(TIM4,500);
						TIM_SetCompare4(TIM4,0);
						OUT2ON();
						OUT3ON();
					}
				}
				else if((IN1_A==ON)&&(H2==1))
				{
					delay_ms(50);
					if((IN1_A==ON)&&(H2==1))
					{
						UI_IN1();
						H2=2;
						LCD_ShowString(0,80,100,10,16," OUT1 ");
	                    LCD_ShowString(0,95,100,10,16,"[]/|{}");
						TIM_SetCompare3(TIM4,370);
						TIM_SetCompare4(TIM4,0);
						OUT2NO();
						OUT3ON();
					}
				}
				else if((IN1_A==ON)&&(H2==0))
				{
					delay_ms(50);
					if((IN1_A==ON)&&(H2==0))
					{	
						UI_IN1();				
						H2=1;
						LCD_ShowString(0,80,100,10,16," OUT1 ");
	                    LCD_ShowString(0,95,100,10,16,"[]/|{}");
						TIM_SetCompare3(TIM4,200);
						TIM_SetCompare4(TIM4,0);
						OUT2ON();
						OUT3NO();	
					}
				}	
				while(IN1_A==ON);
				
			}
		break;
		
		case 34:
			OUT1ON();
			OUT2OFF();
			delay_ms(2000);
			OUT1ON();
			OUT2NO();
			delay_ms(2000);			
			OUT1NO();
			OUT2OFF();
			delay_ms(2000);
			OUT1OFF();
			OUT2ON();
			delay_ms(2000);	
			run=0;
		break;
		
		case 35:
			OUT1ON();
			OUT2OFF();
			delay_ms(2000);
			OUT1NO();
			OUT2OFF();
			delay_ms(2000);			
			OUT1ON();
			OUT2NO();
			delay_ms(2000);
			OUT1ON();
			OUT2OFF();
			delay_ms(2000);	
			run=0;
		break;
		
		case 36:
			OUT1ON();
			OUT2OFF();
			delay_ms(2000);
			OUT1ON();
			OUT2NO();
			delay_ms(2000);			
			OUT1ON();
			OUT2OFF();
			delay_ms(2000);	
			OUT1ON();
			OUT2NO();
			delay_ms(2000);	
			OUT1ON();
			OUT2OFF();
			delay_ms(2000);	
			OUT1ON();
			OUT2NO();
			delay_ms(2000);	
			OUT1ON();
			OUT2NO();
			delay_ms(2000);	
			run=0;
		break;
		
		case 37:
			OUT1ON();
			OUT2OFF();
			OUT3ON();
			OUT4ON();
			delay_ms(1000);	
			OUT3NO();
			OUT4NO();
		    delay_ms(700);	
		break;
		
		case 38:
			if(IN1_A==ON)
			{
				delay_ms(50);	
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
				
			
			}
			else
			{
					delay_ms(50);
					OUT1NO();
					OUT2NO();			
			}
			
		break;
		
		case 39:
			if(IN1_A==ON)
			{
				delay_ms(50);	
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1OFF();
					OUT2ON();
				}
			
			}
			else
			{
					delay_ms(50);
					OUT1NO();
					OUT2NO();			
			}			
		break;
		
		case 40:
			if((IN1_A==ON)&&(H2==0))
			{
				delay_ms(50);	
				if((IN1_A==ON)&&(H2==0))
				{
					UI_IN1();
					H2=1;
					OUT1ON();
					OUT2OFF();
				}
			}
			else if((IN1_A==ON)&&(H2==1))
			{
				delay_ms(50);	
				if((IN1_A==ON)&&(H2==1))
				{
					UI_IN1();
					H2=0;
					OUT1NO();
					OUT2NO();
				}
			}
			while(IN1_A==ON);
			
		break;
		
		case 41:
			if(IN1_A==ON)
			{
				delay_ms(50);	
				if(IN1_A==ON)
				{		
				    UI_IN1();
					OUT1ON();
					OUT2OFF();
					delay_ms(1000);	
					OUT1NO();
					OUT2NO();
				}
			}
			
		break;
		
		case 42:
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{	
					UI_IN2();
					OUT1OFF();
					OUT2ON();
				}
			}
			
		break;
		
		case 44:
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{		
          			UI_IN2();	
					OUT1NO();
					OUT2NO();
				}
			}		
		break;
		
		case 46:
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{	
					UI_IN2();				
					OUT1OFF();
					OUT2ON();
				}
			}
			if(IN3_A==ON)
			{
				
				delay_ms(50);
				if(IN3_A==ON)
				{		
					UI_IN3();
					H6=1;
				}
			}
			if(H6==1)
			{
				OUT3ON();
				OUT4ON();
				delay_ms(100);
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{	
					UI_IN2();				
					OUT1OFF();
					OUT2ON();
				}
			}			
				OUT3ON();
				OUT4ON();
				delay_ms(100);
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{	
					UI_IN2();				
					OUT1OFF();
					OUT2ON();
				}
			}	
				OUT3ON();
				OUT4ON();
				delay_ms(100);
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{	
					UI_IN2();				
					OUT1OFF();
					OUT2ON();
				}
			}	
				OUT3ON();
				OUT4ON();
				delay_ms(100);
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{	
					UI_IN2();				
					OUT1OFF();
					OUT2ON();
				}
			}				
				OUT3NO();
				OUT4NO();
				delay_ms(100);		
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{	
					UI_IN2();				
					OUT1OFF();
					OUT2ON();
				}
			}		
				OUT3NO();
				OUT4NO();
				delay_ms(100);		
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{	
					UI_IN2();				
					OUT1OFF();
					OUT2ON();
				}
			}		
				OUT3NO();
				OUT4NO();
				delay_ms(100);		
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{	
					UI_IN2();				
					OUT1OFF();
					OUT2ON();
				}
			}		
				OUT3NO();
				OUT4NO();
				delay_ms(100);		
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{	
					UI_IN2();				
					OUT1OFF();
					OUT2ON();
				}
			}		
				OUT3NO();
				OUT4NO();
				delay_ms(100);		
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{	
					UI_IN2();				
					OUT1OFF();
					OUT2ON();
				}
			}					
			}
			
		break;
		
		case 47:
			if(IN1_A==ON)
			{
				delay_ms(50);
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1ON();
					OUT2OFF();
				}
			}
			if(IN2_A==ON)
			{
				delay_ms(50);
				if(IN2_A==ON)
				{	
					UI_IN2();				
					OUT1OFF();
					OUT2ON();
				}
			}
			if(IN3_A==ON)
			{
				delay_ms(50);
				if(IN3_A==ON)
				{	
					UI_IN3();	
					OUT3ON();
					OUT4ON();
				}
			}
			
		break;
		
		case 48:
			if(IN3_A==ON)
			{
				UI_IN3();
				LCD_ShowString(0,80,100,10,16," OUT1 ");
				LCD_ShowString(0,95,100,10,16,"[]/|{}");
				OUT1NO();
				OUT2ON();			
			}				
			else if(IN2_A==ON)
			{
				UI_IN2();
				OUT1ON();
				OUT2NO();				
				LCD_ShowString(0,115,100,16,16," OUT2 ");
				LCD_ShowString(0,130,100,16,16,"[]/|{}");	
			}			
			else if(IN1_A==ON)
			{
				UI_IN1();
				OUT1ON();
				OUT2OFF();			
				LCD_ShowString(0,80,100,10,16," OUT1 ");
				LCD_ShowString(0,95,100,10,16,"[]/|{}");
				LCD_ShowString(0,115,100,16,16," OUT2 ");
				LCD_ShowString(0,130,100,16,16,"[]/|{}");					
			}	
			else
			{
				OUT1NO();
				OUT2NO();	
				LCD_ShowString(0,80,100,10,16," OUT1 ");
				LCD_ShowString(0,95,100,10,16,"[]/|{}");
				LCD_ShowString(0,115,100,16,16," OUT2 ");
				LCD_ShowString(0,130,100,16,16,"[]/|{}");					
			}					
		break;
		
		case 49:
			if(IN3_A==ON)
			{
				UI_IN3();
				LCD_ShowString(0,80,100,10,16," OUT1 ");
				LCD_ShowString(0,95,100,10,16,"[]/|{}");
				OUT1NO();
				OUT2ON();	
				LCD_ShowString(0,150,100,16,16," OUT3 ");
				LCD_ShowString(0,165,100,16,16,"[]/|{}");				
				OUT3ON();	
				OUT4NO();
				
			}				
			else if(IN2_A==ON)
			{
				UI_IN2();
				OUT1ON();
				OUT2NO();				
				OUT4ON();
				OUT3NO();		
				LCD_ShowString(0,185,100,16,16," OUT4 ");
				LCD_ShowString(0,200,100,16,16,"[]/|{}");
				
			}			
			else if(IN1_A==ON)
			{
				UI_IN1();
				OUT1ON();
				OUT2OFF();	
				OUT3ON();	
				OUT4ON();				
				LCD_ShowString(0,80,100,10,16," OUT1 ");
				LCD_ShowString(0,95,100,10,16,"[]/|{}");
				LCD_ShowString(0,115,100,16,16," OUT2 ");
				LCD_ShowString(0,130,100,16,16,"[]/|{}");		

				LCD_ShowString(0,150,100,16,16," OUT3 ");
				LCD_ShowString(0,165,100,16,16,"[]/|{}");	
				LCD_ShowString(0,185,100,16,16," OUT4 ");
				LCD_ShowString(0,200,100,16,16,"[]/|{}");				
			}	
			else
			{
				OUT1OFF();
				OUT2ON();	
				OUT3NO();	
				OUT4NO();	
				LCD_ShowString(0,80,100,10,16," OUT1 ");
				LCD_ShowString(0,95,100,10,16,"[]/|{}");
				LCD_ShowString(0,115,100,16,16," OUT2 ");
				LCD_ShowString(0,130,100,16,16,"[]/|{}");	
					
			}		
		break;
		
		case 50:
			if(IN1_A==ON)
			{
				UI_IN1();
				OUT1ON();
				OUT2NO();
						
			}	
			if(IN2_A==ON)
			{
				UI_IN2();
				OUT2OFF();
				OUT1NO();		
			}		
			else
			{
				OUT2NO();
				OUT1NO();					
			}
		break;
		
		case 51:
			
			if(IN1_A==ON)
			{
				UI_IN1();
				
				OUT1NO();
				OUT2NO();			
			}	
			else if(IN2_A==ON)
			{
				UI_IN2();
				
				OUT1NO();
				OUT2NO();			
			}	
			else
			{
				OUT1ON();
				OUT2OFF();			
			}

			
		break;
		
		case 52:
			if(IN1_A==ON)
			{
				UI_IN1();
				
				OUT1OFF();
	LCD_ShowString(0,115,100,16,16," OUT2 ");
	LCD_ShowString(0,130,100,16,16,"[]/|{}");
	TIM_SetCompare1(TIM3,0);	
	TIM_SetCompare2(TIM3,200);		
			}	
			else if(IN2_A==ON)
			{
				UI_IN2();
				
				OUT1OFF();
	LCD_ShowString(0,115,100,16,16," OUT2 ");
	LCD_ShowString(0,130,100,16,16,"[]/|{}");
	TIM_SetCompare1(TIM3,0);	
	TIM_SetCompare2(TIM3,200);			
			}	
			else
			{
				OUT1ON();
				OUT2OFF();			
			}			
			
		break;
		
		case 53:
			if(IN1_A==ON)
			{
				UI_IN1();
				OUT1OFF();
				OUT2ON();
				delay_ms(2000);
				OUT1OFF();
				OUT2NO();
				delay_ms(2000);
			}	
			if(IN2_A==ON)
			{
				UI_IN2();
				OUT1OFF();
				OUT2ON();
				delay_ms(2000);
				OUT1NO();
				OUT2ON();
				delay_ms(2000);			
			}	
			if(H3==0)
			{	
				OUT1ON();
				OUT2OFF();
			}
		
		break;
		
		case 55:
			while(run)
			{
				IWDG_Feed();//喂狗
				OUT1ON();
				OUT2OFF();
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1NO();
					OUT2OFF();	
					delay_ms(500);
				}	
		
			}				
		break;
		
		case 56:
			while(run)
			{
				IWDG_Feed();//喂狗
				OUT1ON();
				OUT2OFF();
				if(IN1_A==ON)
				{
					UI_IN1();
					OUT1NO();
					OUT2OFF();	
					delay_ms(500);
				}	
				if(IN2_A==ON)
				{
					UI_IN2();
					OUT1ON();
					OUT2NO();	
					delay_ms(500);
				}	
				
			}			
		break;
		
		case 57:
			OUT1ON();
			OUT2OFF();
			if(IN1_A==OFF)
			{
				OUT1NO();
				OUT2ON();			
			}
			if(IN2_A==OFF)
			{
				OUT1ON();
				OUT2NO();			
			}	
		break;
		
		case 58:
			while(run)
			{
				IWDG_Feed();//喂狗
				OUT1ON();
				OUT2OFF();
				if((IN1_A==ON)&&(IN2_A==ON))
				{
					UI_IN1();
					UI_IN2();
					OUT1NO();
					OUT2NO();
					run=0;					
				}
				
			}			
		break;
		
		case 59:
			
		break;
		
		case 60:
			
		break;
		
		case 61:
			
		break;
		
		case 62:
			
		break;
		
		case 63:
			
		break;
		
		case 64:
			
		break;
		
		case 65:
			
		break;
		
		case 66:
			
		break;
		
		case 67:
			
		break;
		
		case 68:
			
		break;
		
		case 69:
			
		break;
		
		case 70:
			
		break;
		
		case 71:
			
		break;
		
		case 72:
			
		break;
		
		case 73:
			
		break;
		
		case 74:
			
		break;
	   default:
		    OUT1NO();
			OUT2NO();
			OUT3NO();
			OUT4NO();
			run=0;
		break;
		
		
	 }
	 H0=0;
}



void out_init(void)
{
		
	TIM_SetCompare3(TIM4,500);
	TIM_SetCompare4(TIM4,500);
	TIM_SetCompare1(TIM3,500);	
	TIM_SetCompare2(TIM3,500);
	TIM_SetCompare1(TIM4,500);
	TIM_SetCompare2(TIM4,500);
	TIM_SetCompare3(TIM1,500);
	TIM_SetCompare4(TIM1,500);	
}
void UI_init(void)
{
	LCD_ShowString(0,185,100,10,16," OUT4 ");
	LCD_ShowString(0,200,100,10,16,"......");
	
	LCD_ShowString(0,150,100,16,16," OUT3 ");
	LCD_ShowString(0,165,100,16,16,"......");

	LCD_ShowString(0,115,100,16,16," OUT2 ");
	LCD_ShowString(0,130,100,16,16,"......");
	
	LCD_ShowString(0,80,100,16,16," OUT1 ");
	LCD_ShowString(0,95,100,16,16,"......");			
	


	LCD_ShowString(130,185,100,10,16,"  IN4 ");
	LCD_ShowString(130,200,100,10,16,"......");
	
	LCD_ShowString(130,150,100,16,16,"  IN3 ");
	LCD_ShowString(130,165,100,16,16,"......");

	LCD_ShowString(130,115,100,16,16,"  IN2 ");
	LCD_ShowString(130,130,100,16,16,"......");
	
	LCD_ShowString(130,80,100,16,16,"  IN1 ");
	LCD_ShowString(130,95,100,16,16,"......");			
}








uint8_t	run_flag=1;

void buf_data(void)
{
uint8_t   i,k1=0,k2=0;
	
	
j1=0;j2=0;j3=0;j4=0;j5=0;j6=0;j7=0;j8=0;j9=0;j10=0;	//0x0A为动作卡，0x09为条件卡
i1=0;i2=0;i3=0;i4=0;i5=0;i6=0;i7=0;i8=0;i9=0;	//i为逻辑开始个数，j为逻辑结束个数
cond1=1;cond2=1;cond3=1;cond4=1;cond5=1;cond6=1;cond7=1;cond8=1;cond9=1;
	
	
for(i=1;i<usart1_buffer_data[3]/2+1;i++)
{
		if((usart1_buffer_data[i*2+2]==0x0A)&&(usart1_buffer_data[i*2+4]==0x09))									//判断逻辑开始个数
		{
			k1++;
			switch(k1)
			{
				case 1:i1=i;break;				
				case 2:i2=i;break;
				case 3:i3=i;break;
				case 4:i4=i;break;
				case 5:i5=i;break;
				case 6:i6=i;break;
				case 7:i7=i;break;				
				case 8:i8=i;break;
				case 9:i9=i;break;
				default:break;
			}
		}
		else if((usart1_buffer_data[i*2+2]==0x09)&&(usart1_buffer_data[i*2+4]==0x0A))							//判断逻辑结束个数
		{
		k2++;
			switch(k2)
			{
				case 1:j1=i;break;				
				case 2:j2=i;break;
				case 3:j3=i;break;
				case 4:j4=i;break;
				case 5:j5=i;break;
				case 6:j6=i;break;
				case 7:j7=i;break;				
				case 8:j8=i;break;
				case 9:j9=i;break;
				case 10:j10=i;break;
				default:break;
			}
		}
	}
	
	
	if(usart1_buffer_data[4]==0x0A)				//第一张卡为动作卡
	{
		//无新逻辑
		if((i1==0)&&(run_flag==1))
		{
			for(k4=0;k4<G_ka_num;k4++)
			{
				ka_A((k4+1)*2+3);
				run_flag=0;
			}
		}
		
		//一个新逻辑
		if((i1!=0)&&(i2==0))
		{
			//无逻辑
			if((j1!=0)&&(run_flag==1))
			{
				for(k4=0;k4<i1;k4++)
				{
					ka_A((k4+1)*2+3);
					run_flag=0;
				}
			}
			//1个逻辑
			if((j1!=0)&&(j2==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
		}
		
		//两个新逻辑
		if((i2!=0)&&(i3==0))
		{
			//1个逻辑
			if((j1!=0)&&(j2==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
		
			//2个逻辑
			if((j2!=0)&&(j3==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}		
		}
		
		//三个新逻辑
		if((i3!=0)&&(i4==0))
		{
			//2个逻辑
			if((j2!=0)&&(j3==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}		
			
			
			//3个逻辑
			if((j3!=0)&&(j4==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}		
		}
		
		//四个新逻辑
		if((i4!=0)&&(i5==0))
		{
			//3个逻辑
			if((j3!=0)&&(j4==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}	
			
			
			//4个逻辑
			if((j4!=0)&&(j5==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}	
		}	
		//五个新逻辑
		if((i5!=0)&&(i6==0))
		{
			//4个逻辑
			if((j4!=0)&&(j5==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}	
			
			
			//5个逻辑
			if((j5!=0)&&(j6==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}			
		}
		
		//六个新逻辑
		if((i6!=0)&&(i7==0))
		{
			//5个逻辑
			if((j5!=0)&&(j6==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i6;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
			
			//6个逻辑
			if((j6!=0)&&(j7==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i6;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i6;k5<j6;k5++)
				{
					ka_9((k5+1)*2+3);
					cond6=G_cond&cond6;
				}
				if(cond6==1)
				{
					for(k4=j6;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
		}
		
		//七个新逻辑
		if((i7!=0)&&(i8==0))
		{
			//6个逻辑
			if((j6!=0)&&(j7==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i6;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i6;k5<j6;k5++)
				{
					ka_9((k5+1)*2+3);
					cond6=G_cond&cond6;
				}
				if(cond6==1)
				{
					for(k4=j6;k4<i7;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}

			
			//7个逻辑
			if((j7!=0)&&(j8==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i6;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i6;k5<j6;k5++)
				{
					ka_9((k5+1)*2+3);
					cond6=G_cond&cond6;
				}
				if(cond6==1)
				{
					for(k4=j6;k4<i7;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i7;k5<j7;k5++)
				{
					ka_9((k5+1)*2+3);
					cond7=G_cond&cond7;
				}
				if(cond7==1)
				{
					for(k4=j7;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
		}		
		
		//八个新逻辑
		if((i8!=0)&&(i9==0))
		{
			//7个逻辑
			if((j7!=0)&&(j8==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i6;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i6;k5<j6;k5++)
				{
					ka_9((k5+1)*2+3);
					cond6=G_cond&cond6;
				}
				if(cond6==1)
				{
					for(k4=j6;k4<i7;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i7;k5<j7;k5++)
				{
					ka_9((k5+1)*2+3);
					cond7=G_cond&cond7;
				}
				if(cond7==1)
				{
					for(k4=j7;k4<i8;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
			
			//8个逻辑
			if((j8!=0)&&(j9==0))
			{
				if(run_flag==1)
				{
					for(k4=0;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						run_flag=0;
						G_cond=0;
					}				
				}
				for(k5=i1;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i6;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i6;k5<j6;k5++)
				{
					ka_9((k5+1)*2+3);
					cond6=G_cond&cond6;
				}
				if(cond6==1)
				{
					for(k4=j6;k4<i7;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i7;k5<j7;k5++)
				{
					ka_9((k5+1)*2+3);
					cond7=G_cond&cond7;
				}
				if(cond7==1)
				{
					for(k4=j7;k4<i8;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}	
				for(k5=i8;k5<j8;k5++)
				{
					ka_9((k5+1)*2+3);
					cond8=G_cond&cond8;
				}
				if(cond8==1)
				{
					for(k4=j8;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}					
			}
		}		
		
	}	
	
	
	
	else if(usart1_buffer_data[4]==0x09)		//第一张卡位条件卡
	{
		//无新逻辑
		if(i1==0)
		{
			//0个逻辑，不执行
			//1个逻辑
			if((j1!=0)&&(j2==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
		}
		//一个新逻辑
		if((i1!=0)&&(i2==0))
		{
			//1个逻辑
			if((j1!=0)&&(j2==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}

			//2个逻辑
			if((j2!=0)&&(j3==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}				
		}
			
		//两个新逻辑
		if((i2!=0)&&(i3==0))
		{
			//2个逻辑
			if((j2!=0)&&(j3==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}		
			
			//3个逻辑
			if((j3!=0)&&(j4==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}		
			
		}
		
		//三个新逻辑
		if((i3!=0)&&(i4==0))
		{
			//3个逻辑
			if((j3!=0)&&(j4==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}		
			
			//4个逻辑
			if((j4!=0)&&(j5==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}		
			
		}
		
		//四个新逻辑
		if((i4!=0)&&(i5==0))
		{
			//4个逻辑
			if((j4!=0)&&(j5==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}		
			
			//5个逻辑
			if((j5!=0)&&(j6==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}		
		}
		
		//五个新逻辑
		if((i5!=0)&&(i6==0))
		{
			//5个逻辑
			if((j5!=0)&&(j6==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}			
			
			//6个逻辑
			if((j6!=0)&&(j7==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j6;k5++)
				{
					ka_9((k5+1)*2+3);
					cond6=G_cond&cond6;
				}
				if(cond6==1)
				{
					for(k4=j6;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
		}
		
		//六个新逻辑
		if((i6!=0)&&(i7==0))
		{
			//6个逻辑
			if((j6!=0)&&(j7==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j6;k5++)
				{
					ka_9((k5+1)*2+3);
					cond6=G_cond&cond6;
				}
				if(cond6==1)
				{
					for(k4=j6;k4<i6;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
			//7个逻辑
			if((j7!=0)&&(j8==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j6;k5++)
				{
					ka_9((k5+1)*2+3);
					cond6=G_cond&cond6;
				}
				if(cond6==1)
				{
					for(k4=j6;k4<i6;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i6;k5<j7;k5++)
				{
					ka_9((k5+1)*2+3);
					cond7=G_cond&cond7;
				}
				if(cond7==1)
				{
					for(k4=j7;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
		}

		//七个新逻辑
		if((i7!=0)&&(i8==0))
		{
			//7个逻辑
			if((j7!=0)&&(j8==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j6;k5++)
				{
					ka_9((k5+1)*2+3);
					cond6=G_cond&cond6;
				}
				if(cond6==1)
				{
					for(k4=j6;k4<i6;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i6;k5<j7;k5++)
				{
					ka_9((k5+1)*2+3);
					cond7=G_cond&cond7;
				}
				if(cond7==1)
				{
					for(k4=j7;k4<i7;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
			//8个逻辑
			if((j8!=0)&&(j9==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j6;k5++)
				{
					ka_9((k5+1)*2+3);
					cond6=G_cond&cond6;
				}
				if(cond6==1)
				{
					for(k4=j6;k4<i6;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i6;k5<j7;k5++)
				{
					ka_9((k5+1)*2+3);
					cond7=G_cond&cond7;
				}
				if(cond7==1)
				{
					for(k4=j7;k4<i8;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i8;k5<j9;k5++)
				{
					ka_9((k5+1)*2+3);
					cond8=G_cond&cond8;
				}
				if(cond8==1)
				{
					for(k4=j9;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
		}		
		//八个新逻辑
		if((i8!=0)&&(i9==0))
		{
			//8个逻辑
			if((j8!=0)&&(j9==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j6;k5++)
				{
					ka_9((k5+1)*2+3);
					cond6=G_cond&cond6;
				}
				if(cond6==1)
				{
					for(k4=j6;k4<i6;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i6;k5<j7;k5++)
				{
					ka_9((k5+1)*2+3);
					cond7=G_cond&cond7;
				}
				if(cond7==1)
				{
					for(k4=j7;k4<i7;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i7;k5<j8;k5++)
				{
					ka_9((k5+1)*2+3);
					cond8=G_cond&cond8;
				}
				if(cond8==1)
				{
					for(k4=j8;k4<i8;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
			//9个逻辑
			if((j9!=0)&&(j10==0))
			{
				for(k5=0;k5<j1;k5++)
				{
					ka_9((k5+1)*2+3);
					cond1=G_cond&cond1;
				}
				if(cond1==1)
				{
					for(k4=j1;k4<i1;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i1;k5<j2;k5++)
				{
					ka_9((k5+1)*2+3);
					cond2=G_cond&cond2;
				}
				if(cond2==1)
				{
					for(k4=j2;k4<i2;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i2;k5<j3;k5++)
				{
					ka_9((k5+1)*2+3);
					cond3=G_cond&cond3;
				}
				if(cond3==1)
				{
					for(k4=j3;k4<i3;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i3;k5<j4;k5++)
				{
					ka_9((k5+1)*2+3);
					cond4=G_cond&cond4;
				}
				if(cond4==1)
				{
					for(k4=j4;k4<i4;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i4;k5<j5;k5++)
				{
					ka_9((k5+1)*2+3);
					cond5=G_cond&cond5;
				}
				if(cond5==1)
				{
					for(k4=j5;k4<i5;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i5;k5<j6;k5++)
				{
					ka_9((k5+1)*2+3);
					cond6=G_cond&cond6;
				}
				if(cond6==1)
				{
					for(k4=j6;k4<i6;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i6;k5<j7;k5++)
				{
					ka_9((k5+1)*2+3);
					cond7=G_cond&cond7;
				}
				if(cond7==1)
				{
					for(k4=j7;k4<i7;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i7;k5<j8;k5++)
				{
					ka_9((k5+1)*2+3);
					cond8=G_cond&cond8;
				}
				if(cond8==1)
				{
					for(k4=j8;k4<i8;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
				for(k5=i8;k5<j9;k5++)
				{
					ka_9((k5+1)*2+3);
					cond9=G_cond&cond9;
				}
				if(cond9==1)
				{
					for(k4=j9;k4<G_ka_num;k4++)
					{
						ka_A((k4+1)*2+3);
						G_cond=0;
					}
				}
			}
		}		
	}

	else if(usart1_buffer_data[4]==0x64)
	{
		ka_H(usart1_buffer_data[5]);
	}
	else if(usart1_buffer_data[4]==0x65)
	{
		ka_H(usart1_buffer_data[5]+9);
	}
	else if(usart1_buffer_data[4]==0x66)
	{
		ka_H(usart1_buffer_data[5]+25);	
	}
	else if(usart1_buffer_data[4]==0x67)
	{
		ka_H(usart1_buffer_data[5]+41);	
	}
}	
	
	











	
	
	

















