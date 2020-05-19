#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//2.4��/2.8��/3.5��/4.3��/7�� TFTҺ������	  
//֧������IC�ͺŰ���:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/
//                  SPFD5408/1505/B505/C505/NT35310/NT35510/SSD1963��		    
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2010/7/4
//�汾��V3.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	
//********************************************************************************
//V1.2�޸�˵��
//֧����SPFD5408������,�����Һ��IDֱ�Ӵ�ӡ��HEX��ʽ.����鿴LCD����IC.
//V1.3
//�����˿���IO��֧��
//�޸��˱�����Ƶļ��ԣ�������V1.8���Ժ�Ŀ�����汾��
//����1.8�汾֮ǰ(������1.8)��Һ��ģ��,���޸�LCD_Init������LCD_LED=1;ΪLCD_LED=1;
//V1.4
//�޸���LCD_ShowChar������ʹ�û��㹦�ܻ��ַ���
//�����˺�������ʾ��֧��
//V1.5 20110730
//1,�޸���B505Һ������ɫ�����bug.
//2,�޸��˿���IO�������������÷�ʽ.
//V1.6 20111116
//1,�����LGDP4535Һ��������֧��
//V1.7 20120713
//1,����LCD_RD_DATA����
//2,���Ӷ�ILI9341��֧��
//3,����ILI9325�Ķ�����������
//4,����LCD_Scan_Dir����(����ʹ��)	  
//6,�����޸��˲���ԭ���ĺ���,����Ӧ9341�Ĳ���
//V1.8 20120905
//1,����LCD��Ҫ�������ýṹ��lcddev
//2,����LCD_Display_Dir����,֧�����ߺ������л�
//V1.9 20120911
//1,����RM68042������ID:6804��������6804��֧�ֺ�����ʾ����ԭ�򣺸ı�ɨ�跽ʽ��
//����6804��������ʧЧ���Թ��ܶ෽�������У���ʱ�޽⡣
//V2.0 20120924
//�ڲ�Ӳ����λ�������,ILI9341��ID��ȡ�ᱻ�����9300,�޸�LCD_Init,���޷�ʶ��
//�����������IDΪ9300/�Ƿ�ID��,ǿ��ָ������ICΪILI9341��ִ��9341�ĳ�ʼ����
//V2.1 20120930
//����ILI9325����ɫ��bug��
//V2.2 20121007
//����LCD_Scan_Dir��bug��
//V2.3 20130120
//����6804֧�ֺ�����ʾ
//V2.4 20131120
//1,����NT35310��ID:5310����������֧��
//2,����LCD_Set_Window����,�������ô���,�Կ������,�Ƚ�����,���Ǹú����ں���ʱ,��֧��6804.
//V2.5 20140211
//1,����NT35510��ID:5510����������֧��
//V2.6 20140504
//1,����ASCII 24*24�����֧��(���������û������������)  
//2,�޸Ĳ��ֺ�������,��֧��MDK -O2�Ż�
//3,���9341/35310/35510,дʱ������Ϊ���,�����ܵ�����ٶ�
//4,ȥ����SSD1289��֧��,��Ϊ1289ʵ����̫����,������Ҫ1us...��ֱ����.���ʺ�F4ʹ��
//5,����68042��C505��IC�Ķ���ɫ������bug.
//V2.7 20140710
//1,����LCD_Color_Fill������һ��bug. 
//2,����LCD_Scan_Dir������һ��bug.
//V2.8 20140721
//1,���MDKʹ��-O2�Ż�ʱLCD_ReadPoint��������ʧЧ������.
//2,����LCD_Scan_Dir����ʱ���õ�ɨ�跽ʽ��ʾ��ȫ��bug.
//V2.9 20141130
//1,������SSD1963 LCD��֧��.
//2,����LCD_SSD_BackLightSet����
//3,ȡ��ILI93XX��Rxx�Ĵ�������
//V3.0 20150423
//�޸�SSD1963 LCD������������.
//////////////////////////////////////////////////////////////////////////////////	 

//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	wramcmd;		//��ʼдgramָ��
	u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ�� 
}_lcd_dev; 	  

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ


//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD�˿ڶ���---------------- 
#define	LCD_LED PBout(15)  		//LCD����    		 PB15 	    
//LCD��ַ�ṹ��
typedef struct
{
	vu8 LCD_REG;
	vu8 LCD_RAM;
} LCD_TypeDef;
//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A6��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����! 111 1110=0X7E			    
#define LCD_BASE        ((u32)(0x60000000 | 0X0FFFF))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////
	 

//******************************* ILI9806G ��ʾ��8080ͨѶ���Ŷ��� ***************************/
/******�����ź���******/
#define      FSMC_AF                       GPIO_AF_FSMC
//Ƭѡ��LCD_CS    FSMC_NE1
#define      ILI9225G_CS_CLK                RCC_AHB1Periph_GPIOD 
#define      ILI9225G_CS_PORT               GPIOD
#define      ILI9225G_CS_PIN                GPIO_Pin_7
#define      ILI9225G_CS_PinSource          GPIO_PinSource7

//дʹ�� LED_A16  FSMC_RS
#define      ILI9225G_RS_CLK                RCC_AHB1Periph_GPIOD   
#define      ILI9225G_RS_PORT               GPIOD
#define      ILI9225G_RS_PIN                GPIO_Pin_11
#define      ILI9225G_RS_PinSource          GPIO_PinSource11


