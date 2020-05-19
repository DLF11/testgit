#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"	 
#include "w25qxx.h"



//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 
  
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;
	
u32 FLASH_SIZE=16*1024*1024;	
	
u8 lcd_id[12]; //���LCD ID�ַ���




//д�Ĵ�������
//regval:�Ĵ���ֵ
void LCD_WR_REG(vu16 regval)
{   
	//LCD->LCD_REG=regval;
	
		LCD->LCD_REG=regval>>8;	
		LCD->LCD_REG=regval&0xff;   //д��Ҫд�ļĴ������		 
}

//��LCD����
//����ֵ:������ֵ
u16 LCD_RD_DATA(void)
{
	vu16 ram=0;			//��ֹ���Ż�
	ram=LCD->LCD_RAM;	
	return ram;
}				
/*******************************************************************************
* Function Name  : LCD
_WR_DATA   
* Description    : дLCD���ݺ���,data:Ҫд���ֵ
* Input          : data
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WR_DATA(u16 data)  //16BIT REG
{
//	LCD->LCD_RAM=data;
	LCD->LCD_RAM=data>>8;
	LCD->LCD_RAM=data&0xff;	
}

//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
		LCD->LCD_REG = LCD_Reg>>8;		
		LCD->LCD_REG = LCD_Reg&0xff;
		LCD->LCD_RAM = LCD_RegValue>>8;
		LCD->LCD_RAM = LCD_RegValue&0xff;   		 
}	   


/**
 * @brief  ILI9225G �����λ
 * @param  ��
 * @retval ��
 */
void ILI9225G_Rst ( void )
{		
	GPIO_SetBits ( ILI9225G_RST_PORT, ILI9225G_RST_PIN );		 	 

	delay_ms(30);
	
	GPIO_ResetBits ( ILI9225G_RST_PORT, ILI9225G_RST_PIN );	 //�͵�ƽ��λ

	delay_ms(50);					   

	GPIO_SetBits ( ILI9225G_RST_PORT, ILI9225G_RST_PIN );		 	 

	delay_ms(30);	
	
}

/*******************************************************************************
//���ù��λ��    MODIFY YZ
//Xpos:������
//Ypos:������
*******************************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
	

	LCD_WR_REG(0x0036);LCD_WR_DATA(0x00AF);
	LCD_WR_REG(0x0037);LCD_WR_DATA(Xpos);

	LCD_WR_REG(0x0038);LCD_WR_DATA(0x00DB);
	LCD_WR_REG(0x0039);LCD_WR_DATA(Ypos);
	
	LCD_WR_REG(0x0020);LCD_WR_DATA(Xpos);
	LCD_WR_REG(0x0021);LCD_WR_DATA(Ypos);
	
	LCD_WR_REG(0x0022);	
} 





/*******************************************************************************
//���ٻ���  modify YZ
//x,y:����
//color:��ɫ
*******************************************************************************/
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	  
	LCD_SetCursor(x,y);	

	LCD_WriteRAM(color);
	
//	LCD->LCD_RAM=color;	
	
}

/**********************************************
��������Lcd��ѡ����
���ܣ�ѡ��Lcd��ָ���ľ�������

ע�⣺xStart��yStart��Xend��Yend������Ļ����ת���ı䣬λ���Ǿ��ο���ĸ���

��ڲ�����xStart x�������ʼ��
          ySrart y�������ʼ��
          Xend   y�������ֹ��
          Yend   y�������ֹ��
����ֵ����
***********************************************/
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
//9225G   OK
	LCD_WR_REG(0x0036);LCD_WR_DATA(Xend);
	LCD_WR_REG(0x0037);LCD_WR_DATA(Xstart);

	LCD_WR_REG(0x0038);LCD_WR_DATA(Yend);
	LCD_WR_REG(0x0039);LCD_WR_DATA(Ystart);
	
	LCD_WR_REG(0x0020);LCD_WR_DATA(Xstart);
	LCD_WR_REG(0x0021);LCD_WR_DATA(Ystart);
	
	LCD_WR_REG(0x0022);

}

