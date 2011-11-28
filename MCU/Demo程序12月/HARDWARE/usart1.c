#include "test.h"
#include "usart1.h"
/***********************************************
功能：通过串口一发送1bit的数据
参数：utbyte为发送的ascii值，如0x31 
返回：NULL
说明：
***********************************************/
void Usart1_Send_Byte(u8 utbyte)
{
	USART1->DR=utbyte;
	while((USART1->SR&0X40)==0);//等待发送结束
}
/***********************************************
功能：发送字符串
参数：utstring为发送内容，如“abcdefg”
返回：NULL
说明：
***********************************************/
void Usart1_Send_String(u8 utstring[])
{
	 u16 i=0;
	 for(i=0;i<strlen(utstring);i++)
	 {
	 	 Usart1_Send_Byte(utstring[i]);
	 }
}
/***********************************************
功能：发送字符串
参数：utstring为发送内容，如“abcdefg”
返回：NULL
说明：
***********************************************/
void Usart1_Send_String1(u8 *pstring,u8 ut1num)
{
	 u16 i=0;
	 for(i=0;i<ut1num;i++)
	 {
	 	 Usart1_Send_Byte(*pstring);
		 pstring++;
	 }
}

