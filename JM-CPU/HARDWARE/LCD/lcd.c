#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"	 
#include "w25qxx.h"



//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 
  
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;
	
u32 FLASH_SIZE=16*1024*1024;	
	
u8 lcd_id[12]; //存放LCD ID字符串




//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(vu16 regval)
{   
	//LCD->LCD_REG=regval;
	
		LCD->LCD_REG=regval>>8;	
		LCD->LCD_REG=regval&0xff;   //写入要写的寄存器序号		 
}

//读LCD数据
//返回值:读到的值
u16 LCD_RD_DATA(void)
{
	vu16 ram=0;			//防止被优化
	ram=LCD->LCD_RAM;	
	return ram;
}				
/*******************************************************************************
* Function Name  : LCD
_WR_DATA   
* Description    : 写LCD数据函数,data:要写入的值
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

//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
		LCD->LCD_REG = LCD_Reg>>8;		
		LCD->LCD_REG = LCD_Reg&0xff;
		LCD->LCD_RAM = LCD_RegValue>>8;
		LCD->LCD_RAM = LCD_RegValue&0xff;   		 
}	   


/**
 * @brief  ILI9225G 软件复位
 * @param  无
 * @retval 无
 */
void ILI9225G_Rst ( void )
{		
	GPIO_SetBits ( ILI9225G_RST_PORT, ILI9225G_RST_PIN );		 	 

	delay_ms(30);
	
	GPIO_ResetBits ( ILI9225G_RST_PORT, ILI9225G_RST_PIN );	 //低电平复位

	delay_ms(50);					   

	GPIO_SetBits ( ILI9225G_RST_PORT, ILI9225G_RST_PIN );		 	 

	delay_ms(30);	
	
}

/*******************************************************************************
//设置光标位置    MODIFY YZ
//Xpos:横坐标
//Ypos:纵坐标
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
//快速画点  modify YZ
//x,y:坐标
//color:颜色
*******************************************************************************/
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	  
	LCD_SetCursor(x,y);	

	LCD_WriteRAM(color);
	
//	LCD->LCD_RAM=color;	
	
}

/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart、yStart、Xend、Yend随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的起始点
          Xend   y方向的终止点
          Yend   y方向的终止点
返回值：无
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
* Description    : LCD写GRAM寄存器函数
* Input          : RGB_Code:颜色值
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{
//	LCD->LCD_RAM = RGB_Code;//写十六位GRAM

	LCD->LCD_RAM = RGB_Code>>8;	
	LCD->LCD_RAM = RGB_Code&0xff; 		
}


