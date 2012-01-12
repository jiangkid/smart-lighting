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
extern u16 waitmessagebacktime;		//�ȴ�ָ��ʱ��
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
extern u8 USART2_RXBUF[U2RX_LENGTH];     //���ջ�������,���128���ֽ�.	
/******************************************************************************/
u8 zigbee_checkwrong=0;										//zigbeeУ��������
/******************************************************************************/
/****************************************************************
���ܣ���������ʱ�����������͵���Ϣ
������
���أ�
˵����
****************************************************************/
void Gprs_FirstMessage(void)
{
	USART2_TXBUF[0]=0x2F;				//�������
	USART2_TXBUF[1]=0x2F;
	USART2_TXBUF[2]=0x2F;
	USART2_TXBUF[3]=ee_gid;	
	USART2_TXBUF[4]=0xCC;
	Gprs_Send(USART2_TXBUF,"005");	  //����IMSI	
}
/****************************************************************
���ܣ��ж���ʾ���յ�gprs��Ϣ�󣬴��������Ϣ����
������
���أ�
˵����
****************************************************************/
void Gprs_Receivefunc(void)
{
	if(mc52i_ok==0x12)
	{	
		GPRSSTATE=GPRS_NORMAL;				//��Ϣ������ϣ��л�����ͨ״̬					
		Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴  ^SISR:1
		Clear_U2rxbuf();
		printf("Receive GPRS Message!\n"); 
		Gprs_Rec();
		gprssendmessagetiming=0;			//����ʱ�䣬���¶�ʱ
		printf("GPRS Message have done!\n"); 
		
	}
	else
	{
		GPRSSTATE=GPRS_NORMAL;				//��Ϣ������ϣ��л�����ͨ״̬	
	}
}
/****************************************************************
���ܣ�gprs����������
������
���أ�
˵����
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
		GPRSSTATE=GPRS_NORMAL;				//��Ϣ������ϣ��л�����ͨ״̬	
	}
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void Gprs_DealReceiveMessage(void)
{  
	if(gprsaccept.ID.GID==ee_gid)     //���͹�����GID�Ƿ�����ص�GIDһ��
	{
		memcpy(&zigbeesend,&gprsaccept,21);
		zigbeesend.check=Xor(&(zigbeesend.aim),16);	//ȡ���ֵ
		memcpy(USART3_TXBUF,&zigbeesend,21);


		Usart3_Sendstring(USART3_TXBUF,21);	   //����Ϣ���͸�zigbee
		waitmessagebacktime=0;		   			//�����ʱ
		TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2 	
		Clear_U3rxbuf();							//���Zigbee���ջ���
		while(waitmessagebacktime<50)    				//���ȴ�ʱ�� waitgprsbacktime=50 ʱ Ϊ5s
		{
			if(U3_K>15)   						 //����Ҳ��24���ַ�
			{
				Delay_Nms(50);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
				break;	
			}
		}
		TIM2->CR1&=~(0x01);    					//�����ʱ��2


		if(waitmessagebacktime<50)	   //���յ�����
		{
			memcpy(&zigbeeaccept,USART3_RXBUF,21);						
			if((USART3_RXBUF[0]==0x2F)&&(USART3_RXBUF[20]==0xCC)&&(Xor(&(zigbeeaccept.aim),17)==0))				 //������Ϣ���Ե�
			{															//���а�����ZiGBEE�Լ����������Ϣ�����ֻ���G��Z֮���ͨ����û�г���
				ZIGBEESTATE=ZIGBEE_NORMAL;

				memcpy(&gprssend,&zigbeeaccept,12);							//�������
				gprssend.command=zigbeeaccept.commandtarget;
				gprssend.action=zigbeeaccept.action;
				memcpy(&gprssend.data,&zigbeeaccept.actiondata,4);
				gprssend.end=zigbeesend.end;							
				memcpy(USART2_TXBUF,&gprssend,19); 	     //ת����Ϣ��gprs 
				Gprs_Send(USART2_TXBUF,"019");	  //������Ϣ
				LED1=1;			
				
				printf("Receive zigbee check OK!\n");
				zigbee_checkwrong=0;					 //���
			}
			else											  //У�����
			{	
				ZIGBEESTATE=ZIGBEE_CHECKWRONG;
				printf("Receive zigbee check error!\n");
				zigbee_checkwrong++;
				LED1=1;	
				if(zigbee_checkwrong>=4)
				{
					ZIGBEESTATE=ZIGBEE_NORMAL;

					memcpy(&gprssend,&zigbeeaccept,12);							//�������
					gprssend.mark=0x31;											//0x31��ʾ����,0x30��ʾ�����ɹ�
					gprssend.command=zigbeeaccept.commandtarget;
					gprssend.action=zigbeeaccept.action;
					memcpy(&gprssend.data,&zigbeeaccept.actiondata,4);
					gprssend.end=zigbeesend.end;	
						
					memcpy(USART2_TXBUF,&gprssend,19); 	   			 //ת����Ϣ��gprs 
					Gprs_Send(USART2_TXBUF,"019");	 				 //������Ϣ
					zigbee_checkwrong=0;					 //���	
				}															
			}			
			printf("Zigbee return message:\n");
			Delay_Nms(1);
			Usart1_SendString(USART3_RXBUF);		//���͵����ڲ鿴
			Clear_U3rxbuf();							//���Zigbee���ջ���
		}
		else										 //û�����ݽ���
		{
			ZIGBEESTATE=ZIGBEE_NORMAL;
			memcpy(&gprssend,&zigbeeaccept,12);							//�������
			gprssend.mark=0x31;											//0x31��ʾ����,0x30��ʾ�����ɹ�
			gprssend.command=zigbeeaccept.commandtarget;
			gprssend.action=zigbeeaccept.action;
			memcpy(&gprssend.data,&zigbeeaccept.actiondata,4);
			gprssend.end=zigbeesend.end;	
				
			memcpy(USART2_TXBUF,&gprssend,19); 	   			 //ת����Ϣ��gprs 
			Gprs_Send(USART2_TXBUF,"019");	 				 //������Ϣ
			LED1=0;
			zigbee_checkwrong=0;					 //���	
		}
	}
	else					//���͵������ID��������
	{
		__nop();
	}	
}
