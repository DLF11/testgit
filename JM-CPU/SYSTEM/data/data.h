#ifndef __DATA_H
#define __DATA_H
#include "sys.h" 


#define  OUT1A1    	 TIM_SetCompare3(TIM4,0)
#define  OUT1B1      TIM_SetCompare4(TIM4,0)
#define  OUT1A0      TIM_SetCompare3(TIM4,500)
#define  OUT1B0      TIM_SetCompare4(TIM4,500)


#define  OUT2A1      TIM_SetCompare1(TIM3,0)
#define  OUT2B1      TIM_SetCompare2(TIM3,0)
#define  OUT2A0      TIM_SetCompare1(TIM3,500)
#define  OUT2B0      TIM_SetCompare2(TIM3,500)


#define  OUT3A1      TIM_SetCompare1(TIM4,0)
#define  OUT3B1      TIM_SetCompare2(TIM4,0)
#define  OUT3A0      TIM_SetCompare1(TIM4,500)
#define  OUT3B0      TIM_SetCompare2(TIM4,500)




void buf_data(void);
void ka_9(uint8_t g);
void ka_A(uint8_t k);
void out_init(void);
void UI_init(void);
void UI_IN(void);	

void OUT1NO(void);
void OUT1ON(void);
void OUT1OFF(void);

void OUT2NO(void);
void OUT2ON(void);
void OUT2OFF(void);

void OUT3NO(void);
void OUT3ON(void);
void OUT3OFF(void);

void OUT4NO(void);
void OUT4ON(void);
void OUT4OFF(void);
#endif



