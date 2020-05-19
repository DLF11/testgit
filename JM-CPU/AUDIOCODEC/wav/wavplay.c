#include "wavplay.h" 
#include "audioplay.h"
#include "usart.h" 
#include "delay.h" 
#include "malloc.h"
#include "ff.h"
#include "i2s.h"
#include "wm8978.h"
#include "key.h"
#include "w25qxx.h"


//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//WAV �������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved				
//********************************************************************************
//V1.0 ˵��
//1,֧��16λ/24λWAV�ļ�����
//2,��߿���֧�ֵ�192K/24bit��WAV��ʽ. 
////////////////////////////////////////////////////////////////////////////////// 	

extern uint8_t     flag_Ka;
extern uint8_t     run;	
extern u32 FLASH_SIZE;
extern uint8_t   G_led_download;	
extern uint8_t   sys_ready;
extern uint8_t play_mun;


__wavctrl wavctrl;		//WAV���ƽṹ��
vu8 wavtransferend=0;	//i2s������ɱ�־
vu8 wavwitchbuf=0;		//i2sbufxָʾ��־
u32 addflag=0;

u8 Buffer[8192];

//WAV������ʼ��
//fname:�ļ�·��+�ļ���
//wavx:wav ��Ϣ��Žṹ��ָ��
//����ֵ:0,�ɹ�;1,���ļ�ʧ��;2,��WAV�ļ�;3,DATA����δ�ҵ�.
u8 wav_decode_init(void)
{
	FIL*ftemp;
	u8 *buf; 
	
	ftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
	buf=mymalloc(SRAMIN,512);
	f_close(ftemp);
	myfree(SRAMIN,ftemp);//�ͷ��ڴ�
	myfree(SRAMIN,buf); 
	return 0;
}

