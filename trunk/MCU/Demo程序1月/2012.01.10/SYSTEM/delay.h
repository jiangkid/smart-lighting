#ifndef _DELAY_H_
#define _DELAY_H_ 			   
#include <stm32f10x_lib.h>
/******************************************************************************/
void Delay_Init(u8 SYSCLK);
void Delay_Nms(u16 nms);
void Delay_Nus(u32 nus);
/******************************************************************************/
#endif





























