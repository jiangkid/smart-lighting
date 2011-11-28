#ifndef __GPRS_H
#define __GPRS_H
/********************************************************/
#define GPRS_ON   PCout(6)
#define GPRS_OFF  PCout(7)
/********************************************************/
void USART2_Init(u32 pclk2,u32 bound);
void Clear_U2_Rbuf(void);
void Usart2_Send_Byte(u8 u2tbyte);
void Ctrl_Z(void);
void AT_Command(u8  utstring[]); 
void Check_OK(void);
u8 Check_SISW(void);
u8 Check_SISR(void);
u8  Check_Creg(void);
void Gprs_Command(u8  utstring[],u16 gprscommand_waittime); 
void Mc52i_Init(void);
void Csq_Detec(void);
void Gprs_Init(void);
void Gprs_Open(void);
void Gprs_Send(u8 send_gprs_text[],u8 send_gprs_length[]);
void HeartBeat(void); 
void Gprs_Close(void);
void Gprs_Rec(void);
void Gprs_ReceiveDataID(void);
void Gprs_ReceiveDataPoint(void);
void Gprs_ReceiveDataState(u8 zigbeecommand);
/********************************************************/
#endif
