#ifndef _GPRS_H_
#define _GPRS_H_
/********************************************************/
/********************************************************/
/********************************************************/
void GprsPort_Init(void);
void Ctrl_Z(void);
void AT_Command(u8  utstring[]);
void Send_ATCommand(u8 *psendatcommand,u16 sendatcommandlength); 
u8 Check_OK(void);
u8 Check_SISR(void);
u8  Check_Creg(void);
void Gprs_Command(u8  utstring[],u16 gprscommand_waittime); 
void Mc52i_Init(void);
void Csq_Detec(void);
void Gprs_Init(void);
void Gprs_Open(void);
void Gprs_Send(u8 send_gprstext[],u8 send_gprslength[]);
void HeartBeat(u8 heartbitbyte);
void Gprs_Rec(void);
void Gprs_Close(void);
/********************************************************/
#endif
