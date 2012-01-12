#include "test.h"
#include "12864.h"
#include "disp.h"
#include "delay.h"
#include "rtc.h"
/******************************************************************************/
extern tm timer; 
/******************************************************************************/
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void Normal_Page(void)
{
//	WriteLCDData(1,1,"     STM32      ");

	WriteCommand(0x91);	
	WriteData(' ');
	WriteData(timer.w_year/1000+0x30);		  //��
	WriteData(timer.w_year%1000/100+0x30);
	WriteData(timer.w_year%1000%100/10+0x30);
	WriteData(timer.w_year%10+0x30);
	WriteData('/');
	WriteData(timer.w_month/10+0x30);     //��
	WriteData(timer.w_month%10+0x30);
	WriteData('/');    
	WriteData(timer.w_date/10+0x30);		//��
	WriteData(timer.w_date%10+0x30);  

	WriteCommand(0x8A);	
	WriteData(' ');
	WriteData(timer.hour/10+0x30);	    //ʱ
	WriteData(timer.hour%10+0x30);
	WriteData(':');
	WriteData(timer.min/10+0x30);		//��
	WriteData(timer.min%10+0x30);
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void Input_Code_Page(void)
{
	ClearLCD();
	WriteLCDData(1,2,"���������룺");
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void Function_SetPage1(void)
{
	ClearLCD();	
	WriteLCDData(1,1,"1.����GPRS��ID");
	WriteLCDData(2,1,"2.�����ն˵�ID");
	WriteLCDData(3,1,"3.����ϵͳ����");
	WriteLCDData(4,1,"4.����ϵͳʱ��");
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void Function_SetPage2(void)
{
	ClearLCD();
	WriteLCDData(1,1,"5.���÷�����IP");
	WriteLCDData(2,1,"6.����CT��ֵ");
	WriteLCDData(3,1,"7.����GPRS����");
	WriteLCDData(4,1,"8.�Ͽ�GPRS����");
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void Function_SetPage3(void)
{
	ClearLCD();
	WriteLCDData(1,1,"9.���õ�¼����");
	WriteLCDData(2,1,"10. �ָ���������");
	WriteLCDData(3,1,"11.����");
	WriteLCDData(4,1,"12.����");
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void Function_CheckPage1(void)
{
	ClearLCD();	
	WriteLCDData(1,1,"1.�鿴ID����");
	WriteLCDData(2,1,"2.�鿴������IP");
	WriteLCDData(3,1,"3.�鿴CT��ֵ");
}
