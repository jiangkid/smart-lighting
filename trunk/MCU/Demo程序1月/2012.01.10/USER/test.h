#ifndef _TEST_H_
#define _TEST_H_

#include <string.h>
#include <stm32f10x_lib.h>
#include "sys.h"
/******************************************************************************/
//ID地址结构体
typedef struct		  
{
	u8 GID;
	u8 TID;
	u8 RID;
	u8 LID[5];
}structID;
//通信用结构体
typedef struct		  
{
	u8 head[3];					//头
	u8 aim;						//目的识别
	structID ID;				//目的地址
	u8 commandtype;				//命令分类
	u8 commandtarget;			//命令对象
	u8 action;					//动作类型
	u8 actiondata[4];			//动作要求
	u8 check;					//异或和校验
	u8 end;						//结束符
}zigbeecommunication_control;
typedef struct		  		  //发送gprs消息使用，，，去除了保留位和校验位，其他都一样
{
	u8 head[2];				   //头
	u8 mark;				   //执行命令失败与否标志
	u8 device;				   //器件类型
	structID ID;			   //IEEE地址
	u8 command;				   //命令对象
	u8 action;				   //动作类型
	u8 data[4];				   //数据内容
	u8 end;					   //结束符
}gprssend_control;
/******************************************************************************/
//参数长度定义
#define IP_LENGTH						20
#define U2RX_LENGTH						128
#define EEIP_LENGTH						6
#define KEYDATA_LENGTH					16
#define U3TX_LENGTH						21
#define U3RX_LENGTH						21
#define EECODE_LENGTH					3
#define EECT_LENGTH						4
#define U2TX_LENGTH						21
/******************************************************************************/
#define NULL 							0
/******************************************************************************/
//State define
#define NORMAL_STATE	      			0x10				 

#define INPUT_CODE_STATE				0x21
#define FUNCTION_SET1_STATE   			0x22
#define FUNCTION_SET2_STATE   			0x23
#define FUNCTION_SET3_STATE				0x24
#define FUNCTION_CHECK1_STATE			0x25


#define SET_GID_STATE       		   	0x31
#define SET_TID_STATE     	 			0x32
#define SET_DATE_STATE       		 	0x33
#define SET_TIME_STATE        			0x34
#define SET_IP_STATE         		 	0x35
#define SET_CT_STATE					0x36
#define	CONNECT_NET_STATE				0x37
#define	DETACH_NET_STATE				0x38
#define SET_CODE_STATE					0x39


#define CHECK_ID_STATE					0x51
#define CHECK_IP_STATE					0x52
#define CHECK_CT_STATE					0x53					

/******************************************************************************/
//Gprs State
#define GPRS_NORMAL						0x10
#define GPRS_RECEIVE					0x11
#define GPRS_HEARTBEAT					0x12
/******************************************************************************/
//Zigbee State
#define ZIGBEE_NORMAL					0x10
#define ZIGBEE_ALARM					0x11
#define ZIGBEE_CHECKWRONG				0x12
/******************************************************************************/
//EEprom define
#define EE_GID							0			//共1字节							
#define EE_TID							1			//共1字节	
#define EE_IP   						2  			//共6字节
#define EE_CODE							8 			//共3字节
#define EE_CT	   						11			//共4字节
/******************************************************************************/	
#define LCD_CS  						PBout(7) 	//LCD	Port define
#define LCD_SID							PBout(8)
#define LCD_SCK 						PBout(9)
	 
#define IIC_SCL   						PAout(0) 	//SCL			   //IIC IO操作函数
#define IIC_SDA   						PAout(1) 	//SDA	 
#define READ_SDA						PAin(1)  	//输入SDA 

#define SDA_IN()  						{GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=8<<4;}	    //PA1  //IIC IO方向设置
#define SDA_OUT() 						{GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=3<<4;}

#define GPRS_ON  						PCout(8)	   				  //GPRS开关机控制
#define GPRS_OFF 						PCout(9)

#define LED0 							PBout(12)// PB12				  //LED端口定义
#define LED1 							PBout(13)// PB13
#define LED2 							PBout(14)// PB14
#define LED3 							PBout(15)// PB15
/******************************************************************************/
#define AT24C01							127
#define AT24C02							255
#define AT24C04							511
#define AT24C08							1023
#define AT24C16							2047
#define AT24C32							4095
#define AT24C64	   						8191
#define AT24C128						16383
#define AT24C256						32767
#define AT24C512   						65535
 
//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE 						AT24C512 
/******************************************************************************/
#endif
