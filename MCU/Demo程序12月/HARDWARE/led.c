#include <stm32f10x_lib.h>	   
#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/5/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved											  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PA8和PD2为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	   	 
	RCC->APB2ENR|=1<<4;    //使能PORTC时钟	    	 
	GPIOA->CRH&=0XFFFFFF00; 				 
	GPIOA->CRH|=0X00000033;//PA8 9推挽输出  
	GPIOA->ODR|=1<<8;
	GPIOA->ODR|=1<<9;

	GPIOC->CRL&=0x00FF0000; //PC01按键输入	  PC6-gprs_on Pc7-gprs_off
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





