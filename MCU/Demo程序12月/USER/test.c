#include "test.h" 
#include "timer.h"
#include "delay.h"
#include "sys.h"
#include "gprs.h"
#include "usart1.h"
#include "led.h"
#include "spi.h"
#include "rtc.h"
/***************************************************************************/
const u8 *COMPILED_DATE=__DATE__;//获得编译日期
const u8 *COMPILED_TIME=__TIME__;//获得编译时间
/***********************************************************************/
//u8 IP[20]={"122.224.101.205:5003"}; //服务器IP
u8 IP[20]={"060.176.41.095:5003"};
u8 ID[2]={0x00,0x01};
u8 USART2_RX_BUF[128];     //接收缓冲,最大128个字节.	
u8 GPRS_RDATA[128];     //接收缓冲,最大128个字节.	
u8 spirec3[128];
u8 U2_K=0;
u16 time2count;
u16 time3count=0;
u8 mc52i_ok=0;
u8 gprs_sisr=0;
u8 creg_ok=1;      //模块初始化标志
u8 time3_flag=0;   //定时发送gprs消息标志
u8 gprs_send=0;
u8 gprs_rec=0;
u8 GPRS_FIRSTMESSAGE[11]={0x23,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x24};
u8 GPRS_NORMALMESSAGE[6]={0x23,0x30,0x31,0x33,0x33,0x24};
//u8 GPRS_WARNMESSAGE[11];	
/**************************************************/
int main(void)
{
	Stm32_Clock_Init(9);
	delay_init(72);			  
	LED_Init();	  //gprs开关在这里面
	RTC_Init();
	USART2_Init(36,9600);
	uart_init(72,9600);

	SPIx_Init();
	SPIx_SetSpeed(SPI_SPEED_256);	//

	Timer3_Init(5000,7199);
	Timer2_Init(1000,7199);		 			 //0.1ms
	
	Mc52i_Init();
	Board_Communicate();	 //spi通信 这句相当于初始化模块	
	delay_ms(100);
	while(1)
	{
		if(creg_ok==1)					   //开机检测到联网后，只执行一次	搜索网络时间大概在40s
		{
			if(Check_Creg()==1)
			{
				delay_ms(1);
				Usart1_Send_String(USART2_RX_BUF);		//发送到串口查看
				Clear_U2_Rbuf();
				Gprs_Init();	        //grps init

				GPRS_FIRSTMESSAGE[1]=0x30;
				GPRS_FIRSTMESSAGE[2]=0x31;
				GPRS_FIRSTMESSAGE[3]=0x32;
				GPRS_FIRSTMESSAGE[4]=0x33;
				GPRS_FIRSTMESSAGE[5]=0x34;
				GPRS_FIRSTMESSAGE[6]=0x35;
				GPRS_FIRSTMESSAGE[7]=0x36;
				GPRS_FIRSTMESSAGE[8]=0x37;
				GPRS_FIRSTMESSAGE[9]=0x38;
				Gprs_Send(GPRS_FIRSTMESSAGE,"11");	  //发送起始码

				Gprs_Send(GPRS_NORMALMESSAGE,"06");	   //发送正常通信信息
 
				TIM3->CR1|=0x01;    //使能定时器3，定时发送消息
				creg_ok=0;
			}
		}
		if(gprs_sisr==1)		 				 //接收到消息
		{  			
			Usart1_Send_String(USART2_RX_BUF);		//发送到串口查看
			Clear_U2_Rbuf();			
			Gprs_Rec();	
			gprs_sisr=0;
		}
//		if(mc52i_ok==2)
//		{
//			LED0=0;
//		}
//		else
//		{
//			LED0=1;
//		}

		if(time3_flag==1)
		{
			time3_flag=0;
			if(mc52i_ok==2)
			{
				 HeartBeat();
			}
		}
		else if(time3_flag==2)    //600*0.5=300s
		{
			time3_flag=0;
			if(mc52i_ok==2)
			{ 
				Gprs_Send(GPRS_NORMALMESSAGE,"06");
				Gprs_Rec();								//这句可以不要
			}
		}
	}
}
