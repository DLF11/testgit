#ifndef __TIME_H
#define __TIME_H
#include "sys.h" 



#define BASIC_TIM7           		TIM7
#define BASIC_TIM7_CLK       		RCC_APB1Periph_TIM7

#define BASIC_TIM7_IRQn					TIM7_IRQn
#define BASIC_TIM7_IRQHandler    TIM7_IRQHandler




void TIM2_int_Init(u32 arr,u32 psc);
void TIM7_Init(void);
#endif
