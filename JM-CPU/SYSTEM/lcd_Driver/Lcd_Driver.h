//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：https://item.taobao.com/item.htm?spm=a1z10.5-c-s.w4002-15846751795.11.nbzr7m&id=547741672512
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   :0.96寸LCD 4接口演示例程(STM32系列)
/******************************************************************************
//本程序适用与STM32F103RC
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PB13（SCL）
//              SDA   接PB15（SDA）
//              RES   接PB11
//              DC    接PB10
//              BLK    接PAB12 
*******************************************************************************/
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#include "sys.h"
#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111




#define LCD_CTRLA   	  	GPIOA		//定义TFT数据端口
#define LCD_CTRLD   	  	GPIOD		//定义TFT数据端口



#define LCD_SCL        	GPIO_Pin_11	//PB13--->>TFT --SCL/SCK
#define LCD_SDA        	GPIO_Pin_14	//PB15 MOSI--->>TFT --SDA/DIN

#define LCD_LED        	GPIO_Pin_8  //MCU_PB9--->>TFT --BL
#define LCD_RS         	GPIO_Pin_5   //PB11--->>TFT --RS/DC
#define LCD_RST     		GPIO_Pin_15	//PB10--->>TFT --RST

//#define LCD_CS_SET(x) LCD_CTRL->ODR=(LCD_CTRL->ODR&~LCD_CS)|(x ? LCD_CS:0)

//液晶控制口置1操作语句宏定义
#define	LCD_SCL_SET  	GPIO_SetBits(GPIOD,GPIO_Pin_11)  
#define	LCD_SDA_SET  	GPIO_SetBits(GPIOD,GPIO_Pin_14)  
//#define	LCD_CS_SET  	GPIO_SetBits(GPIOA,GPIO_Pin_8)  

    
#define	LCD_LED_SET  	GPIO_SetBits(GPIOA,GPIO_Pin_8)    
#define	LCD_RS_SET  	GPIO_SetBits(GPIOD,GPIO_Pin_5)  
#define	LCD_RST_SET  	GPIO_SetBits(GPIOC,GPIO_Pin_10) 
//液晶控制口置0操作语句宏定义
#define	LCD_SCL_CLR  	GPIO_ResetBits(GPIOD,GPIO_Pin_11)  
#define	LCD_SDA_CLR  	GPIO_ResetBits(GPIOD,GPIO_Pin_14)  
//#define	LCD_CS_CLR  	LCD_CTRLB->GPIOx_BRR=LCD_CS 
    
#define	LCD_LED_CLR  	GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define	LCD_RST_CLR  	GPIO_ResetBits(GPIOC,GPIO_Pin_10)
#define	LCD_RS_CLR  	GPIO_ResetBits(GPIOD,GPIO_Pin_5)

#define LCD_DATAOUT(x) LCD_DATA->GPIOx_ODR=x; //数据输出
#define LCD_DATAIN     LCD_DATA->GPIOx_IDR;   //数据输入

#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
LCD_DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 



void LCD_GPIO_Init(void);
void Lcd_WriteIndex(u8 Index);
void Lcd_WriteData(u8 Data);
void Lcd_WriteReg(u8 Index,u8 Data);
u16 Lcd_ReadReg(u8 LCD_Reg);
void Lcd_Reset(void);
void Lcd_Init(void);
void Lcd_Clear(u16 Color);
void Lcd_SetXY(u16 x,u16 y);
void Gui_DrawPoint(u16 x,u16 y,u16 Data);
unsigned int Lcd_ReadPoint(u16 x,u16 y);
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end);
void LCD_WriteData_16Bit(u16 Data);

