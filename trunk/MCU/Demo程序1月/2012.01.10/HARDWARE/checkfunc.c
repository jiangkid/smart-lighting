#include "test.h"
#include "checkfunc.h"
#include "disp.h"
#include "12864.h"
/******************************************************************************/
extern u8 STATE;
extern u8 key_value;
/******************************************************************************/
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Check_ID_Func(void)
{
	switch(key_value)
	{
		case 'E' :
				key_value=NULL;
				STATE=FUNCTION_CHECK1_STATE;
				Function_CheckPage1();
				break;
		case 'C' :
				key_value=NULL;
				STATE=FUNCTION_CHECK1_STATE;
				Function_CheckPage1();
				break;
		default :
				key_value=NULL;
				break;
	}
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Check_IP_Func(void)
{
	switch(key_value)
	{
		case 'E' :
				key_value=NULL;
				STATE=FUNCTION_CHECK1_STATE;
				Function_CheckPage1();
				break;
		case 'C' :
				key_value=NULL;
				STATE=FUNCTION_CHECK1_STATE;
				Function_CheckPage1();
				break;
		default :
				key_value=NULL;
				break;
	}
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Check_CT_Func(void)
{
	switch(key_value)
	{
		case 'E' :
				key_value=NULL;
				STATE=FUNCTION_CHECK1_STATE;
				Function_CheckPage1();
				break;
		case 'C' :
				key_value=NULL;
				STATE=FUNCTION_CHECK1_STATE;
				Function_CheckPage1();
				break;
		default :
				key_value=NULL;
				break;
	}
}