//���buf
//buf:������
//size:���������
//bits:λ��(16/24)
//����ֵ:���������ݸ���
u32 wav_buffill(u8 *buf,u16 size,u8 bits)
{
	u32 bread;
	u32 i=0;
	if((G_led_download==0)&&(run==0)&&(play_mun==1))
	{
		if(addflag<838844)
		{
			W25QXX_Read_Num(FLASH_SIZE+addflag-1420000,8192); 
			for(i=0;i<=8192;i++)buf[i]=Buffer[i]; 
		
			bread=1;
		}
		else
		{ 
			for(i=0;i<8192;i++)
			{
				audiodev.i2sbuf1[i]=0;
				audiodev.i2sbuf2[i]=0;
				play_mun=0;
			} 
			bread=0;
		}	
		
	}
	else if((G_led_download==1)&&(run==0)&&(play_mun==1))
	{
		if(addflag<60000)
		{
			W25QXX_Read_Num(FLASH_SIZE+addflag-495000,8192); 
			for(i=0;i<=8192;i++)buf[i]=Buffer[i]; 
			bread=1;
		}
		else
		{ 
			for(i=0;i<8192;i++)
			{
				audiodev.i2sbuf1[i]=0;
				audiodev.i2sbuf2[i]=0;
				play_mun=0;
			} 
			bread=0;
		}	
	}
	else if((G_led_download==0)&&(run==1)&&(play_mun==1))
	{
		if(addflag<70000)
		{
			W25QXX_Read_Num(FLASH_SIZE+addflag-200000,8192); 
			for(i=0;i<=8192;i++)buf[i]=Buffer[i]; 
		
			bread=1;
		}
		else
		{ 
			for(i=0;i<8192;i++)
			{
				audiodev.i2sbuf1[i]=0;
				audiodev.i2sbuf2[i]=0;
				play_mun=0;
			} 
			bread=0;
		}	
	}
	addflag=addflag+8192;
return bread;
}  
//WAV����ʱ,I2S DMA����ص�����
void wav_i2s_dma_tx_callback(void) 
{   
	u16 i;
	if(DMA1_Stream5->CR&(1<<19))
	{
		wavwitchbuf=0;
		if((audiodev.status&0X01)==0)
		{
			for(i=0;i<WAV_I2S_TX_DMA_BUFSIZE;i++)//��ͣ
			{
				audiodev.i2sbuf1[i]=0;//���0
			}
		}
	}else 
	{
		wavwitchbuf=1;
		if((audiodev.status&0X01)==0)
		{
			for(i=0;i<WAV_I2S_TX_DMA_BUFSIZE;i++)//��ͣ
			{
				audiodev.i2sbuf2[i]=0;//���0
			}
		}
	}
	wavtransferend=1;
} 
//�õ���ǰ����ʱ��
//fx:�ļ�ָ��
//wavx:wav���ſ�����
void wav_get_curtime(FIL*fx,__wavctrl *wavx)
{
	long long fpos;  	
 	wavx->totsec=wavx->datasize/(wavx->bitrate/8);	//�����ܳ���(��λ:��) 
	fpos=fx->fptr-wavx->datastart; 					//�õ���ǰ�ļ����ŵ��ĵط� 
	wavx->cursec=fpos*wavx->totsec/wavx->datasize;	//��ǰ���ŵ��ڶ�������?	
}
//����ĳ��WAV�ļ�
//fname:wav�ļ�·��.
//����ֵ:
//KEY0_PRES:��һ��
//KEY1_PRES:��һ��
//����:����
void wav_play_song(void)
{
	u8 res;  
	u32 fillnum; 
	audiodev.file=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
	audiodev.i2sbuf1=mymalloc(SRAMIN,WAV_I2S_TX_DMA_BUFSIZE);
	audiodev.i2sbuf2=mymalloc(SRAMIN,WAV_I2S_TX_DMA_BUFSIZE);
	audiodev.tbuf=mymalloc(SRAMIN,WAV_I2S_TX_DMA_BUFSIZE);
	if(audiodev.file&&audiodev.i2sbuf1&&audiodev.i2sbuf2&&audiodev.tbuf)
	{ 
		res=wav_decode_init();//�õ��ļ�����Ϣ
		if(res==0)//�����ļ��ɹ�
		{
			
			WM8978_I2S_Cfg(2,0);	//�����ֱ�׼,16λ���ݳ���
			I2S2_Init(I2S_Standard_Phillips,I2S_Mode_MasterTx,I2S_CPOL_Low,I2S_DataFormat_16bextended);		//�����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,16λ��չ֡����
		
			I2S2_SampleRate_Set();//���ò�����
			I2S2_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE/2); //����TX DMA
			i2s_tx_callback=wav_i2s_dma_tx_callback;			//�ص�����ָwav_i2s_dma_callback
			audio_stop();
			res=0;
			if(res==0)
			{
				fillnum=wav_buffill(audiodev.i2sbuf1,WAV_I2S_TX_DMA_BUFSIZE,16);
				fillnum=wav_buffill(audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE,16);
				
				audio_start();  
				while(res==0)
				{ 
					
					while(wavtransferend==0);//�ȴ�wav�������; 
					wavtransferend=0;
					if(fillnum!=1)//���Ž���?
					{
						addflag=0;
						break;
					} 
 					if(wavwitchbuf)fillnum=wav_buffill(audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE,16);//���buf2
					else fillnum=wav_buffill(audiodev.i2sbuf1,WAV_I2S_TX_DMA_BUFSIZE,16);//���buf1

				}
				audio_stop(); 
			}else res=0XFF; 
		}else res=0XFF;
	}else res=0XFF; 
	myfree(SRAMIN,audiodev.tbuf);	//�ͷ��ڴ�
	myfree(SRAMIN,audiodev.i2sbuf1);//�ͷ��ڴ�
	myfree(SRAMIN,audiodev.i2sbuf2);//�ͷ��ڴ� 
	myfree(SRAMIN,audiodev.file);	//�ͷ��ڴ� 
} 
	









