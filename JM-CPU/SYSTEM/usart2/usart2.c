#include "usart2.h" 


extern uint32_t   usart2_buffer_len;	  //����2�������ݳ���
extern uint32_t    usart2_buffer_data[10];




//���ڷ��ͻ����� 
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; //���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
#ifdef USART2_RX_EN   //���ʹ���˽���    
//���ڽ��ջ����� 
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; //���ջ���,���USART2_MAX_RECV_LEN���ֽ�.


u16 USART2_RX_STA = 0;
void USART2_IRQHandler(void)
{
	u8 res;    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		res =USART_ReceiveData(USART2);//(USART1->DR); //��ȡ���յ�������

		if((USART2_RX_STA&0x8000)==0)//����δ���
		{
			if(USART2_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
				else USART2_RX_STA|=0x8000; //��������� 
			}
			else //��û�յ�0X0D
			{ 
				if(res==0x0d)USART2_RX_STA|=0x4000;
					else
					{
						USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res ;
						USART2_RX_STA++;
						if(USART2_RX_STA>(USART2_MAX_RECV_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����  
					}  
			}
		}    
	} 
		 
}  
#endif 



//bound:������  
void usart2_init(u32 bound)
{  
NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

//1.����ʱ�Ӻͺ�GPIOʱ��ʹ�ܡ�
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
//2.�������Ÿ�����ӳ�� 
GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2  
//3.GPIO�˿�ģʽ����
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3��ʼ��
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��GPIOA2����GPIOA3
//4.���ڲ�����ʼ�������ò����ʣ��ֳ�����żУ��Ȳ���
USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
USART_Init(USART2, &USART_InitStructure); //��ʼ������2����
//5.��ʼ��NVIC
NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�3
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
//6.�����ж�
USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�  
//7.ʹ�ܴ��� 
USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
}
















