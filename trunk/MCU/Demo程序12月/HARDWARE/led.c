#include <stm32f10x_lib.h>	   
#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/5/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved											  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PA8��PD2Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	   	 
	RCC->APB2ENR|=1<<4;    //ʹ��PORTCʱ��	    	 
	GPIOA->CRH&=0XFFFFFF00; 				 
	GPIOA->CRH|=0X00000033;//PA8 9�������  
	GPIOA->ODR|=1<<8;
	GPIOA->ODR|=1<<9;

	GPIOC->CRL&=0x00FF0000; //PC01��������	  PC6-gprs_on Pc7-gprs_off
	GPIOC->CRL|=0x33008888; 
	
	GPIOC->ODR|=1<<6;
	GPIOC->ODR|=1<<7;											 
}
void LED_A8(void)
{
	LED0=1;
	delay_ms(200);
	LED0=0;
	delay_ms(200);
}

void LED_A9(void)
{
	LED1=1;
	delay_ms(500);
	LED1=0;
	delay_ms(500);
}





