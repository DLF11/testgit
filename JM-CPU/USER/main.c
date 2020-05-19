#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "PWM.h"
#include "data.h"
#include "lcd.h"
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	 
#include "wm8978.h"	 
#include "audioplay.h"	
#include "wkup.h"
#include "adc.h"
#include "time.h"
#include "iwdg.h"
#include "power.h" 



uint32_t  usart1_buffer_len=0;	         //串口1接收数据长度
uint32_t  usart2_buffer_len=0;		     //串口2接收数据长度
uint32_t  r_buffer_len=0;	             //串口1接收数据长度
uint32_t  usart1_buffer_data[60];        //数据接收缓存数组
uint32_t  usart2_buffer_data[10];  	     //
uint32_t  r_buffer_data[3];				 //
uint8_t   usart1_ka=0;
uint8_t   rc;
uint8_t   sys_ready=0;
uint8_t   G_ka_num;
uint8_t   G_cond=0;						//条件通过标志
uint8_t   G_led_download=0;	
uint8_t   audioplay=0;					//音乐标志位

					

extern uint8_t     flag_Ka;
extern uint8_t     run;	
extern uint8_t play_mun;
extern uint8_t time;			//自动待机标志位
int main(void)
{ 
//	SCB->VTOR=FLASH_BASE|0x10000;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);							//延时初始化 
	Usart1Init();									//
	usart2_init(9600);					//串口2初始化波特率为115200
	LED_Init();		  							//初始化与LED连接的硬件接口 
 	LCD_Init();         				  //初始化LCD FSMC接口		
	Data_Clear() ;	
	KEY_Init();
//	WKUP_Init();				//待机唤醒初始化
//	TIM2_int_Init(500-1,10000-1);
	TIM4_PWM_Init(500-1,84-1);
	TIM3_PWM_Init(500-1,84-1);	
	TIM1_PWM_Init(500-1,84-1);	
	Adc_Init();                 //初始化ADC
	WM8978_Init();				//初始化WM8978
	WM8978_HPvol_Set(63,63);	//耳机音量设置
	WM8978_SPKvol_Set(63);		//喇叭音量设置
    IWDG_Init(7,800); //与分频数为64,重载值为500,溢出时间为1s	
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池 
	exfuns_init();				//为fatfs相关变量申请内存   
	W25QXX_Init();			    //W25QXX初始化	
	
	
	
	
	LCD_Display(BRED);
	DisplayPicture();
	GPIO_SetBits( ILI9225G_BK_PORT, ILI9225G_BK_PIN );			//开启背光
	audio_play();
	LCD_DisplayJiuGongGe();
	UI_init();      //复位界面
	TIM2_int_Init(1000-1,2000-1);
	TIM7_Init();				/*定时器检测输入状态*/	
	while(1)
	{
		powe();
		key_L();
		VBAT();
		if(flag_Ka==1)
		{
			if(G_led_download==1)
			{
				out_init();		//输出复位
				UI_init();      //复位界面				
				LCD_ShowString(10,40,210,16,16,"Download successful"); 
				LCD_ShowString(70,80,200,16,16,"^_^");
				audio_play();
			}
			else if(run==1)
			{	
				audio_play();
				LCD_ShowString(10,40,210,16,16,"        RUN        ");
				LCD_ShowString(70,80,200,16,16,"*_*");
				buf_data();		//程序运行函数
				time=0;		    //运行状态清空运行标志位
			}
			if((G_led_download==0)&&(run==0))
			{
				LCD_ShowString(10,40,210,16,16,"        STOP        ");
				LCD_ShowString(70,80,200,16,16,"o_o");
				out_init();		//输出复位
				UI_init();      //复位界面
			}
		}
		sys_ready=1;
		IWDG_Feed();//喂狗
		
	}
	
}
	






		





