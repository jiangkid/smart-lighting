#pragma once

#define MAX_BUF_SIZE 4096			//���պͷ��ͻ����С
#define HEADLEN (sizeof(HDR))        //��ͷ����
#define WM_TRAYICON_MSG (WM_USER+100)

typedef struct _HEADER
{
	char dataCheck;
	u_short dataLen;
}HDR,*LPHDR;

typedef struct _UserInfo
{
	char UserName[10];
	char PassWord[12];
	char Idetify;
}USERINFO,*LPUSERINFO;

typedef struct _GInfo//G����Ϣ
{
	CString GName;
	UINT    nTnum;
	char    GID[2];
	INT     GCurrent;
	INT     GVoltage;
	BOOL    GStatus;
}GInfo,*LPGInfo;

typedef struct _TInfo//T����Ϣ
{
	CString TName;
	UINT    uRnum;
	char    TID[4];
	INT     TCurrent;
	INT     TVoltage;
	BOOL    TStatus;
}TInfo,*LPTInfo;

typedef struct _RInfo//R����Ϣ
{
	CString RName;
	UINT    uLnum;
	char    RID[6];
	INT     RCurrent;
	INT     RVoltage;
	BOOL    RStatus;
}RInfo,*RPRInfo;

typedef struct _LInfo//L����Ϣ
{
	CString LName;
	char    LID[16];
	INT     LCurrent;
	INT     LVoltage;
	BOOL    LStatus;
}LInfo,*LPLInfo;

typedef struct _NumInfo//L����Ϣ
{
	INT UserNum;
	INT GNum;
	INT TNum;
	INT RNum;
	INT LNum;
}NumInfo,*LPNumInfo;

extern HDR hdr;
extern USERINFO userInfo[9];
extern bool BGTrue;
extern bool BTTrue;
extern bool BRTrue;
extern bool BLTrue;

void ChenkLogin(char* buff,int nRecvLength);
void ChenkSet(char* buff,int nRecvLength);
void ChenkModify(char* buff,int nRecvLength);
void ChenkGetGID(char* buff,int nRecvLength);
void ChenkBGTRL(char* buff,int nRecvLength);
void ChenkInitInfo(char* buff,int nRecvLength);
void SendUserInfo(LPHDR hdr,LPUSERINFO userInfo);
DWORD WINAPI ConnectThreadFunc(LPVOID pParam);
BOOL SOCKET_Select(SOCKET hSocket, int nTimeOut = 100, BOOL bRead = FALSE);
