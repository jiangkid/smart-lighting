#include "test.h"
#include "usartx.h"
#include "usart.h"
/******************************************************************************/
extern u8 USART3_RXBUF[U3RX_LENGTH];
extern u8 U3_K;
extern u8 USART2_RXBUF[U2RX_LENGTH];     //接收缓冲数组,最大128个字节.
extern u8 U2_K;					  //接收数据长度标志 
extern u8 GPRSSTATE;
extern u8 ZIGBEESTATE;
/******************************************************************************/
/****************************************************************
功能：通过串口一发送1bit的数据
参数：utbyte为发送的ascii值，如0x31 
返回：NULL
说明：
****************************************************************/
void Usart1_SendByte(u8 utbyte)
{
	USART1->DR=utbyte;
	while((USART1->SR&0X40)==0);//等待发送结束
}
/****************************************************************
功能：发送字符串
参数：utstring为发送内容，如“abcdefg”
返回：NULL
说明：这个函数不能发送包括0x00字符的函数
****************************************************************/
void Usart1_SendString(u8 utstring[])
{
	u16 i=0;
	for(i=0;i<mstrlen(utstring);i++)
	{
	 	Usart1_SendByte(utstring[i]);
	}
}
/****************************************************************
功能：初始化串口2
参数：pclk2,PCLK2时钟频率(最大36M)；bound为波特率
返回：NULL
说明：
****************************************************************/
void Usart2_Init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 

 	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;  //使能串口2时钟 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO状态设置
		  
	RCC->APB1RSTR|=1<<17;   //复位串口2
	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; // 波特率设置	 
	USART2->CR1|=0X200C;  //1位停止,无校验位.
	USART2->SR&=0x00000040; // 先读一次TC值，防止第一个数据出错

	//使能接收中断
	USART2->CR1|=1<<8;    //PE中断使能
	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(1,2,USART2_IRQChannel,2);//组2
}
/****************************************************************
功能：串口2接收中断函数
参数：GPRS_RECEIVE，接收gprs信息状态（^SISR: 1, 1）
返回：NULL
说明：缓存最多接收128个数据，超过数组自动清零
****************************************************************/
void USART2_IRQHandler(void)
{   
	if(USART2->SR&(1<<5))//接收到数据 
	{	 
		USART2_RXBUF[U2_K]=USART2->DR;	

		if(USART2_RXBUF[U2_K]=='1')									   //^SISR: 1, 1
		{  
			if((USART2_RXBUF[U2_K-8]=='I')&&(USART2_RXBUF[U2_K-7]=='S')&&(USART2_RXBUF[U2_K-6]=='R'))
				GPRSSTATE=GPRS_RECEIVE;	
		}
		U2_K++;
		if(U2_K>127)
		{
			U2_K=0;
		}
	}										       	 		 									     									 
}
/****************************************************************
功能：通过串口2一发送1bit的数据
参数：utbyte为发送的ASCII码值，如0x31 
返回：NULL
说明：
****************************************************************/
void Usart2_SendByte(u8 u2tbyte)
{
	USART2->DR=u2tbyte;
	while((USART2->SR&0X40)==0);//等待发送结束
}
/****************************************************************
功能：清除缓存数据函数
参数：NULL
返回：NULL
说明：
****************************************************************/
void Clear_U2rxbuf(void)
{
	u8 i=0;
	for(i=0;i<128;i++)
	{
		USART2_RXBUF[i]=0;
	}
	U2_K=0;
}
/****************************************************************
功能：初始化串口3
参数：pclk2,PCLK2时钟频率(最大36M)；bound为波特率
返回：NULL
说明：
****************************************************************/
void Usart3_Init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 

 	RCC->APB2ENR|=1<<3;   //使能PORTB口时钟  
	RCC->APB1ENR|=1<<18;  //使能串口3时钟 
	GPIOB->CRH&=0XFFFF00FF; 
	GPIOB->CRH|=0X00008B00;//IO状态设置
		  
	RCC->APB1RSTR|=1<<18;   //复位串口3
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
	USART3->SR&=0x00000040; // 先读一次TC值，防止第一个数据出错


	//使能接收中断
	USART3->CR1|=1<<8;    //PE中断使能
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(2,2,USART3_IRQChannel,2);//组2
}
/****************************************************************
功能：串口3接收中断函数
参数：
返回：NULL
说明：
****************************************************************/
void USART3_IRQHandler(void)
{   
	if(USART3->SR&(1<<5))//接收到数据
	{
		USART3_RXBUF[U3_K]=USART3->DR;	
		U3_K++;
		if(U3_K>=21)
		{			
			U3_K=0;
		}
		if((USART3_RXBUF[0]==0x2F)&&(USART3_RXBUF[1]==0x2F)&&(USART3_RXBUF[2]==0x2F)&&(USART3_RXBUF[20]==0xCC))  //zigbee协议的起始符和结束符
		{
			ZIGBEESTATE=ZIGBEE_ALARM;
		}	 
	}										       	 		 									     									 
}
/****************************************************************
功能：通过串口3一发送1bit的数据
参数：utbyte为发送的ASCII码值，如0x31 
返回：NULL
说明：
****************************************************************/
void Usart3_SendByte(u8 u3tbyte)
{
	USART3->DR=u3tbyte;
	while((USART3->SR&0X40)==0);//等待发送结束
}
/****************************************************************
功能：
参数：*pusart3send为发送内容，usart3sendlength为发送的长度
返回：NULL
说明：
****************************************************************/
void Usart3_Sendstring(u8 pusart3send[],u16 usart3sendlength)
{
	u16 i=0;
	for(i=0;i<usart3sendlength;i++)		 //发送字符串
	{
		Usart3_SendByte(pusart3send[i]);
	}
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Clear_U3rxbuf(void)
{
	u8 i=0;
	for(i=0;i<21;i++)
	{
		USART3_RXBUF[i]=0;
	}
	U3_K=0;
}
