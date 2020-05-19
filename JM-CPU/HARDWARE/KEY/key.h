#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//按键输入驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

/*下面的方式是通过直接操作库函数方式读取IO*/
#define IN1_A 	    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)	//PB0		
#define IN2_A 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) //PC2
#define IN3_A		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)	//PC0 
#define IN4_A 	    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)	//PC4


#define DEC			GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)//PE11 		充电线插入检测引脚	
#define BAT 	    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)	//PE0		是否充满检测引脚



void KEY_Init(void);	//IO初始化
void key_L(void);

#endif
