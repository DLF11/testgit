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



uint32_t  usart1_buffer_len=0;	         //����1�������ݳ���
uint32_t  usart2_buffer_len=0;		     //����2�������ݳ���
uint32_t  r_buffer_len=0;	             //����1�������ݳ���
uint32_t  usart1_buffer_data[60];        //���ݽ��ջ�������
uint32_t  usart2_buffer_data[10];  	     //
uint32_t  r_buffer_data[3];				 //
uint8_t   usart1_ka=0;
uint8_t   rc;
uint8_t   sys_ready=0;
uint8_t   G_ka_num;
uint8_t   G_cond=0;						//����ͨ����־
uint8_t   G_led_download=0;	
uint8_t   audioplay=0;					//���ֱ�־λ

					

extern uint8_t     flag_Ka;
extern uint8_t     run;	
extern uint8_t play_mun;
extern uint8_t time;			//�Զ�������־λ
int main(void)
{ 
//	SCB->VTOR=FLASH_BASE|0x10000;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);							//��ʱ��ʼ�� 
	Usart1Init();									//
	usart2_init(9600);					//����2��ʼ��������Ϊ115200
	LED_Init();		  							//��ʼ����LED���ӵ�Ӳ���ӿ� 
 	LCD_Init();         				  //��ʼ��LCD FSMC�ӿ�		
	Data_Clear() ;	
	KEY_Init();
//	WKUP_Init();				//�������ѳ�ʼ��
//	TIM2_int_Init(500-1,10000-1);
	TIM4_PWM_Init(500-1,84-1);
	TIM3_PWM_Init(500-1,84-1);	
	TIM1_PWM_Init(500-1,84-1);	
	Adc_Init();                 //��ʼ��ADC
	WM8978_Init();				//��ʼ��WM8978
	WM8978_HPvol_Set(63,63);	//������������
	WM8978_SPKvol_Set(63);		//������������
    IWDG_Init(7,800); //���Ƶ��Ϊ64,����ֵΪ500,���ʱ��Ϊ1s	
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 
	exfuns_init();				//Ϊfatfs��ر��������ڴ�   
	W25QXX_Init();			    //W25QXX��ʼ��	
	
	
	
	
	LCD_Display(BRED);
	DisplayPicture();
	GPIO_SetBits( ILI9225G_BK_PORT, ILI9225G_BK_PIN );			//��������
	audio_play();
	LCD_DisplayJiuGongGe();
	UI_init();      //��λ����
	TIM2_int_Init(1000-1,2000-1);
	TIM7_Init();				/*��ʱ���������״̬*/	
	while(1)
	{
		powe();
		key_L();
		VBAT();
		if(flag_Ka==1)
		{
			if(G_led_download==1)
			{
				out_init();		//�����λ
				UI_init();      //��λ����				
				LCD_ShowString(10,40,210,16,16,"Download successful"); 
				LCD_ShowString(70,80,200,16,16,"^_^");
				audio_play();
			}
			else if(run==1)
			{	
				audio_play();
				LCD_ShowString(10,40,210,16,16,"        RUN        ");
				LCD_ShowString(70,80,200,16,16,"*_*");
				buf_data();		//�������к���
				time=0;		    //����״̬������б�־λ
			}
			if((G_led_download==0)&&(run==0))
			{
				LCD_ShowString(10,40,210,16,16,"        STOP        ");
				LCD_ShowString(70,80,200,16,16,"o_o");
				out_init();		//�����λ
				UI_init();      //��λ����
			}
		}
		sys_ready=1;
		IWDG_Feed();//ι��
		
	}
	
}
	






		





