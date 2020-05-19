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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//WAV 解码代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/6/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved				
//********************************************************************************
//V1.0 说明
//1,支持16位/24位WAV文件播放
//2,最高可以支持到192K/24bit的WAV格式. 
////////////////////////////////////////////////////////////////////////////////// 	

extern uint8_t     flag_Ka;
extern uint8_t     run;	
extern u32 FLASH_SIZE;
extern uint8_t   G_led_download;	
extern uint8_t   sys_ready;
extern uint8_t play_mun;


__wavctrl wavctrl;		//WAV控制结构体
vu8 wavtransferend=0;	//i2s传输完成标志
vu8 wavwitchbuf=0;		//i2sbufx指示标志
u32 addflag=0;

u8 Buffer[8192];

//WAV解析初始化
//fname:文件路径+文件名
//wavx:wav 信息存放结构体指针
//返回值:0,成功;1,打开文件失败;2,非WAV文件;3,DATA区域未找到.
u8 wav_decode_init(void)
{
	FIL*ftemp;
	u8 *buf; 
	
	ftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
	buf=mymalloc(SRAMIN,512);
	f_close(ftemp);
	myfree(SRAMIN,ftemp);//释放内存
	myfree(SRAMIN,buf); 
	return 0;
}

//填充buf
//buf:数据区
//size:填充数据量
//bits:位数(16/24)
//返回值:读到的数据个数
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
//WAV播放时,I2S DMA传输回调函数
void wav_i2s_dma_tx_callback(void) 
{   
	u16 i;
	if(DMA1_Stream5->CR&(1<<19))
	{
		wavwitchbuf=0;
		if((audiodev.status&0X01)==0)
		{
			for(i=0;i<WAV_I2S_TX_DMA_BUFSIZE;i++)//暂停
			{
				audiodev.i2sbuf1[i]=0;//填充0
			}
		}
	}else 
	{
		wavwitchbuf=1;
		if((audiodev.status&0X01)==0)
		{
			for(i=0;i<WAV_I2S_TX_DMA_BUFSIZE;i++)//暂停
			{
				audiodev.i2sbuf2[i]=0;//填充0
			}
		}
	}
	wavtransferend=1;
} 
//得到当前播放时间
//fx:文件指针
//wavx:wav播放控制器
void wav_get_curtime(FIL*fx,__wavctrl *wavx)
{
	long long fpos;  	
 	wavx->totsec=wavx->datasize/(wavx->bitrate/8);	//歌曲总长度(单位:秒) 
	fpos=fx->fptr-wavx->datastart; 					//得到当前文件播放到的地方 
	wavx->cursec=fpos*wavx->totsec/wavx->datasize;	//当前播放到第多少秒了?	
}
//播放某个WAV文件
//fname:wav文件路径.
//返回值:
//KEY0_PRES:下一曲
//KEY1_PRES:上一曲
//其他:错误
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
		res=wav_decode_init();//得到文件的信息
		if(res==0)//解析文件成功
		{
			
			WM8978_I2S_Cfg(2,0);	//飞利浦标准,16位数据长度
			I2S2_Init(I2S_Standard_Phillips,I2S_Mode_MasterTx,I2S_CPOL_Low,I2S_DataFormat_16bextended);		//飞利浦标准,主机发送,时钟低电平有效,16位扩展帧长度
		
			I2S2_SampleRate_Set();//设置采样率
			I2S2_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE/2); //配置TX DMA
			i2s_tx_callback=wav_i2s_dma_tx_callback;			//回调函数指wav_i2s_dma_callback
			audio_stop();
			res=0;
			if(res==0)
			{
				fillnum=wav_buffill(audiodev.i2sbuf1,WAV_I2S_TX_DMA_BUFSIZE,16);
				fillnum=wav_buffill(audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE,16);
				
				audio_start();  
				while(res==0)
				{ 
					
					while(wavtransferend==0);//等待wav传输完成; 
					wavtransferend=0;
					if(fillnum!=1)//播放结束?
					{
						addflag=0;
						break;
					} 
 					if(wavwitchbuf)fillnum=wav_buffill(audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE,16);//填充buf2
					else fillnum=wav_buffill(audiodev.i2sbuf1,WAV_I2S_TX_DMA_BUFSIZE,16);//填充buf1

				}
				audio_stop(); 
			}else res=0XFF; 
		}else res=0XFF;
	}else res=0XFF; 
	myfree(SRAMIN,audiodev.tbuf);	//释放内存
	myfree(SRAMIN,audiodev.i2sbuf1);//释放内存
	myfree(SRAMIN,audiodev.i2sbuf2);//释放内存 
	myfree(SRAMIN,audiodev.file);	//释放内存 
} 
	









