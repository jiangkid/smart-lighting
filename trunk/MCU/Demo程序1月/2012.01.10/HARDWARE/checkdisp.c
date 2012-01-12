#include "test.h"
#include "checkdisp.h"
#include "12864.h"
/******************************************************************************/
extern u8 ee_gid;
extern u8 ee_tid;
extern u8 ee_ip[EEIP_LENGTH];
extern u8 ee_ct[EECT_LENGTH];
/******************************************************************************/
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Check_ID_Page(void)
{
	ClearLCD();
	WriteLCDData(2,1,"GPRS的ID：");
	WriteData(ee_gid/100+0x30);		  //GID.1
	WriteData(ee_gid%100/10+0x30);		  //GID.2
	WriteData(ee_gid%10+0x30);		  //GID.3
	WriteLCDData(3,1,"终端的ID：");
	WriteData(ee_tid/100+0x30);		  //TID.1
	WriteData(ee_tid%100/10+0x30);		  //TID.2
	WriteData(ee_tid%10+0x30);		  //TID.3
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Check_IP_Page(void)
{
	ClearLCD();
	WriteLCDData(1,1,"IP地址：");
	WriteCommand(0x90);
	WriteData(ee_ip[0]/100+0x30);	     //IP1
	WriteData(ee_ip[0]%100/10+0x30);
	WriteData(ee_ip[0]%10+0x30);
	WriteData('.');
	WriteData(ee_ip[1]/100+0x30);	     //IP2
	WriteData(ee_ip[1]%100/10+0x30);
	WriteData(ee_ip[1]%10+0x30);
	WriteData('.');
	WriteData(ee_ip[2]/100+0x30);	     //IP3
	WriteData(ee_ip[2]%100/10+0x30);
	WriteData(ee_ip[2]%10+0x30);
	WriteData('.');
	WriteData(ee_ip[3]/100+0x30);	     //IP4
	WriteData(ee_ip[3]%100/10+0x30);
	WriteData(ee_ip[3]%10+0x30);
	WriteLCDData(3,1,"端口：");
	WriteData(ee_ip[4]%100/10+0x30);	     //PORT1
	WriteData(ee_ip[4]%10+0x30);
	WriteData(ee_ip[5]%100/10+0x30);	     //PORT2
	WriteData(ee_ip[5]%10+0x30);
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Check_CT_Page(void)
{
	ClearLCD();
	WriteLCDData(2,1,"CT比值为：");
	WriteCommand(0x88);
	WriteData(ee_ct[0]%10+0x30);		  //1
	WriteData(ee_ct[1]/10+0x30);		  //2
	WriteData(ee_ct[1]%10+0x30);		  //3
	WriteData('A');
	WriteData('/');
	WriteData(ee_ct[2]%10+0x30);		  //1
	WriteData(ee_ct[3]/10+0x30);		  //2
	WriteData(ee_ct[3]%10+0x30);		  //3
	WriteData('m');
	WriteData('A');
}
