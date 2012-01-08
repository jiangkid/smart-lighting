#pragma once

#define MAX_BUF_SIZE 4096			//接收和发送缓存大小
#define HEADLEN (sizeof(HDR))        //包头长度
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

typedef struct _GInfo//G的信息
{
	char    GName[20];
	char    GID[2];
	INT     GCurrent;
	INT     GVoltage;
	BOOL    GStatus;
}GInfo,*LPGInfo;

typedef struct _TInfo//T的信息
{
	char    TName[40];
	char    GID[2];
	char    TID[4];
	INT     TCurrent;
	INT     TVoltage;
	BOOL    TStatus;
}TInfo,*LPTInfo;

typedef struct _RInfo//R的信息
{
	char    RName[40];
	char    TID[4];
	char    RID[6];
	INT     RCurrent;
	INT     RVoltage;
	BOOL    RStatus;
}RInfo,*RPRInfo;

typedef struct _LInfo//L的信息
{
	char    LName[80];
	char    LID[16];
	INT     LCurrent;
	INT     LVoltage;
	BOOL    LStatus;
}LInfo,*LPLInfo;

typedef struct _NumInfo//L的信息
{
	INT UserNum;
	INT GNum;
	INT TNum;
	INT RNum;
	INT LNum;
}NumInfo,*LPNumInfo;

typedef struct _IintInfo//L的信息
{
	INT GNum;
	INT TNum;
	INT RNum;
	GInfo m_InitGInfo[4];
	TInfo m_InitTInfo[32];
	RInfo m_InitRInfo[255];
}IintInfo,*LPIintInfo;

extern HDR hdr;				//全局包体
extern USERINFO userInfo[9];//全局用户信息（要动态，在想办法解决）
extern IintInfo m_InitInfo; //初始化信息
extern bool BGTrue;
extern bool BTTrue;
extern bool BRTrue;
extern bool BLTrue;

/************************************************************************************
功能:判断用户登录是否成功
*************************************************************************************/
void ChenkLogin(char* buff,int nRecvLength);
/************************************************************************************
功能:判断设置新用户是否成功
*************************************************************************************/
void ChenkSet(char* buff,int nRecvLength);
/************************************************************************************
功能:判断修改用户密码是否成功
*************************************************************************************/
void ChenkModify(char* buff,int nRecvLength);
/************************************************************************************
功能:获取存在服务器的ID
*************************************************************************************/
void ChenkGetGID(char* buff,int nRecvLength);
/************************************************************************************
功能:判断是否添加单纯ID成功
*************************************************************************************/
void ChenkBGTRL(char* buff,int nRecvLength);
/************************************************************************************
功能:接收初始化信息
*************************************************************************************/
void ChenkInitInfo(char* buff,int nRecvLength);
/************************************************************************************
功能:发送用户登录信息
*************************************************************************************/
void SendUserInfo(LPHDR hdr,LPUSERINFO userInfo);
/************************************************************************************
功能:开启连接线程
*************************************************************************************/
DWORD WINAPI ConnectThreadFunc(LPVOID pParam);
/************************************************************************************
功能:select 封装函数
*************************************************************************************/
BOOL SOCKET_Select(SOCKET hSocket, int nTimeOut = 100, BOOL bRead = FALSE);
///************************************************************************************
//功能:char-cstring 封装函数
//*************************************************************************************/
//CString CHARTOCSTring(unsigned char* str,int nLength);
///************************************************************************************
//函数功能：将16byte的char型ID转换为8byte的char型ID
//*************************************************************************************/
//char* CStringTOChar(char* buffer, int Length);

