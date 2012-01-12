#include "test.h"
#include "gprs.h"
#include "led.h"
#include "usartx.h"	
#include "delay.h"
#include "usart.h"
#include "gprsfunc.h"
#include "12864.h"
/****************************************************************************/
extern u8 USART2_RXBUF[U2RX_LENGTH];     //���ջ�������,���128���ֽ�.
extern u8 U2_K;					  //�������ݳ��ȱ�־ 	
extern u8 IP[IP_LENGTH];				    //������Ip��ַ
extern u16 waitmessagebacktime;			   //��ʱ��2�ۼ�ֵ,�ȴ�gprs����ʱ�� 
extern u8 mc52i_ok;					// Mc52iͨ���������ı�־λ

extern u8 ee_ip[EEIP_LENGTH];					//����EEPROM�е�IP��ַ+�˿ں���	  
/******************************************************************************/
extern zigbeecommunication_control gprsaccept;
/******************************************************************************/
/****************************************************************
���ܣ���ʼ��GPRS�˿�
������NULL
���أ�NULL
˵����PC8:gprs_on  PC9:gprs_off 
****************************************************************/
void GprsPort_Init(void)
{
	RCC->APB2ENR|=1<<4;      //ʹ��PORTCʱ��
	GPIOC->CRH&=0xFFFFFF00;	 //PC8 PC9 ����gprs����
	GPIOC->CRH|=0x00000033;
	GPIOC->ODR|=1<<8;
	GPIOC->ODR|=1<<9;
}
/****************************************************************
���ܣ�����ATָ��
������utstringΪ�������ݣ��硰abcdefg��
���أ�NULL
˵�����������0x00��β�ģ����Ͳ���0x00
****************************************************************/
void AT_Command(u8  utstring[])
{
	u16 i=0;
	for(i=0;i<mstrlen(utstring);i++)			 //�����ַ���
	{
		Usart2_SendByte(utstring[i]);
	}
}
/****************************************************************
���ܣ�����ATָ��
������*psendatcommandΪ�������ݣ�sendatcommandlengthΪ���͵ĳ���
���أ�NULL
˵�������Է���0x00
****************************************************************/
void Send_ATCommand(u8 *psendatcommand,u16 sendatcommandlength)
{
	while(sendatcommandlength--)		 //�����ַ���
	{
		Usart2_SendByte(*psendatcommand);
		psendatcommand++;
	}
}
/****************************************************************
���ܣ����鴮��2���յ������ݣ�GPRS�����Ƿ������OK��	����"ERROR"
�����������а���"OK"��GPRSͨ��������mc52i_ok=2;����"ERROR"����ͨ�ų���mc52i_ok=1;
���أ�����OK�򣬷���0x12������ERROR�򷵻�0x11����û�м�������ʱ�򷵻�0x10
˵�����������mc52i_ok=0ѡ��
****************************************************************/
u8 Check_OK(void)
{
	u8 i=0;
	for(i=0;i<U2RX_LENGTH;i++)
	{
		if((USART2_RXBUF[i]=='O')&&(USART2_RXBUF[i+1]=='K'))
		{
			return 0x12;
		}
		if((USART2_RXBUF[i]=='E')&&(USART2_RXBUF[i+1]=='R')&&(USART2_RXBUF[i+2]=='R')&&(USART2_RXBUF[i+3]=='O')&&(USART2_RXBUF[i+4]=='R'))		  // &&(USART2_RX_BUF[i+3]=='O')&&(USART2_RX_BUF[i+4]=='R')
		{
			return 0x11;
		} 
	}	
	return 0x10;	
}
/****************************************************************
���ܣ���ͨATָ�����OK���������ͺ���
������utstring[]��ATָ�gprscommand_waittime���ȴ����ص��ʱ��ʱ��
���أ� NULL
˵���� ����һ��ATָ��󣬿�ʼ�ȴ����أ������ȴ�ʱ���������û�н��յ����ݣ��򱨴�
****************************************************************/
void Gprs_Command(u8  utstring[],u16 gprscommand_waittime)
{	
	Clear_U2rxbuf();				//��������ջ���
	AT_Command(utstring); 							
	waitmessagebacktime=0;		   			//�����ʱ
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(waitmessagebacktime<gprscommand_waittime)    				//���ȴ�ʱ�� waitgprsbacktime=50 ʱ Ϊ5s
	{
		if(U2_K>5)   						 //����Ҳ��6���ַ���**OK**	
		{
			Delay_Nms(10);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2

	if(waitmessagebacktime<gprscommand_waittime)	   //���յ�����
	{  
		LED0=1;											//������GPRSģ��	
		mc52i_ok=Check_OK();
		if(mc52i_ok==0x12)
		{
			printf("Receive model message OK!\n");
			LED2=1;								//����ָʾͨ����OK����ERROR
		}
		else if(mc52i_ok==0x11)
		{
			printf("Receive model message ERROR!\n");
			LED2=0;
		}
		else
			__nop();
		Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴
	}
	else										 //û�����ݽ���
	{
		mc52i_ok=0x00;
		LED0=0;	
		LED2=0;
	}
}
/****************************************************************
���ܣ���ʼ��Mc52iģ��
������NULL
���أ�NULL
˵����1�����ò�����2.����SYSSTART��3��ATE0��4��AT+CREG=1;5.���棻6������+CREG: 2��
		7.���+CREG: 1(����)����+CREG: 5(����)
****************************************************************/
void Mc52i_Init(void)
{	
	u8 i=0;
	GPRS_OFF=0;								  //ģ��ر�
	Delay_Nms(500);
	GPRS_OFF=1;
	Delay_Nms(100);

	GPRS_ON=0;								//ģ�鿪��
	Delay_Nms(500);	
	GPRS_ON=1;	

//	Clear_U2rxbuf();
////1.���ò�����Ϊ9600������ȫ�µ�ģ����û�д�Ų�����ֵ�ģ���Ҫ�ֶ�����
//	Gprs_Command("AT+IPR=9600\r\n",50); 
//	Delay_Nms(1);
//2.SYSSTART
	Clear_U2rxbuf();	
	waitmessagebacktime=0; 					//�����ʱ					
 	TIM2->CR1|=0x01; 						//ʹ�ܶ�ʱ��2
	while(waitmessagebacktime<100)    				//���ȴ�ʱ��waitgprsbacktime=100 ʱ Ϊ10s
	{
		if(U2_K>5)   						 //����Ҳ��6���ַ���"^SYSSTART"
		{
			Delay_Nms(5);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	if(waitmessagebacktime<100)				//���յ�����   
	{
		for(i=0;i<U2RX_LENGTH;i++)
		{
			if((USART2_RXBUF[i]=='S')&&(USART2_RXBUF[i+1]=='Y')&&(USART2_RXBUF[i+2]=='S'))  //^SYSSTART
			{
				mc52i_ok=0x12;
				LED2=1;
				LED0=1;
				break;
			}
		} 
		Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴
	}
	else									 //û�н��յ�����,����Ϊģ��û������
	{
	  	mc52i_ok=0x00 ;
		LED0=0;
		LED2=0;
	}
//3.ATE0
   	if(mc52i_ok==0x12)							 //�رջ���
	{
		printf("ATE0\n");
		Gprs_Command("ATE0\r\n",50); 
		Delay_Nms(1);
	}
//4.AT+CREG=1
	if(mc52i_ok==0x12)						   //��������ע�������ṩ�������
	{   
		printf("AT+CREG=1\n");
		Gprs_Command("AT+CREG=1\r\n",50);
		Delay_Nms(1);
	}
//5.����AT+CREG=1��ֵ
	if(mc52i_ok==0x12)
	{
		printf("AT&W\n");
		Gprs_Command("AT&W\r\n",50);
		Delay_Nms(1);
	}
////6.+CREG:2
//	Clear_U2rxbuf();
//	waitmessagebacktime=0;					   //�����ʱ
//	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
//	while(waitmessagebacktime<100)    				//���ȴ�ʱ��waitgprsbacktime=100 ʱ Ϊ10s
//	{
//		if(U2_K>7)   						 //����Ҳ��6���ַ���"+CREG: 2"
//		{
//			Delay_Nms(5);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
//			break;	
//		}
//	}
//	TIM2->CR1&=~(0x01);    					//�����ʱ��2
//
//	if(waitmessagebacktime<100)			    //���յ�����	   
//	{
//	   	for(i=0;i<U2RX_LENGTH;i++)
//		{
//			if((USART2_RXBUF[i]=='C')&&(USART2_RXBUF[i+1]=='R')&&(USART2_RXBUF[i+2]=='E')&&	  //+CREG: 2��ע�ᵽ��������
//					(USART2_RXBUF[i+3]=='G')&&(USART2_RXBUF[i+6]=='2'))  					
//			{
//				mc52i_ok=0x12;
//				LED2=1;
//				LED0=1;
//				break;
//			}
//		} 
//		Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴
//	}
//	else									 //û�н��յ�����,����Ϊģ��û������
//	{
//		mc52i_ok=0x00;
//		LED0=0;
//		LED2=0;
//	} 		
 //7.+CREG: 1��5  
 	Clear_U2rxbuf(); 	
 	waitmessagebacktime=0;						//�����ʱ
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(waitmessagebacktime<600)    				//���ȴ�ʱ��waitgprsbacktime=600 ʱ Ϊ60s
	{
		if(U2_K>7)   						 //����Ҳ��6���ַ���"+CREG: 1"����"+CREG: 5"
		{
			Delay_Nms(5);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	if(waitmessagebacktime<600)				   //���յ�����  
	{
		for(i=0;i<100;i++)
		{
			if((USART2_RXBUF[i]=='C')&&(USART2_RXBUF[i+1]=='R')&&(USART2_RXBUF[i+2]=='E')&&
			(USART2_RXBUF[i+3]=='G')&&(USART2_RXBUF[i+6]=='1')||(USART2_RXBUF[i+6]=='5'))  //+CREG: 5����  1����
			{
				LED0=1;						  //��ʾ��������
				LED2=1;
				mc52i_ok=0x12;
				WriteLCDData(2,1,"GPRS           Y");				//��ʾ����
				break;
			}
		} 
		Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴
	}
	else									  //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0x00;
		WriteLCDData(2,1,"GPRS           N");				//��ʾδ����
		LED0=0;
		LED2=0;
	}	
}
/****************************************************************
���ܣ�����ź�ǿ��
������
���أ�
˵����
****************************************************************/
void  Csq_Detec(void)
{	
	Clear_U2rxbuf();	
	AT_Command("AT+CSQ\r\n"); 
	waitmessagebacktime=0;					//�����ʱ
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(waitmessagebacktime<50)    				//���ȴ�ʱ��waitgprsbacktime=50 ʱ Ϊ5s
	{
		if(U2_K>8)   						 //����Ҳ��10���ַ���^SISW:1,6
		{
			Delay_Nms(5);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	if(waitmessagebacktime<50)				   //���յ�����
	{	
		LED0=1;
		LED2=1;
		mc52i_ok=0x12;
		Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴
	}
	else								  //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0x00;
		LED0=0;
		LED2=0;
	} 	
}
/****************************************************************
���ܣ�����GPRS��ʼ��
������NULL
���أ�NULL
˵����8�����裬optional���п���
****************************************************************/
void Gprs_Init(void)
{
//1.ѡ��GPRS����ģʽ
	if(mc52i_ok==0x12)
	{	
		printf("Set GPRS work model\n");
		Gprs_Command("AT^SICS=0,conType,GPRS0\r\n",50);
		Delay_Nms(1);			
	}
//2.�û�����	
	if(mc52i_ok==0x12)
	{	
		printf("Set GPRS name \n");
		Gprs_Command("AT^SICS=0,user,gprs\r\n",50);			 //optional
		Delay_Nms(1);
	}
//3.�û�����
	if(mc52i_ok==0x12)
	{	
		printf("Set GPRS code\n");
		Gprs_Command("AT^SICS=0,passwd,gprs\r\n",50);		//optional	
		Delay_Nms(1);	
	}
//4.������Ӫ��	
	if(mc52i_ok==0x12)
	{	
		printf("SetGPRS operator\n");
		Gprs_Command("AT^SICS=0,apn,cmnet\r\n",50);	
		Delay_Nms(1);	
	} 
//5.��������	
	if(mc52i_ok==0x12)
	{
		printf("Set GPRS Type Server\n");
		Gprs_Command("AT^SISS=1,srvType,socket\r\n",50);
		Delay_Nms(1);		
	} 
//6.I.D.	
	if(mc52i_ok==0x12)
	{
		printf("Set GPRS I.D. \n");
		Gprs_Command("AT^SISS=1,conId,0\r\n",50);
		Delay_Nms(1);
	} 
//7.���÷�������ַ���˿�	
	if(mc52i_ok==0x12)
	{
		IP[0]=ee_ip[0]/100+0x30;
		IP[1]=ee_ip[0]%100/10+0x30;
		IP[2]=ee_ip[0]%10+0x30;

		IP[4]=ee_ip[1]/100+0x30;
		IP[5]=ee_ip[1]%100/10+0x30;
		IP[6]=ee_ip[1]%10+0x30;

		IP[8]=ee_ip[2]/100+0x30;
		IP[9]=ee_ip[2]%100/10+0x30;
		IP[10]=ee_ip[2]%10+0x30;

		IP[12]=ee_ip[3]/100+0x30;
		IP[13]=ee_ip[3]%100/10+0x30;
		IP[14]=ee_ip[3]%10+0x30;

		IP[16]=ee_ip[4]/10+0x30;		 //port
		IP[17]=ee_ip[4]%10+0x30;
		IP[18]=ee_ip[5]/10+0x30;
		IP[19]=ee_ip[5]%10+0x30;

		printf("Set GPRS I.P. \n");
		AT_Command("AT^SISS=1,address,");  
		Usart2_SendByte('"');
		AT_Command("socktcp://");
		AT_Command(IP);
		Usart2_SendByte('"');
		Gprs_Command("\r\n",100);	   //10s
		Delay_Nms(1);
	}
//8. �򿪶˿�
	if(mc52i_ok==0x12)
	{
		printf("Set GPRS open port\n");
		Gprs_Open();
		Delay_Nms(10); 
		Clear_U2rxbuf();	
	}

}
/****************************************************************
���ܣ���GPRSͨ�Ŷ˿�
������NULL
���أ�NULL
˵�����յ�^SISW:1����ʾ�˿ڴ򿪳ɹ�
****************************************************************/
void Gprs_Open(void)
{  	
	Clear_U2rxbuf();
	AT_Command("AT^SISO=1\r\n");
	waitmessagebacktime=0;				  //�����ʱ
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(waitmessagebacktime<600)    				//���ȴ�ʱ��waitgprsbacktime=1200 ʱ Ϊ120s
	{
		if(U2_K>15)   						 //����Ҳ��10���ַ��������**OK**  **^SISW**
		{
			Delay_Nms(50);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}							   
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	if(waitmessagebacktime<600)				//���յ�����   
	{	
		LED0=1;
		mc52i_ok=Check_OK();
		if(U2_K<30)
		{									  //������
			printf("Open port OK !\n");
			mc52i_ok=0x12;
			LED2=1;		
		}
		else
		{
			if(U2_K>=30)						//��Ϣ�ܳ� ��������û�д�	 ^SIS: 1, 0, 21, "Remote host has rejected the connection"
			{
				mc52i_ok=0x11; 				 //�൱��ERROR
				LED2=0;                 
				printf("Server closed!\n");
			}	
		} 
		Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴
	}
   	else							  //û�н��յ�����,����Ϊģ��û������
	{
	   	mc52i_ok=0x00;
		LED0=0;
		LED2=0;
	}	
}
/****************************************************************
���ܣ� ����GPRS��Ϣ
������ send_gprs_length,�������ݳ���
���أ� NULL
˵����
****************************************************************/
void Gprs_Send(u8 send_gprstext[],u8 send_gprslength[3])
{
	u16  sendgprslength=0;
	sendgprslength=(send_gprslength[0]-0x30)*100+(send_gprslength[1]-0x30)*10+(send_gprslength[2]-0x30);	 //���ȣ�ASCIIת����ʮ������

	Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴    
	Clear_U2rxbuf(); 
	printf("Sending gprs message!\n");
	AT_Command("AT^SISW=1,");
	AT_Command(send_gprslength);
	AT_Command("\r\n");

	waitmessagebacktime=0;				  //�����ʱ	
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(waitmessagebacktime<100)    				//���ȴ�ʱ��waitgprsbacktime=100 ʱ Ϊ10s
	{
		if(U2_K>10)   						 //����Ҳ��10���ַ���^SISW:1,6
		{
			Delay_Nms(5);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	if(waitmessagebacktime<100)					   
	{
		mc52i_ok=0x12;
		LED0=1;
		LED2=1;		 	
		Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴		 ^SISW: 1, 1
	}
	else								//û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0x00;
		LED0=0;	
		LED2=0;	
	}

	Clear_U2rxbuf();
	Send_ATCommand(send_gprstext,sendgprslength);	         //������Ϣ	 ���ﲻ��Ҫ���ͻس����У�ֻҪ���������ݾͿ����ˣ�����һ��Ҫ�� 
	waitmessagebacktime=0;				  //�����ʱ
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(waitmessagebacktime<100)    				//���ȴ�ʱ��waitgprsbacktime=100 ʱ Ϊ10s
	{
		if(U2_K>10)   						 //����Ҳ��10���ַ���OK
		{
			Delay_Nms(20);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	if(waitmessagebacktime<100)			//���յ�����	  
	{  
		mc52i_ok=Check_OK();
		Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴		^SISW: 1, 21, 21
		Clear_U2rxbuf();
		LED0=1;
		if(mc52i_ok==0x12)
		{
			LED2=1;
			printf("Send gprs message complete!\n");
		}
		else 
		{
			LED2=0;
			printf("Send gprs message error!\n");
		}	
	}
	else							    //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0x00;
		LED0=0;	
		LED2=0;
	} 		
}
/****************************************************************
���ܣ��������ƺ�����ÿ��һ��ʱ������������Ͷ��ֽ����ݣ��Է����ӶϿ�
������heartbitbyte,һ���ֽ���
���أ�NULL
˵����
****************************************************************/
void HeartBeat(u8 heartbitbyte)
{
	Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴 
	Clear_U2rxbuf();
	printf("Heartbeat!\n");
	AT_Command("AT^SISW=1,1\r\n");
	waitmessagebacktime=0;				  //�����ʱ
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(waitmessagebacktime<100)    				//���ȴ�ʱ��waitgprsbacktime=100 ʱ Ϊ10s
	{
		if(U2_K>10)   						 //����Ҳ��10���ַ���^SISW:1,1
		{
			Delay_Nms(5);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	if(waitmessagebacktime<100)				//���յ�����   
	{
		Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴
		LED0=1;
	}
	else							   //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0;
		LED0=0;
	}

	Clear_U2rxbuf();
	Usart2_SendByte(heartbitbyte);	      		 //����һ���ֽ���Ϣ
	waitmessagebacktime=0;				   //�����ʱ	
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(waitmessagebacktime<100)    				//���ȴ�ʱ��waitgprsbacktime=100 ʱ Ϊ10s
	{
		if(U2_K>10)   						 //����Ҳ��10���ַ���OK
		{
			Delay_Nms(20);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2

	if(waitmessagebacktime<100)				   
	{
		LED0=1;
		Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴
		mc52i_ok=Check_OK();
		if(mc52i_ok==0x12)
		{
			printf("Heartbeat complete!\n");
			LED2=1;
			mc52i_ok=0x12;
		}
		else
		{
			LED2=0;
			mc52i_ok=0x11;
			printf("Heartbeat ERROR!\n");
		}							
	}
	else								//û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0;
		LED0=0;
		LED2=0;
	} 			
}
/****************************************************************
���ܣ�����GPRS���ݺ���
������rec_gprs_length���������ݳ��ȣ����ֵ�������õô��
���أ�NULL
˵����
****************************************************************/
void Gprs_Rec(void)
{
	u8 i=0;
 	Clear_U2rxbuf();
	AT_Command("AT^SISR=1,");
	AT_Command("100");
	AT_Command("\r\n");

	waitmessagebacktime=0;				  //�����ʱ
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(waitmessagebacktime<100)    				//���ȴ�ʱ��waitgprsbacktime=100 ʱ Ϊ10s
	{
		if(U2_K>20)   						 //����Ҳ�ô��ڽ��ܳ���
		{
			Delay_Nms(20);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2

	if(waitmessagebacktime<100)				   
	{
		LED0=1;	
		mc52i_ok=0x12;
				
		printf("The Message is:\n"); 				
		Usart1_SendString(USART2_RXBUF);		//���͵����ڲ鿴

		for(i=0;i<U2RX_LENGTH;i++)
		{
			if((USART2_RXBUF[i]==0x2F)&&(USART2_RXBUF[i+1]==0x43)&&(USART2_RXBUF[i+2]==0x2F)&&(USART2_RXBUF[i+20]==0xCC))	//��ʼ��0x2F432F��������0x3C
			{																		  
				memcpy(&gprsaccept,USART2_RXBUF+i,21);	   //�����ݸ��Ƹ�gprs���սṹ��
				printf("Wright start and end!\n");				
				Gprs_DealReceiveMessage();         //������յ���GPRS��Ϣ	
				break;		//����for����	
			}	
		}  				

	}
	else								//û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0;
		LED0=0;
	}	
}
/****************************************************************
���ܣ��ر�GPRS�˿ں���
������NULL
���أ�NULL
˵����
****************************************************************/
void Gprs_Close(void)
{
	printf("Close GPRS port!\n");
	Gprs_Command("AT^SISC=1\r\n",50);
	Delay_Nms(1);	
}
