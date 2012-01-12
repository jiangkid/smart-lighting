#ifndef _USART1_H_
#define _USART1_H_
/******************************************************************************/
void Usart1_SendByte(u8 utbyte);
void Usart1_SendString(u8 utstring[]);
void Usart2_Init(u32 pclk2,u32 bound);
void Usart2_SendByte(u8 u2tbyte);
void Clear_U2rxbuf(void);
void Usart3_Init(u32 pclk1,u32 bound);
void Usart3_SendByte(u8 u3tbyte);
void Usart3_Sendstring(u8 *pusart3send,u16 usart3sendlength);
void Clear_U3rxbuf(void);
/******************************************************************************/
#endif
