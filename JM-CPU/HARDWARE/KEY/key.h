#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define IN1_A 	    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)	//PB0		
#define IN2_A 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) //PC2
#define IN3_A		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)	//PC0 
#define IN4_A 	    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)	//PC4


#define DEC			GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)//PE11 		����߲���������	
#define BAT 	    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)	//PE0		�Ƿ�����������



void KEY_Init(void);	//IO��ʼ��
void key_L(void);

#endif
