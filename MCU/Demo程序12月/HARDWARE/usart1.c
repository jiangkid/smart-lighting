#include "test.h"
#include "usart1.h"
/***********************************************
���ܣ�ͨ������һ����1bit������
������utbyteΪ���͵�asciiֵ����0x31 
���أ�NULL
˵����
***********************************************/
void Usart1_Send_Byte(u8 utbyte)
{
	USART1->DR=utbyte;
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���
}
/***********************************************
���ܣ������ַ���
������utstringΪ�������ݣ��硰abcdefg��
���أ�NULL
˵����
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
���ܣ������ַ���
������utstringΪ�������ݣ��硰abcdefg��
���أ�NULL
˵����
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

