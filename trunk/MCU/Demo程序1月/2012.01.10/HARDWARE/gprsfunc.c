#include "test.h"
#include "gprsfunc.h"
#include "gprs.h"
#include "usartx.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "zigbeefunc.h"
/******************************************************************************/
extern u8 ee_gid;
extern u8 ee_tid;
extern u8 USART3_RXBUF[U3RX_LENGTH];
extern u8 U3_K;
extern u16 waitmessagebacktime;		//等待指令时间
extern u8 USART2_TXBUF[U2TX_LENGTH];  
extern u8 USART3_TXBUF[U3TX_LENGTH];
extern zigbeecommunication_control gprsaccept;
extern gprssend_control gprssend; 
extern zigbeecommunication_control zigbeesend;
extern zigbeecommunication_control zigbeeaccept;  
extern u8 ZIGBEESTATE;
extern u8 GPRSSTATE;
extern u8 mc52i_ok;
extern u16 gprssendmessagetiming;
extern u8 USART2_RXBUF[U2RX_LENGTH];     //接收缓冲数组,最大128个字节.	
/******************************************************************************/
u8 zigbee_checkwrong=0;										//zigbee校验出错次数
/******************************************************************************/
/****************************************************************
功能：初次连接时给服务器发送的消息
参数：
返回：
说明：
****************************************************************/
void Gprs_FirstMessage(void)
{
	USART2_TXBUF[0]=0x2F;				//打包过程
	USART2_TXBUF[1]=0x2F;
	USART2_TXBUF[2]=0x2F;
	USART2_TXBUF[3]=ee_gid;	
	USART2_TXBUF[4]=0xCC;
	Gprs_Send(USART2_TXBUF,"005");	  //发送IMSI	
}
/****************************************************************
功能：中断提示接收到gprs消息后，处理接收消息函数
参数：
返回：
说明：
****************************************************************/
void Gprs_Receivefunc(void)
{
	if(mc52i_ok==0x12)
	{	
		GPRSSTATE=GPRS_NORMAL;				//消息处理完毕，切换到普通状态					
		Usart1_SendString(USART2_RXBUF);		//发送到串口查看  ^SISR:1
		Clear_U2rxbuf();
		printf("Receive GPRS Message!\n"); 
		Gprs_Rec();
		gprssendmessagetiming=0;			//心跳时间，重新定时
		printf("GPRS Message have done!\n"); 
		
	}
	else
	{
		GPRSSTATE=GPRS_NORMAL;				//消息处理完毕，切换到普通状态	
	}
}
/****************************************************************
功能：gprs心跳函数包
参数：
返回：
说明：
****************************************************************/
void Gprs_Heartbeatfunc(void)
{
	if(mc52i_ok==0x12)
	{	
		 GPRSSTATE=GPRS_NORMAL;
		 HeartBeat(0x1F);		 
	}
	else
	{
		GPRSSTATE=GPRS_NORMAL;				//消息处理完毕，切换到普通状态	
	}
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Gprs_DealReceiveMessage(void)
{  
	if(gprsaccept.ID.GID==ee_gid)     //发送过来的GID是否跟本地的GID一样
	{
		memcpy(&zigbeesend,&gprsaccept,21);
		zigbeesend.check=Xor(&(zigbeesend.aim),16);	//取异或值
		memcpy(USART3_TXBUF,&zigbeesend,21);


		Usart3_Sendstring(USART3_TXBUF,21);	   //将消息发送给zigbee
		waitmessagebacktime=0;		   			//清除计时
		TIM2->CR1|=0x01;    					//使能定时器2 	
		Clear_U3rxbuf();							//清除Zigbee接收缓存
		while(waitmessagebacktime<50)    				//最多等待时间 waitgprsbacktime=50 时 为5s
		{
			if(U3_K>15)   						 //至少也得24个字符
			{
				Delay_Nms(50);              	   //延时下，有些时候不止这么点长度
				break;	
			}
		}
		TIM2->CR1&=~(0x01);    					//清除定时器2


		if(waitmessagebacktime<50)	   //接收到数据
		{
			memcpy(&zigbeeaccept,USART3_RXBUF,21);						
			if((USART3_RXBUF[0]==0x2F)&&(USART3_RXBUF[20]==0xCC)&&(Xor(&(zigbeeaccept.aim),17)==0))				 //返回信息都对的
			{															//其中包含了ZiGBEE自己检测出错的消息，这边只检测G与Z之间的通信有没有出错
				ZIGBEESTATE=ZIGBEE_NORMAL;

				memcpy(&gprssend,&zigbeeaccept,12);							//打包数据
				gprssend.command=zigbeeaccept.commandtarget;
				gprssend.action=zigbeeaccept.action;
				memcpy(&gprssend.data,&zigbeeaccept.actiondata,4);
				gprssend.end=zigbeesend.end;							
				memcpy(USART2_TXBUF,&gprssend,19); 	     //转送消息给gprs 
				Gprs_Send(USART2_TXBUF,"019");	  //发送消息
				LED1=1;			
				
				printf("Receive zigbee check OK!\n");
				zigbee_checkwrong=0;					 //清空
			}
			else											  //校验出错
			{	
				ZIGBEESTATE=ZIGBEE_CHECKWRONG;
				printf("Receive zigbee check error!\n");
				zigbee_checkwrong++;
				LED1=1;	
				if(zigbee_checkwrong>=4)
				{
					ZIGBEESTATE=ZIGBEE_NORMAL;

					memcpy(&gprssend,&zigbeeaccept,12);							//打包数据
					gprssend.mark=0x31;											//0x31表示出错,0x30表示操作成功
					gprssend.command=zigbeeaccept.commandtarget;
					gprssend.action=zigbeeaccept.action;
					memcpy(&gprssend.data,&zigbeeaccept.actiondata,4);
					gprssend.end=zigbeesend.end;	
						
					memcpy(USART2_TXBUF,&gprssend,19); 	   			 //转送消息给gprs 
					Gprs_Send(USART2_TXBUF,"019");	 				 //发送消息
					zigbee_checkwrong=0;					 //清空	
				}															
			}			
			printf("Zigbee return message:\n");
			Delay_Nms(1);
			Usart1_SendString(USART3_RXBUF);		//发送到串口查看
			Clear_U3rxbuf();							//清除Zigbee接收缓存
		}
		else										 //没有数据接收
		{
			ZIGBEESTATE=ZIGBEE_NORMAL;
			memcpy(&gprssend,&zigbeeaccept,12);							//打包数据
			gprssend.mark=0x31;											//0x31表示出错,0x30表示操作成功
			gprssend.command=zigbeeaccept.commandtarget;
			gprssend.action=zigbeeaccept.action;
			memcpy(&gprssend.data,&zigbeeaccept.actiondata,4);
			gprssend.end=zigbeesend.end;	
				
			memcpy(USART2_TXBUF,&gprssend,19); 	   			 //转送消息给gprs 
			Gprs_Send(USART2_TXBUF,"019");	 				 //发送消息
			LED1=0;
			zigbee_checkwrong=0;					 //清空	
		}
	}
	else					//发送到错误的ID上面来了
	{
		__nop();
	}	
}
