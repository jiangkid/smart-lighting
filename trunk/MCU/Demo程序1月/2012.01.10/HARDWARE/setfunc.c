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
���ܣ�
������
���أ�
˵����
****************************************************************/
void Input_Code_Func(void)
{
	u8 i=0;
	u8 k=0;
	u8 icode[6];
	WriteCommand(0x89+cursorbit);    //�ӵ����е�0x8a����ʼ��ʾ
	if(cursorbit<6) 
		WriteCommand(0x0E);//��ʾON,�α�ON,�α�λ��OFF
	else		
		WriteCommand( 0x0c ); 
	if((key_value>='0')&&(key_value<='9'))   
	{
	    if(cursorbit<6)
	    {
	   	    WriteData(0xa3);                  //д��һ��˫�ַ���" * "��0xa3aa
			WriteData(0xaa);	
		    set_data[cursorbit]=key_value;
		    cursorbit++;				
	    }
	    key_value=NULL;
	}
	else if(key_value=='E')  
	{
	    key_value=NULL;
	    /***********�˶���λ�����Ƿ���ȷ"***********/
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
				set_data[i]=0x20;	//���ԭ�Ⱥ���
			}
		}
		/****����λ������ȷ����������ҳ��0����������Ҫ����������****/
		if(k==6)
		{
			STATE=FUNCTION_SET1_STATE; 		//���빦������״̬0
			Function_SetPage1();			  //��ʾ����ҳ��0
			cursorbit=0;	
		}
		else
		{	
			ClearLCD();
			WriteLCDData(1,2,"�����������");
			WriteLCDData(3,2,"���������룡");
			Delay_Nms(1000);
			Delay_Nms(1000);
			STATE=INPUT_CODE_STATE;//������������״̬
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
			set_data[i]=0x20;	//���ԭ�Ⱥ���
		}		
	}
	else __nop();
}	
/****************************************************************
���ƣ�
���ܣ�
˵����idֵҪС��300
���أ�
****************************************************************/
void Set_GID_Func(void)
{
	if(cursorbit<3)
		WriteCommand(0x0E);  //������ʾ���α�ON���α�λ������
	else 
		WriteCommand(0x0C);	 //�α�OFF
	if((key_value>='0')&&(key_value<='9'))
	{
		if(cursorbit<3)
		{
			if(cursorbit==0)		   //1		 
			{
				if(key_value<'3')			//	ҪС��3
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
		AT24CXX_WriteOneByte(EE_GID,ee_gid);   //eeprom�洢
		printf("set gid:%d\n",ee_gid);             //����鿴��ʾ
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
���ܣ�
������
���أ�
˵����
****************************************************************/
void Set_TID_Func(void)
{
	if(cursorbit<3)
		WriteCommand(0x0E);  //������ʾ���α�ON���α�λ������
	else 
		WriteCommand(0x0C);	 //�α�OFF
	if((key_value>='0')&&(key_value<='9'))
	{
		if(cursorbit<3)
		{
			if(cursorbit==0)		   //1		 
			{
				if(key_value<'3')			//	ҪС��3
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
		AT24CXX_WriteOneByte(EE_TID,ee_tid);   //eeprom�洢
		printf("set tid:%d\n",ee_tid);             //����鿴��ʾ
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
���ƣ�
���ܣ�
˵����
���أ�
****************************************************************/
void Set_Date_Func(void)
{
	u32 year;
	u8 month;
	u8 date;

	if(cursorbit<8)
		WriteCommand(0x0E);  //������ʾ���α�ON���α�λ������
	else 
		WriteCommand(0x0C);	 //�α�OFF
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
			else if(cursorbit==4)			//month	  ��λֻ����0����1
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
			else if(cursorbit==5)				  	    //��λΪ1ʱ��λֻ����012
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
				if(month!=2)					   //��ͨ�·ݣ���ĸ�λ������0123
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
					if(key_value<'3')			  //2�·ݵ�������λֻ����012
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
					if((month==4)||(month==6)||(month==9)||(month==11))   											//С��ֻ��30��
					{
						if(((set_data[6]=='3')&&(key_value>'0'))||((set_data[6]=='0')&&(key_value=='0')))			  //����Ϊ0�����ܴ���30
						{
							__nop();	
						}
						else
						{
							WriteCommand(0x9B);
							WriteData(0xa3);
							WriteData(0x80+key_value);
							WriteData(' ');		        //������������һλ��ʾ������
							set_data[7]=key_value;
							cursorbit++;	
						}
					}
					else 
					{
						if(((set_data[6]=='3')&&(key_value>'1'))||((set_data[6]=='0')&&(key_value=='0')))	 	//����ֻ��31�� 
						{																						 //������31������Ϊ0
							__nop();
						}
						else
						{
							WriteCommand(0x9B);
							WriteData(0xa3);
							WriteData(0x80+key_value);
							WriteData(' ');		        //������������һλ��ʾ������
							set_data[7]=key_value;
							cursorbit++;	
						}
					}
				}
				else
				{
					if(((year%100)!=0)&&(year%4==0))    //����
					{
						WriteCommand(0x9B);
						WriteData(0xa3);
						WriteData(0x80+key_value);
						WriteData(' ');		        //������������һλ��ʾ������
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
							WriteData(' ');		        //������������һλ��ʾ������
							set_data[7]=key_value;
							cursorbit++;	
						}
					}
				}

			}	
		}
		key_value=NULL; //�������	
	}
	else if((key_value=='E')&&(cursorbit==8))
	{
		key_value=NULL;
 		year=(set_data[0]-0x30)*1000+(set_data[1]-0x30)*100+(set_data[2]-0x30)*10+(set_data[3]-0x30);
		month=(set_data[4]-0x30)*10+(set_data[5]-0x30);
		date=(set_data[6]-0x30)*10+(set_data[7]-0x30);

		RTC_Set(year,month,date,timer.hour,timer.min,timer.sec);  //����������

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
���ƣ�
���ܣ�
˵����
���أ�
****************************************************************/
void Set_Time_Func(void)
{
	u8 hour;
	u8 min;
	u8 sec;
	if(cursorbit<6)
		WriteCommand(0x0E);  //������ʾ���α�ON���α�λ������
	else 
		WriteCommand(0x0C);	 //�α�OFF
	if((key_value>='0')&&(key_value<='9'))
	{
		if(cursorbit<6)	
		{
			if(cursorbit==0)
			{
				if(key_value<'3')	    //Сʱ�ĸ�λҪС��3
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
			else if(cursorbit==1)			  //Сʱ�ĵ�λ
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
			else if(cursorbit==2)     //���ӵĸ�λ
			{
				if(key_value<'6')	    //���ӵĸ�λҪС��6�� 00-59
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
			else if(cursorbit==3)//���ӵ�λ����ֵ
			{
				WriteCommand(0x8B);
				WriteData(0xa3);
				WriteData(0x80+key_value);
				WriteCommand(0x9A);
				set_data[3]=key_value;
				cursorbit++;	
			}
			else if(cursorbit==4)     //���ӵĸ�λ
			{
				if(key_value<'6')	    //��ĸ�λҪС��6�� 00-59
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
			else if(cursorbit==5)//���λ����ֵ
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
	else if((key_value=='E')&&(cursorbit==6))	    //����ʱ�����ȷ����������
	{
		key_value=NULL;
		hour=(set_data[0]-0x30)*10+(set_data[1]-0x30);
		min= (set_data[2]-0x30)*10+(set_data[3]-0x30);
		sec= (set_data[4]-0x30)*10+(set_data[5]-0x30);
		RTC_Set(timer.w_year,timer.w_month,timer.w_date,hour,min,sec);  //����ʱ����

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
���ƣ�
���ܣ�
˵����255.255.255:9999
���أ�
****************************************************************/
void Set_IP_Func(void)
{
	if((key_value>='0')&&(key_value<='9'))
	{
		if(cursorbit<16)
		{
			if(cursorbit==0)					    //P1.1
			{	
				if(key_value<'3')		 				//ҪС��3
				{
					WriteCommand(0x88);
					WriteData(key_value);
					set_data[0]=key_value;
					cursorbit++;
				}
			}
			else if(cursorbit==1)		 			  //P1.2
			{
				if(set_data[0]<'2')	   					//P1.1��'0'����'1'
				{
					WriteCommand(0x88);
					WriteData(set_data[0]);
					WriteData(key_value);
					set_data[1]=key_value;
					cursorbit++;
				}
				else if((set_data[0]=='2')&&(key_value<'6'))  //P1.1��'2'
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
				if(set_data[0]<'2')						   //P1.1��'0'����'1'
				{
					WriteCommand(0x89);
					WriteData(key_value);
					set_data[2]=key_value; 	
					cursorbit++;
				}
				else if(set_data[0]=='2')					 //P1.1��'2'
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
			else if(cursorbit==3)	    //P2.1ҪС�ڡ�3��
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
				else if((set_data[3]=='2')&&(key_value<'6'))	   //P2.1��'2'
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
				else if(set_data[3]=='2')	   //P2.1��'2'
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
			else if(cursorbit==6)	    //P3.1ҪС�ڡ�3��
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
				else if((set_data[6]=='2')&&(key_value<'6'))	   //P3.1��'2'
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
				else if(set_data[6]=='2')	   //P3.1��'2'
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
			else if(cursorbit==9)	    //P4.1ҪС�ڡ�3��
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
				else if((set_data[9]=='2')&&(key_value<'6'))	   //P4.1��'2'
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
				else if(set_data[9]=='2')	   //P4.1��'2'
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
		AT24CXX_Write(EE_IP,ee_ip,6);  //eeprom�洢
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
���ܣ�
������
���أ�
˵����
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
			AT24CXX_Write(EE_CT,ee_ct,4);  //eeprom�洢
			printf("set ct:%s\n",ee_ct);             //����鿴��ʾ
			ClearLCD();
			WriteLCDData(2,2,"CTֵ���óɹ�");
			Delay_Nms(1000);
			Delay_Nms(1000);
			STATE=FUNCTION_SET2_STATE;
			Function_SetPage2();
			cursorbit=0;
		}
		else
		{
			ClearLCD();
			WriteLCDData(2,2,"CTֵ����ʧ��");
			WriteLCDData(3,3,"��������");
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
���ܣ�
������
���أ�
˵����
****************************************************************/
void Connect_Net_Func(void)
{
	switch(key_value)
	{
		case 'E' :
					ClearLCD();
					mc52i_ok=0x12;				//ǿ������
					WriteLCDData(1,2,"������������");
					WriteLCDData(3,2,"....�Ե�....");
					Gprs_Init();	        //grps init
					if(mc52i_ok==0x12)
					{
						WriteLCDData(3,2,"���ӳɹ�!   ");
						Gprs_FirstMessage(); //���η���GPRS��Ϣ������������Ϣ
						gprssendmessagetiming=0;			   //�����ʱ
						TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3����ʱ������Ϣ
						Delay_Nms(1000);
						Delay_Nms(1000);
						STATE=FUNCTION_SET2_STATE;
						Function_SetPage2();
					}
					else
					{
						WriteLCDData(3,2,"����ʧ��!   ");
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
���ܣ�
������
���أ�
˵����
****************************************************************/
void Detach_Net_Func(void)
{
	switch(key_value)
	{
		case 'E' :					
					ClearLCD();
					WriteLCDData(1,2,"���ڶϿ�����");
					WriteLCDData(3,2,"....�Ե�....");
					Gprs_Close();         //�Ͽ�����
					if(mc52i_ok==0x12)
					{
						WriteLCDData(3,2,"�����ɹ�!   ");	
						gprssendmessagetiming=0;			   //�����ʱ
						TIM3->CR1&=~(0x01);    					//�����ʱ��3						
						Delay_Nms(1000);
						Delay_Nms(1000);
						STATE=FUNCTION_SET2_STATE;
						Function_SetPage2();
					}
					else
					{
						WriteLCDData(3,2,"����ʧ��!   ");
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
���ܣ�
������
���أ�
˵����
****************************************************************/
void Set_Code_Func(void)
{
	u8 i=0;
	u8 k=0;
	
	if(cursorbit<12) 
	{
		WriteCommand(0x0E);//��ʾON,�α�ON,�α�λ��OFF	
		if(cursorbit<6)
		{
			WriteCommand(0x91+cursorbit);    //�ӵ�2�е�0x8a����ʼ��ʾ
		}
	}
	else		
		WriteCommand( 0x0c ); 
	if((key_value>='0')&&(key_value<='9'))   
	{
	    if(cursorbit<12)
	    {
	   	    WriteData(0xa3);                  //д��һ��˫�ַ���" * "��0xa3aa
			WriteData(0xaa);	
		    set_data[cursorbit]=key_value;
		    cursorbit++;
			if(cursorbit==6)
			{
				WriteLCDData(3,1,"��������һ�Σ�");
				WriteCommand(0x93+cursorbit);    //�ӵ�4�е�0x8a����ʼ��ʾ
			}				
	    }
	    key_value=NULL;
	}
	else if(key_value=='E')  
	{
	    key_value=NULL;
	    /***********�˶���λ�����Ƿ���ȷ"***********/
		for(i=0;i<6;i++)
		{
			if((set_data[i]==set_data[i+6])&&(set_data[i]>='0')&&(set_data[i]<='9')) 
			{
				k++;
			}
		}
		/****����λ������ȷ����������ҳ��0����������Ҫ����������****/
		if(k==6)
		{	
			STATE=FUNCTION_SET3_STATE; 		//���빦������״̬3
			ee_code[0]=(set_data[0]-0x30)*10+(set_data[1]-0x30);  //eeprom�洢
			ee_code[1]=(set_data[2]-0x30)*10+(set_data[3]-0x30);
			ee_code[2]=(set_data[4]-0x30)*10+(set_data[5]-0x30);
			AT24CXX_Write(EE_CODE,ee_code,3); 
			cursorbit=0;
			for(i=0;i<12;i++)
			{
				set_data[i]=0x20;
			} 
			ClearLCD();
			WriteLCDData(2,1,"�������óɹ���");
			Delay_Nms(1000);						//����ͣ��ʱ��
			Delay_Nms(1000);			
			ClearLCD();
			Function_SetPage3();			  //��ʾ����ҳ��3				
		}
		else							  //������������벻һ��
		{ 		
			for(i=0;i<12;i++)
			{
				set_data[i]=0x20;
			}
			STATE=SET_CODE_STATE;
			cursorbit=0;
			ClearLCD();
			WriteLCDData(2,1,"�������벻һ�£�");
			WriteLCDData(3,2,"���������룡");
			Delay_Nms(1000);						//����ͣ��ʱ��
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


