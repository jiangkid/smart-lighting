#ifndef _TEST_H_
#define _TEST_H_

#include <string.h>
#include <stm32f10x_lib.h>
#include "sys.h"
/******************************************************************************/
//ID��ַ�ṹ��
typedef struct		  
{
	u8 GID;
	u8 TID;
	u8 RID;
	u8 LID[5];
}structID;
//ͨ���ýṹ��
typedef struct		  
{
	u8 head[3];					//ͷ
	u8 aim;						//Ŀ��ʶ��
	structID ID;				//Ŀ�ĵ�ַ
	u8 commandtype;				//�������
	u8 commandtarget;			//�������
	u8 action;					//��������
	u8 actiondata[4];			//����Ҫ��
	u8 check;					//����У��
	u8 end;						//������
}zigbeecommunication_control;
typedef struct		  		  //����gprs��Ϣʹ�ã�����ȥ���˱���λ��У��λ��������һ��
{
	u8 head[2];				   //ͷ
	u8 mark;				   //ִ������ʧ������־
	u8 device;				   //��������
	structID ID;			   //IEEE��ַ
	u8 command;				   //�������
	u8 action;				   //��������
	u8 data[4];				   //��������
	u8 end;					   //������
}gprssend_control;
/******************************************************************************/
//�������ȶ���
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
#define EE_GID							0			//��1�ֽ�							
#define EE_TID							1			//��1�ֽ�	
#define EE_IP   						2  			//��6�ֽ�
#define EE_CODE							8 			//��3�ֽ�
#define EE_CT	   						11			//��4�ֽ�
/******************************************************************************/	
#define LCD_CS  						PBout(7) 	//LCD	Port define
#define LCD_SID							PBout(8)
#define LCD_SCK 						PBout(9)
	 
#define IIC_SCL   						PAout(0) 	//SCL			   //IIC IO��������
#define IIC_SDA   						PAout(1) 	//SDA	 
#define READ_SDA						PAin(1)  	//����SDA 

#define SDA_IN()  						{GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=8<<4;}	    //PA1  //IIC IO��������
#define SDA_OUT() 						{GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=3<<4;}

#define GPRS_ON  						PCout(8)	   				  //GPRS���ػ�����
#define GPRS_OFF 						PCout(9)

#define LED0 							PBout(12)// PB12				  //LED�˿ڶ���
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
 
//Mini STM32������ʹ�õ���24c02�����Զ���EE_TYPEΪAT24C02
#define EE_TYPE 						AT24C512 
/******************************************************************************/
#endif
