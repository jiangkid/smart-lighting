#include "test.h"
#include "gprs.h"
#include "usart1.h"	
#include "usart.h"
#include "led.h"
#include "spi.h"
/************************************************************************
PC6:gprs_on
PC7:gprs_off
***********************************************************************/
extern u8 USART2_RX_BUF[128];     //���ջ�������,���128���ֽ�.
extern u8 GPRS_RDATA[128];
extern u8 U2_K;					  //�������ݳ��ȱ�־
extern u16 time2count;			   //��ʱ��2�ۼ�ֵ
extern u8 mc52i_ok;					// Mc52iͨ���������ı�־λ
extern u8 creg_ok;
extern u8 gprs_sisr;
extern u8 gprs_sisr;
extern u8 gprs_send;
extern u8 gprs_rec;
extern u8 ID[2];
/******************************************************************************/
extern u8 IP[20];
//u8 send_gprs_length[4]={"10"};
u8 rec_gprs_length[4]={"100"};
extern u8 GPRS_FIRSTMESSAGE[11];
extern u8 GPRS_NORMALMESSAGE[6];
/*******************************************************************************/
/****************************************************************
���ܣ���ʼ������2
������pclk2,PCLK2ʱ��Ƶ��(���36M)��boundΪ������
���أ�NULL
˵����
****************************************************************/
void USART2_Init(u32 pclk1,u32 bound)
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

	//ʹ�ܽ����ж�
	USART2->CR1|=1<<8;    //PE�ж�ʹ��
	USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(1,2,USART2_IRQChannel,2);//��2
}
/****************************************************************
���ܣ�����2�����жϺ���
������
���أ�NULL
˵��������������128�����ݣ����������Զ�����
****************************************************************/
void USART2_IRQHandler(void)
{   
	if(USART2->SR&(1<<5))//���յ�����
	{	 
		USART2_RX_BUF[U2_K]=USART2->DR;	

		if(USART2_RX_BUF[U2_K]=='R')
		{  
			if((USART2_RX_BUF[U2_K-3]=='S')&&(USART2_RX_BUF[U2_K-2]=='I')&&(USART2_RX_BUF[U2_K-1]=='S'))
				gprs_sisr=1;	
		}
		U2_K++;
		if(U2_K>127)
		{
			U2_K=0;
		}
	}										       	 		 									     									 
}
/****************************************************************
���ܣ�����������ݺ���
������NULL
���أ�NULL
˵����
****************************************************************/
void Clear_U2_Rbuf(void)
{
	u8 i=0;
	for(i=0;i<128;i++)
	{
		USART2_RX_BUF[i]=0;
	}
	U2_K=0;
}
/****************************************************************
���ܣ�ͨ������2һ����1bit������
������utbyteΪ���͵�ASCII��ֵ����0x31 
���أ�NULL
˵����
****************************************************************/
void Usart2_Send_Byte(u8 u2tbyte)
{
	USART2->DR=u2tbyte;
	while((USART2->SR&0X40)==0);//�ȴ����ͽ���
}
/****************************************************************
���ܣ����ڷ���ctrl+Z
������Ctrl+z��ASCII��Ϊ0x1A
���أ�
****************************************************************/
void Ctrl_Z(void)
{
	Usart2_Send_Byte(0x1A);	
}
/****************************************************************
���ܣ�����ATָ��
������utstringΪ�������ݣ��硰abcdefg��
���أ�NULL
˵����
****************************************************************/
void AT_Command(u8  utstring[])
{
	u16 i=0;
	for(i=0;i<strlen(utstring);i++)			 //�����ַ���
	{
		Usart2_Send_Byte(utstring[i]);
	}
}
/****************************************************************
���ܣ����鴮��2���յ������ݣ�GPRS�����Ƿ������OK��	����"ERROR"
�����������а���"OK"��GPRSͨ��������mc52i_ok=2;����"ERROR"����ͨ�ų���mc52i_ok=1;
���أ�NULL
˵�����������mc52i_ok=0ѡ��
****************************************************************/
void Check_OK(void)
{
	u8 i=0;
	for(i=0;i<128;i++)
	{
		if((USART2_RX_BUF[i]=='O')&&(USART2_RX_BUF[i+1]=='K'))
		{
			mc52i_ok=2;
			break;
		}
		if((USART2_RX_BUF[i]=='E')&&(USART2_RX_BUF[i+1]=='R')&&(USART2_RX_BUF[i+2]=='R')&&(USART2_RX_BUF[i+3]=='O')&&(USART2_RX_BUF[i+4]=='R'))
		{
			mc52i_ok=1;
			break;
		} 
	}		
}
/****************************************************************
���ܣ����鷵��ֵ���Ƿ����"SISW"
������NULL
���أ�����"SISW"ʱ����1����������0
˵����GPRS�˿ڴ򿪳ɹ�����������֮ʱ�᷵��"SISW"
****************************************************************/
u8 Check_SISW(void)
{
	u8 i=0;
	for(i=0;i<5;i++)
	{
		if((USART2_RX_BUF[i]=='S')&&(USART2_RX_BUF[i+1]=='I')&&(USART2_RX_BUF[i+2]=='S')&&(USART2_RX_BUF[i+3]=='W'))
		{
			
			return 1;
		}
	}
	return 0;
}
/****************************************************************
���ܣ����鷵��ֵ���Ƿ����"SISR"
������NULL
���أ�����"SISR"ʱ����1����������0
˵����GPRS�յ�����֮ʱ�ᴥ��"SISR"
****************************************************************/
//u8 Check_SISR(void)
//{
//	u8 i=0;
//	for(i=0;i<128;i++)
//	{
//		if((USART2_RX_BUF[i]=='S')&&(USART2_RX_BUF[i+1]=='I')&&(USART2_RX_BUF[i+2]=='S')&&(USART2_RX_BUF[i+3]=='R'))
//		{
//			return 1;
//		}
//	} 
//	return 0;
//}
/****************************************************************
���ܣ����鷵��ֵ���Ƿ����"CREG: 5"
������NULL
���أ�����"CREG: 5"ʱ����1����������0
˵����ģ��ע��ɹ���ᴥ������"+CREG: 5"
****************************************************************/
u8  Check_Creg(void)
{
	u8 i=0;
	for(i=0;i<100;i++)
	{
		if((USART2_RX_BUF[i]=='C')&&(USART2_RX_BUF[i+1]=='R')&&(USART2_RX_BUF[i+2]=='E')&&(USART2_RX_BUF[i+3]=='G')&&(USART2_RX_BUF[i+4]==':')&&(USART2_RX_BUF[i+5]==' ')&&(USART2_RX_BUF[i+6]=='5'))  //+CREG: 5	ע�ᵽ������
		{
			return 1;
		}
	} 
	return 0;
}
/****************************************************************
���ܣ���ͨATָ�����OK���������ͺ���
������utstring[]��ATָ�gprscommand_waittime���ȴ����ص��ʱ��ʱ��
���أ� NULL
˵���� ����һ��ATָ��󣬿�����ʱ��2�������ȴ�ʱ���������û�н��յ����ݣ��򱨴�
****************************************************************/
void Gprs_Command(u8  utstring[],u16 gprscommand_waittime)
{
	AT_Command(utstring);
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(time2count<gprscommand_waittime)    				//���ȴ�ʱ�� time2=50 ʱ Ϊ5s
	{
		if(U2_K>5)   						 //����Ҳ��6���ַ���**OK**	
		{
			delay_ms(1);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	if(time2count>=gprscommand_waittime)	   //û�н��յ�����
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	time2count=0;

	Check_OK();	
	Usart1_Send_String(USART2_RX_BUF);		//���͵����ڲ鿴
	Clear_U2_Rbuf();
}
/****************************************************************
���ܣ���ʼ��Mc52iģ��
������NULL
���أ�NULL
˵����1������SYSSTART��2��ATE0��3��AT+CREG=1��4������+CREG: 2
****************************************************************/
void Mc52i_Init(void)
{	
	u8 i=0;
	GPRS_OFF=0;								  //ģ��ر�
	delay_ms(500);
	GPRS_OFF=1;
	delay_ms(100);

	GPRS_ON=0;								//ģ�鿪��
	delay_ms(500);	
	GPRS_ON=1;	

	Clear_U2_Rbuf();					
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(time2count<100)    				//���ȴ�ʱ�� time2=100 ʱ Ϊ10s
	{
		if(U2_K>5)   						 //����Ҳ��6���ַ���"^SYSSTART"
		{
			delay_ms(1);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	} 
	if(time2count>=100)				   //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	time2count=0;

	for(i=0;i<128;i++)
	{
		if((USART2_RX_BUF[i]=='S')&&(USART2_RX_BUF[i+1]=='Y')&&(USART2_RX_BUF[i+2]=='S'))  //^SYSSTART
		{
			mc52i_ok=2;
			break;
		}
	} 

	Usart1_Send_String(USART2_RX_BUF);		//���͵����ڲ鿴
	Clear_U2_Rbuf();

   	if(mc52i_ok==2)							 //�رջ���
	{
		Gprs_Command("ATE0\r\n",50); 
		delay_ms(1);
	}

	if(mc52i_ok==2)						   //��������ע�������ṩ�������
	{   
		Gprs_Command("AT+CREG=1\r\n",50);
		delay_ms(1);
	}

	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(time2count<100)    				//���ȴ�ʱ�� time2=100 ʱ Ϊ10s
	{
		if(U2_K>5)   						 //����Ҳ��6���ַ���"+CREG: 2"
		{
			delay_ms(1);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	if(time2count>=100)				   //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	time2count=0;

	for(i=0;i<127;i++)
	{
		if((USART2_RX_BUF[i]=='C')&&(USART2_RX_BUF[i+1]=='R')&&(USART2_RX_BUF[i+2]=='E')&&(USART2_RX_BUF[i+3]=='G'))  //+CREG: 2��ע�ᵽ��������
		{
			mc52i_ok=2;
			break;
		}
	} 
	Usart1_Send_String(USART2_RX_BUF);		//���͵����ڲ鿴
	Clear_U2_Rbuf();
}
/****************************************************************
���ܣ�����ź�ǿ��
������
���أ�
˵����
****************************************************************/
void  Csq_Detec(void)
{
	AT_Command("AT+CSQ\r\n");
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(time2count<50)    				//���ȴ�ʱ�� time2=50 ʱ Ϊ5s
	{
		if(U2_K>8)   						 //����Ҳ��10���ַ���^SISW:1,6
		{
			delay_ms(1);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	if(time2count>=50)				   //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	time2count=0;

	Usart1_Send_String(USART2_RX_BUF);		//���͵����ڲ鿴
	Clear_U2_Rbuf();
}
/****************************************************************
���ܣ�����GPRS��ʼ��
������NULL
���أ�NULL
˵����8�����裬optional���п���
****************************************************************/
void Gprs_Init(void)
{
	if(mc52i_ok==2)
	{
		Gprs_Command("AT^SICS=0,conType,GPRS0\r\n",50);	
		delay_ms(1);
	}
	
	if(mc52i_ok==2)
	{
		Gprs_Command("AT^SICS=0,user,gprs\r\n",50);			 //optional
		delay_ms(1);
	}
	if(mc52i_ok==2)
	{
		Gprs_Command("AT^SICS=0,passwd,gprs\r\n",50);		//optional	
		delay_ms(1);	
	}
	
	if(mc52i_ok==2)
	{
		Gprs_Command("AT^SICS=0,apn,cmnet\r\n",50);	
		delay_ms(1);	
	} 
	
	if(mc52i_ok==2)
	{
		Gprs_Command("AT^SISS=1,srvType,socket\r\n",50);
		delay_ms(1);		
	} 
	
	if(mc52i_ok==2)
	{
		Gprs_Command("AT^SISS=1,conId,0\r\n",50);
		delay_ms(1);
	} 
	
	if(mc52i_ok==2)
	{
		AT_Command("AT^SISS=1,address,");  
		Usart2_Send_Byte('"');
		AT_Command("socktcp://");
		AT_Command(IP);
	//	AT_Command("060.176.037.106:5002");
		Usart2_Send_Byte('"');
		Gprs_Command("\r\n",50);
		delay_ms(1);	
	} 
	
	if(mc52i_ok==2)
	{
		Gprs_Open();
		delay_ms(1);
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
	AT_Command("AT^SISO=1\r\n");
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(time2count<600)    				//���ȴ�ʱ�� time2=600 ʱ Ϊ60s
	{
		if(U2_K>10)   						 //����Ҳ��10���ַ��������**OK**  **^SISW**
		{
			delay_ms(1);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	if(time2count>=600)				   //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	time2count=0;

	Check_OK();
	Check_SISW();
	Usart1_Send_String(USART2_RX_BUF);		//���͵����ڲ鿴
	Clear_U2_Rbuf();
}
/****************************************************************
���ܣ� ����GPRS��Ϣ
������ send_gprs_length,�������ݳ���
���أ� NULL
˵����
****************************************************************/
void Gprs_Send(u8 send_gprs_text[],u8 send_gprs_length[])
{
	AT_Command("AT^SISW=1,");
	AT_Command(send_gprs_length);
	AT_Command("\r\n");

	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(time2count<100)    				//���ȴ�ʱ�� time2=100 ʱ Ϊ10s
	{
		if(U2_K>8)   						 //����Ҳ��10���ַ���^SISW:1,6
		{
			delay_ms(1);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	if(time2count>=100)					   //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	time2count=0;
	Usart1_Send_String(USART2_RX_BUF);		//���͵����ڲ鿴
	Clear_U2_Rbuf();

	AT_Command(send_gprs_text);	         //������Ϣ
//	AT_Command("\r\n");					  //���ﲻ��Ҫ���ͻس����У�ֻҪ���������ݾͿ����ˣ�����һ��Ҫ�� 
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(time2count<100)    				//���ȴ�ʱ�� time2=100 ʱ Ϊ10s
	{
		if(U2_K>10)   						 //����Ҳ��10���ַ���OK
		{
			delay_ms(1);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	if(time2count>=100)				   //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	time2count=0;

	Check_OK();
	if(mc52i_ok==2)
	{
		gprs_send=1;
	}
	else
	{
		gprs_send=0; 
		mc52i_ok=0; 
	}
	Usart1_Send_String(USART2_RX_BUF);		//���͵����ڲ鿴
	Clear_U2_Rbuf();
}
/****************************************************************
���ܣ��������ƺ�����ÿ��һ��ʱ������������Ͷ��ֽ����ݣ��Է����ӶϿ�
������NULL
���أ�NULL
˵����
****************************************************************/
void HeartBeat(void)
{
	AT_Command("AT^SISW=1,1\r\n");
	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(time2count<100)    				//���ȴ�ʱ�� time2=100 ʱ Ϊ10s
	{
		if(U2_K>8)   						 //����Ҳ��10���ַ���^SISW:1,1
		{
			delay_ms(1);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	if(time2count>=100)				   //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	time2count=0;
	Usart1_Send_String(USART2_RX_BUF);		//���͵����ڲ鿴
	Clear_U2_Rbuf();

	Usart2_Send_Byte(0x1F);	      		 //����һ���ֽ���Ϣ

	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(time2count<100)    				//���ȴ�ʱ�� time2=100 ʱ Ϊ10s
	{
		if(U2_K>10)   						 //����Ҳ��10���ַ���OK
		{
			delay_ms(1);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	if(time2count>=100)				   //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	time2count=0;

	Check_OK();
	if(mc52i_ok==2)
	{
		gprs_send=1;
	}
	else
	{
		gprs_send=0;
		mc52i_ok=0; 
	}
	Usart1_Send_String(USART2_RX_BUF);		//���͵����ڲ鿴
	Clear_U2_Rbuf();	
}
/****************************************************************
���ܣ��ر�GPRS�˿ں���
������NULL
���أ�NULL
˵����
****************************************************************/
void Gprs_Close(void)
{	
	if(mc52i_ok==2)
	{
		Gprs_Command("AT^SISC=1\r\n",50);
		delay_ms(1);
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
	u8 j=0;
	u8 gprs_f=0;
	Clear_U2_Rbuf();                      //���ж���ʾ��^SISR:1,1�����

	AT_Command("AT^SISR=1,");
	AT_Command(rec_gprs_length);
	AT_Command("\r\n");

	TIM2->CR1|=0x01;    					//ʹ�ܶ�ʱ��2
	while(time2count<100)    				//���ȴ�ʱ�� time2=100 ʱ Ϊ10s
	{
		if(U2_K>20)   						 //����Ҳ�ô��ڽ��ܳ���
		{
			delay_ms(10);              	   //��ʱ�£���Щʱ��ֹ��ô�㳤��
			break;	
		}
	}
	if(time2count>=100)				   //û�н��յ�����,����Ϊģ��û������
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//�����ʱ��2
	time2count=0;

	Check_OK();
	if(mc52i_ok==2)
	{
		gprs_rec=1;
	}
	else
	{
		gprs_rec=0;
		mc52i_ok=0;	 
	}
	for(i=0;i<128;i++)								  //�������ݴ���
	{
		if((USART2_RX_BUF[i]==0x7B)||(gprs_f==1))
		{
			gprs_f=1;
			GPRS_RDATA[j]=USART2_RX_BUF[i];
			j++;
		}
		if(USART2_RX_BUF[i]==0x7D)
		{
			gprs_f=0;	
		}
	}
//	for(i=0;i<20;i++)							//��������
//	{
//		GPRS_RDATA[i]=USART2_RX_BUF[i+15];
//	}
	Gprs_ReceiveDataID();							//���ݴ���
	Usart1_Send_String(USART2_RX_BUF);		//���͵����ڲ鿴
	Clear_U2_Rbuf();
	printf("gprs received data:");
	printf("%s\n",GPRS_RDATA);
}
/****************************************************************
���ܣ��鿴�ն�ID����
������
���أ�
˵����#01**$
****************************************************************/
void Gprs_ReceiveDataID(void)
{
	u8	terminal_id;
	if((GPRS_RDATA[0]==0x7B)&&(GPRS_RDATA[5]==0x7D))	//��β��֤��ȷ
	{
		terminal_id=(GPRS_RDATA[1]-0x30)*10+(GPRS_RDATA[2]-0x30);	
		switch(terminal_id)
		{
			case 0:
					break;
			case 1:
					Gprs_ReceiveDataPoint();
					break;
			case 2:
					break;
			case 3:
					break;
			default:
					break;				
		}
	}	
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void Gprs_ReceiveDataPoint(void)
{
	switch(GPRS_RDATA[3])
	{
		case 0x21:
					LED0=1;
					if(GPRS_RDATA[4]==0x21)	   //��
					{
						Gprs_ReceiveDataState(0x41);	
					}
					else if(GPRS_RDATA[4]==0x22) //��
					{
						Gprs_ReceiveDataState(0x40);
					}	
					break;
		case 0x22:
					LED0=0;
					if(GPRS_RDATA[4]==0x21)	   //��
					{
						Gprs_ReceiveDataState(0x51);	
					}
					else if(GPRS_RDATA[4]==0x22) //��
					{
						Gprs_ReceiveDataState(0x50);
					}
					break;
		case 0x23:
					if(GPRS_RDATA[4]==0x21)	   //��
					{
						Gprs_ReceiveDataState(0x61);	
					}
					else if(GPRS_RDATA[4]==0x22) //��
					{
						Gprs_ReceiveDataState(0x60);
					}
					break;
		case 0x24:
					if(GPRS_RDATA[4]==0x21)	   //��
					{
						Gprs_ReceiveDataState(0x71);	
					}
					else if(GPRS_RDATA[4]==0x22) //��
					{
						Gprs_ReceiveDataState(0x70);
					}
					break;
		case 0x25:
					break;
		case 0x26:
					break;
		default:
					break;	
	}
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void Gprs_ReceiveDataState(u8 zigbeecommand)
{
	CS3=0;
	delay_ms(1);
	SPIx_ReadWriteByte(zigbeecommand);			//����
	delay_ms(2);
	if(SPIx_ReadWriteByte(0x30)=='1')	   	//�鿴�����Ƿ�ɹ�
		printf("Control Zigbee OK!\n");
	else 
		printf("error!\n");
	delay_ms(2);
	CS3=1;
	delay_us(100);		
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/

