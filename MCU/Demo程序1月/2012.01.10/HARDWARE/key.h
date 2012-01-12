#ifndef _KEY_H_
#define _KEY_H_
/******************************************************************************/
void Key_Init(void);
u8 Read_Row(void);
void KeyEXTI_Init(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
/******************************************************************************/
#endif
