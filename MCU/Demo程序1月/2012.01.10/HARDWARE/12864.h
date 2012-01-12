#ifndef _12864_H_
#define _12864_H_
/******************************************************************************/
#define LCDLINE_LENGTH					4					
/******************************************************************************/
void WriteBit(u8 bBit);
void WriteByte(u8 Wbyte);
void WriteCommand(u8 Cbyte);
void WriteData(u8 Dbyte);
void LCDInit(void);
void LCDClear(void);
void ClearLCD(void);
void WriteLCDData(u8 bLine,u8 bNum,u8 bString[]);
void LCDPort_Init(void);
/******************************************************************************/
#endif
