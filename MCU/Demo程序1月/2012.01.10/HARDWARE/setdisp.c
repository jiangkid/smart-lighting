#include "test.h"
#include "setdisp.h"
#include "12864.h"
/******************************************************************************/
/******************************************************************************/
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Set_GID_Page(void)
{	
	ClearLCD();			     //LCD清屏，整体显示，游标关
	WriteLCDData(1,2,"GPRS的ID设置");
	WriteLCDData(3,1,"ID：* * * ");
	WriteCommand(0x8A);
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Set_TID_Page(void)
{
	ClearLCD();
	WriteLCDData(1,2,"终端的ID设置");
	WriteLCDData(3,1,"ID：* * * ");
	WriteCommand(0x8A);
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Set_Date_Page(void)
{	
	ClearLCD();			     //LCD清屏，整体显示，游标关
	WriteLCDData(1,3,"日期设置");
	WriteLCDData(2,1,"年：* * * * ");
	WriteLCDData(3,1,"月：* * ");
	WriteLCDData(4,1,"日：* * ");
	WriteCommand(0x92);
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Set_Time_Page(void)
{
	ClearLCD();			     //LCD清屏，整体显示，游标关
	WriteLCDData(1,3,"时间设置");
	WriteLCDData(2,1,"时：* *  ");
	WriteLCDData(3,1,"分：* * ");
	WriteLCDData(4,1,"秒：* * ");
	WriteCommand(0x92);	
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Set_IP_Page(void)
{	
	ClearLCD();			     //LCD清屏，整体显示，游标关
	WriteLCDData(1,2,"服务器IP设置");
	WriteLCDData(3,1,"***.***.***.*** ");
	WriteLCDData(4,1,"端口：**** ");
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Set_CT_Page(void)
{
	ClearLCD();
	WriteLCDData(1,2,"请输入CT比值");
	WriteLCDData(2,1,"如：020A/020mA");
	WriteLCDData(3,2,"***A/***mA");

}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Connect_Net_Page(void)
{
	ClearLCD();
	WriteLCDData(1,3,"连接网络？");
	WriteLCDData(3,2,"SURE/CANSEL");
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Detach_Net_Page(void)
{
	ClearLCD();
	WriteLCDData(1,3,"断开网络？");
	WriteLCDData(3,2,"SURE/CANSEL");
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Set_Code_Page(void)
{
	ClearLCD();
	WriteLCDData(1,1,"请输入新密码:");
}
