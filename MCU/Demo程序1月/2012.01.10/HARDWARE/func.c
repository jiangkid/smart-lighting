#include "test.h"
#include "delay.h"
#include "12864.h"
#include "rtc.h"
#include "func.h"	
#include "setdisp.h"
#include "disp.h"
#include "checkdisp.h"
/******************************************************************************/
extern u8 key_value;
extern u8 STATE;
/******************************************************************************/
/****************************************************************
名称：Normal_Func()
功能：
说明：
返回：NULL
****************************************************************/
void Normal_Func(void)
{
	switch(key_value)
	{
		case 'S' :
				key_value=NULL;
				STATE=INPUT_CODE_STATE;
				Input_Code_Page();
				break;
		case 'L' :
				key_value=NULL;
				STATE=FUNCTION_CHECK1_STATE;
				Function_CheckPage1();
				break;	
		case 'C' :
				key_value=NULL;
				STATE=NORMAL_STATE;
				ClearLCD();
				Normal_Page();
				break;
		default :
				key_value=NULL;	
				break;
	}
}
/****************************************************************
名称：
功能：
说明：
返回：
****************************************************************/
void Function_Set1(void)
{
	switch(key_value)
	{
		case '1' :
				key_value=NULL;
				STATE=SET_GID_STATE;
				Set_GID_Page();
				break;
		case '2' :

				key_value=NULL;
				STATE=SET_TID_STATE;
				Set_TID_Page();
				break;
		case '3' :
				key_value=NULL;	
				STATE=SET_DATE_STATE;
				Set_Date_Page(); 
				break;
		case '4' :
				key_value=NULL;	
				STATE=SET_TIME_STATE;
				Set_Time_Page(); 
				break;
		case 'D' :
				key_value=NULL;	
				STATE=FUNCTION_SET2_STATE;
				Function_SetPage2();
				break;
		case 'C' :
				key_value=NULL;
				STATE=NORMAL_STATE;
				ClearLCD();
				Normal_Page();
				break;
		default :
				key_value=NULL;
				break;
	}
}

/****************************************************************
名称：
功能：
说明：
返回：
****************************************************************/
void Function_Set2(void)
{
	switch(key_value)
	{
		case '5' :
				key_value=NULL;
				STATE=SET_IP_STATE;
				Set_IP_Page();
				break;
		case '6' :
				key_value=NULL;
				STATE=SET_CT_STATE;
				Set_CT_Page();
				break;
		case '7' :
				key_value=NULL;
				STATE=CONNECT_NET_STATE;
				Connect_Net_Page();
				break;
		case '8' :
				key_value=NULL;
				STATE=DETACH_NET_STATE;
				Detach_Net_Page();
				break;

		case 'U' :
				key_value=NULL;	
				STATE=FUNCTION_SET1_STATE;
				Function_SetPage1();
				break;
		case 'D' :
				key_value=NULL;	
				STATE=FUNCTION_SET3_STATE;
				Function_SetPage3();
				break;
		case 'C' :
				key_value=NULL;
				STATE=NORMAL_STATE;
				ClearLCD();
				Normal_Page();
				break;
		default :
				key_value=NULL;
				break;
	}
}
/****************************************************************
名称：
功能：
说明：
返回：
****************************************************************/
void Function_Set3(void)
{
	switch(key_value)
	{
		case '9' :
				key_value=NULL;
				STATE=SET_CODE_STATE;
				Set_Code_Page();
				break;

		case 'U' :
				key_value=NULL;	
				STATE=FUNCTION_SET2_STATE;
				Function_SetPage2();
				break;
		case 'C' :
				key_value=NULL;
				STATE=NORMAL_STATE;
				ClearLCD();
				Normal_Page();
				break;
		default :
				key_value=NULL;
				break;
	}
}
/****************************************************************
名称：
功能：
说明：
返回：
****************************************************************/
void Function_Check1(void)
{
	switch(key_value)
	{
		case '1' :
				key_value=NULL;
				STATE=CHECK_ID_STATE;
				Check_ID_Page();
				break;
		case '2' :
				key_value=NULL;
				STATE=CHECK_IP_STATE;
				Check_IP_Page();
				break;
		case '3' :
				key_value=NULL;
				STATE=CHECK_CT_STATE;
				Check_CT_Page();
				break;
		case 'C' :
				key_value=NULL;
				STATE=NORMAL_STATE;
				ClearLCD();
				Normal_Page();
				break;
		default :
				key_value=NULL;
				break;
	}
}

