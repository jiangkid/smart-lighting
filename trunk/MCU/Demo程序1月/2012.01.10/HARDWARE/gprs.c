#include "test.h"
#include "gprs.h"
#include "led.h"
#include "usartx.h"	
#include "delay.h"
#include "usart.h"
#include "gprsfunc.h"
#include "12864.h"
/****************************************************************************/
extern u8 USART2_RXBUF[U2RX_LENGTH];     //接收缓冲数组,最大128个字节.
extern u8 U2_K;					  //接收数据长度标志 	
extern u8 IP[IP_LENGTH];				    //服务器Ip地址
extern u16 waitmessagebacktime;			   //定时器2累加值,等待gprs返回时间 
extern u8 mc52i_ok;					// Mc52i通信正常与否的标志位

extern u8 ee_ip[EEIP_LENGTH];					//存在EEPROM中的IP地址+端口号码	  
/******************************************************************************/
extern zigbeecommunication_control gprsaccept;
/******************************************************************************/
/****************************************************************
功能：初始化GPRS端口
参数：NULL
返回：NULL
说明：PC8:gprs_on  PC9:gprs_off 
****************************************************************/
void GprsPort_Init(void)
{
	RCC->APB2ENR|=1<<4;      //使能PORTC时钟
	GPIOC->CRH&=0xFFFFFF00;	 //PC8 PC9 控制gprs开关
	GPIOC->CRH|=0x00000033;
	GPIOC->ODR|=1<<8;
	GPIOC->ODR|=1<<9;
}
/****************************************************************
功能：发送AT指令
参数：utstring为发送内容，如“abcdefg”
返回：NULL
说明：这个是以0x00结尾的，发送不了0x00
****************************************************************/
void AT_Command(u8  utstring[])
{
	u16 i=0;
	for(i=0;i<mstrlen(utstring);i++)			 //发送字符串
	{
		Usart2_SendByte(utstring[i]);
	}
}
/****************************************************************
功能：发送AT指令
参数：*psendatcommand为发送内容，sendatcommandlength为发送的长度
返回：NULL
说明：可以发送0x00
****************************************************************/
void Send_ATCommand(u8 *psendatcommand,u16 sendatcommandlength)
{
	while(sendatcommandlength--)		 //发送字符串
	{
		Usart2_SendByte(*psendatcommand);
		psendatcommand++;
	}
}
/****************************************************************
功能：检验串口2接收到的数据（GPRS）中是否包含”OK“	或者"ERROR"
参数：代码中包含"OK"，GPRS通信正常，mc52i_ok=2;包含"ERROR"，则通信出错，mc52i_ok=1;
返回：检测出OK则，返回0x12；检测出ERROR则返回0x11；都没有检测出来的时候返回0x10
说明：可以添加mc52i_ok=0选项
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
功能：普通AT指令，返回OK字样，发送函数
参数：utstring[]，AT指令；gprscommand_waittime，等待返回的最长时间时间
返回： NULL
说明： 发送一个AT指令后，开始等待返回，在最大等待时间里面如果没有接收到数据，则报错
****************************************************************/
void Gprs_Command(u8  utstring[],u16 gprscommand_waittime)
{	
	Clear_U2rxbuf();				//先清除接收缓存
	AT_Command(utstring); 							
	waitmessagebacktime=0;		   			//清除计时
	TIM2->CR1|=0x01;    					//使能定时器2
	while(waitmessagebacktime<gprscommand_waittime)    				//最多等待时间 waitgprsbacktime=50 时 为5s
	{
		if(U2_K>5)   						 //至少也得6个字符，**OK**	
		{
			Delay_Nms(10);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2

	if(waitmessagebacktime<gprscommand_waittime)	   //接收到数据
	{  
		LED0=1;											//连接了GPRS模块	
		mc52i_ok=Check_OK();
		if(mc52i_ok==0x12)
		{
			printf("Receive model message OK!\n");
			LED2=1;								//用于指示通信是OK还是ERROR
		}
		else if(mc52i_ok==0x11)
		{
			printf("Receive model message ERROR!\n");
			LED2=0;
		}
		else
			__nop();
		Usart1_SendString(USART2_RXBUF);		//发送到串口查看
	}
	else										 //没有数据接收
	{
		mc52i_ok=0x00;
		LED0=0;	
		LED2=0;
	}
}
/****************************************************************
功能：初始化Mc52i模块
参数：NULL
返回：NULL
说明：1，设置波特率2.检验SYSSTART；3，ATE0；4，AT+CREG=1;5.保存；6，检验+CREG: 2；
		7.检测+CREG: 1(本地)或者+CREG: 5(漫游)
****************************************************************/
void Mc52i_Init(void)
{	
	u8 i=0;
	GPRS_OFF=0;								  //模块关闭
	Delay_Nms(500);
	GPRS_OFF=1;
	Delay_Nms(100);

	GPRS_ON=0;								//模块开启
	Delay_Nms(500);	
	GPRS_ON=1;	

//	Clear_U2rxbuf();
////1.设置波特率为9600，由于全新的模块是没有存放波特率值的，需要手动设置
//	Gprs_Command("AT+IPR=9600\r\n",50); 
//	Delay_Nms(1);
//2.SYSSTART
	Clear_U2rxbuf();	
	waitmessagebacktime=0; 					//清除计时					
 	TIM2->CR1|=0x01; 						//使能定时器2
	while(waitmessagebacktime<100)    				//最多等待时间waitgprsbacktime=100 时 为10s
	{
		if(U2_K>5)   						 //至少也得6个字符，"^SYSSTART"
		{
			Delay_Nms(5);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	if(waitmessagebacktime<100)				//接收到数据   
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
		Usart1_SendString(USART2_RXBUF);		//发送到串口查看
	}
	else									 //没有接收到数据,则认为模块没有连接
	{
	  	mc52i_ok=0x00 ;
		LED0=0;
		LED2=0;
	}
//3.ATE0
   	if(mc52i_ok==0x12)							 //关闭回显
	{
		printf("ATE0\n");
		Gprs_Command("ATE0\r\n",50); 
		Delay_Nms(1);
	}
//4.AT+CREG=1
	if(mc52i_ok==0x12)						   //允许网络注册主动提供结果代码
	{   
		printf("AT+CREG=1\n");
		Gprs_Command("AT+CREG=1\r\n",50);
		Delay_Nms(1);
	}
//5.保存AT+CREG=1的值
	if(mc52i_ok==0x12)
	{
		printf("AT&W\n");
		Gprs_Command("AT&W\r\n",50);
		Delay_Nms(1);
	}
////6.+CREG:2
//	Clear_U2rxbuf();
//	waitmessagebacktime=0;					   //清除计时
//	TIM2->CR1|=0x01;    					//使能定时器2
//	while(waitmessagebacktime<100)    				//最多等待时间waitgprsbacktime=100 时 为10s
//	{
//		if(U2_K>7)   						 //至少也得6个字符，"+CREG: 2"
//		{
//			Delay_Nms(5);              	   //延时下，有些时候不止这么点长度
//			break;	
//		}
//	}
//	TIM2->CR1&=~(0x01);    					//清除定时器2
//
//	if(waitmessagebacktime<100)			    //接收到数据	   
//	{
//	   	for(i=0;i<U2RX_LENGTH;i++)
//		{
//			if((USART2_RXBUF[i]=='C')&&(USART2_RXBUF[i+1]=='R')&&(USART2_RXBUF[i+2]=='E')&&	  //+CREG: 2已注册到本地网络
//					(USART2_RXBUF[i+3]=='G')&&(USART2_RXBUF[i+6]=='2'))  					
//			{
//				mc52i_ok=0x12;
//				LED2=1;
//				LED0=1;
//				break;
//			}
//		} 
//		Usart1_SendString(USART2_RXBUF);		//发送到串口查看
//	}
//	else									 //没有接收到数据,则认为模块没有连接
//	{
//		mc52i_ok=0x00;
//		LED0=0;
//		LED2=0;
//	} 		
 //7.+CREG: 1或5  
 	Clear_U2rxbuf(); 	
 	waitmessagebacktime=0;						//清除计时
	TIM2->CR1|=0x01;    					//使能定时器2
	while(waitmessagebacktime<600)    				//最多等待时间waitgprsbacktime=600 时 为60s
	{
		if(U2_K>7)   						 //至少也得6个字符，"+CREG: 1"或者"+CREG: 5"
		{
			Delay_Nms(5);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	if(waitmessagebacktime<600)				   //接收到数据  
	{
		for(i=0;i<100;i++)
		{
			if((USART2_RXBUF[i]=='C')&&(USART2_RXBUF[i+1]=='R')&&(USART2_RXBUF[i+2]=='E')&&
			(USART2_RXBUF[i+3]=='G')&&(USART2_RXBUF[i+6]=='1')||(USART2_RXBUF[i+6]=='5'))  //+CREG: 5漫游  1本地
			{
				LED0=1;						  //显示连接正常
				LED2=1;
				mc52i_ok=0x12;
				WriteLCDData(2,1,"GPRS           Y");				//显示连接
				break;
			}
		} 
		Usart1_SendString(USART2_RXBUF);		//发送到串口查看
	}
	else									  //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0x00;
		WriteLCDData(2,1,"GPRS           N");				//显示未连接
		LED0=0;
		LED2=0;
	}	
}
/****************************************************************
功能：检测信号强度
参数：
返回：
说明：
****************************************************************/
void  Csq_Detec(void)
{	
	Clear_U2rxbuf();	
	AT_Command("AT+CSQ\r\n"); 
	waitmessagebacktime=0;					//清除计时
	TIM2->CR1|=0x01;    					//使能定时器2
	while(waitmessagebacktime<50)    				//最多等待时间waitgprsbacktime=50 时 为5s
	{
		if(U2_K>8)   						 //至少也得10个字符，^SISW:1,6
		{
			Delay_Nms(5);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	if(waitmessagebacktime<50)				   //接收到数据
	{	
		LED0=1;
		LED2=1;
		mc52i_ok=0x12;
		Usart1_SendString(USART2_RXBUF);		//发送到串口查看
	}
	else								  //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0x00;
		LED0=0;
		LED2=0;
	} 	
}
/****************************************************************
功能：连接GPRS初始化
参数：NULL
返回：NULL
说明：8个步骤，optional可有可无
****************************************************************/
void Gprs_Init(void)
{
//1.选择GPRS工作模式
	if(mc52i_ok==0x12)
	{	
		printf("Set GPRS work model\n");
		Gprs_Command("AT^SICS=0,conType,GPRS0\r\n",50);
		Delay_Nms(1);			
	}
//2.用户名称	
	if(mc52i_ok==0x12)
	{	
		printf("Set GPRS name \n");
		Gprs_Command("AT^SICS=0,user,gprs\r\n",50);			 //optional
		Delay_Nms(1);
	}
//3.用户密码
	if(mc52i_ok==0x12)
	{	
		printf("Set GPRS code\n");
		Gprs_Command("AT^SICS=0,passwd,gprs\r\n",50);		//optional	
		Delay_Nms(1);	
	}
//4.网络运营商	
	if(mc52i_ok==0x12)
	{	
		printf("SetGPRS operator\n");
		Gprs_Command("AT^SICS=0,apn,cmnet\r\n",50);	
		Delay_Nms(1);	
	} 
//5.服务类型	
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
//7.设置服务器地址及端口	
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
//8. 打开端口
	if(mc52i_ok==0x12)
	{
		printf("Set GPRS open port\n");
		Gprs_Open();
		Delay_Nms(10); 
		Clear_U2rxbuf();	
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
	Clear_U2rxbuf();
	AT_Command("AT^SISO=1\r\n");
	waitmessagebacktime=0;				  //清除计时
	TIM2->CR1|=0x01;    					//使能定时器2
	while(waitmessagebacktime<600)    				//最多等待时间waitgprsbacktime=1200 时 为120s
	{
		if(U2_K>15)   						 //至少也得10个字符，大概是**OK**  **^SISW**
		{
			Delay_Nms(50);              	   //延时下，有些时候不止这么点长度
			break;	
		}							   
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	if(waitmessagebacktime<600)				//接收到数据   
	{	
		LED0=1;
		mc52i_ok=Check_OK();
		if(U2_K<30)
		{									  //正常打开
			printf("Open port OK !\n");
			mc52i_ok=0x12;
			LED2=1;		
		}
		else
		{
			if(U2_K>=30)						//消息很长 ，服务器没有打开	 ^SIS: 1, 0, 21, "Remote host has rejected the connection"
			{
				mc52i_ok=0x11; 				 //相当于ERROR
				LED2=0;                 
				printf("Server closed!\n");
			}	
		} 
		Usart1_SendString(USART2_RXBUF);		//发送到串口查看
	}
   	else							  //没有接收到数据,则认为模块没有连接
	{
	   	mc52i_ok=0x00;
		LED0=0;
		LED2=0;
	}	
}
/****************************************************************
功能： 发送GPRS消息
参数： send_gprs_length,发送数据长度
返回： NULL
说明：
****************************************************************/
void Gprs_Send(u8 send_gprstext[],u8 send_gprslength[3])
{
	u16  sendgprslength=0;
	sendgprslength=(send_gprslength[0]-0x30)*100+(send_gprslength[1]-0x30)*10+(send_gprslength[2]-0x30);	 //长度：ASCII转换成十进制数

	Usart1_SendString(USART2_RXBUF);		//发送到串口查看    
	Clear_U2rxbuf(); 
	printf("Sending gprs message!\n");
	AT_Command("AT^SISW=1,");
	AT_Command(send_gprslength);
	AT_Command("\r\n");

	waitmessagebacktime=0;				  //清除计时	
	TIM2->CR1|=0x01;    					//使能定时器2
	while(waitmessagebacktime<100)    				//最多等待时间waitgprsbacktime=100 时 为10s
	{
		if(U2_K>10)   						 //至少也得10个字符，^SISW:1,6
		{
			Delay_Nms(5);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	if(waitmessagebacktime<100)					   
	{
		mc52i_ok=0x12;
		LED0=1;
		LED2=1;		 	
		Usart1_SendString(USART2_RXBUF);		//发送到串口查看		 ^SISW: 1, 1
	}
	else								//没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0x00;
		LED0=0;	
		LED2=0;	
	}

	Clear_U2rxbuf();
	Send_ATCommand(send_gprstext,sendgprslength);	         //发送消息	 这里不需要发送回车换行，只要发送满数据就可以了，数据一定要满 
	waitmessagebacktime=0;				  //清除计时
	TIM2->CR1|=0x01;    					//使能定时器2
	while(waitmessagebacktime<100)    				//最多等待时间waitgprsbacktime=100 时 为10s
	{
		if(U2_K>10)   						 //至少也得10个字符，OK
		{
			Delay_Nms(20);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	if(waitmessagebacktime<100)			//接收到数据	  
	{  
		mc52i_ok=Check_OK();
		Usart1_SendString(USART2_RXBUF);		//发送到串口查看		^SISW: 1, 21, 21
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
	else							    //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0x00;
		LED0=0;	
		LED2=0;
	} 		
}
/****************************************************************
功能：心跳控制函数，每隔一段时间向服务器发送二字节数据，以防连接断开
参数：heartbitbyte,一个字节数
返回：NULL
说明：
****************************************************************/
void HeartBeat(u8 heartbitbyte)
{
	Usart1_SendString(USART2_RXBUF);		//发送到串口查看 
	Clear_U2rxbuf();
	printf("Heartbeat!\n");
	AT_Command("AT^SISW=1,1\r\n");
	waitmessagebacktime=0;				  //清除计时
	TIM2->CR1|=0x01;    					//使能定时器2
	while(waitmessagebacktime<100)    				//最多等待时间waitgprsbacktime=100 时 为10s
	{
		if(U2_K>10)   						 //至少也得10个字符，^SISW:1,1
		{
			Delay_Nms(5);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2
	if(waitmessagebacktime<100)				//接收到数据   
	{
		Usart1_SendString(USART2_RXBUF);		//发送到串口查看
		LED0=1;
	}
	else							   //没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0;
		LED0=0;
	}

	Clear_U2rxbuf();
	Usart2_SendByte(heartbitbyte);	      		 //发送一个字节消息
	waitmessagebacktime=0;				   //清除计时	
	TIM2->CR1|=0x01;    					//使能定时器2
	while(waitmessagebacktime<100)    				//最多等待时间waitgprsbacktime=100 时 为10s
	{
		if(U2_K>10)   						 //至少也得10个字符，OK
		{
			Delay_Nms(20);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2

	if(waitmessagebacktime<100)				   
	{
		LED0=1;
		Usart1_SendString(USART2_RXBUF);		//发送到串口查看
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
	else								//没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0;
		LED0=0;
		LED2=0;
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
 	Clear_U2rxbuf();
	AT_Command("AT^SISR=1,");
	AT_Command("100");
	AT_Command("\r\n");

	waitmessagebacktime=0;				  //清除计时
	TIM2->CR1|=0x01;    					//使能定时器2
	while(waitmessagebacktime<100)    				//最多等待时间waitgprsbacktime=100 时 为10s
	{
		if(U2_K>20)   						 //至少也得大于接受长度
		{
			Delay_Nms(20);              	   //延时下，有些时候不止这么点长度
			break;	
		}
	}
	TIM2->CR1&=~(0x01);    					//清除定时器2

	if(waitmessagebacktime<100)				   
	{
		LED0=1;	
		mc52i_ok=0x12;
				
		printf("The Message is:\n"); 				
		Usart1_SendString(USART2_RXBUF);		//发送到串口查看

		for(i=0;i<U2RX_LENGTH;i++)
		{
			if((USART2_RXBUF[i]==0x2F)&&(USART2_RXBUF[i+1]==0x43)&&(USART2_RXBUF[i+2]==0x2F)&&(USART2_RXBUF[i+20]==0xCC))	//起始符0x2F432F，结束符0x3C
			{																		  
				memcpy(&gprsaccept,USART2_RXBUF+i,21);	   //将数据复制给gprs接收结构体
				printf("Wright start and end!\n");				
				Gprs_DealReceiveMessage();         //处理接收到的GPRS消息	
				break;		//跳出for函数	
			}	
		}  				

	}
	else								//没有接收到数据,则认为模块没有连接
	{
		mc52i_ok=0;
		LED0=0;
	}	
}
/****************************************************************
功能：关闭GPRS端口函数
参数：NULL
返回：NULL
说明：
****************************************************************/
void Gprs_Close(void)
{
	printf("Close GPRS port!\n");
	Gprs_Command("AT^SISC=1\r\n",50);
	Delay_Nms(1);	
}
