//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��https://item.taobao.com/item.htm?spm=a1z10.5-c-s.w4002-15846751795.11.nbzr7m&id=547741672512
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   :0.96��LCD 4�ӿ���ʾ����(STM32ϵ��)
/******************************************************************************
//������������STM32F103RC
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PB13��SCL��
//              SDA   ��PB15��SDA��
//              RES   ��PB11
//              DC    ��PB10
//              BLK    ��PAB12 
*******************************************************************************/
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
#include "sys.h"
#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//��ɫ0 3165 00110 001011 00101
#define GRAY1   0x8410      	//��ɫ1      00000 000000 00000
#define GRAY2   0x4208      	//��ɫ2  1111111111011111




#define LCD_CTRLA   	  	GPIOA		//����TFT���ݶ˿�
#define LCD_CTRLD   	  	GPIOD		//����TFT���ݶ˿�



#define LCD_SCL        	GPIO_Pin_11	//PB13--->>TFT --SCL/SCK
#define LCD_SDA        	GPIO_Pin_14	//PB15 MOSI--->>TFT --SDA/DIN

#define LCD_LED        	GPIO_Pin_8  //MCU_PB9--->>TFT --BL
#define LCD_RS         	GPIO_Pin_5   //PB11--->>TFT --RS/DC
#define LCD_RST     		GPIO_Pin_15	//PB10--->>TFT --RST

//#define LCD_CS_SET(x) LCD_CTRL->ODR=(LCD_CTRL->ODR&~LCD_CS)|(x ? LCD_CS:0)

//Һ�����ƿ���1�������궨��
#define	LCD_SCL_SET  	GPIO_SetBits(GPIOD,GPIO_Pin_11)  
#define	LCD_SDA_SET  	GPIO_SetBits(GPIOD,GPIO_Pin_14)  
//#define	LCD_CS_SET  	GPIO_SetBits(GPIOA,GPIO_Pin_8)  

    
#define	LCD_LED_SET  	GPIO_SetBits(GPIOA,GPIO_Pin_8)    
#define	LCD_RS_SET  	GPIO_SetBits(GPIOD,GPIO_Pin_5)  
#define	LCD_RST_SET  	GPIO_SetBits(GPIOC,GPIO_Pin_10) 
//Һ�����ƿ���0�������궨��
#define	LCD_SCL_CLR  	GPIO_ResetBits(GPIOD,GPIO_Pin_11)  
#define	LCD_SDA_CLR  	GPIO_ResetBits(GPIOD,GPIO_Pin_14)  
//#define	LCD_CS_CLR  	LCD_CTRLB->GPIOx_BRR=LCD_CS 
    
#define	LCD_LED_CLR  	GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define	LCD_RST_CLR  	GPIO_ResetBits(GPIOC,GPIO_Pin_10)
#define	LCD_RS_CLR  	GPIO_ResetBits(GPIOD,GPIO_Pin_5)

#define LCD_DATAOUT(x) LCD_DATA->GPIOx_ODR=x; //�������
#define LCD_DATAIN     LCD_DATA->GPIOx_IDR;   //��������

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