/*******************************************************************************
//在指定位置显示一个字符   YZ
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
//解释：
//1、字符宽度(size/2)＝高度(size)一半。
//2、字符取模为竖向取模，即每列占几个字节，最后不够完整字节数的占一字节。
//3、字符所占空间为：每列所占字节＊列数。
//csize=(size/8+((size%8)?1:0))*(size/2)
//乘号*前为计算每列所占字节数，乘号*后为列数（字符高度一半）
*******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}  



//初始化lcd
//该初始化函数可以初始化各种ILI93XX液晶,但是其他函数是基于ILI9320的!!!
//在其他型号的驱动芯片上没有测试! 
void LCD_Init(void)
{ 	
	vu32 i=0;
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
  FSMC_NORSRAMTimingInitTypeDef  writeTiming;
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);	//使能FSMC时钟

	
	/* 使能FSMC对应相应管脚时钟*/
	RCC_AHB1PeriphClockCmd ( 	
													/*控制信号*/
				ILI9225G_RD_CLK|ILI9225G_WR_CLK|ILI9225G_CS_CLK|
				ILI9225G_RS_CLK	|ILI9225G_BK_CLK|ILI9225G_RST_CLK|
				/*数据信号*/
				ILI9225G_D0_CLK|ILI9225G_D1_CLK|	ILI9225G_D2_CLK | 
				ILI9225G_D3_CLK | ILI9225G_D4_CLK|ILI9225G_D5_CLK|
				ILI9225G_D6_CLK | ILI9225G_D7_CLK	, ENABLE );

	
	/* 配置FSMC相对应的数据线,FSMC-D0~D15 */	
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


	/* 配置FSMC相对应的控制线
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
	

  /* 配置LCD复位RST控制管脚*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = ILI9225G_RST_PIN; 
	GPIO_Init ( ILI9225G_RST_PORT, & GPIO_InitStructure );
		
	/* 配置LCD背光控制管脚BK*/
	GPIO_InitStructure.GPIO_Pin = ILI9225G_BK_PIN; 
	GPIO_Init ( ILI9225G_BK_PORT, & GPIO_InitStructure );
	GPIO_ResetBits( ILI9225G_BK_PORT, ILI9225G_BK_PIN );			//开启背光
	


  readWriteTiming.FSMC_AddressSetupTime = 0XF; //地址建立时间（ADDSET）为16个HCLK 1/168M=6ns*16=96ns	
  readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
  readWriteTiming.FSMC_DataSetupTime =  60;		//数据保存时间为60个HCLK	=6*60=360ns
  readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
  readWriteTiming.FSMC_CLKDivision = 0x00;
  readWriteTiming.FSMC_DataLatency = 0x00;
  readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
    

	writeTiming.FSMC_AddressSetupTime =9;	      //地址建立时间（ADDSET）为9个HCLK =54ns 
  writeTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A		
  writeTiming.FSMC_DataSetupTime = 8;	 //数据保存时间为6ns*9个HCLK=54ns
  writeTiming.FSMC_BusTurnAroundDuration = 0x00;
  writeTiming.FSMC_CLKDivision = 0x00;
  writeTiming.FSMC_DataLatency = 0x00;
  writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 

 
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
  FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;//存储器数据宽度为8bit   
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // 使能BANK1 
		
	ILI9225G_Rst(); 
 	LCD_WR_REG(0X00);	
	lcddev.id=LCD_RD_DATA();	//读到92
	lcddev.id<<=8;
	lcddev.id|=LCD_RD_DATA();	//读到26  
	ILI9225G_Rst(); 
	/*************对ＬＣＤ屏参数设置************************/	
	LCD_Set();  //设置显示屏参数	 需要设置，显示功能都用到设置的．		
	
	/*************以下开始初始化*******************/	
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
函数名：LCD_Set
功能：Lcd设置相关寄存器参数集，
返回值：无
***********************************************/
void LCD_Set(void)
{
//9225	OK
	lcddev.dir=0;	     //竖屏
	lcddev.width=176;    //LCD 宽度
	lcddev.height=220;   //LCD 高度

	lcddev.setxcmd=0X0020;  //设置x坐标指令
	lcddev.setycmd=0X0021;  //设置y坐标指令
	lcddev.wramcmd=0X0022;  //开始写gram指令	
	
}


/*******************************************************************************
* Function Name  : LCD_Clear   modify YZ
* Description    ://清屏函数   
* Input          : color:要清屏的填充色
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Clear(u16 color)
{
	u32 index=0;     
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//得到总点数
	
	BlockWrite(0,lcddev.width,0,lcddev.height);

	for(index=0;index<totalpoint;index++)
	{
		LCD_WriteRAM(color);
//		LCD->LCD_RAM=color;
	
	}	
	
} 


/*******************************************************************************
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
字符串起始地址		
说明：字符是有点组成的，故在字符函数中不断调用画点函数LCD_DrawPoint();来画点（往LCD->LCD_RAM里写颜色值，这个颜色值在lcd.h里被定义为16位的地址）。
*******************************************************************************/
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}



/*******************************************************************************
* Function Name  : LCD_Display
* Description    : 显示填充色
* Input          : color:要显示的填充色
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
* Description    : 九宫格显示:RGB-BGR-RGB
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
	LCD_ShowString(0,0,210,16,16,"~00%    "); 	//电压在3.60一下视为无电
}

/*******************************************************************************
* Function Name  : Show_Gray
* Description    : 灰阶显示
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
			c16h=c16h+0x1082;//32阶 0841 16阶 1082 8阶2104
			}
		}
}


/*******************************************************************************
* Function Name  : LCD_Frame
* Description    : 四边线＋中间区域内填充单个颜色  
* Input          :RGB_Data为中间纯色， f_data为四边框线色
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
*一下TFT的彩色,一个点占2个字节RGB565,也就是一个点可以有2^16种色调,一个个点组成了图片
* 图片为：176－220 在屏中间显示
*注：瓦力LOG存在外部FLASH的第 16*1024*1024-80000之后77448个字节 
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
//			datatemp[0]=gImage[pos*lcddev.width*2+t]*256+gImage[pos*lcddev.width*2+t+1];  //这个地方  改成gImage[pos*480+t]<<8+gImage[pos*480+t+1];  好像有问题
			
			datatemp[0]=W25QXX_Read_Dyte(FLASH_SIZE-80000+pos*lcddev.width*2+t)*256+W25QXX_Read_Dyte(FLASH_SIZE-80000+pos*lcddev.width*2+t+1);  //这个地方  改成gImage[pos*480+t]<<8+gImage[pos*480+t+1];  好像有问题
			t++;                                
			LCD_WriteRAM(datatemp[0]);                               
		}                                
	}         	
	
}	






