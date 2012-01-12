#include "test.h"
#include "usartx.h"
#include "usart.h"
/******************************************************************************/
extern u8 USART3_RXBUF[U3RX_LENGTH];
extern u8 U3_K;
extern u8 USART2_RXBUF[U2RX_LENGTH];     //���ջ�������,���128���ֽ�.
extern u8 U2_K;					  //�������ݳ��ȱ�־ 
extern u8 GPRSSTATE;
extern u8 ZIGBEESTATE;
/******************************************************************************/
/****************************************************************
���ܣ�ͨ������һ����1bit������
������utbyteΪ���͵�asciiֵ����0x31 
���أ�NULL
˵����
****************************************************************/
void Usart1_SendByte(u8 utbyte)
{
	USART1->DR=utbyte;
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���
}
/****************************************************************
���ܣ������ַ���
������utstringΪ�������ݣ��硰abcdefg��
���أ�NULL
˵��������������ܷ��Ͱ���0x00�ַ��ĺ���
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
���ܣ���ʼ������2
������pclk2,PCLK2ʱ��Ƶ��(���36M)��boundΪ������
���أ�NULL
˵����
****************************************************************/
void Usart2_Init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 

 	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB1ENR|=1<<17;  //ʹ�ܴ���2ʱ�� 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO״̬����
		  
	RCC->APB1RSTR|=1<<17;   //��λ����2
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=mantissa; // ����������	 
	USART2->CR1|=0X200C;  //1λֹͣ,��У��λ.
	USART2->SR&=0x00000040; // �ȶ�һ��TCֵ����ֹ��һ�����ݳ���

	//ʹ�ܽ����ж�
	USART2->CR1|=1<<8;    //PE�ж�ʹ��
	USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(1,2,USART2_IRQChannel,2);//��2
}
/****************************************************************
���ܣ�����2�����жϺ���
������GPRS_RECEIVE������gprs��Ϣ״̬��^SISR: 1, 1��
���أ�NULL
˵��������������128�����ݣ����������Զ�����
****************************************************************/
void USART2_IRQHandler(void)
{   
	if(USART2->SR&(1<<5))//���յ����� 
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
���ܣ�ͨ������2һ����1bit������
������utbyteΪ���͵�ASCII��ֵ����0x31 
���أ�NULL
˵����
****************************************************************/
void Usart2_SendByte(u8 u2tbyte)
{
	USART2->DR=u2tbyte;
	while((USART2->SR&0X40)==0);//�ȴ����ͽ���
}
/****************************************************************
���ܣ�����������ݺ���
������NULL
���أ�NULL
˵����
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
���ܣ���ʼ������3
������pclk2,PCLK2ʱ��Ƶ��(���36M)��boundΪ������
���أ�NULL
˵����
****************************************************************/
void Usart3_Init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 

 	RCC->APB2ENR|=1<<3;   //ʹ��PORTB��ʱ��  
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���3ʱ�� 
	GPIOB->CRH&=0XFFFF00FF; 
	GPIOB->CRH|=0X00008B00;//IO״̬����
		  
	RCC->APB1RSTR|=1<<18;   //��λ����3
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	   	   
	//����������
 	USART3->BRR=mantissa; // ����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ.
	USART3->SR&=0x00000040; // �ȶ�һ��TCֵ����ֹ��һ�����ݳ���


	//ʹ�ܽ����ж�
	USART3->CR1|=1<<8;    //PE�ж�ʹ��
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(2,2,USART3_IRQChannel,2);//��2
}
/****************************************************************
���ܣ�����3�����жϺ���
������
���أ�NULL
˵����
****************************************************************/
void USART3_IRQHandler(void)
{   
	if(USART3->SR&(1<<5))//���յ�����
	{
		USART3_RXBUF[U3_K]=USART3->DR;	
		U3_K++;
		if(U3_K>=21)
		{			
			U3_K=0;
		}
		if((USART3_RXBUF[0]==0x2F)&&(USART3_RXBUF[1]==0x2F)&&(USART3_RXBUF[2]==0x2F)&&(USART3_RXBUF[20]==0xCC))  //zigbeeЭ�����ʼ���ͽ�����
		{
			ZIGBEESTATE=ZIGBEE_ALARM;
		}	 
	}										       	 		 									     									 
}
/****************************************************************
���ܣ�ͨ������3һ����1bit������
������utbyteΪ���͵�ASCII��ֵ����0x31 
���أ�NULL
˵����
****************************************************************/
void Usart3_SendByte(u8 u3tbyte)
{
	USART3->DR=u3tbyte;
	while((USART3->SR&0X40)==0);//�ȴ����ͽ���
}
/****************************************************************
���ܣ�
������*pusart3sendΪ�������ݣ�usart3sendlengthΪ���͵ĳ���
���أ�NULL
˵����
****************************************************************/
void Usart3_Sendstring(u8 pusart3send[],u16 usart3sendlength)
{
	u16 i=0;
	for(i=0;i<usart3sendlength;i++)		 //�����ַ���
	{
		Usart3_SendByte(pusart3send[i]);
	}
}
/****************************************************************
���ܣ�
������
���أ�
˵����
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
