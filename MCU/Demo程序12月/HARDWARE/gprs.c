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
extern u8 USART2_RX_BUF[128];     //接收缓冲数组,最大128个字节.
extern u8 GPRS_RDATA[128];
extern u8 U2_K;					  //接收数据长度标志
extern u16 time2count;			   //定时器2累加值
extern u8 mc52i_ok;					// Mc52i通信正常与否的标志位
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
功能：初始化串口2
参数：pclk2,PCLK2时钟频率(最大36M)；bound为波特率
返回：NULL
说明：
****************************************************************/
void USART2_Init(u32 pclk1,u32 bound)
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

	//使能接收中断
	USART2->CR1|=1<<8;    //PE中断使能
	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(1,2,USART2_IRQChannel,2);//组2
}
/****************************************************************
功能：串口2接收中断函数
参数：
返回：NULL
说明：缓存最多接收128个数据，超过数组自动清零
****************************************************************/
void USART2_IRQHandler(void)
{   
	if(USART2->SR&(1<<5))//接收到数据
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
功能：清除缓存数据函数
参数：NULL
返回：NULL
说明：
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
功能：通过串口2一发送1bit的数据
参数：utbyte为发送的ASCII码值，如0x31 
返回：NULL
说明：
****************************************************************/
void Usart2_Send_Byte(u8 u2tbyte)
{
	USART2->DR=u2tbyte;
	while((USART2->SR&0X40)==0);//等待发送结束
}
/****************************************************************
功能：串口发送ctrl+Z
参数：Ctrl+z的ASCII码为0x1A
返回：
****************************************************************/
void Ctrl_Z(void)
{
	Usart2_Send_Byte(0x1A);	
}
/****************************************************************
功能：发送AT指令
参数：utstring为发送内容，如“abcdefg”
返回：NULL
说明：
****************************************************************/
void AT_Command(u8  utstring[])
{
	u16 i=0;
	for(i=0;i<strlen(utstring);i++)			 //发送字符串
	{
		Usart2_Send_Byte(utstring[i]);
	}
}
/****************************************************************
功能：检验串口2接收到的数据（GPRS）中是否包含”OK“	或者"ERROR"
参数：代码中包含"OK"，GPRS通信正常，mc52i_ok=2;包含"ERROR"，则通信出错，mc52i_ok=1;
返回：NULL
说明：可以添加mc52i_ok=0选项
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
功能：检验返回值中是否包含"SISW"
参数：NULL
返回：包含"SISW"时返回1，其他返回0
说明：GPRS端口打开成功，发送数据之时会返回"SISW"
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
功能：检验返回值中是否包含"SISR"
参数：NULL
返回：包含"SISR"时返回1，其他返回0
说明：GPRS收到数据之时会触发"SISR"
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
功能：检验返回值中是否包含"CREG: 5"
参数：NULL
返回：包含"CREG: 5"时返回1，其他返回0
说明：模块注册成功后会触发接收"+CREG: 5"
****************************************************************/
u8  Check_Creg(void)
{
	u8 i=0;
	for(i=0;i<100;i++)
	{
		if((USART2_RX_BUF[i]=='C')&&(USART2_RX_BUF[i+1]=='R')&&(USART2_RX_BUF[i+2]=='E')&&(USART2_RX_BUF[i+3]=='G')&&(USART2_RX_BUF[i+4]==':')&&(USART2_RX_BUF[i+5]==' ')&&(USART2_RX_BUF[i+6]=='5'))  //+CREG: 5	注册到了网络
		{
			return 1;
		}
	} 
	return 0;
}
/****************************************************************
功能：普通AT指令，返回OK字样，发送函数
参数：utstring[]，AT指令；gprscommand_waittime，等待返回的最长时间时间
返回： NULL
说明： 发送一个AT指令后，开启定时器2，在最大等待时间里面如果没有接收到数据，则报错
****************************************************************/
void Gprs_Command(u8  utstring[],u16 gprscommand_waittime)
{
	AT_Command(utstring);
	TIM2->CR1|=0x01;    					//使能定时器2
	while(time2count<gprscommand_waittime)    				//最多等待时间 time2=50 时 为5s
	{
		if(U2_K>5)   						 //至少也得6个字符，**OK**	
		{
			delay_ms(1);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	if(time2count>=gprscommand_waittime)	   //没有接收到数据
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	time2count=0;

	Check_OK();	
	Usart1_Send_String(USART2_RX_BUF);		//发送到串口查看
	Clear_U2_Rbuf();
}
/****************************************************************
功能：初始化Mc52i模块
参数：NULL
返回：NULL
说明：1，检验SYSSTART；2，ATE0；3，AT+CREG=1；4，检验+CREG: 2
****************************************************************/
void Mc52i_Init(void)
{	
	u8 i=0;
	GPRS_OFF=0;								  //模块关闭
	delay_ms(500);
	GPRS_OFF=1;
	delay_ms(100);

	GPRS_ON=0;								//模块开启
	delay_ms(500);	
	GPRS_ON=1;	

	Clear_U2_Rbuf();					
	TIM2->CR1|=0x01;    					//使能定时器2
	while(time2count<100)    				//最多等待时间 time2=100 时 为10s
	{
		if(U2_K>5)   						 //至少也得6个字符，"^SYSSTART"
		{
			delay_ms(1);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	} 
	if(time2count>=100)				   //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	time2count=0;

	for(i=0;i<128;i++)
	{
		if((USART2_RX_BUF[i]=='S')&&(USART2_RX_BUF[i+1]=='Y')&&(USART2_RX_BUF[i+2]=='S'))  //^SYSSTART
		{
			mc52i_ok=2;
			break;
		}
	} 

	Usart1_Send_String(USART2_RX_BUF);		//发送到串口查看
	Clear_U2_Rbuf();

   	if(mc52i_ok==2)							 //关闭回显
	{
		Gprs_Command("ATE0\r\n",50); 
		delay_ms(1);
	}

	if(mc52i_ok==2)						   //允许网络注册主动提供结果代码
	{   
		Gprs_Command("AT+CREG=1\r\n",50);
		delay_ms(1);
	}

	TIM2->CR1|=0x01;    					//使能定时器2
	while(time2count<100)    				//最多等待时间 time2=100 时 为10s
	{
		if(U2_K>5)   						 //至少也得6个字符，"+CREG: 2"
		{
			delay_ms(1);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	if(time2count>=100)				   //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	time2count=0;

	for(i=0;i<127;i++)
	{
		if((USART2_RX_BUF[i]=='C')&&(USART2_RX_BUF[i+1]=='R')&&(USART2_RX_BUF[i+2]=='E')&&(USART2_RX_BUF[i+3]=='G'))  //+CREG: 2已注册到本地网络
		{
			mc52i_ok=2;
			break;
		}
	} 
	Usart1_Send_String(USART2_RX_BUF);		//发送到串口查看
	Clear_U2_Rbuf();
}
/****************************************************************
功能：检测信号强度
参数：
返回：
说明：
****************************************************************/
void  Csq_Detec(void)
{
	AT_Command("AT+CSQ\r\n");
	TIM2->CR1|=0x01;    					//使能定时器2
	while(time2count<50)    				//最多等待时间 time2=50 时 为5s
	{
		if(U2_K>8)   						 //至少也得10个字符，^SISW:1,6
		{
			delay_ms(1);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	if(time2count>=50)				   //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	time2count=0;

	Usart1_Send_String(USART2_RX_BUF);		//发送到串口查看
	Clear_U2_Rbuf();
}
/****************************************************************
功能：连接GPRS初始化
参数：NULL
返回：NULL
说明：8个步骤，optional可有可无
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
功能：打开GPRS通信端口
参数：NULL
返回：NULL
说明：收到^SISW:1，表示端口打开成功
****************************************************************/
void Gprs_Open(void)
{
	AT_Command("AT^SISO=1\r\n");
	TIM2->CR1|=0x01;    					//使能定时器2
	while(time2count<600)    				//最多等待时间 time2=600 时 为60s
	{
		if(U2_K>10)   						 //至少也得10个字符，大概是**OK**  **^SISW**
		{
			delay_ms(1);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	if(time2count>=600)				   //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	time2count=0;

	Check_OK();
	Check_SISW();
	Usart1_Send_String(USART2_RX_BUF);		//发送到串口查看
	Clear_U2_Rbuf();
}
/****************************************************************
功能： 发送GPRS消息
参数： send_gprs_length,发送数据长度
返回： NULL
说明：
****************************************************************/
void Gprs_Send(u8 send_gprs_text[],u8 send_gprs_length[])
{
	AT_Command("AT^SISW=1,");
	AT_Command(send_gprs_length);
	AT_Command("\r\n");

	TIM2->CR1|=0x01;    					//使能定时器2
	while(time2count<100)    				//最多等待时间 time2=100 时 为10s
	{
		if(U2_K>8)   						 //至少也得10个字符，^SISW:1,6
		{
			delay_ms(1);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	if(time2count>=100)					   //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	time2count=0;
	Usart1_Send_String(USART2_RX_BUF);		//发送到串口查看
	Clear_U2_Rbuf();

	AT_Command(send_gprs_text);	         //发送消息
//	AT_Command("\r\n");					  //这里不需要发送回车换行，只要发送满数据就可以了，数据一定要满 
	TIM2->CR1|=0x01;    					//使能定时器2
	while(time2count<100)    				//最多等待时间 time2=100 时 为10s
	{
		if(U2_K>10)   						 //至少也得10个字符，OK
		{
			delay_ms(1);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	if(time2count>=100)				   //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
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
	Usart1_Send_String(USART2_RX_BUF);		//发送到串口查看
	Clear_U2_Rbuf();
}
/****************************************************************
功能：心跳控制函数，每隔一段时间向服务器发送二字节数据，以防连接断开
参数：NULL
返回：NULL
说明：
****************************************************************/
void HeartBeat(void)
{
	AT_Command("AT^SISW=1,1\r\n");
	TIM2->CR1|=0x01;    					//使能定时器2
	while(time2count<100)    				//最多等待时间 time2=100 时 为10s
	{
		if(U2_K>8)   						 //至少也得10个字符，^SISW:1,1
		{
			delay_ms(1);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	if(time2count>=100)				   //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	time2count=0;
	Usart1_Send_String(USART2_RX_BUF);		//发送到串口查看
	Clear_U2_Rbuf();

	Usart2_Send_Byte(0x1F);	      		 //发送一个字节消息

	TIM2->CR1|=0x01;    					//使能定时器2
	while(time2count<100)    				//最多等待时间 time2=100 时 为10s
	{
		if(U2_K>10)   						 //至少也得10个字符，OK
		{
			delay_ms(1);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	if(time2count>=100)				   //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
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
	Usart1_Send_String(USART2_RX_BUF);		//发送到串口查看
	Clear_U2_Rbuf();	
}
/****************************************************************
功能：关闭GPRS端口函数
参数：NULL
返回：NULL
说明：
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
功能：接收GPRS数据函数
参数：rec_gprs_length，接收数据长度，这个值可以设置得大点
返回：NULL
说明：
****************************************************************/
void Gprs_Rec(void)
{
	u8 i=0;
	u8 j=0;
	u8 gprs_f=0;
	Clear_U2_Rbuf();                      //把中断提示的^SISR:1,1给清除

	AT_Command("AT^SISR=1,");
	AT_Command(rec_gprs_length);
	AT_Command("\r\n");

	TIM2->CR1|=0x01;    					//使能定时器2
	while(time2count<100)    				//最多等待时间 time2=100 时 为10s
	{
		if(U2_K>20)   						 //至少也得大于接受长度
		{
			delay_ms(10);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	if(time2count>=100)				   //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0;
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
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
	for(i=0;i<128;i++)								  //接收数据处理
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
//	for(i=0;i<20;i++)							//接收数据
//	{
//		GPRS_RDATA[i]=USART2_RX_BUF[i+15];
//	}
	Gprs_ReceiveDataID();							//数据处理
	Usart1_Send_String(USART2_RX_BUF);		//发送到串口查看
	Clear_U2_Rbuf();
	printf("gprs received data:");
	printf("%s\n",GPRS_RDATA);
}
/****************************************************************
功能：查看终端ID号码
参数：
返回：
说明：#01**$
****************************************************************/
void Gprs_ReceiveDataID(void)
{
	u8	terminal_id;
	if((GPRS_RDATA[0]==0x7B)&&(GPRS_RDATA[5]==0x7D))	//首尾验证正确
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
功能：
参数：
返回：
说明：
****************************************************************/
void Gprs_ReceiveDataPoint(void)
{
	switch(GPRS_RDATA[3])
	{
		case 0x21:
					LED0=1;
					if(GPRS_RDATA[4]==0x21)	   //开
					{
						Gprs_ReceiveDataState(0x41);	
					}
					else if(GPRS_RDATA[4]==0x22) //关
					{
						Gprs_ReceiveDataState(0x40);
					}	
					break;
		case 0x22:
					LED0=0;
					if(GPRS_RDATA[4]==0x21)	   //开
					{
						Gprs_ReceiveDataState(0x51);	
					}
					else if(GPRS_RDATA[4]==0x22) //关
					{
						Gprs_ReceiveDataState(0x50);
					}
					break;
		case 0x23:
					if(GPRS_RDATA[4]==0x21)	   //开
					{
						Gprs_ReceiveDataState(0x61);	
					}
					else if(GPRS_RDATA[4]==0x22) //关
					{
						Gprs_ReceiveDataState(0x60);
					}
					break;
		case 0x24:
					if(GPRS_RDATA[4]==0x21)	   //开
					{
						Gprs_ReceiveDataState(0x71);	
					}
					else if(GPRS_RDATA[4]==0x22) //关
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
功能：
参数：
返回：
说明：
****************************************************************/
void Gprs_ReceiveDataState(u8 zigbeecommand)
{
	CS3=0;
	delay_ms(1);
	SPIx_ReadWriteByte(zigbeecommand);			//控制
	delay_ms(2);
	if(SPIx_ReadWriteByte(0x30)=='1')	   	//查看控制是否成功
		printf("Control Zigbee OK!\n");
	else 
		printf("error!\n");
	delay_ms(2);
	CS3=1;
	delay_us(100);		
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/

