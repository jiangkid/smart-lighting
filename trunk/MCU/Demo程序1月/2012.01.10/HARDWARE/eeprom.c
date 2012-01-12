#include "test.h"
#include "eeprom.h"
#include "delay.h"
#include "usart.h"
/******************************************************************************/
extern u8 ee_gid;
extern u8 ee_tid;
extern u8 ee_ip[EEIP_LENGTH];
extern u8 ee_code[EECODE_LENGTH];
extern u8 ee_ct[EECT_LENGTH];
/******************************************************************************/
/****************************************************************
名称：
功能：
说明：
返回：
****************************************************************/
void IIC_Init(void)
{					     
 	RCC->APB2ENR|=1<<2;//先使能外设IO PORTA时钟 							 
	GPIOA->CRL&=0XFFFFFF00;//PA0,1 推挽输出
	GPIOA->CRL|=0X00000033;	   
	GPIOA->ODR|=3<<0;     //PA0,1 输出高
}
/****************************************************************
名称：
功能：
说明：
返回：
****************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	Delay_Nus(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delay_Nus(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
/****************************************************************
名称：
功能：
说明：
返回：
****************************************************************/
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delay_Nus(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	Delay_Nus(4);							   	
}
/****************************************************************
名称：
功能：
说明：
返回：1，接收应答失败 0，接收应答成功
****************************************************************/       
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;
	Delay_Nus(1);	   
	IIC_SCL=1;
	Delay_Nus(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
/****************************************************************
名称：
功能：
说明：
返回：
****************************************************************/
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	Delay_Nus(2);
	IIC_SCL=1;
	Delay_Nus(2);
	IIC_SCL=0;
}
/****************************************************************
名称：
功能：
说明：
返回：
****************************************************************/	    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	Delay_Nus(2);
	IIC_SCL=1;
	Delay_Nus(2);
	IIC_SCL=0;
}
/****************************************************************
名称：IIC发送一个字节
功能：返回从机有无应答
说明：
返回：1，有应答 0，无应答	
****************************************************************/					 				     		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		Delay_Nus(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		Delay_Nus(2); 
		IIC_SCL=0;	
		Delay_Nus(2);
    }	 
} 
/****************************************************************
名称：读1个字节，ack=1时，发送ACK，ack=0，发送nACK 
功能：
说明：
返回：
****************************************************************/	      
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        Delay_Nus(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		Delay_Nus(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
/****************************************************************
名称：在AT24CXX指定地址读出一个数据
功能：
说明：ReadAddr:开始读数的地址 
返回：读到的数据
****************************************************************/
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}
/****************************************************************
名称：在AT24CXX指定地址写入一个数据
功能：
说明：WriteAddr  :写入数据的目的地址  DataToWrite:要写入的数据
返回：
****************************************************************/
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	Delay_Nms(10);	 
}
/****************************************************************
名称：在AT24CXX里面的指定地址开始写入长度为Len的数据
功能：该函数用于写入16bit或者32bit的数据.
说明：WriteAddr  :开始写入的地址  DataToWrite:数据数组首地址 Len:要写入数据的长度2,4
返回：
****************************************************************/
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}
/****************************************************************
名称：在AT24CXX里面的指定地址开始读出长度为Len的数据
功能：该函数用于读出16bit或者32bit的数据.
说明：ReadAddr   :开始读出的地址  Len        :要读出数据的长度2,4
返回：数据
****************************************************************/
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
/****************************************************************
名称：
功能：检查AT24CXX是否正常
说明：这里用了24XX的最后一个地址(255)来存储标志字. 如果用其他24C系列,这个地址要修改
返回：返回1:检测失败   返回0:检测成功
****************************************************************/
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}
/****************************************************************
名称：
功能：在AT24CXX里面的指定地址开始读出指定个数的数据
说明：NumToRead:要读出数据的个数  pBuffer  :数据数组首地址	ReadAddr :开始读出的地址 对24c02为0~255
返回：
****************************************************************/
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
} 
/****************************************************************
名称：
功能：在AT24CXX里面的指定地址开始写入指定个数的数据
说明：pBuffer   :数据数组首地址  	NumToWrite:要写入数据的个数		WriteAddr :开始写入的地址 对24c02为0~255
返回：
****************************************************************/ 
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
/****************************************************************
名称：
功能：
说明：
返回：
****************************************************************/
void AT24CXX_Init(void)
{
	IIC_Init();
	ee_gid=AT24CXX_ReadOneByte(EE_GID);	    //获取EEPROM中的GID号码
	printf("the gid is=%d\n",ee_gid);
	ee_tid=AT24CXX_ReadOneByte(EE_TID);	    //获取EEPROM中的TID号码
	printf("the tid is=%d\n",ee_tid);
	AT24CXX_Read(EE_IP,ee_ip,6);			//获取EEPROM中的IP号码
	printf("the ip is=%s\n",ee_ip);
	AT24CXX_Read(EE_CODE,ee_code,3);		//获取EEPROM中的code号码
	if(ee_code[0]>99)
	{
		ee_code[0]=12;
		ee_code[1]=34; 
		ee_code[2]=56;
	}
	AT24CXX_Read(EE_CT,ee_ct,4);  			//获取EEPROM中的CT比值
	if(ee_ct[0]>10)
	{
		ee_ct[0]=0;
		ee_ct[1]=20;
		ee_ct[2]=0;
		ee_ct[3]=20;
	}
	printf("the ct is=%s\n",ee_ct);
}
