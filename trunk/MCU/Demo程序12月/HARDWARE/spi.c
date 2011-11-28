#include "test.h"
#include "spi.h"

/****************************************************/
extern u8 spirec3[128];
u8 start3=0;
u8 s3;
/*************************************************/
void SPIx_Init(void)
{	 
	RCC->APB2ENR|=1<<2;       //PORTA时钟使能 	 
	RCC->APB2ENR|=1<<12;      //SPI1时钟使能 
		   
	//这里只针对SPI口初始化
	GPIOA->CRL&=0X000FFFFF; 
	GPIOA->CRL|=0XBBB00000;//PA5.6.7复用 
	GPIOA->ODR|=0X7<<5;    //PA5.6.7上拉

	GPIOA->CRL&=0XFFF0FFFF;  //CS控制 PA4
	GPIOA->CRL|=0X00030000;//推挽输出

		
	SPI1->CR1|=0<<10;//全双工模式	
	SPI1->CR1|=1<<9; //软件nss管理
	SPI1->CR1|=1<<8;  

	SPI1->CR1|=1<<2; //SPI主机
	SPI1->CR1|=0<<11;//8bit数据格式	
//	SPI1->CR1|=1<<1; //空闲模式下SCK为1 CPOL=1
//	SPI1->CR1|=1<<0; //数据采样从第二个时间边沿开始,CPHA=1  
	SPI1->CR1&=0xFFFC; //CPOL=0,CPHA=0;
	SPI1->CR1|=7<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR1|=1<<6; //SPI设备使能
	SPIx_ReadWriteByte(0xff);//启动传输		 
}
/*************************************************************/
//SPI 速度设置函数
//SpeedSet:
//SPI_SPEED_2   2分频   (SPI 36M@sys 72M)
//SPI_SPEED_8   8分频   (SPI 9M@sys 72M)
//SPI_SPEED_16  16分频  (SPI 4.5M@sys 72M)
//SPI_SPEED_256 256分频 (SPI 281.25K@sys 72M)
void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI1->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//二分频
	{
		SPI1->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI_SPEED_8)//八分频 
	{
		SPI1->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI_SPEED_16)//十六分频
	{
		SPI1->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
	}else			 	 //256分频
	{
		SPI1->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz 低速模式
	}
	SPI1->CR1|=1<<6; //SPI设备使能	  
}
/********************************************************************/
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPIx_ReadWriteByte(u8 TxData)
{															  //先发送一个自己数据，后读取一个字节数据
	u8 retry=0;			 
	while((SPI1->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=TxData;	 	  //发送一个byte
	retry=0;
	delay_us(100);	   //延时试下
   	while((SPI1->SR&1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>200)return 0;
	}					    
	return SPI1->DR;          //返回收到的数据				    
}
/***************************************************
名称：Board_Communicate
功能：stm32与pic，zigbee之间的spi通信
说明：通过定时器3，
返回：NULL
***************************************************/
void Board_Communicate(void)
{
	u8 retry=0;
	u32 i=0;
	CS3=0;										  //   Board3	 Zigbee
	delay_us(10);
	if(start3==0)			           //初次连接检测连接情况
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
	else if(start3==1)			   //后续发送有效指令
	{
		SPIx_ReadWriteByte(0x38);		  //第一个								    
		delay_ms(5);

//		SPIx_ReadWriteByte(0x38);
//		delay_ms(2);
		for(i=0;i<128;i++)
		{
			spirec3[i]=SPIx_ReadWriteByte(0x30);
			delay_ms(2);	
		}

		for(i=0;i<128;i++)	    //打印输出
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
名称：
功能：
说明：
返回：
***************************************************/
void Zigbee_ReceiveData(void)
{
	
}
