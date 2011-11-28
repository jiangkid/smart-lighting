#include "test.h"
#include "spi.h"

/****************************************************/
extern u8 spirec3[128];
u8 start3=0;
u8 s3;
/*************************************************/
void SPIx_Init(void)
{	 
	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ�� 	 
	RCC->APB2ENR|=1<<12;      //SPI1ʱ��ʹ�� 
		   
	//����ֻ���SPI�ڳ�ʼ��
	GPIOA->CRL&=0X000FFFFF; 
	GPIOA->CRL|=0XBBB00000;//PA5.6.7���� 
	GPIOA->ODR|=0X7<<5;    //PA5.6.7����

	GPIOA->CRL&=0XFFF0FFFF;  //CS���� PA4
	GPIOA->CRL|=0X00030000;//�������

		
	SPI1->CR1|=0<<10;//ȫ˫��ģʽ	
	SPI1->CR1|=1<<9; //���nss����
	SPI1->CR1|=1<<8;  

	SPI1->CR1|=1<<2; //SPI����
	SPI1->CR1|=0<<11;//8bit���ݸ�ʽ	
//	SPI1->CR1|=1<<1; //����ģʽ��SCKΪ1 CPOL=1
//	SPI1->CR1|=1<<0; //���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1  
	SPI1->CR1&=0xFFFC; //CPOL=0,CPHA=0;
	SPI1->CR1|=7<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR1|=1<<6; //SPI�豸ʹ��
	SPIx_ReadWriteByte(0xff);//��������		 
}
/*************************************************************/
//SPI �ٶ����ú���
//SpeedSet:
//SPI_SPEED_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_SPEED_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_SPEED_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_SPEED_256 256��Ƶ (SPI 281.25K@sys 72M)
void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI1->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//����Ƶ
	{
		SPI1->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI_SPEED_8)//�˷�Ƶ 
	{
		SPI1->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI_SPEED_16)//ʮ����Ƶ
	{
		SPI1->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
	}else			 	 //256��Ƶ
	{
		SPI1->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz ����ģʽ
	}
	SPI1->CR1|=1<<6; //SPI�豸ʹ��	  
}
/********************************************************************/
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPIx_ReadWriteByte(u8 TxData)
{															  //�ȷ���һ���Լ����ݣ����ȡһ���ֽ�����
	u8 retry=0;			 
	while((SPI1->SR&1<<1)==0)//�ȴ���������	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=TxData;	 	  //����һ��byte
	retry=0;
	delay_us(100);	   //��ʱ����
   	while((SPI1->SR&1<<0)==0) //�ȴ�������һ��byte  
	{
		retry++;
		if(retry>200)return 0;
	}					    
	return SPI1->DR;          //�����յ�������				    
}
/***************************************************
���ƣ�Board_Communicate
���ܣ�stm32��pic��zigbee֮���spiͨ��
˵����ͨ����ʱ��3��
���أ�NULL
***************************************************/
void Board_Communicate(void)
{
	u8 retry=0;
	u32 i=0;
	CS3=0;										  //   Board3	 Zigbee
	delay_us(10);
	if(start3==0)			           //�������Ӽ���������
	{
		retry=0; 
		while((retry<10)&&(s3=(SPIx_ReadWriteByte(0x36)))!=0x37)
		{	
			retry++;
			delay_ms(5);
		}
		delay_ms(5);
		s3=SPIx_ReadWriteByte(0x36);
		printf("%d",s3);				
		if(s3==0x37)
		{
			start3=1;
		}
		else 
		{
			start3=0;
		}
   						
	}
	else if(start3==1)			   //����������Чָ��
	{
		SPIx_ReadWriteByte(0x38);		  //��һ��								    
		delay_ms(5);

//		SPIx_ReadWriteByte(0x38);
//		delay_ms(2);
		for(i=0;i<128;i++)
		{
			spirec3[i]=SPIx_ReadWriteByte(0x30);
			delay_ms(2);	
		}

		for(i=0;i<128;i++)	    //��ӡ���
		{
			printf("%x",spirec3[i]);
		}
		printf("\n"); 
		 
		if(spirec3[127]==0x7F)
		{
			printf("Received Data is right!\n");
		}
		else 
		{
			printf("Received Data is wrong!\n");
			start3=0;
		}
	}
	CS3=1;
	delay_ms(1);
}
/***************************************************
���ƣ�
���ܣ�
˵����
���أ�
***************************************************/
void Zigbee_ReceiveData(void)
{
	
}