/*******************************************************************************
* Function Name  : LCD_WriteRAM    
* Description    : LCDдGRAM�Ĵ�������
* Input          : RGB_Code:��ɫֵ
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{
//	LCD->LCD_RAM = RGB_Code;//дʮ��λGRAM

	LCD->LCD_RAM = RGB_Code>>8;	
	LCD->LCD_RAM = RGB_Code&0xff; 		
}


/*******************************************************************************
//��ָ��λ����ʾһ���ַ�   YZ
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//���ͣ�
//1���ַ����(size/2)���߶�(size)һ�롣
//2���ַ�ȡģΪ����ȡģ����ÿ��ռ�����ֽڣ���󲻹������ֽ�����ռһ�ֽڡ�
//3���ַ���ռ�ռ�Ϊ��ÿ����ռ�ֽڣ�������
//csize=(size/8+((size%8)?1:0))*(size/2)
//�˺�*ǰΪ����ÿ����ռ�ֽ������˺�*��Ϊ�������ַ��߶�һ�룩
*******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}  



//��ʼ��lcd
//�ó�ʼ���������Գ�ʼ������ILI93XXҺ��,�������������ǻ���ILI9320��!!!
//�������ͺŵ�����оƬ��û�в���! 
void LCD_Init(void)
{ 	
	vu32 i=0;
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
  FSMC_NORSRAMTimingInitTypeDef  writeTiming;
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);	//ʹ��FSMCʱ��

	
	/* ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��*/
	RCC_AHB1PeriphClockCmd ( 	
													/*�����ź�*/
				ILI9225G_RD_CLK|ILI9225G_WR_CLK|ILI9225G_CS_CLK|
				ILI9225G_RS_CLK	|ILI9225G_BK_CLK|ILI9225G_RST_CLK|
				/*�����ź�*/
				ILI9225G_D0_CLK|ILI9225G_D1_CLK|	ILI9225G_D2_CLK | 
				ILI9225G_D3_CLK | ILI9225G_D4_CLK|ILI9225G_D5_CLK|
				ILI9225G_D6_CLK | ILI9225G_D7_CLK	, ENABLE );

	
	/* ����FSMC���Ӧ��������,FSMC-D0~D15 */	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = ILI9225G_D0_PIN; 
    GPIO_Init(ILI9225G_D0_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9225G_D0_PORT,ILI9225G_D0_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9225G_D1_PIN; 
    GPIO_Init(ILI9225G_D1_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9225G_D1_PORT,ILI9225G_D1_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9225G_D2_PIN; 
    GPIO_Init(ILI9225G_D2_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9225G_D2_PORT,ILI9225G_D2_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9225G_D3_PIN; 
    GPIO_Init(ILI9225G_D3_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9225G_D3_PORT,ILI9225G_D3_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9225G_D4_PIN; 
    GPIO_Init(ILI9225G_D4_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9225G_D4_PORT,ILI9225G_D4_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9225G_D5_PIN; 
    GPIO_Init(ILI9225G_D5_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9225G_D5_PORT,ILI9225G_D5_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9225G_D6_PIN; 
    GPIO_Init(ILI9225G_D6_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9225G_D6_PORT,ILI9225G_D6_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9225G_D7_PIN; 
    GPIO_Init(ILI9225G_D7_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9225G_D7_PORT,ILI9225G_D7_PinSource,FSMC_AF);


	/* ����FSMC���Ӧ�Ŀ�����
	 * FSMC_NOE   :LCD-RD
	 * FSMC_NWE   :LCD-WR
	 * FSMC_NE1   :LCD-CS
	 * FSMC_A0    :LCD-RS
	 */
    GPIO_InitStructure.GPIO_Pin = ILI9225G_RD_PIN; 
    GPIO_Init(ILI9225G_RD_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9225G_RD_PORT,ILI9225G_RD_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9225G_WR_PIN; 
    GPIO_Init(ILI9225G_WR_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9225G_WR_PORT,ILI9225G_WR_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = ILI9225G_CS_PIN; 
    GPIO_Init(ILI9225G_CS_PORT, &GPIO_InitStructure);   
    GPIO_PinAFConfig(ILI9225G_CS_PORT,ILI9225G_CS_PinSource,FSMC_AF);  

    GPIO_InitStructure.GPIO_Pin = ILI9225G_RS_PIN; 
    GPIO_Init(ILI9225G_RS_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(ILI9225G_RS_PORT,ILI9225G_RS_PinSource,FSMC_AF);
	

  /* ����LCD��λRST���ƹܽ�*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = ILI9225G_RST_PIN; 
	GPIO_Init ( ILI9225G_RST_PORT, & GPIO_InitStructure );
		
	/* ����LCD������ƹܽ�BK*/
	GPIO_InitStructure.GPIO_Pin = ILI9225G_BK_PIN; 
	GPIO_Init ( ILI9225G_BK_PORT, & GPIO_InitStructure );
	GPIO_ResetBits( ILI9225G_BK_PORT, ILI9225G_BK_PIN );			//��������
	


  readWriteTiming.FSMC_AddressSetupTime = 0XF; //��ַ����ʱ�䣨ADDSET��Ϊ16��HCLK 1/168M=6ns*16=96ns	
  readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
  readWriteTiming.FSMC_DataSetupTime =  60;		//���ݱ���ʱ��Ϊ60��HCLK	=6*60=360ns
  readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
  readWriteTiming.FSMC_CLKDivision = 0x00;
  readWriteTiming.FSMC_DataLatency = 0x00;
  readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    

	writeTiming.FSMC_AddressSetupTime =9;	      //��ַ����ʱ�䣨ADDSET��Ϊ9��HCLK =54ns 
  writeTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨A		
  writeTiming.FSMC_DataSetupTime = 8;	 //���ݱ���ʱ��Ϊ6ns*9��HCLK=54ns
  writeTiming.FSMC_BusTurnAroundDuration = 0x00;
  writeTiming.FSMC_CLKDivision = 0x00;
  writeTiming.FSMC_DataLatency = 0x00;
  writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 

 
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//  ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
  FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;//�洢�����ݿ��Ϊ8bit   
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //��дʱ��
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //дʱ��

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // ʹ��BANK1 
		
	ILI9225G_Rst(); 
 	LCD_WR_REG(0X00);	
	lcddev.id=LCD_RD_DATA();	//����92
	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA();	//����26  
	ILI9225G_Rst(); 
	/*************�ԣ̣ã�����������************************/	
	LCD_Set();  //������ʾ������	 ��Ҫ���ã���ʾ���ܶ��õ����õģ�		
	
	/*************���¿�ʼ��ʼ��*******************/	
	LCD_WriteReg(0x00D0,0x0003);
	LCD_WriteReg(0x00EB,0x0B00);	  		   
	LCD_WriteReg(0x00EC,0x000F);
	LCD_WriteReg(0x00C7,0x030F);	  
	LCD_WriteReg(0x0001,0x011C);	 
	LCD_WriteReg(0x0002,0x0100);
	LCD_WriteReg(0x0003,0x1030);

	LCD_WriteReg(0x0008,0x0808);	
	LCD_WriteReg(0x0009,0x0000);
	LCD_WriteReg(0x000A,0x0000);
	LCD_WriteReg(0x000B,0x1100);
	LCD_WriteReg(0x000C,0x0000);
	LCD_WriteReg(0x000F,0x0901);
	LCD_WriteReg(0x0020,0x0000);
	LCD_WriteReg(0x0021,0x0000);
	delay_ms(132);
	LCD_WriteReg(0x0010,0x0000);
	LCD_WriteReg(0x0011,0x1B41);
	LCD_WriteReg(0x0012,0x200E);
	LCD_WriteReg(0x0013,0x0066);
	LCD_WriteReg(0x0014,0x5c6C);//586c
	LCD_WriteReg(0x0015,0x0020);
	LCD_WriteReg(0x0030,0x0000);
	LCD_WriteReg(0x0031,0x00DB);
	LCD_WriteReg(0x0032,0x0000);
	LCD_WriteReg(0x0033,0x0000);
	LCD_WriteReg(0x0034,0x00DB);

	LCD_WriteReg(0x0035,0x0000);
	LCD_WriteReg(0x0036,0x00AF);
	LCD_WriteReg(0x0037,0x0000);
							  
	LCD_WriteReg(0x0038,0x00DB);
	LCD_WriteReg(0x0039,0x0000);  

	//GAMMA
	LCD_WriteReg(0x0050,0x0000);
	LCD_WriteReg(0x0051,0x0A02);
	LCD_WriteReg(0x0052,0x0E0B);
	LCD_WriteReg(0x0053,0x0800);	
	LCD_WriteReg(0x0054,0x0B0C);	
	LCD_WriteReg(0x0055,0x020A);		
	LCD_WriteReg(0x0056,0x0000);
	LCD_WriteReg(0x0057,0x0008);
	LCD_WriteReg(0x0058,0x0000);
	LCD_WriteReg(0x0059,0x0000);	 
	LCD_WriteReg(0x00EC,0x128A);
	LCD_WriteReg(0x0007,0x1017);
	LCD_WR_REG(0x0022);
	delay_ms(132);
}  

///===========END INITIAL=====================//
/**********************************************
��������LCD_Set
���ܣ�Lcd������ؼĴ�����������
����ֵ����
***********************************************/
void LCD_Set(void)
{
//9225	OK
	lcddev.dir=0;	     //����
	lcddev.width=176;    //LCD ���
	lcddev.height=220;   //LCD �߶�

	lcddev.setxcmd=0X0020;  //����x����ָ��
	lcddev.setycmd=0X0021;  //����y����ָ��
	lcddev.wramcmd=0X0022;  //��ʼдgramָ��	
	
}


/*******************************************************************************
* Function Name  : LCD_Clear   modify YZ
* Description    ://��������   
* Input          : color:Ҫ���������ɫ
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Clear(u16 color)
{
	u32 index=0;     
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//�õ��ܵ���
	
	BlockWrite(0,lcddev.width,0,lcddev.height);

	for(index=0;index<totalpoint;index++)
	{
		LCD_WriteRAM(color);
//		LCD->LCD_RAM=color;
	
	}	
	
} 


/*******************************************************************************
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
�ַ�����ʼ��ַ		
˵�����ַ����е���ɵģ������ַ������в��ϵ��û��㺯��LCD_DrawPoint();�����㣨��LCD->LCD_RAM��д��ɫֵ�������ɫֵ��lcd.h�ﱻ����Ϊ16λ�ĵ�ַ����
*******************************************************************************/
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}



/*******************************************************************************
* Function Name  : LCD_Display
* Description    : ��ʾ���ɫ
* Input          : color:Ҫ��ʾ�����ɫ
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Display(u16 color)
{
	u16 i,j;
	
	u16 sx=0,sy=0;
	u16 twidth,theight;
	twidth=sx+lcddev.width-1;
	theight=sy+lcddev.height-1;

	BlockWrite(0,lcddev.width,0,lcddev.height);

	for(i=0;i<lcddev.height;i++)
	{
		for(j=0;j<lcddev.width;j++)
		{	
			LCD_WriteRAM(color);
//			LCD->LCD_RAM=color;			
		}
	
	}
	
}




/*******************************************************************************
* Function Name  : LCD_DisplayJiuGongGe
* Description    : �Ź�����ʾ:RGB-BGR-RGB
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DisplayJiuGongGe(void)
{

	u16 Row,Column,aa;
	
	BlockWrite(0,lcddev.width,0,lcddev.height);
	
	aa=lcddev.height/3;

	for(Row=0;Row<aa;Row++)
	{
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0xf800);
//		LCD->LCD_RAM=0xf800;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x07e0);
//		LCD->LCD_RAM=0x07e0;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x001f);
//		LCD->LCD_RAM=0x001f;
		for(Column=0;Column<(lcddev.width%3);Column++) 	LCD_WriteRAM(0x001f);
//		LCD->LCD_RAM=0x001f;
	}

	for(Row=0;Row<aa;Row++)
	{			  
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x001f);
//		LCD->LCD_RAM=0x001f;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0xf800);		
//		LCD->LCD_RAM=0xf800;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x07e0);
//		LCD->LCD_RAM=0x07e0;
		for(Column=0;Column<(lcddev.width%3);Column++)	LCD_WriteRAM(0x07e0); 
//		LCD->LCD_RAM=0x07e0;
	}

	for(Row=0;Row<aa;Row++)
	{			  				   
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x07e0); 
//		LCD->LCD_RAM=0x07e0;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x001f);  
//		LCD->LCD_RAM=0x001f;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0xf800); 
//		LCD->LCD_RAM=0xf800;
		for(Column=0;Column<(lcddev.width%3);Column++)	LCD_WriteRAM(0xf800);  
//		LCD->LCD_RAM=0xf800;
	}

	for(Row=0;Row<(lcddev.height%3);Row++)
	{			  				   
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x07e0); 
//		LCD->LCD_RAM=0x07e0;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0x001f);  
//		LCD->LCD_RAM=0x001f;
		for(Column=0;Column<(lcddev.width/3);Column++)	LCD_WriteRAM(0xf800); 
//		LCD->LCD_RAM=0xf800;
		for(Column=0;Column<(lcddev.width%3);Column++)	LCD_WriteRAM(0xf800);  
//		LCD->LCD_RAM=0xf800;
	}
	LCD_ShowString(0,0,210,16,16,"~00%    "); 	//��ѹ��3.60һ����Ϊ�޵�
}

/*******************************************************************************
* Function Name  : Show_Gray
* Description    : �ҽ���ʾ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Show_Gray(void) 
{
	unsigned int i,j,k,c16h;
	
	BlockWrite(0,lcddev.width,0,lcddev.height);
	
	for(j=0;j<(lcddev.height);j++)  
		{
		c16h=0x0000;
		for(k=0;k<16;k++)
			{
		    for(i=0;i<(lcddev.width/16);i++)
				{
					LCD_WriteRAM(c16h);	
//					LCD->LCD_RAM=c16h;
				}
			c16h=c16h+0x1082;//32�� 0841 16�� 1082 8��2104
			}
		}
}


/*******************************************************************************
* Function Name  : LCD_Frame
* Description    : �ı��ߣ��м���������䵥����ɫ  
* Input          :RGB_DataΪ�м䴿ɫ�� f_dataΪ�ı߿���ɫ
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Frame(u16 RGB_Data,u16 f_data)
{
	u16 i,j;

	BlockWrite(0,lcddev.width,0,lcddev.height);

	for(i=0;i<1;i++)
	{
		for(j=0;j<lcddev.width;j++)	LCD_WriteRAM(f_data);	
//		LCD->LCD_RAM=f_data;
	}
	
	for (i=0;i<(lcddev.height-2);i++)
	{
		for (j=0;j<1;j++)	LCD_WriteRAM(f_data);
//		LCD->LCD_RAM=f_data;
			
		for (j=0;j<(lcddev.width-2);j++)	LCD_WriteRAM(RGB_Data);	
//		LCD->LCD_RAM=RGB_Data;
			
		for (j=0;j<1;j++)	LCD_WriteRAM(f_data);
//		LCD->LCD_RAM=f_data;
	}

	for(i=0;i<1;i++)
	{
		for(j=0;j<lcddev.width;j++)	LCD_WriteRAM(f_data);
//		LCD->LCD_RAM=f_data;
	}	
	
}	
	


/*******************************************************************************
* Function Name  : DisplayPicture
* Description    :   
* Input          : 
* Output         : None
* Return         : None
*һ��TFT�Ĳ�ɫ,һ����ռ2���ֽ�RGB565,Ҳ����һ���������2^16��ɫ��,һ�����������ͼƬ
* ͼƬΪ��176��220 �����м���ʾ
*ע������LOG�����ⲿFLASH�ĵ� 16*1024*1024-80000֮��77448���ֽ� 
*******************************************************************************/
void DisplayPicture(void)
{
	u16 pos,t; 
	u16 datatemp[1]; 	
	BlockWrite(0,lcddev.width,0,lcddev.height);
//	BlockWrite((lcddev.width-240)/2,(lcddev.width+240)/2-1,(lcddev.height-320)/2,(lcddev.height+320)/2-1); //OK 20170927
  
	for(pos=0;pos<lcddev.height;pos++) //(pos=0;pos<320;pos++) //
	{
		for(t=0;t<lcddev.width*2;t++) //(t=0;t<240*2;t++) //
		{
//			W25QXX_Read(datatemp,FLASH_SIZE-80000,2);
//			datatemp[0]=gImage[pos*lcddev.width*2+t]*256+gImage[pos*lcddev.width*2+t+1];  //����ط�  �ĳ�gImage[pos*480+t]<<8+gImage[pos*480+t+1];  ����������
			
			datatemp[0]=W25QXX_Read_Dyte(FLASH_SIZE-80000+pos*lcddev.width*2+t)*256+W25QXX_Read_Dyte(FLASH_SIZE-80000+pos*lcddev.width*2+t+1);  //����ط�  �ĳ�gImage[pos*480+t]<<8+gImage[pos*480+t+1];  ����������
			t++;                                
			LCD_WriteRAM(datatemp[0]);                               
		}                                
	}         	
	
}	






