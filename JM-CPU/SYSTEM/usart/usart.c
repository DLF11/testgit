#include "sys.h"
#include "usart.h"	
#include "led.h" 
#include "data.h"


extern uint32_t   usart1_buffer_len;	 //串口1接收数据长度
extern uint32_t   r_buffer_len;			 //串口1接收数据长度
extern uint32_t   usart1_buffer_data[60];
extern uint8_t    usart1_ka;
extern uint32_t	r_buffer_data[3];
extern uint8_t 	  sys_ready;
extern uint8_t   	G_ka_num;
uint8_t   	state,u1_ka_num;
uint8_t     flag_Ka;
uint8_t     run;						//运行指令标志位
extern uint8_t H0;						//组合卡变量复位标志位
extern uint8_t  G_led_download;	
extern  uint8_t play_mun;
extern uint16_t   time;						//待机标志

extern run_flag;
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
	USART2->DR = (u8) ch;      
	return ch;
}
#endif
 

//单字节发送
void USART1_SendByte(  uint8_t ch )
{
	/* 发送一个字节数据到USART1 */
	USART_SendData(USART1,ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
	
}


void USART2_SendByte(  uint8_t ch )
{
	/* 发送一个字节数据到USART1 */
	USART_SendData(USART2,ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
	
}


void  Usart1Init(void)
{
	GPIO_InitTypeDef  gpioinit ;
	USART_InitTypeDef   initusart;
	NVIC_InitTypeDef   nvicinit;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  //使能串口一的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);    //使能引脚的时钟


	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);//GPIO引脚复用配置映射pa9映射到串口
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//GPIO引脚复用配置映射pa10映射到串口

	gpioinit.GPIO_Pin = GPIO_Pin_9;    // 引脚9 
	gpioinit.GPIO_Mode = GPIO_Mode_AF;  // 模式位复用模式
	gpioinit.GPIO_OType = GPIO_OType_PP ;// 输出类型是推免输出 初始状态
	gpioinit.GPIO_PuPd = GPIO_PuPd_UP; //上下拉
	gpioinit.GPIO_Speed = GPIO_Speed_100MHz ; // 速度

	GPIO_Init(GPIOA , &gpioinit);

	gpioinit.GPIO_Pin = GPIO_Pin_10;    // 引脚9 
	gpioinit.GPIO_Mode = GPIO_Mode_AF;  // 模式位复用模式
	gpioinit.GPIO_OType = GPIO_OType_PP ;// 输出类型是推免输出 初始状态
	gpioinit.GPIO_PuPd = GPIO_PuPd_UP; //上下拉
	gpioinit.GPIO_Speed = GPIO_Speed_100MHz ; // 速度

	GPIO_Init(GPIOA , &gpioinit);

	initusart.USART_BaudRate=9600;                            //波特率
	initusart.USART_HardwareFlowControl=    USART_HardwareFlowControl_None      ;       //硬件流  TEC CTC 什么一些鬼东西哦
	initusart.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;                   //模式,使能发送或者接受
	initusart.USART_Parity =    USART_Parity_No;                                    //奇偶校验位
	initusart.USART_StopBits =  USART_StopBits_1 ;                              //停止位
	initusart.USART_WordLength = USART_WordLength_8b;                   //字长

	USART_Init(USART1,&initusart);    //进行串口的初始化

	USART_Cmd(USART1,ENABLE);                   // 串口使能函数

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);   //使能串口接受完成中断
	// 初始化中断
	nvicinit.NVIC_IRQChannel=       USART1_IRQn             ;//通道
	nvicinit.NVIC_IRQChannelCmd=        ENABLE      ;//
	nvicinit.NVIC_IRQChannelPreemptionPriority= 1   ;//
	nvicinit.NVIC_IRQChannelSubPriority  =      1       ;//

	NVIC_Init(&nvicinit);
}



