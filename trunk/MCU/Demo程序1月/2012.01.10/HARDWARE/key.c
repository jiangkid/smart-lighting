#include "test.h"
#include "key.h"
#include "delay.h"
/******************************************************************************/
extern u8 key_value;
/******************************************************************************/
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void Key_Init(void)
{
	RCC->APB2ENR|=1<<4;      //ʹ��PORTCʱ��
  	GPIOC->CRL&=0X00000000; 
//	GPIOC->CRL|=0X88888888;  //PC0-PC7 ����������
//	GPIOC->ODR|=0x000000F0;   //�����������룬������������
	GPIOC->CRL|=0X33338888;  //PC0-PC3 ���룬PC4-PC7���
	GPIOC->ODR|=0x000000F0;   //�����������룬�����������
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
u8 Read_Row(void)
{
	u8 c=0;
	u32 temp;
	GPIOC->CRL&=0x00000000;
	GPIOC->CRL|=0x88883333;
	GPIOC->ODR|=0x000000F0;
	temp=(GPIOC->IDR&0x000000F0)>>4;
	if(temp==0x0000000E) 
		c=1;
	else if(temp==0x0000000D) 
		c=2;
	else if(temp==0x0000000B) 
		c=3;
	else if(temp==0x00000007) 
		c=4;
	else 
		c=0;
	Key_Init();
	return c;
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void KeyEXTI_Init(void)
{
	Ex_NVIC_Config(GPIO_C,0,RTIR);  //�����ش���
	Ex_NVIC_Config(GPIO_C,1,RTIR);
	Ex_NVIC_Config(GPIO_C,2,RTIR);
	Ex_NVIC_Config(GPIO_C,3,RTIR);

	MY_NVIC_Init(3,2,EXTI0_IRQChannel,2);	   //��ռ2�������ȼ�2����2
	MY_NVIC_Init(3,2,EXTI1_IRQChannel,2);
	MY_NVIC_Init(3,2,EXTI2_IRQChannel,2);
	MY_NVIC_Init(3,2,EXTI3_IRQChannel,2);
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void EXTI0_IRQHandler(void)
{
	u8 key0=0;
	EXTI->IMR&=0xFFFFFFF1;//  ����line 0�ϵ��ж�
	Delay_Nms(10);
	key0=Read_Row();
	if(key0>0)
	{
		switch(key0)
		{
			case 1 :
					key_value='1';
					break;
			case 2 :
					key_value='2';
					break;
			case 3 :
					key_value='3';
					break;
			case 4 :
					key_value='U';
					break;
			default :
					break; 
		}
	}
	EXTI->PR=1<<0;
	EXTI->IMR|=0x0000000F;//  ����line �ϵ��ж�
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void EXTI1_IRQHandler(void)
{
	u8 key1=0;
	EXTI->IMR&=0xFFFFFFF2;//  ����line 1�ϵ��ж�
	Delay_Nms(10);
	key1=Read_Row();
	if(key1>0)
	{
		switch(key1)
		{
			case 1 :
					key_value='4';
					break;
			case 2 :
					key_value='5';
					break;
			case 3 :
					key_value='6';
					break;
			case 4 :
					key_value='D';
					break;
			default :
					break; 
		}
	}
	EXTI->PR=1<<1;
	EXTI->IMR|=0x0000000F;//  ����line �ϵ��ж�
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void EXTI2_IRQHandler(void)
{
	u8 key2=0;
	EXTI->IMR&=0xFFFFFFF4;//  ����line 2�ϵ��ж�
 	Delay_Nms(10);
	key2=Read_Row();
	if(key2>0)
	{
		switch(key2)
		{
			case 1 :
					key_value='7';
					break;
			case 2 :
					key_value='8';
					break;
			case 3 :
					key_value='9';
					break;
			case 4 :
					key_value='S';
					break;
			default :
					break; 
		}
	}
	EXTI->PR=1<<2;
	EXTI->IMR|=0x0000000F;//  ����line �ϵ��ж�
}
/****************************************************************
���ܣ�
������
���أ�
˵����
****************************************************************/
void EXTI3_IRQHandler(void)
{
	u8 key3=0;
	EXTI->IMR&=0xFFFFFFF8;//  ����line 3�ϵ��ж�
	Delay_Nms(10);
	key3=Read_Row();
	if(key3>0)
	{
		switch(key3)
		{
			case 1 :
					key_value='C';
					break;
			case 2 :
					key_value='0';
					break;
			case 3 :
					key_value='L';
					break;
			case 4 :
					key_value='E';
					break;
			default :
					break; 
		}
	}
	EXTI->PR=1<<3;
	EXTI->IMR|=0x0000000F;//  ����line �ϵ��ж�
}
