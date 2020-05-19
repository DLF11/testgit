#include "sys.h"
#include "usart.h"	
#include "led.h" 
#include "data.h"


extern uint32_t   usart1_buffer_len;	 //����1�������ݳ���
extern uint32_t   r_buffer_len;			 //����1�������ݳ���
extern uint32_t   usart1_buffer_data[60];
extern uint8_t    usart1_ka;
extern uint32_t	r_buffer_data[3];
extern uint8_t 	  sys_ready;
extern uint8_t   	G_ka_num;
uint8_t   	state,u1_ka_num;
uint8_t     flag_Ka;
uint8_t     run;						//����ָ���־λ
extern uint8_t H0;						//��Ͽ�������λ��־λ
extern uint8_t  G_led_download;	
extern  uint8_t play_mun;
extern uint16_t   time;						//������־

extern run_flag;
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
	USART2->DR = (u8) ch;      
	return ch;
}
#endif
 

//���ֽڷ���
void USART1_SendByte(  uint8_t ch )
{
	/* ����һ���ֽ����ݵ�USART1 */
	USART_SendData(USART1,ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
	
}


void USART2_SendByte(  uint8_t ch )
{
	/* ����һ���ֽ����ݵ�USART1 */
	USART_SendData(USART2,ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
	
}


void  Usart1Init(void)
{
	GPIO_InitTypeDef  gpioinit ;
	USART_InitTypeDef   initusart;
	NVIC_InitTypeDef   nvicinit;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  //ʹ�ܴ���һ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);    //ʹ�����ŵ�ʱ��


	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);//GPIO���Ÿ�������ӳ��pa9ӳ�䵽����
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//GPIO���Ÿ�������ӳ��pa10ӳ�䵽����

	gpioinit.GPIO_Pin = GPIO_Pin_9;    // ����9 
	gpioinit.GPIO_Mode = GPIO_Mode_AF;  // ģʽλ����ģʽ
	gpioinit.GPIO_OType = GPIO_OType_PP ;// ���������������� ��ʼ״̬
	gpioinit.GPIO_PuPd = GPIO_PuPd_UP; //������
	gpioinit.GPIO_Speed = GPIO_Speed_100MHz ; // �ٶ�

	GPIO_Init(GPIOA , &gpioinit);

	gpioinit.GPIO_Pin = GPIO_Pin_10;    // ����9 
	gpioinit.GPIO_Mode = GPIO_Mode_AF;  // ģʽλ����ģʽ
	gpioinit.GPIO_OType = GPIO_OType_PP ;// ���������������� ��ʼ״̬
	gpioinit.GPIO_PuPd = GPIO_PuPd_UP; //������
	gpioinit.GPIO_Speed = GPIO_Speed_100MHz ; // �ٶ�

	GPIO_Init(GPIOA , &gpioinit);

	initusart.USART_BaudRate=9600;                            //������
	initusart.USART_HardwareFlowControl=    USART_HardwareFlowControl_None      ;       //Ӳ����  TEC CTC ʲôһЩ����Ŷ
	initusart.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;                   //ģʽ,ʹ�ܷ��ͻ��߽���
	initusart.USART_Parity =    USART_Parity_No;                                    //��żУ��λ
	initusart.USART_StopBits =  USART_StopBits_1 ;                              //ֹͣλ
	initusart.USART_WordLength = USART_WordLength_8b;                   //�ֳ�

	USART_Init(USART1,&initusart);    //���д��ڵĳ�ʼ��

	USART_Cmd(USART1,ENABLE);                   // ����ʹ�ܺ���

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);   //ʹ�ܴ��ڽ�������ж�
	// ��ʼ���ж�
	nvicinit.NVIC_IRQChannel=       USART1_IRQn             ;//ͨ��
	nvicinit.NVIC_IRQChannelCmd=        ENABLE      ;//
	nvicinit.NVIC_IRQChannelPreemptionPriority= 1   ;//
	nvicinit.NVIC_IRQChannelSubPriority  =      1       ;//

	NVIC_Init(&nvicinit);
}



//�жϷ�����
void USART1_IRQHandler(void)
{
	u8  data,temp=0;
	u8  j;
	//�ж��ǲ��ǽ����ж� 
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
					out_init();		//���״̬��ʼ��
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
					out_init();		//���״̬��ʼ��
					usart1_buffer_len=0;
				}
				break;
			case 2:
				if(usart1_buffer_data[usart1_buffer_len]==0x55)			//����ָ��
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
					out_init();		//���״̬��ʼ��
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

					out_init();		//���״̬��ʼ��
				}
				break;
			case 4:
				run=0;
			    G_led_download=0;
				G_ka_num=usart1_buffer_data[usart1_buffer_len]/2;   //��Э���3λΪ��������2����
			  u1_ka_num=2*(G_ka_num-1)+1;							//
				if(G_ka_num>0&&G_ka_num<30)							//�ƶ����ˢ������30
				{
						state++;	
						usart1_buffer_len++;	
				}
				else if(usart1_buffer_data[usart1_buffer_len]==0x01)//����01��Ϊ��������ָ��
				{
						state++;	
						usart1_buffer_len++;	
						out_init();		//���״̬��ʼ��								
											
				}
				else
				{
					state=0;
					usart1_buffer_len=0;
					flag_Ka=0;
					G_led_download=0;
					out_init();		//���״̬��ʼ��
					usart1_buffer_len=0;
				}			
		  	break;
			case 5:
				run=0;
			    G_led_download=0;
				if(usart1_buffer_len!=(2*G_ka_num+6))     ///ִ�п�����ָ�G_ka_num=usart1_buffer_data[usart1_buffer_len]/2�õ�G_ka_num=0;
				{ 
					state++;	
//					Data_Clear()  ;      				 //���֮ǰ���ݻ���
				}
				else if((G_ka_num==0x00)&&(usart1_buffer_data[usart1_buffer_len]==0x00))		//��ָ��ȷ��
				{	
					
					state=7;
					sys_ready=1;
					play_mun=1;
				}

			    out_init();		//���״̬��ʼ��
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
								out_init();		//���״̬��ʼ��
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
					out_init();		//���״̬��ʼ��
				}else G_led_download=0;run=0;state=0;usart1_buffer_len=0;
				break;
			default:
						state=0;
						usart1_buffer_len=0;
						run=0;
						out_init();		//���״̬��ʼ��
						flag_Ka=0;	

			break;
			
				
		}
		
	}
}



void Data1_Clear(void)           //��ջ���
{
	r_buffer_data[0]= 0x00 ;
	r_buffer_data[1]= 0x00 ;
	r_buffer_data[2]= 0x00 ;
	r_buffer_len=0;
}





void Data2_Clear(void)           //��ջ���
{
	usart1_buffer_data[0]= 0x00 ;
	usart1_buffer_data[1]= 0x00 ;
	usart1_buffer_data[2]= 0x00 ;
}

void Data3_Clear(void)           //��ջ���
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


void Data_Clear(void)           //��ջ���
{
	u8 i;
	usart1_buffer_data[0]= 0x00 ;
	for(i=6;i<30;i++)
	{
		usart1_buffer_data[i]= 0x00 ;
	}
}



