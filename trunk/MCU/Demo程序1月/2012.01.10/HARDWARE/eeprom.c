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
���ƣ�
���ܣ�
˵����
���أ�
****************************************************************/
void IIC_Init(void)
{					     
 	RCC->APB2ENR|=1<<2;//��ʹ������IO PORTAʱ�� 							 
	GPIOA->CRL&=0XFFFFFF00;//PA0,1 �������
	GPIOA->CRL|=0X00000033;	   
	GPIOA->ODR|=3<<0;     //PA0,1 �����
}
/****************************************************************
���ƣ�
���ܣ�
˵����
���أ�
****************************************************************/
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	Delay_Nus(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delay_Nus(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
/****************************************************************
���ƣ�
���ܣ�
˵����
���أ�
****************************************************************/
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delay_Nus(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	Delay_Nus(4);							   	
}
/****************************************************************
���ƣ�
���ܣ�
˵����
���أ�1������Ӧ��ʧ�� 0������Ӧ��ɹ�
****************************************************************/       
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
/****************************************************************
���ƣ�
���ܣ�
˵����
���أ�
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
���ƣ�
���ܣ�
˵����
���أ�
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
���ƣ�IIC����һ���ֽ�
���ܣ����شӻ�����Ӧ��
˵����
���أ�1����Ӧ�� 0����Ӧ��	
****************************************************************/					 				     		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		Delay_Nus(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		Delay_Nus(2); 
		IIC_SCL=0;	
		Delay_Nus(2);
    }	 
} 
/****************************************************************
���ƣ���1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK 
���ܣ�
˵����
���أ�
****************************************************************/	      
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}
/****************************************************************
���ƣ���AT24CXXָ����ַ����һ������
���ܣ�
˵����ReadAddr:��ʼ�����ĵ�ַ 
���أ�����������
****************************************************************/
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}
/****************************************************************
���ƣ���AT24CXXָ����ַд��һ������
���ܣ�
˵����WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ  DataToWrite:Ҫд�������
���أ�
****************************************************************/
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	Delay_Nms(10);	 
}
/****************************************************************
���ƣ���AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
���ܣ��ú�������д��16bit����32bit������.
˵����WriteAddr  :��ʼд��ĵ�ַ  DataToWrite:���������׵�ַ Len:Ҫд�����ݵĳ���2,4
���أ�
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
���ƣ���AT24CXX�����ָ����ַ��ʼ��������ΪLen������
���ܣ��ú������ڶ���16bit����32bit������.
˵����ReadAddr   :��ʼ�����ĵ�ַ  Len        :Ҫ�������ݵĳ���2,4
���أ�����
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
���ƣ�
���ܣ����AT24CXX�Ƿ�����
˵������������24XX�����һ����ַ(255)���洢��־��. ���������24Cϵ��,�����ַҪ�޸�
���أ�����1:���ʧ��   ����0:���ɹ�
****************************************************************/
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}
/****************************************************************
���ƣ�
���ܣ���AT24CXX�����ָ����ַ��ʼ����ָ������������
˵����NumToRead:Ҫ�������ݵĸ���  pBuffer  :���������׵�ַ	ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
���أ�
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
���ƣ�
���ܣ���AT24CXX�����ָ����ַ��ʼд��ָ������������
˵����pBuffer   :���������׵�ַ  	NumToWrite:Ҫд�����ݵĸ���		WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
���أ�
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
���ƣ�
���ܣ�
˵����
���أ�
****************************************************************/
void AT24CXX_Init(void)
{
	IIC_Init();
	ee_gid=AT24CXX_ReadOneByte(EE_GID);	    //��ȡEEPROM�е�GID����
	printf("the gid is=%d\n",ee_gid);
	ee_tid=AT24CXX_ReadOneByte(EE_TID);	    //��ȡEEPROM�е�TID����
	printf("the tid is=%d\n",ee_tid);
	AT24CXX_Read(EE_IP,ee_ip,6);			//��ȡEEPROM�е�IP����
	printf("the ip is=%s\n",ee_ip);
	AT24CXX_Read(EE_CODE,ee_code,3);		//��ȡEEPROM�е�code����
	if(ee_code[0]>99)
	{
		ee_code[0]=12;
		ee_code[1]=34; 
		ee_code[2]=56;
	}
	AT24CXX_Read(EE_CT,ee_ct,4);  			//��ȡEEPROM�е�CT��ֵ
	if(ee_ct[0]>10)
	{
		ee_ct[0]=0;
		ee_ct[1]=20;
		ee_ct[2]=0;
		ee_ct[3]=20;
	}
	printf("the ct is=%s\n",ee_ct);
}
