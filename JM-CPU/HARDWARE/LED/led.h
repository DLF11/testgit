#ifndef __LED_H
#define	__LED_H


#include "sys.h"  


//LED端口定义
#define	POWER_EN		 PEout(11)		// E11  5V电源使能信号
#define	CHARG_EN		 PEout(6)		// E6	充电开关使能信号


#define OUT4A    PEout(13) 	//因为未配置4输出口PWM，模拟PWM
#define OUT4B 	 PEout(14) 	//因为未配置4输出口PWM，模拟PWM

#define 	ON       0
#define 	OFF      1

void LED_Init(void);//初始化	

#endif /* __LED_H */