//PF0Ϊ  LCD_WR     FSMC_NWE
//дʹ��
#define      ILI9225G_WR_CLK                RCC_AHB1Periph_GPIOD   
#define      ILI9225G_WR_PORT               GPIOD
#define      ILI9225G_WR_PIN                GPIO_Pin_5
#define      ILI9225G_WR_PinSource          GPIO_PinSource5

//��ʹ��		LED_RD  FSMC_NOE
#define      ILI9225G_RD_CLK                RCC_AHB1Periph_GPIOD   
#define      ILI9225G_RD_PORT               GPIOD
#define      ILI9225G_RD_PIN                GPIO_Pin_4
#define      ILI9225G_RD_PinSource          GPIO_PinSource4

//��λ����	
#define      ILI9225G_RST_CLK               RCC_AHB1Periph_GPIOE 
#define      ILI9225G_RST_PORT              GPIOE
#define      ILI9225G_RST_PIN               GPIO_Pin_15

//��������
#define      ILI9225G_BK_CLK                RCC_AHB1Periph_GPIOA   
#define      ILI9225G_BK_PORT               GPIOA
#define      ILI9225G_BK_PIN                GPIO_Pin_8

/********�����ź���***************/
#define      ILI9225G_D0_CLK                RCC_AHB1Periph_GPIOD   
#define      ILI9225G_D0_PORT               GPIOD
#define      ILI9225G_D0_PIN                GPIO_Pin_14
#define      ILI9225G_D0_PinSource          GPIO_PinSource14

#define      ILI9225G_D1_CLK                RCC_AHB1Periph_GPIOD   
#define      ILI9225G_D1_PORT               GPIOD
#define      ILI9225G_D1_PIN                GPIO_Pin_15
#define      ILI9225G_D1_PinSource          GPIO_PinSource15

#define      ILI9225G_D2_CLK                RCC_AHB1Periph_GPIOD   
#define      ILI9225G_D2_PORT               GPIOD
#define      ILI9225G_D2_PIN                GPIO_Pin_0
#define      ILI9225G_D2_PinSource          GPIO_PinSource0

#define      ILI9225G_D3_CLK                RCC_AHB1Periph_GPIOD  
#define      ILI9225G_D3_PORT               GPIOD
#define      ILI9225G_D3_PIN                GPIO_Pin_1
#define      ILI9225G_D3_PinSource          GPIO_PinSource1

#define      ILI9225G_D4_CLK                RCC_AHB1Periph_GPIOE   
#define      ILI9225G_D4_PORT               GPIOE
#define      ILI9225G_D4_PIN                GPIO_Pin_7
#define      ILI9225G_D4_PinSource          GPIO_PinSource7

#define      ILI9225G_D5_CLK                RCC_AHB1Periph_GPIOE   
#define      ILI9225G_D5_PORT               GPIOE
#define      ILI9225G_D5_PIN                GPIO_Pin_8
#define      ILI9225G_D5_PinSource          GPIO_PinSource8

#define      ILI9225G_D6_CLK                RCC_AHB1Periph_GPIOE   
#define      ILI9225G_D6_PORT               GPIOE
#define      ILI9225G_D6_PIN                GPIO_Pin_9
#define      ILI9225G_D6_PinSource          GPIO_PinSource9

#define      ILI9225G_D7_CLK                RCC_AHB1Periph_GPIOE  
#define      ILI9225G_D7_PORT               GPIOE
#define      ILI9225G_D7_PIN                GPIO_Pin_10
#define      ILI9225G_D7_PinSource          GPIO_PinSource10


//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    		

void LCD_Set(void);			//������ʾ������	 ��Ҫ���ã���ʾ���ܶ��õ����õģ�
void LCD_Init(void);		//��ʼ������
void ILI9225G_Rst ( void ); //��λ����
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue); //LCD_RegValue:Ҫд�������
u16 LCD_RD_DATA(void);		//����ֵ:������ֵ
void LCD_WR_REG(vu16 regval);	//д�Ĵ�������			
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);//���ٻ���  modify YZ
void LCD_SetCursor(u16 Xpos, u16 Ypos);        //���ù��λ��    MODIFY YZ


void LCD_Clear(u16 color);  //��������  
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) ;//��Lcd��ѡ����
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);//��ָ��λ����ʾһ���ַ�   YZ
void LCD_WR_DATA(u16 data);  //16BIT REG
void LCD_WriteRAM(u16 RGB_Code);
void LCD_Display(u16 color);  //��ʾ���ɫ
void LCD_DisplayJiuGongGe(void);  //�Ź�����ʾ:RGB-BGR-RGB
void Show_Gray(void) ;//�ҽ���ʾ
void LCD_Frame(u16 RGB_Data,u16 f_data);//�ı��ߣ��м���������䵥����ɫ  
void DisplayPicture(void);//ͼƬ��ʾ����

			   						   																			 
//LCD�ֱ�������
#define SSD_HOR_RESOLUTION		800		//LCDˮƽ�ֱ���
#define SSD_VER_RESOLUTION		480		//LCD��ֱ�ֱ���
//LCD������������
#define SSD_HOR_PULSE_WIDTH		1		//ˮƽ����
#define SSD_HOR_BACK_PORCH		46		//ˮƽǰ��
#define SSD_HOR_FRONT_PORCH		210		//ˮƽ����

#define SSD_VER_PULSE_WIDTH		1		//��ֱ����
#define SSD_VER_BACK_PORCH		23		//��ֱǰ��
#define SSD_VER_FRONT_PORCH		22		//��ֱǰ��
//���¼����������Զ�����
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

#endif  
	 
	 



