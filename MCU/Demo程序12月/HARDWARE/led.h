#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//Mini STM32开发板
//LED驱动代码			 
//正点原子@ALIENTEK
//2010/5/27

//LED端口定义
#define LED0 PAout(8)// PA8
#define LED1 PAout(9)// PA9	
#define KEY0 PCin(0)
#define KEY1 PCin(1)
#define KEY2 PCin(2)
#define KEY3 PCin(3)


void LED_Init(void);//初始化
void LED_A8(void);
void LED_A9(void);		 				    
#endif

















