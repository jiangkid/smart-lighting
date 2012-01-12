#include "test.h"
#include "12864.h"
#include "disp.h"
#include "delay.h"
#include "rtc.h"
/******************************************************************************/
extern tm timer; 
/******************************************************************************/
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Normal_Page(void)
{
//	WriteLCDData(1,1,"     STM32      ");

	WriteCommand(0x91);	
	WriteData(' ');
	WriteData(timer.w_year/1000+0x30);		  //年
	WriteData(timer.w_year%1000/100+0x30);
	WriteData(timer.w_year%1000%100/10+0x30);
	WriteData(timer.w_year%10+0x30);
	WriteData('/');
	WriteData(timer.w_month/10+0x30);     //月
	WriteData(timer.w_month%10+0x30);
	WriteData('/');    
	WriteData(timer.w_date/10+0x30);		//日
	WriteData(timer.w_date%10+0x30);  

	WriteCommand(0x8A);	
	WriteData(' ');
	WriteData(timer.hour/10+0x30);	    //时
	WriteData(timer.hour%10+0x30);
	WriteData(':');
	WriteData(timer.min/10+0x30);		//分
	WriteData(timer.min%10+0x30);
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Input_Code_Page(void)
{
	ClearLCD();
	WriteLCDData(1,2,"请输入密码：");
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Function_SetPage1(void)
{
	ClearLCD();	
	WriteLCDData(1,1,"1.设置GPRS的ID");
	WriteLCDData(2,1,"2.设置终端的ID");
	WriteLCDData(3,1,"3.设置系统日期");
	WriteLCDData(4,1,"4.设置系统时间");
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Function_SetPage2(void)
{
	ClearLCD();
	WriteLCDData(1,1,"5.设置服务器IP");
	WriteLCDData(2,1,"6.设置CT比值");
	WriteLCDData(3,1,"7.连接GPRS网络");
	WriteLCDData(4,1,"8.断开GPRS网络");
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Function_SetPage3(void)
{
	ClearLCD();
	WriteLCDData(1,1,"9.设置登录密码");
	WriteLCDData(2,1,"10. 恢复出厂设置");
	WriteLCDData(3,1,"11.设置");
	WriteLCDData(4,1,"12.设置");
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Function_CheckPage1(void)
{
	ClearLCD();	
	WriteLCDData(1,1,"1.查看ID号码");
	WriteLCDData(2,1,"2.查看服务器IP");
	WriteLCDData(3,1,"3.查看CT比值");
}
