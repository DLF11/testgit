#ifndef __PWM_
#define __PWM_
#include "sys.h" 


//µ„¡¡LED∫Í∂®“Â
#define LED1  TIM_SetCompare3(TIM4,0);
#define LED2  TIM_SetCompare1(TIM3,0);
#define LED3  TIM_SetCompare1(TIM4,0);
#define LED4  TIM_SetCompare3(TIM4,0);





void TIM4_PWM_Init(u32 arr,u32 psc);
void TIM3_PWM_Init(u32 arr,u32 psc);
void TIM1_PWM_Init(u32 arr,u32 psc);

#endif /* __PWM_ */



