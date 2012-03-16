#pragma once
//////////
#include "stdafx.h"
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
}GInfo,*LPGInfo;

typedef struct _TInfo//T����Ϣ
{
	char    TName[40];
	char    TID[4];
	char    GID[2];
}TInfo,*LPTInfo;

typedef struct _RInfo//R����Ϣ
{
	char    RName[40];
	char    RID[6];
	char    TID[4];
}RInfo,*RPRInfo;


typedef struct _RoadInfo//R����Ϣ
{
	char    RID[6];
	bool    MainStatus;
}RoadInfo,*LPRoadInfo;

typedef struct _ZigbeeInfo//���ڵ����Ϣ
{
	U8	  m_GID[2];
	U8	  m_GName[20];
	U8    m_TName[20];
	U8    RName[20];  
	U8    LID[16];
	U8    LName[50];
	bool  MainStatus;
	bool  AssistStatus;
	U8  Update;
	float  current;
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

typedef struct _RoadListViewInfo
{
	U8 m_TernimalName[20];
	U8 m_RoadName[20];
	U8 m_RoadID[6];
	bool m_RoadStatus;
	U8 m_Update;
	float nCurrent1;
	float nCurrent2;
	float nCurrent3;
}RoadListViewInfo, *LPRoadListViewInfo;

typedef struct _TreeInfo//Tree����Ϣ
{
	char GID[2];
	char TID[4];
	char RID[6];
}TreeInfo,*LPTreeInfo;

typedef struct _GPRSInfo//GPRS������Ϣ
{
	U8 gID[2];
	U8 gName[20];
	U8 gArea[20];//������
	U8 gTelephone[11];
	U8 gTime[17];
	U8 gLocation[20];//��װ�ص�
}GPRSInfo,*LPGPRSInfo;

typedef struct _TerminalInfo//Terminal������Ϣ
{
	U8 TID[4];
	U8 TerminalName[20];
	U8 TerminalLocation[20];//��װ�ص�
	U8 TerminalArea[20];//������
	U8 TerminalTime[17];
}TerminalInfo,*LPTerminalInfo;

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
//�������ܣ�����·�����еƵ���Ϣ����
//***************************************************************/
void TranslateLInfo(U8* buffer);
//***************************************************************/
//�������ܣ������ն�������·��Ϣ����
//***************************************************************/
void TranslateRInfo(U8* buffer,int Length);
//***************************************************************/
//�������ܣ�ˢ�����νṹ
//***************************************************************/
void ChenkConnectAgain(char* buff,int nRecvLength);
//***************************************************************/
//�������ܣ����µ��Ƶ�״̬
//***************************************************************/
void UpdataZigbeeStatusInfo(char* buff,int nRecvLength);
//***************************************************************/
//�������ܣ����µ��Ƶ�����״̬
//***************************************************************/
void UpdataZigbeeCurrentInfo(char* buff,int nRecvLength);
//***************************************************************/
//�������ܣ����µ��Ƶ�״̬
//***************************************************************/
void CheckCtrlBackInfo(char* buff,int nRecvLength);
//***************************************************************/
//�������ܣ����µ�·������״̬
//***************************************************************/
void UpdataRoadCurrentInfo(char* buff,int nRecvLength);
//***************************************************************/
//�������ܣ�1��2
//***************************************************************/
CString CharToCString(unsigned char* str, int nLength);
//***************************************************************/
//�������ܣ����µ�·��״̬
//***************************************************************/
void UpdataRoadStatusInfo(char* buff,int nRecvLength);
//***************************************************************/
//�������ܣ�GPRSIbfo��Ϣ
//***************************************************************/
void GPRSLocalInfo(char* buff,int nRecvLength);