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
���ܣ�zigbee��ʼ�����
������
���أ�
˵��������0x4F,0x3F,0x2F ������zigbee��zigbee�᷵��0x2F,0x3F,0x4F
****************************************************************/
void Zigbee_InitCheck(void)
{  	
	zigbeesend.head[0]=0x4F;
	zigbeesend.head[1]=0x3F;
	zigbeesend.head[2]=0x2F; 
	memcpy(USART3_TXBUF,&zigbeesend,21);

	Clear_U3rxbuf();							//���Zigbee���ջ���	
	Usart3_Sendstring(USART3_TXBUF,21);	   //����Ϣ���͸�zigbee	   //��һ�����͵�ʱ�򣬴�����ʾֻ�յ�0x3f2f����ɶ���
	waitmessagebacktime=0;		   			//�����ʱ
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2 	
	while(waitmessagebacktime<50)    				//���ȴ�ʱ�� waitgprsbacktime=500 ʱ Ϊ50s
	{
		if(U3_K>2)   						 //����Ҳ��3���ַ�
		{
			Delay_Nms(1);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	if(waitmessagebacktime<50)	   //���յ�����
	{
		if((USART3_RXBUF[0]==0x2F)&&(USART3_RXBUF[1]==0x3F)&&(USART3_RXBUF[2]==0x4F))	   //zigbee����0x2f3f4f����ʼ���ɹ�
		{
			printf("zigbee check ok!\n");
			WriteLCDData(1,1,"ZIGBEE         Y");				//��ʾ����
			zigbee_connect=1;
			LED1=1;
		}
		else
		{
			printf("zigbee check wrong!\n");
			WriteLCDData(1,1,"ZIGBEE         N");				//��ʾδ����
			LED1=0;
			zigbee_connect=0;
		}
		printf("zigbee receive message:");
		Usart1_SendString(USART3_RXBUF);		//���͵����ڲ鿴
		Clear_U3rxbuf();							//���Zigbee���ջ���
	}
	else										 //û�����ݽ���
	{
		WriteLCDData(1,1,"ZIGBEE         N");				//��ʾ����
		LED1=0;	
		zigbee_connect=0;
	}
}
/****************************************************************
���ܣ�zigbee������Ϣ
������
���أ�
˵����
****************************************************************/
void Zigbee_Alarmessage(void)
{
	memcpy(&zigbeeaccept,USART3_RXBUF,21);
	if(Xor(&(zigbeeaccept.aim),17)==0)	//У��ɹ�
	{
		memcpy(&gprssend,&zigbeeaccept,21);	
		memcpy(USART2_TXBUF,&gprssend,21);
		Gprs_Send(USART2_TXBUF,"021");	  //������Ϣ										
	}
	else									//У�����
	{
		__nop();
	}
}
/****************************************************************
���ܣ�����У��
������*pxor����Ҫ����У������ݣ�xorlength�����ݳ���
���أ�����ֵ
˵����
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

