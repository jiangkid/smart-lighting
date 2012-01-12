#include "test.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "12864.h"
#include "beep.h"
#include "led.h"
#include "func.h"
#include "disp.h"
#include "rtc.h"
#include "eeprom.h"
#include "gprs.h"
#include "timer.h"
#include "usartx.h"
#include "checkfunc.h"
#include "checkdisp.h"
#include "setfunc.h"
#include "setdisp.h"
#include "zigbeefunc.h"
#include "gprsfunc.h"
/***************************************************************************/
const u8 *COMPILED_DATE=__DATE__;				//获得编译日期
const u8 *COMPILED_TIME=__TIME__;				//获得编译时间
/***************************************************************************/
//串口数据
u8 USART2_RXBUF[U2RX_LENGTH]={0};    			//接收缓冲,最大128个字节. 
u8 U2_K=0;
u8 USART2_TXBUF[U2TX_LENGTH]={0};  
u8 USART3_RXBUF[U3RX_LENGTH]; 
u8 U3_K=0;
u8 USART3_TXBUF[U3TX_LENGTH]; 
//按键状态
vu8 STATE=NORMAL_STATE;	  						//状态机状态标志  
u8 key_value=0;	   								//按键获取值
u8 cursorbit=0;									//光标标志位 
u8 set_data[KEYDATA_LENGTH]={0};    			//设置时临时数组,按键的临时保存	
//EEPROM  
u8 ee_gid=0;		  							//gprs的ID，也是区域的ID
u8 ee_tid=0;									//终端的ID，亦是GPRS下面的子终端号码 ,默认0x01
u8 ee_ip[EEIP_LENGTH]={0};         				//服务器IP 
u8 ee_code[EECODE_LENGTH]={12,34,56};
u8 ee_ct[EECT_LENGTH]={0,20,0,20};
//someother
u16 waitmessagebacktime=0;						//等待AT指令时间

//zigbee
vu8 ZIGBEESTATE=ZIGBEE_NORMAL;	
u8 zigbee_connect=0;
zigbeecommunication_control gprsaccept={0};	 														//接收到gprs消息					
zigbeecommunication_control zigbeeaccept={0};														//接收到zigbee消息
//gprs
vu8 GPRSSTATE=GPRS_NORMAL;
u8 IP[IP_LENGTH]={"060.176.043.187:5003"};	   	//服务器IP地址
u16 gprssendmessagetiming=0;  					//定时发送消息 
u8 mc52i_ok=0;				 					// Mc52i通信正常与否的标志位  
gprssend_control gprssend  ={0x2F,0x2F,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0xCC};						//发送gprs消息
zigbeecommunication_control zigbeesend={0x2F,0x43,0x2F,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0xCC};		//发送zigbee消息 
/***************************************************************************/
/****************************************************************
版本：
功能：
时间：
****************************************************************/
int main(void)
{
	Stm32Clock_Init(9);
	Delay_Init(72);		 
	RTC_Init();
	Timer2_Init(1000,7199);
	Timer3_Init(5000,7199);
	uart1_init(72,9600);  //没有定义接收
	Usart2_Init(36,9600);
	Usart3_Init(36,9600);
	Beep_Init();
	GprsPort_Init();
	LCDPort_Init();
	Key_Init();
	KeyEXTI_Init();
	AT24CXX_Init();
	Led_Init();
	LCDInit();
	ClearLCD();
	WriteLCDData(2,3,"welcome");
	Delay_Nms(1000);
	ClearLCD();
	WriteLCDData(1,1,"ZIGBEE");
	WriteLCDData(2,1,"GPRS");
	Zigbee_InitCheck();	
	Mc52i_Init();	//gprs初始化
	Delay_Nms(1000);
	ClearLCD();
	Normal_Page();
	while(1)
	{		
//gprs状态切换
 		switch(GPRSSTATE)
		{
			case GPRS_NORMAL		:
									__nop();
									break;			
			case GPRS_RECEIVE		:
									Gprs_Receivefunc();
									break;
			case GPRS_HEARTBEAT		:
									Gprs_Heartbeatfunc();
									break;
			default					:					
									break;
		}
//zigbee状态切换
		switch(ZIGBEESTATE)
		{
			case ZIGBEE_NORMAL		:		//普通状态这条为空
									__nop();
										break;
			case ZIGBEE_ALARM		:		//zigbee主动报警，需要将消息发送到服务器
									__nop();
									break;
			case ZIGBEE_CHECKWRONG	:						//校验出错，多发几次的状态
									Gprs_DealReceiveMessage();
									break;
			default					:
									break;
		}
//按键状态机切换
		switch(STATE)
		{
			case NORMAL_STATE		:
									Normal_Page();
									Normal_Func();
									break;

			case INPUT_CODE_STATE	:						//设置功能
									Input_Code_Func();
									break;						 
			case FUNCTION_SET1_STATE :
									Function_Set1();
									break;
			case FUNCTION_SET2_STATE :
									Function_Set2();
									break;
			case FUNCTION_SET3_STATE :
									Function_Set3();
									break;		
			case SET_GID_STATE  		:
									Set_GID_Func();
									break;								
			case SET_TID_STATE  	:
									Set_TID_Func();
									break;

			case SET_DATE_STATE  	:
									Set_Date_Func();
									break;
			case SET_TIME_STATE  	:
									Set_Time_Func();
									break;
			case SET_IP_STATE  		:
									Set_IP_Func();
									break;
			case SET_CT_STATE		:
									Set_CT_Func();
									break;
			case CONNECT_NET_STATE	:
									Connect_Net_Func();
									break;
			case DETACH_NET_STATE	:
									Detach_Net_Func();
									break;
			case SET_CODE_STATE  		:
									Set_Code_Func();
									break;
															//查看功能
			case FUNCTION_CHECK1_STATE		:
									Function_Check1();
									break;
			case CHECK_ID_STATE		:
									Check_ID_Func();
									break;
			case CHECK_IP_STATE		:
									Check_IP_Func();
									break;
			case CHECK_CT_STATE		:
									Check_CT_Func();
									break;
			default         		:
									break;
		}
	}
}
