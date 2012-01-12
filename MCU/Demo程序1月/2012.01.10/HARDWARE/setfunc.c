#include "test.h"
#include "setfunc.h"
#include "disp.h"
#include "rtc.h"
#include "disp.h"
#include "12864.h"
#include "eeprom.h"
#include "usart.h"
#include "gprs.h"
#include "gprsfunc.h"
#include "delay.h"
#include "setdisp.h"
/******************************************************************************/
extern u8 key_value;
extern u8 STATE;
extern u8 cursorbit;
extern u8 set_data[KEYDATA_LENGTH];
extern u8 ee_gid;
extern u8 ee_tid;
extern u8 ee_ip[EEIP_LENGTH];
extern u8 mc52i_ok;
extern u16 gprssendmessagetiming;
extern u8 ee_code[EECODE_LENGTH];
extern u8 ee_ct[EECT_LENGTH];
/******************************************************************************/
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Input_Code_Func(void)
{
	u8 i=0;
	u8 k=0;
	u8 icode[6];
	WriteCommand(0x89+cursorbit);    //从第三行的0x8a处开始显示
	if(cursorbit<6) 
		WriteCommand(0x0E);//显示ON,游标ON,游标位置OFF
	else		
		WriteCommand( 0x0c ); 
	if((key_value>='0')&&(key_value<='9'))   
	{
	    if(cursorbit<6)
	    {
	   	    WriteData(0xa3);                  //写入一个双字符组" * "，0xa3aa
			WriteData(0xaa);	
		    set_data[cursorbit]=key_value;
		    cursorbit++;				
	    }
	    key_value=NULL;
	}
	else if(key_value=='E')  
	{
	    key_value=NULL;
	    /***********核对六位密码是否正确"***********/
		icode[0]=ee_code[0]/10+0x30;
		icode[1]=ee_code[0]%10+0x30;
		icode[2]=ee_code[1]/10+0x30;
		icode[3]=ee_code[1]%10+0x30;
		icode[4]=ee_code[2]/10+0x30;
		icode[5]=ee_code[2]%10+0x30;
		for(i=0;i<6;i++)
		{
			if(set_data[i]==icode[i]) 
			{
				k++;
				set_data[i]=0x20;	//清除原先号码
			}
		}
		/****若六位密码正确，进入设置页面0，否则重新要求输入密码****/
		if(k==6)
		{
			STATE=FUNCTION_SET1_STATE; 		//进入功能设置状态0
			Function_SetPage1();			  //显示设置页面0
			cursorbit=0;	
		}
		else
		{	
			ClearLCD();
			WriteLCDData(1,2,"密码输入错误");
			WriteLCDData(3,2,"请重新输入！");
			Delay_Nms(1000);
			Delay_Nms(1000);
			STATE=INPUT_CODE_STATE;//进入密码输入状态
			cursorbit=0;
			ClearLCD();
			Input_Code_Page();
		}
	}
	else if(key_value=='C') 
	{
	    key_value=NULL;
		cursorbit=0;
	    STATE=NORMAL_STATE;
		ClearLCD();
		Normal_Page();
		for(i=0;i<6;i++)
		{
			set_data[i]=0x20;	//清除原先号码
		}		
	}
	else __nop();
}	
/****************************************************************
名称：
功能：
说明：id值要小于300
返回：
****************************************************************/
void Set_GID_Func(void)
{
	if(cursorbit<3)
		WriteCommand(0x0E);  //整体显示，游标ON，游标位置闪动
	else 
		WriteCommand(0x0C);	 //游标OFF
	if((key_value>='0')&&(key_value<='9'))
	{
		if(cursorbit<3)
		{
			if(cursorbit==0)		   //1		 
			{
				if(key_value<'3')			//	要小于3
				{
					WriteCommand(0x8A);
					WriteData(0xa3);
					WriteData(0x80+key_value);
					set_data[0]=key_value;
					cursorbit++;
				}
			}
			else if(cursorbit==1)		   //2
			{
				WriteCommand(0x8B);
				WriteData(0xa3);
				WriteData(0x80+key_value);
				set_data[1]=key_value;
				cursorbit++;
			}
			else if(cursorbit==2)		 //3
			{
				WriteCommand(0x8C);
				WriteData(0xa3);
				WriteData(0x80+key_value);
				set_data[2]=key_value;
				cursorbit++;
			}
		}
		key_value=NULL;
	}
	else if((key_value=='E')&&(cursorbit==3))
	{
		key_value=NULL;
		ee_gid=(set_data[0]-0x30)*100+(set_data[1]-0x30)*10+(set_data[2]-0x30);
		AT24CXX_WriteOneByte(EE_GID,ee_gid);   //eeprom存储
		printf("set gid:%d\n",ee_gid);             //输出查看显示
		STATE=FUNCTION_SET1_STATE;
		Function_SetPage1();
		cursorbit=0;	
	}
	else if(key_value=='C')
	{
		key_value=NULL;
		cursorbit=0;
		STATE=FUNCTION_SET1_STATE;
		Function_SetPage1();	
	}
	else 
		key_value=NULL;
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Set_TID_Func(void)
{
	if(cursorbit<3)
		WriteCommand(0x0E);  //整体显示，游标ON，游标位置闪动
	else 
		WriteCommand(0x0C);	 //游标OFF
	if((key_value>='0')&&(key_value<='9'))
	{
		if(cursorbit<3)
		{
			if(cursorbit==0)		   //1		 
			{
				if(key_value<'3')			//	要小于3
				{
					WriteCommand(0x8A);
					WriteData(0xa3);
					WriteData(0x80+key_value);
					set_data[0]=key_value;
					cursorbit++;
				}
			}
			else if(cursorbit==1)		   //2
			{
				WriteCommand(0x8B);
				WriteData(0xa3);
				WriteData(0x80+key_value);
				set_data[1]=key_value;
				cursorbit++;
			}
			else if(cursorbit==2)		 //3
			{
				WriteCommand(0x8C);
				WriteData(0xa3);
				WriteData(0x80+key_value);
				set_data[2]=key_value;
				cursorbit++;
			}
		}
		key_value=NULL;
	}
	else if((key_value=='E')&&(cursorbit==3))
	{
		key_value=NULL;
		ee_tid=(set_data[0]-0x30)*100+(set_data[1]-0x30)*10+(set_data[2]-0x30);
		AT24CXX_WriteOneByte(EE_TID,ee_tid);   //eeprom存储
		printf("set tid:%d\n",ee_tid);             //输出查看显示
		STATE=FUNCTION_SET1_STATE;
		Function_SetPage1();
		cursorbit=0;	
	}
	else if(key_value=='C')
	{
		key_value=NULL;
		cursorbit=0;
		STATE=FUNCTION_SET1_STATE;
		Function_SetPage1();	
	}
	else 
		key_value=NULL;
}
/****************************************************************
名称：
功能：
说明：
返回：
****************************************************************/
void Set_Date_Func(void)
{
	u32 year;
	u8 month;
	u8 date;

	if(cursorbit<8)
		WriteCommand(0x0E);  //整体显示，游标ON，游标位置闪动
	else 
		WriteCommand(0x0C);	 //游标OFF
	if((key_value>='0')&&(key_value<='9'))
	{
		if(cursorbit<8)
		{
			if(cursorbit==0)				   //year
			{
				WriteCommand(0x92);
				WriteData(0xa3);
				WriteData(0x80+key_value);
				set_data[0]=key_value;
				cursorbit++;
			}
			else if(cursorbit==1)
			{
				WriteCommand(0x93);
				WriteData(0xa3);
				WriteData(0x80+key_value);
				set_data[1]=key_value;
				cursorbit++;
			}
			else if(cursorbit==2)			   
			{
				WriteCommand(0x94);
				WriteData(0xa3);
				WriteData(0x80+key_value);
				set_data[2]=key_value;
				cursorbit++;
			}
			else if(cursorbit==3)
			{
				WriteCommand(0x95);
				WriteData(0xa3);
				WriteData(0x80+key_value);
				WriteCommand(0x8A);
				set_data[3]=key_value;
				cursorbit++;
			}
			else if(cursorbit==4)			//month	  高位只能是0或者1
			{
				if(key_value<'2')
				{
					WriteCommand(0x8A);
					WriteData(0xa3);
					WriteData(0x80+key_value);
					set_data[4]=key_value;
					cursorbit++;
				}
				else 
				{
					__nop();
				}
			}
			else if(cursorbit==5)				  	    //高位为1时低位只能是012
			{
				if((set_data[4]=='1')&&(key_value<'3'))
				{
					WriteCommand(0x8B);
					WriteData(0xa3);
					WriteData(0x80+key_value);
					set_data[5]=key_value;
					WriteCommand(0x9A);
					cursorbit++;
				}
				else if((set_data[4]=='0')&&(key_value<='9')&&(key_value>'0'))
				{
					WriteCommand(0x8B);
					WriteData(0xa3);
					WriteData(0x80+key_value);
					set_data[5]=key_value;
					WriteCommand(0x9A);
					cursorbit++;
				}
			}
			else if(cursorbit==6)				 //	day
			{
				month=(set_data[4]-0x30)*10+(set_data[5]-0x30);
				if(month!=2)					   //普通月份，天的高位可以是0123
				{	  
					if(key_value<'4')
					{
					//	WriteCommand(0x9A);
						WriteData(0xa3);
						WriteData(0x80+key_value);
						set_data[6]=key_value;
						cursorbit++;
					}
				}
				else 
				{
					if(key_value<'3')			  //2月份的天数高位只能是012
					{
					//	WriteCommand(0x9A);
						WriteData(0xa3);
						WriteData(0x80+key_value);
						set_data[6]=key_value;
						cursorbit++;
					}
				}
			}
			else if(cursorbit==7)				  
			{
				year=(set_data[0]-0x30)*1000+(set_data[1]-0x30)*100+(set_data[2]-0x30)*10+(set_data[3]-0x30);
				month=(set_data[4]-0x30)*10+(set_data[5]-0x30);
				if(month!=2)
				{
					if((month==4)||(month==6)||(month==9)||(month==11))   											//小月只有30天
					{
						if(((set_data[6]=='3')&&(key_value>'0'))||((set_data[6]=='0')&&(key_value=='0')))			  //不能为0，不能大于30
						{
							__nop();	
						}
						else
						{
							WriteCommand(0x9B);
							WriteData(0xa3);
							WriteData(0x80+key_value);
							WriteData(' ');		        //不加这个，最后一位显示不出来
							set_data[7]=key_value;
							cursorbit++;	
						}
					}
					else 
					{
						if(((set_data[6]=='3')&&(key_value>'1'))||((set_data[6]=='0')&&(key_value=='0')))	 	//大月只有31天 
						{																						 //不大于31，不能为0
							__nop();
						}
						else
						{
							WriteCommand(0x9B);
							WriteData(0xa3);
							WriteData(0x80+key_value);
							WriteData(' ');		        //不加这个，最后一位显示不出来
							set_data[7]=key_value;
							cursorbit++;	
						}
					}
				}
				else
				{
					if(((year%100)!=0)&&(year%4==0))    //闰年
					{
						WriteCommand(0x9B);
						WriteData(0xa3);
						WriteData(0x80+key_value);
						WriteData(' ');		        //不加这个，最后一位显示不出来
						set_data[7]=key_value;
						cursorbit++;	
					}
					else
					{
						if((set_data[6]=='2')&&(key_value=='9')||(set_data[6]=='0')&&(key_value=='0'))
						{
							__nop();
						}
						else
						{
							WriteCommand(0x9B);
							WriteData(0xa3);
							WriteData(0x80+key_value);
							WriteData(' ');		        //不加这个，最后一位显示不出来
							set_data[7]=key_value;
							cursorbit++;	
						}
					}
				}

			}	
		}
		key_value=NULL; //按键清空	
	}
	else if((key_value=='E')&&(cursorbit==8))
	{
		key_value=NULL;
 		year=(set_data[0]-0x30)*1000+(set_data[1]-0x30)*100+(set_data[2]-0x30)*10+(set_data[3]-0x30);
		month=(set_data[4]-0x30)*10+(set_data[5]-0x30);
		date=(set_data[6]-0x30)*10+(set_data[7]-0x30);

		RTC_Set(year,month,date,timer.hour,timer.min,timer.sec);  //设置年月日

		STATE=FUNCTION_SET1_STATE;
		Function_SetPage1();
		cursorbit=0;	
	}
	else if(key_value=='C')
	{
		key_value=NULL;
		cursorbit=0;
		STATE=FUNCTION_SET1_STATE;
		Function_SetPage1();	
	}
	else 
		key_value=NULL;
}
/****************************************************************
名称：
功能：
说明：
返回：
****************************************************************/
void Set_Time_Func(void)
{
	u8 hour;
	u8 min;
	u8 sec;
	if(cursorbit<6)
		WriteCommand(0x0E);  //整体显示，游标ON，游标位置闪动
	else 
		WriteCommand(0x0C);	 //游标OFF
	if((key_value>='0')&&(key_value<='9'))
	{
		if(cursorbit<6)	
		{
			if(cursorbit==0)
			{
				if(key_value<'3')	    //小时的高位要小于3
				{
					WriteCommand(0x92);
					WriteData(0xa3);
					WriteData(0x80+key_value);
					set_data[0]=key_value;
					cursorbit++;
				}
				else
				{
					__nop();
				} 	
			}
			else if(cursorbit==1)			  //小时的低位
			{
				if((set_data[0]=='2')&&(key_value>'3'))
				{
					__nop();
				}
				else
				{
					WriteCommand(0x93);
					WriteData(0xa3);
					WriteData(0x80+key_value);
					WriteCommand(0x8A);
					set_data[1]=key_value;
					cursorbit++;
				}
			}
			else if(cursorbit==2)     //分钟的高位
			{
				if(key_value<'6')	    //分钟的高位要小于6， 00-59
				{
					WriteCommand(0x8A);
					WriteData(0xa3);
					WriteData(0x80+key_value);
					set_data[2]=key_value;
					cursorbit++;
				}
				else
				{
					__nop();
				}
			}
			else if(cursorbit==3)//分钟低位任意值
			{
				WriteCommand(0x8B);
				WriteData(0xa3);
				WriteData(0x80+key_value);
				WriteCommand(0x9A);
				set_data[3]=key_value;
				cursorbit++;	
			}
			else if(cursorbit==4)     //秒钟的高位
			{
				if(key_value<'6')	    //秒的高位要小于6， 00-59
				{
					WriteCommand(0x9A);
					WriteData(0xa3);
					WriteData(0x80+key_value);
					set_data[4]=key_value;
					cursorbit++;
				}
				else
				{
					__nop();
				}
			}
			else if(cursorbit==5)//秒低位任意值
			{
				WriteCommand(0x9B);
				WriteData(0xa3);
				WriteData(0x80+key_value);
				set_data[5]=key_value;
				cursorbit++;	
			}
		}
		key_value=NULL;
	}
	else if((key_value=='E')&&(cursorbit==6))	    //按满时分秒后按确定键才有用
	{
		key_value=NULL;
		hour=(set_data[0]-0x30)*10+(set_data[1]-0x30);
		min= (set_data[2]-0x30)*10+(set_data[3]-0x30);
		sec= (set_data[4]-0x30)*10+(set_data[5]-0x30);
		RTC_Set(timer.w_year,timer.w_month,timer.w_date,hour,min,sec);  //设置时分秒

		STATE=FUNCTION_SET1_STATE;
		Function_SetPage1();
		cursorbit=0;	
	}
	else if(key_value=='C')
	{
		key_value=NULL;
		cursorbit=0;
		STATE=FUNCTION_SET1_STATE;
		Function_SetPage1();
	}
	else 
		key_value=NULL;	
}
/****************************************************************
名称：
功能：
说明：255.255.255:9999
返回：
****************************************************************/
void Set_IP_Func(void)
{
	if((key_value>='0')&&(key_value<='9'))
	{
		if(cursorbit<16)
		{
			if(cursorbit==0)					    //P1.1
			{	
				if(key_value<'3')		 				//要小于3
				{
					WriteCommand(0x88);
					WriteData(key_value);
					set_data[0]=key_value;
					cursorbit++;
				}
			}
			else if(cursorbit==1)		 			  //P1.2
			{
				if(set_data[0]<'2')	   					//P1.1是'0'或者'1'
				{
					WriteCommand(0x88);
					WriteData(set_data[0]);
					WriteData(key_value);
					set_data[1]=key_value;
					cursorbit++;
				}
				else if((set_data[0]=='2')&&(key_value<'6'))  //P1.1是'2'
				{
					WriteCommand(0x88);
					WriteData(set_data[0]);
					WriteData(key_value);
					set_data[1]=key_value;
					cursorbit++;
				}
			}
			else if(cursorbit==2)							 //P1.3
			{
				if(set_data[0]<'2')						   //P1.1是'0'或者'1'
				{
					WriteCommand(0x89);
					WriteData(key_value);
					set_data[2]=key_value; 	
					cursorbit++;
				}
				else if(set_data[0]=='2')					 //P1.1是'2'
				{
					if(set_data[1]<'5')
					{
						WriteCommand(0x89);
						WriteData(key_value);
						set_data[2]=key_value; 	
						cursorbit++;
					}
					else if(set_data[1]=='5')	  			  
					{	
						if(key_value<'6')
						{
							WriteCommand(0x89);
							WriteData(key_value);
							set_data[2]=key_value; 	
							cursorbit++;
						}
					}	
				}
			}
			else if(cursorbit==3)	    //P2.1要小于‘3’
			{
				if(key_value<'3')			    
				{
					WriteCommand(0x8A);
					WriteData(key_value);
					set_data[3]=key_value;
					cursorbit++;
				}
			}
			else if(cursorbit==4)	  				  //P2.2
			{
				if(set_data[3]<'2')	
				{
					WriteCommand(0x8A);
					WriteData(set_data[3]);
					WriteData(key_value);
					set_data[4]=key_value; 
					cursorbit++;
				}
				else if((set_data[3]=='2')&&(key_value<'6'))	   //P2.1是'2'
				{
					WriteCommand(0x8A);
					WriteData(set_data[3]);
					WriteData(key_value);
					set_data[4]=key_value; 
					cursorbit++;			
				}
			}
			else if(cursorbit==5)	    //P2.3
			{
				if(set_data[3]<'2')	
				{
					WriteCommand(0x8B);
					WriteData(key_value);
					set_data[5]=key_value;
					cursorbit++;
				}
				else if(set_data[3]=='2')	   //P2.1是'2'
				{
					if(set_data[4]<'5')
					{						
						WriteCommand(0x8B);
						WriteData(key_value);
						set_data[5]=key_value;
						cursorbit++;
					}
					else if(set_data[4]=='5')
					{
						if(key_value<'6')
						{
							WriteCommand(0x8B);
							WriteData(key_value);
							set_data[5]=key_value;
							cursorbit++;	
						}
					}
				}
			}
			else if(cursorbit==6)	    //P3.1要小于‘3’
			{
				if(key_value<'3')
				{
					WriteCommand(0x8C);
					WriteData(key_value);
					set_data[6]=key_value;
					cursorbit++;
				}
			}
			else if(cursorbit==7)	    //P3.2
			{
				if(set_data[6]<'2')
				{
					WriteCommand(0x8C);
					WriteData(set_data[6]);
					WriteData(key_value);
					set_data[7]=key_value;
					cursorbit++;
				}
				else if((set_data[6]=='2')&&(key_value<'6'))	   //P3.1是'2'
				{
					WriteCommand(0x8C);
					WriteData(set_data[6]);
					WriteData(key_value);
					set_data[7]=key_value;
					cursorbit++;			
				}
			}
			else if(cursorbit==8)	    //P3.3
			{
				if(set_data[6]<'2')
				{
					WriteCommand(0x8D);
					WriteData(key_value);
					set_data[8]=key_value;
					cursorbit++;
				}
				else if(set_data[6]=='2')	   //P3.1是'2'
				{
					if(set_data[7]<'5')  
					{
						WriteCommand(0x8D);
						WriteData(key_value);		
						set_data[8]=key_value;
						cursorbit++;
					}
					else if(set_data[7]=='5') 
					{
						if(key_value<'6')
						{
							WriteCommand(0x8D);
							WriteData(key_value);		
							set_data[8]=key_value;
							cursorbit++;	
						}
					}
				}
			}
			else if(cursorbit==9)	    //P4.1要小于‘3’
			{
				if(key_value<'3')
				{
					WriteCommand(0x8E);
					WriteData(key_value);
					set_data[9]=key_value;
					cursorbit++;
				}
			}
			else if(cursorbit==10)	    //P4.2
			{
				if(set_data[9]<'2')
				{
					WriteCommand(0x8E);
					WriteData(set_data[9]);
					WriteData(key_value);
					set_data[10]=key_value;
					cursorbit++;
				}
				else if((set_data[9]=='2')&&(key_value<'6'))	   //P4.1是'2'
				{
					WriteCommand(0x8E);
					WriteData(set_data[9]);
					WriteData(key_value);
					set_data[10]=key_value;
					cursorbit++;
				}
			}
			else if(cursorbit==11)	    //P4.3
			{
				if(set_data[9]<'2')
				{
					WriteCommand(0x8F);
					WriteData(key_value);
					set_data[11]=key_value;
					cursorbit++;
				}
				else if(set_data[9]=='2')	   //P4.1是'2'
				{
					if(set_data[10]<'5')
					{
						WriteCommand(0x8F);		  
						WriteData(key_value);
						set_data[11]=key_value;
						cursorbit++;
					}
					else if(set_data[10]=='5')
					{
						if(key_value<'6')
						{
							WriteCommand(0x8F);		  
							WriteData(key_value);
							set_data[11]=key_value;
							cursorbit++;	
						}
					}					
				}
			}
			else if(cursorbit==12)	    //Port1
			{
				WriteCommand(0x9B);
				WriteData(key_value);
				set_data[12]=key_value;
				cursorbit++;				
			}
			else if(cursorbit==13)	    //Port2
			{
				WriteCommand(0x9B);
				WriteData(set_data[12]);
				WriteData(key_value);
				set_data[13]=key_value;
				cursorbit++;				
			}
			else if(cursorbit==14)	    //Port3
			{
				WriteCommand(0x9C);
				WriteData(key_value);
				set_data[14]=key_value;
				cursorbit++;				
			}
			else if(cursorbit==15)	    //Port2
			{
				WriteCommand(0x9C);
				WriteData(set_data[14]);
				WriteData(key_value);
				set_data[15]=key_value;
				cursorbit++;				
			}
		}
		key_value=NULL;
	}
	else if((key_value=='E')&&(cursorbit==16))
	{
		key_value=NULL;
		ee_ip[0]=(set_data[0]-0x30)*100+(set_data[1]-0x30)*10+(set_data[2]-0x30);
		ee_ip[1]=(set_data[3]-0x30)*100+(set_data[4]-0x30)*10+(set_data[5]-0x30);
		ee_ip[2]=(set_data[6]-0x30)*100+(set_data[7]-0x30)*10+(set_data[8]-0x30);
		ee_ip[3]=(set_data[9]-0x30)*100+(set_data[10]-0x30)*10+(set_data[11]-0x30);
		ee_ip[4]=(set_data[12]-0x30)*10+(set_data[13]-0x30);
		ee_ip[5]=(set_data[14]-0x30)*10+(set_data[15]-0x30);
		AT24CXX_Write(EE_IP,ee_ip,6);  //eeprom存储
		printf("set ip=%s\n",ee_ip);
		STATE=FUNCTION_SET2_STATE;
		Function_SetPage2();
		cursorbit=0;
	}
	else if(key_value=='C')
	{
		key_value=NULL;
		cursorbit=0;
		STATE=FUNCTION_SET2_STATE;
		Function_SetPage2();	
	}
	else 
		key_value=NULL;	
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Set_CT_Func(void)
{
	if((key_value>='0')&&(key_value<='9'))
	{
		if(cursorbit<6)
		{
			if(cursorbit==0)		   //1		 
			{
				WriteCommand(0x89);
				WriteData(key_value);
				set_data[0]=key_value;
				cursorbit++;
			}
			else if(cursorbit==1)		   //2
			{
				WriteData(key_value);
				set_data[1]=key_value;
				cursorbit++;
			}
			else if(cursorbit==2)		 //3
			{
				WriteData(key_value);
				set_data[2]=key_value;
				cursorbit++;
			}
			else if(cursorbit==3)		 //4
			{
				WriteCommand(0x8B);
				WriteData('/');
				WriteData(key_value);
				set_data[3]=key_value;
				cursorbit++;
			}
			else if(cursorbit==4)		 //5
			{
				WriteData(key_value);
				set_data[4]=key_value;
				cursorbit++;
			}
			else if(cursorbit==5)		 //6
			{
				WriteData(key_value);
				set_data[5]=key_value;
				cursorbit++;
			}
			else
				__nop();
		}
		key_value=NULL;
	}
	else if(key_value=='E')
	{	
		key_value=NULL;	
		if(cursorbit==6)
		{
			ee_ct[0]=set_data[0]-0x30;
			ee_ct[1]=(set_data[1]-0x30)*10+(set_data[2]-0x30);
			ee_ct[2]=set_data[3]-0x30;
			ee_ct[3]=(set_data[4]-0x30)*10+(set_data[5]-0x30);
			AT24CXX_Write(EE_CT,ee_ct,4);  //eeprom存储
			printf("set ct:%s\n",ee_ct);             //输出查看显示
			ClearLCD();
			WriteLCDData(2,2,"CT值设置成功");
			Delay_Nms(1000);
			Delay_Nms(1000);
			STATE=FUNCTION_SET2_STATE;
			Function_SetPage2();
			cursorbit=0;
		}
		else
		{
			ClearLCD();
			WriteLCDData(2,2,"CT值设置失败");
			WriteLCDData(3,3,"重新输入");
			Delay_Nms(1000);
			Delay_Nms(1000);	
			cursorbit=0;
			STATE=SET_CT_STATE;
			Set_CT_Page();
		}			
	}
	else if(key_value=='C')
	{
		key_value=NULL;
		cursorbit=0;
		STATE=FUNCTION_SET2_STATE;
		Function_SetPage2();	
	}
	else 
		key_value=NULL;
}
/****************************************************************
功能：
参数：
返回：
说明：
****************************************************************/
void Connect_Net_Func(void)
{
	switch(key_value)
	{
		case 'E' :
					ClearLCD();
					mc52i_ok=0x12;				//强制连接
					WriteLCDData(1,2,"正在连接网络");
					WriteLCDData(3,2,"....稍等....");
					Gprs_Init();	        //grps init
					if(mc52i_ok==0x12)
					{
						WriteLCDData(3,2,"连接成功!   ");
						Gprs_FirstMessage(); //初次发送GPRS消息，填充服务器信息
						gprssendmessagetiming=0;			   //清除定时
						TIM3->CR1|=0x01;    //使能定时器3，定时发送消息
						Delay_Nms(1000);
						Delay_Nms(1000);
						STATE=FUNCTION_SET2_STATE;
						Function_SetPage2();
					}
					else
					{
						WriteLCDData(3,2,"连接失败!   ");
						Delay_Nms(1000);
						Delay_Nms(1000);
						STATE=NORMAL_STATE;
						ClearLCD();
						Normal_Page();	
					}
					key_value=NULL;
					break;
		case 'C' :
					key_value=NULL;
					STATE=FUNCTION_SET2_STATE;
					Function_SetPage2();
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
void Detach_Net_Func(void)
{
	switch(key_value)
	{
		case 'E' :					
					ClearLCD();
					WriteLCDData(1,2,"正在断开网络");
					WriteLCDData(3,2,"....稍等....");
					Gprs_Close();         //断开网络
					if(mc52i_ok==0x12)
					{
						WriteLCDData(3,2,"断网成功!   ");	
						gprssendmessagetiming=0;			   //清除定时
						TIM3->CR1&=~(0x01);    					//清除定时器3						
						Delay_Nms(1000);
						Delay_Nms(1000);
						STATE=FUNCTION_SET2_STATE;
						Function_SetPage2();
					}
					else
					{
						WriteLCDData(3,2,"断网失败!   ");
						Delay_Nms(1000);
						Delay_Nms(1000);
						STATE=NORMAL_STATE;
						ClearLCD();
						Normal_Page();	
					}
					key_value=NULL;
					break;
		case 'C' :
					key_value=NULL;
					STATE=FUNCTION_SET2_STATE;
					Function_SetPage2();
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
void Set_Code_Func(void)
{
	u8 i=0;
	u8 k=0;
	
	if(cursorbit<12) 
	{
		WriteCommand(0x0E);//显示ON,游标ON,游标位置OFF	
		if(cursorbit<6)
		{
			WriteCommand(0x91+cursorbit);    //从第2行的0x8a处开始显示
		}
	}
	else		
		WriteCommand( 0x0c ); 
	if((key_value>='0')&&(key_value<='9'))   
	{
	    if(cursorbit<12)
	    {
	   	    WriteData(0xa3);                  //写入一个双字符组" * "，0xa3aa
			WriteData(0xaa);	
		    set_data[cursorbit]=key_value;
		    cursorbit++;
			if(cursorbit==6)
			{
				WriteLCDData(3,1,"请再输入一次：");
				WriteCommand(0x93+cursorbit);    //从第4行的0x8a处开始显示
			}				
	    }
	    key_value=NULL;
	}
	else if(key_value=='E')  
	{
	    key_value=NULL;
	    /***********核对六位密码是否正确"***********/
		for(i=0;i<6;i++)
		{
			if((set_data[i]==set_data[i+6])&&(set_data[i]>='0')&&(set_data[i]<='9')) 
			{
				k++;
			}
		}
		/****若六位密码正确，进入设置页面0，否则重新要求输入密码****/
		if(k==6)
		{	
			STATE=FUNCTION_SET3_STATE; 		//进入功能设置状态3
			ee_code[0]=(set_data[0]-0x30)*10+(set_data[1]-0x30);  //eeprom存储
			ee_code[1]=(set_data[2]-0x30)*10+(set_data[3]-0x30);
			ee_code[2]=(set_data[4]-0x30)*10+(set_data[5]-0x30);
			AT24CXX_Write(EE_CODE,ee_code,3); 
			cursorbit=0;
			for(i=0;i<12;i++)
			{
				set_data[i]=0x20;
			} 
			ClearLCD();
			WriteLCDData(2,1,"密码设置成功！");
			Delay_Nms(1000);						//两秒停顿时间
			Delay_Nms(1000);			
			ClearLCD();
			Function_SetPage3();			  //显示设置页面3				
		}
		else							  //输入的两个密码不一样
		{ 		
			for(i=0;i<12;i++)
			{
				set_data[i]=0x20;
			}
			STATE=SET_CODE_STATE;
			cursorbit=0;
			ClearLCD();
			WriteLCDData(2,1,"输入密码不一致！");
			WriteLCDData(3,2,"请重新输入！");
			Delay_Nms(1000);						//两秒停顿时间
			Delay_Nms(1000);			
			ClearLCD();
			Set_Code_Page();	  			
		}

	}
	else if(key_value=='C') 
	{
	    key_value=NULL;
		STATE=FUNCTION_SET3_STATE;
		cursorbit=0;
	    for(i=0;i<12;i++)
		{
			set_data[i]=0x20;
		}
		ClearLCD();
		Function_SetPage3();	
	}
	else __nop();	
}


