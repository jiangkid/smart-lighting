#include "test.h"
#include "zigbeefunc.h"
#include "usartx.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "gprs.h"
#include "12864.h"
/******************************************************************************/
extern u8 USART3_RXBUF[U3RX_LENGTH];
extern u8 U3_K;
extern u8 zigbee_connect;  
extern u16 waitmessagebacktime;
extern u8 USART3_TXBUF[U3TX_LENGTH]; 
extern zigbeecommunication_control zigbeesend;
extern zigbeecommunication_control zigbeeaccept;
extern gprssend_control gprssend;
extern u8 USART2_TXBUF[U2TX_LENGTH];
/******************************************************************************/
/****************************************************************
功能：zigbee初始化检测
参数：
返回：
说明：发送0x4F,0x3F,0x2F 连接了zigbee后，zigbee会返回0x2F,0x3F,0x4F
****************************************************************/
void Zigbee_InitCheck(void)
{  	
	zigbeesend.head[0]=0x4F;
	zigbeesend.head[1]=0x3F;
	zigbeesend.head[2]=0x2F; 
	memcpy(USART3_TXBUF,&zigbeesend,21);

	Clear_U3rxbuf();							//清除Zigbee接收缓存	
	Usart3_Sendstring(USART3_TXBUF,21);	   //将消息发送给zigbee	   //第一个发送的时候，串口显示只收到0x3f2f，，啥情况
	waitmessagebacktime=0;		   			//清除计时
	TIM2->CR1|=0x01;    					//使能定时器2 	
	while(waitmessagebacktime<50)    				//最多等待时间 waitgprsbacktime=500 时 为50s
	{
		if(U3_K>2)   						 //至少也得3个字符
		{
			Delay_Nms(1);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	if(waitmessagebacktime<50)	   //接收到数据
	{
		if((USART3_RXBUF[0]==0x2F)&&(USART3_RXBUF[1]==0x3F)&&(USART3_RXBUF[2]==0x4F))	   //zigbee返回0x2f3f4f，初始化成功
		{
			printf("zigbee check ok!\n");
			WriteLCDData(1,1,"ZIGBEE         Y");				//显示连接
			zigbee_connect=1;
			LED1=1;
		}
		else
		{
			printf("zigbee check wrong!\n");
			WriteLCDData(1,1,"ZIGBEE         N");				//显示未连接
			LED1=0;
			zigbee_connect=0;
		}
		printf("zigbee receive message:");
		Usart1_SendString(USART3_RXBUF);		//发送到串口查看
		Clear_U3rxbuf();							//清除Zigbee接收缓存
	}
	else										 //没有数据接收
	{
		WriteLCDData(1,1,"ZIGBEE         N");				//显示连接
		LED1=0;	
		zigbee_connect=0;
	}
}
/****************************************************************
功能：zigbee报警信息
参数：
返回：
说明：
****************************************************************/
void Zigbee_Alarmessage(void)
{
	memcpy(&zigbeeaccept,USART3_RXBUF,21);
	if(Xor(&(zigbeeaccept.aim),17)==0)	//校验成功
	{
		memcpy(&gprssend,&zigbeeaccept,21);	
		memcpy(USART2_TXBUF,&gprssend,21);
		Gprs_Send(USART2_TXBUF,"021");	  //发送消息										
	}
	else									//校验出错
	{
		__nop();
	}
}
/****************************************************************
功能：异或和校验
参数：*pxor，需要异或和校验的数据；xorlength，数据长度
返回：异或和值
说明：
****************************************************************/
u8 Xor(u8 *pxor, u16 xorlength)
{
	u16 i=0;
	u8 xorsum=0;
	for(i=0;i<xorlength;i++)
	{
		xorsum=xorsum^(*pxor);	
		pxor++;
	}
	return xorsum;
}

