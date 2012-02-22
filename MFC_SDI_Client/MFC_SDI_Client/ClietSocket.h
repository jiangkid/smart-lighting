#pragma once

#define MAX_BUF_SIZE 4096			//���պͷ��ͻ����С
#define HEADLEN (sizeof(HDR))        //��ͷ����
#define WM_TRAYICON_MSG (WM_USER+100)
#define LENTH (sizeof(ZigbeeInfo))
#define U8 unsigned char
////////��ͷ//////
typedef struct _HEADER
{
	char dataCheck[1];
	u_short dataLen;
}HDR,*LPHDR;

///////�û���Ϣ/////
typedef struct _UserInfo
{
	char UserName[10];
	char PassWord[12];
	char Idetify;
}USERINFO,*LPUSERINFO;

typedef struct _GInfo//G����Ϣ
{
	char    GName[20];
	char    GID[2];
	INT     GCurrent;
	INT     GVoltage;
	BOOL    GStatus;
}GInfo,*LPGInfo;

typedef struct _TInfo//T����Ϣ
{
	char    TName[40];
	char    TID[4];
	char    GID[2];
	INT     TCurrent;
	INT     TVoltage;
	BOOL    TStatus;
}TInfo,*LPTInfo;

typedef struct _RInfo//R����Ϣ
{
	char    RName[40];
	char    RID[6];
	char    TID[4];
	INT     RCurrent;
	INT     RVoltage;
	BOOL    RStatus;
}RInfo,*RPRInfo;

typedef struct _ZigbeeInfo//���ڵ����Ϣ
{
	U8    LID[16];
	U8    LName[50];
	bool  MainStatus;
	bool  AssistStatus;
//	BOOL    Update;
}ZigbeeInfo,*LPZigbeeInfo;

typedef struct _LInfo//���Ƶ���Ϣ
{
	char    LID[16];
}LInfo,*LPInfo;


typedef struct _NumInfo//L����Ϣ
{
	INT UserNum;
	INT GNum;
	INT TNum;
	INT RNum;
	INT LNum;
}NumInfo,*LPNumInfo;

typedef struct _IintInfo//L����Ϣ
{
	INT GNum;
	INT TNum;
	INT RNum;
	GInfo m_InitGInfo[4];
	TInfo m_InitTInfo[32];
	RInfo m_InitRInfo[255];
}IintInfo,*LPIintInfo;

typedef struct _TreeInfo//Tree����Ϣ
{
	char GID[2];
	char TID[4];
	char RID[6];
}TreeInfo,*LPTreeInfo;

typedef struct _BackInfo//back����Ϣ
{
	char BeginBuffer[2];
	char Judge[1];
	char Type[1];
	char LID[16];
	char Order[1];
	char ActiveType[1];
	char CheckData[4];
	char EndBuffer[1];
}BackInfo,*LPBackInfo;

typedef struct _ConTrlInfo//back����Ϣ
{
	U8 m_First[4];
	U8 m_ID[16];
	U8 m_OrderObject[1];
	U8 m_OrderType[1];
	U8 m_ActiveType[1];
	U8 m_CheckData[4];
	U8 m_EndBuffer[2];
}ConTrlInfo,*LPConTrlInfo;

extern HDR hdr;				//ȫ�ְ���
extern USERINFO userInfo[9];//ȫ���û���Ϣ��Ҫ��̬������취�����
extern IintInfo m_InitInfo; //��ʼ����Ϣ
extern LInfo m_LInfo;
extern bool BGTrue;
extern bool BTTrue;
extern bool BRTrue;
extern bool BLTrue;

//***************************************************************/
//����:�ж��û���¼�Ƿ�ɹ�
//***************************************************************/
void ChenkLogin(char* buff,int nRecvLength);
//***************************************************************/
//����:�ж��������û��Ƿ�ɹ�
//***************************************************************/
void ChenkSet(char* buff,int nRecvLength);
//***************************************************************/
//����:�ж��޸��û������Ƿ�ɹ�
//***************************************************************/
void ChenkModify(char* buff,int nRecvLength);
//***************************************************************/
//����:��ȡ���ڷ�������ID
//***************************************************************/
void ChenkGetGID(char* buff,int nRecvLength);
//***************************************************************/
//����:�ж��Ƿ���ӵ���ID�ɹ�
//***************************************************************/
void ChenkBGTRL(char* buff,int nRecvLength);
//***************************************************************/
//����:���ճ�ʼ����Ϣ
//***************************************************************/
void ChenkInitInfo(char* buff,int nRecvLength);
//***************************************************************/
//����:���շ�����Ϣ
//***************************************************************/
void CheckBack(unsigned char* buff,int nRecvLength);
//***************************************************************/
//����:�����û���¼��Ϣ
//***************************************************************/
void SendUserInfo(LPHDR hdr,LPUSERINFO userInfo);
//***************************************************************/
//����:���������߳�
//***************************************************************/
DWORD WINAPI ConnectThreadFunc(LPVOID pParam);
//***************************************************************/
//����:select ��װ����
//***************************************************************/
BOOL SOCKET_Select(SOCKET hSocket, int nTimeOut = 100, BOOL bRead = FALSE);
//***************************************************************/
//����:char-cstring ��װ����
//***************************************************************/
//CString CHARTOCSTring(unsigned char* str,int nLength);
//***************************************************************/
//�������ܣ���16byte��char��IDת��Ϊ8byte��char��ID
//***************************************************************/
//char* CStringTOChar(char* buffer, int Length);
//�������ܣ���16byte��char��IDת��Ϊ8byte��char��ID
//***************************************************************/
void SendContrlInfo(LPHDR hdr,LPConTrlInfo contrlInfo);
void SendContrlInfo1(LPHDR hdr,LPConTrlInfo contrlInfo);
//***************************************************************/
//�������ܣ�������4096���ļ����ղ��洢
//***************************************************************/
void UnpackLightInfo(char* buffer, int Length);
//***************************************************************/
//�������ܣ�������4096���ļ���Ϣ����
//***************************************************************/
void TranslateLInfo(U8* buffer);