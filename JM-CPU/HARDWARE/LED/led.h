#ifndef __LED_H
#define	__LED_H


#include "sys.h"  


//LED�˿ڶ���
#define	POWER_EN		 PEout(11)		// E11  5V��Դʹ���ź�
#define	CHARG_EN		 PEout(6)		// E6	��翪��ʹ���ź�


#define OUT4A    PEout(13) 	//��Ϊδ����4�����PWM��ģ��PWM
#define OUT4B 	 PEout(14) 	//��Ϊδ����4�����PWM��ģ��PWM

#define 	ON       0
#define 	OFF      1

void LED_Init(void);//��ʼ��	

#endif /* __LED_H */