//中断服务函数
void USART1_IRQHandler(void)
{
	u8  data,temp=0;
	u8  j;
	//判断是不是接受中断 
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		data=USART_ReceiveData(USART1);
		time=0;
		usart1_buffer_data[usart1_buffer_len] = data;
		data=0;
		//r_buffer_data[r_buffer_len]=data;
		switch(state)
		{
			case 0:
				if(usart1_buffer_data[usart1_buffer_len]==0x00)
				{
					state++;
					usart1_buffer_len++;
				}
				else
				{
					state=0;
					usart1_buffer_len=0;
					flag_Ka=0;
					run=0;
					out_init();		//输出状态初始化
					G_led_download=0;
					usart1_buffer_len=0; 
				}
				break;
			case 1:
				if(usart1_buffer_data[usart1_buffer_len]==0x53)
				{
					state=3;
					usart1_buffer_len++;
				}
				else if(usart1_buffer_data[usart1_buffer_len]==0x52)
				{
					state++;
					usart1_buffer_len++;
				}
				else
				{
					state=0;
	 				usart1_buffer_len=0;
					flag_Ka=0;
					run=0;
					G_led_download=0;
					out_init();		//输出状态初始化
					usart1_buffer_len=0;
				}
				break;
			case 2:
				if(usart1_buffer_data[usart1_buffer_len]==0x55)			//运行指令
				{
					if(run==0)
					{
						run=1;
						run_flag=1;
						play_mun=1;
						H0=1;
						G_led_download=0;

					}
					else
					{
						run=0;
						G_led_download=0;						
					}
					state=0;
					usart1_buffer_len=0;
				}
				else
				{
					state=0;
					usart1_buffer_len=0;
					flag_Ka=0;
					run=0;
					G_led_download=0;
					out_init();		//输出状态初始化
				}
				break;
			case 3:
				run=0;
				G_led_download=0;			
				if(usart1_buffer_data[usart1_buffer_len]==0x54)
				{
					state++;
					usart1_buffer_len++;
				}
				else
				{
					state=0;
					usart1_buffer_len=0;	
					flag_Ka=0;

					out_init();		//输出状态初始化
				}
				break;
			case 4:
				run=0;
			    G_led_download=0;
				G_ka_num=usart1_buffer_data[usart1_buffer_len]/2;   //老协议第3位为卡数量的2倍，
			  u1_ka_num=2*(G_ka_num-1)+1;							//
				if(G_ka_num>0&&G_ka_num<30)							//制定最大刷卡数量30
				{
						state++;	
						usart1_buffer_len++;	
				}
				else if(usart1_buffer_data[usart1_buffer_len]==0x01)//等于01即为，空数据指令
				{
						state++;	
						usart1_buffer_len++;	
						out_init();		//输出状态初始化								
											
				}
				else
				{
					state=0;
					usart1_buffer_len=0;
					flag_Ka=0;
					G_led_download=0;
					out_init();		//输出状态初始化
					usart1_buffer_len=0;
				}			
		  	break;
			case 5:
				run=0;
			    G_led_download=0;
				if(usart1_buffer_len!=(2*G_ka_num+6))     ///执行空数据指令，G_ka_num=usart1_buffer_data[usart1_buffer_len]/2得到G_ka_num=0;
				{ 
					state++;	
//					Data_Clear()  ;      				 //清空之前数据缓存
				}
				else if((G_ka_num==0x00)&&(usart1_buffer_data[usart1_buffer_len]==0x00))		//空指令确认
				{	
					
					state=7;
					sys_ready=1;
					play_mun=1;
				}

			    out_init();		//输出状态初始化
			usart1_buffer_len++;
				break;
			case 6:
				run=0;
				if(u1_ka_num==2)
				 {
					sys_ready=1;
				 }
			   if(u1_ka_num)
				 {
					 usart1_buffer_len++;
					 u1_ka_num--;
				 }
				 else
				 {				
					   for(j=0;j<=2*G_ka_num;j++)
						 {
							 
							 temp=temp+usart1_buffer_data[3+j];
						 }
						 if(usart1_buffer_data[usart1_buffer_len]==temp)
						 {
								flag_Ka=1;
								state=0;
								usart1_buffer_len=0;	
								G_led_download=1;
								run=0;
							    play_mun=1;
								out_init();		//输出状态初始化
						 }else	G_led_download=0;run=0;state=0;usart1_buffer_len=0;
				 }

				break;
				case 7:
				if(usart1_buffer_data[usart1_buffer_len]==0X01)
				{
					flag_Ka=1;
					usart1_buffer_len=0;
					state=0;
					G_led_download=1;
					run=0;
					play_mun=1;
					out_init();		//输出状态初始化
				}else G_led_download=0;run=0;state=0;usart1_buffer_len=0;
				break;
			default:
						state=0;
						usart1_buffer_len=0;
						run=0;
						out_init();		//输出状态初始化
						flag_Ka=0;	

			break;
			
				
		}
		
	}
}



void Data1_Clear(void)           //清空缓存
{
	r_buffer_data[0]= 0x00 ;
	r_buffer_data[1]= 0x00 ;
	r_buffer_data[2]= 0x00 ;
	r_buffer_len=0;
}





void Data2_Clear(void)           //清空缓存
{
	usart1_buffer_data[0]= 0x00 ;
	usart1_buffer_data[1]= 0x00 ;
	usart1_buffer_data[2]= 0x00 ;
}

void Data3_Clear(void)           //清空缓存
{
	usart1_buffer_data[3]= 0x00 ;
	usart1_buffer_data[4]= 0x00 ;
	usart1_buffer_data[5]= 0x00 ;
	usart1_buffer_data[6]= 0x00 ;
	usart1_buffer_data[7]= 0x00 ;
	usart1_buffer_data[8]= 0x00 ;
	usart1_buffer_data[9]= 0x00 ;
	usart1_buffer_data[10]= 0x00 ;
	usart1_buffer_data[11]= 0x00 ;
	usart1_buffer_data[12]= 0x00 ;
	usart1_buffer_data[13]= 0x00 ;
	usart1_buffer_data[14]= 0x00 ;
	usart1_buffer_data[15]= 0x00 ;
	usart1_buffer_data[16]= 0x00 ;
	usart1_buffer_data[17]= 0x00 ;
	usart1_buffer_data[18]= 0x00 ;
	usart1_buffer_data[19]= 0x00 ;
	usart1_buffer_data[20]= 0x00 ;
}


void Data_Clear(void)           //清空缓存
{
	u8 i;
	usart1_buffer_data[0]= 0x00 ;
	for(i=6;i<30;i++)
	{
		usart1_buffer_data[i]= 0x00 ;
	}
}



