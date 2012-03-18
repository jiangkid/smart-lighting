#pragma once
#include "DBConnection.h"
#include "AdminRecordset.h"
#include "AdminCommand.h"
#include "AreaRecordset.h"
#include "AreaCommand.h"
#include "TerminalCommand.h"
#include "LightCommand.h"
#include "RoadCommand.h"
#include "DBCommand.h"
#include "DBRecordset.h"
#include "HRLightCommand.h"
#include "HRLightRecordset.h"
#include "HRRoadCommand.h"
#include "HRRoadRecordset.h"
#include "HRTerminalCommand.h"
#include "HRTerminalRecordset.h"
#include "LightRecordset.h"
#include "RoadRecordset.h"
#include "TerminalRecordset.h"
#include "UserRecordset.h"
#include "UserCommand.h"

#define BUFFER_SIZE		1024*4    //I/O
#define MAX_THREAD      4         //
#define WAIT4THREAD_MILLISECS 3000         //��ʾ�߳�A�ڵȴ��߳�B����ĳ�¼�����ʱ�����ֻ�ȴ�3����
#define QMAXSize  100
#define HeadLen   sizeof(HDR)
#define U8  unsigned char


typedef enum
{
	SERVER_RUNNING,SERVER_STOP
}ServerState;
/******�������ݵı�ͷ�ṹ��******/
typedef struct _HEADER
{
	char dataCheck;
	u_short dataLen;
}HDR,*LPHDR;
/******�û���Ϣ******/
typedef struct _UserInfo
{
	char UserName[10];
	char PassWord[12];
	char Idetify;
}USERINFO,*LPUSERINFO;
/******��Ϣ�ṹ��******/
typedef struct _ThreadMessage
{
	char sData[BUFFER_SIZE];
	struct _ThreadMessage *pnext;//���ڶ��еĽ����ڵ�
	int  strlen;
}ThreadMessage,*LPThreadMessage;

/**********��Ϣ����*************/
typedef struct _Qlist
{
	LPThreadMessage front;
	LPThreadMessage rear;
}Qlist,*LPQlist;

//ö�����ͣ�����ָʾ������I/O����������
typedef enum _IO_OPER
{
	SVR_IO_READ,
	SVR_IO_WRITE,
}IO_OPER,*LPIO_OPER;

//////////////////////////////////////////////
//�ṹ�幦�ܣ������ײ������ṹ����Ϣ//
/////////////////////////////////////////////
typedef struct _ConTrlInfo
{
	U8 m_First[4];
	U8 m_ID[16];
	U8 m_OrderObject[1];
	U8 m_OrderType[1];
	U8 m_ActiveType[1];
	U8 m_CheckData[4];
	U8 m_EndBuffer[2];
}ConTrlInfo,*LPConTrlInfo;
//////////////////////////////////////////////
//�ṹ�幦�ܣ�����ĵ�GPRS������//
/////////////////////////////////////////////
typedef struct _GPRSSendInfo
{
	U8 m_First[4];
	U8 m_ID[8];
	U8 m_OrderObject[1];
	U8 m_OrderType[1];
	U8 m_ActiveType[1];
	U8 m_CheckData[4];
	U8 m_EndBuffer[2];
}GPRSSendInfo,*LPGPRSSendInfo;
//////////////////////////////////////////////
//�ṹ�幦�ܣ����յ�GPRS������//
/////////////////////////////////////////////
typedef struct _GPRSRecvInfo
{
	U8 m_First[4];
	U8 m_ID[8];
	U8 m_OrderObject[1];
	U8 m_OrderType[1];
	U8 m_ActiveType[1];
	U8 m_Data[4];
	U8 m_EndBuffer[2];
}GPRSRecvInfo,*LPGPRSRecvInfo;

//��չ�ص��ṹ��
typedef struct _OverLappedEx
{
	OVERLAPPED OverLapped;
	WSABUF     wbuf;                              //I/O���������ݶ���
	char  data[BUFFER_SIZE];           //ʵ�ʵ����ݻ�����
	IO_OPER    oper;                                //���ڱ�־I/O���������ͣ�IO_OPERö���ͣ�������SVR_IO_READ������SVR_IO_WRITE     
	DWORD      flags;                               //�����趨���߷���I/O�����ı�־
}CIOCPBuffer,*LPCIOCPBuffer;

//��ɼ��ṹ�壬��������ݣ���Ӧÿ�������׽ӿڡ�ÿ������
typedef struct _CONN_CTX
{
	SOCKET            sockAccept;                 //�����ӵķ������˷����׽ӿ�
	SOCKADDR_IN       addrAccept;
	LPCIOCPBuffer     pPerIOData;              //ָ������ӵ�I/O������Ϣ
	struct            _CONN_CTX *pPrec;              //�����γɷ�������ǰ����������Ϣ��˫������
	struct            _CONN_CTX *pNext;              //�ֱ�ָ�������е�ǰһ���ڵ�ͺ�һ���ڵ�
	BOOL			  LogIn;
	BOOL			  ID;
	char			  Identify[10];
	_CONN_CTX() {pPerIOData = (LPCIOCPBuffer)malloc(sizeof(CIOCPBuffer));};
	~_CONN_CTX(){free(pPerIOData);};
}CIOCPContext,*LPCIOCPContext;

class CGenericServer
{
public:
	CGenericServer(void);
	~CGenericServer(void);	
//	CServerDB   ServerDB;
	CString GetPswByAdminName(CString AdminName);

	/*****ֹͣ����*****/
	//virtual void Shutdown();
	/*****�ر�һ�����Ӻ͹ر���������*****/
	void CloseAllConnection();
	/*****ȡ�õ�ǰ���ӵ�����*****/
	ULONG GetCurrentConnection();
	/*************************����*************************/
	/*****������ͷ��׽���������*****/
	CIOCPContext *AllocateContext(SOCKET s, SOCKADDR_IN  addraccept);
	void ReleaseContext(CIOCPContext *pContext);
    void ResetAll();
	/*****�������б������һ������******/
	void  ConnListAdd(LPCIOCPContext lpConnCtx);
	void  ConnListRemove(LPCIOCPContext  lpConnCtx);
	void  ConnListClear();
	//����I/O��ɶ˿�
	HANDLE CreateNewIoCompletionPort(DWORD dwNumberOfConcurrentThreads);
	//����I/O��ɶ˿�
	BOOL AssociateWithIoCompletionPort(HANDLE hComPort,HANDLE hDevice,DWORD dwCompKey);
	BOOL Initialize_Socket(int nPort);//��ʼ�������׽���
	void InitializeBuffer(LPCIOCPBuffer pBuffer, IO_OPER  operation);
	/*����: ��˫�����в���Ԫ��Identify ,���ҵ���Ӧ�������ĵ�ַ*/
	LPCIOCPContext  LocateElement(char * identify);
	LPCIOCPContext LocateGPRSElement(char * identify,  int length);
	static BOOL InitQueue();
	BOOL DestroyQueue(LPQlist Qlist);
	BOOL InQueue(LPQlist Qlist,CHAR * buffer,int lenth);
	char* OutQueue(LPQlist Qlist,int &length);
	char* Translation_ID(char* buffer, int Length);
	U8 *GPRSTranslationID(U8 * buffer,int length);
	char *CharToCString(UCHAR* str, int nLength);//��8byte��char��IDת��Ϊ16byte��char��ID
	static LPQlist  QlistCtG; //�ͻ��˵�GPRS�Ķ���
	static LPQlist  QlistGtC; //GPRS���ͻ��˵Ķ���
	
	bool CommandRecord(char *Command,  char *name, int length);//constconst�����¼���������û����͵��������txt			
	void ShowMessage(char *buffer, int length, CString name, int ID);		//��ʾ����

protected:	
	/*************************����*************************/
	/*****��¼���нṹ��Ϣ ******/
	CIOCPBuffer *m_pFreeBufferList;
	CIOCPContext *m_pFreeContextList;
	LPCIOCPContext m_ptrConnCtxHead;                                     //˫���������ڱ������������������Ϣ
	LPThreadMessage  Messagebuffer;

	CString  strSend;    //���͵��ַ���
	CString strRecv;    //���յ��ַ���

	int m_nFreeBufferCount;
	int m_nFreeContextCount;

	CRITICAL_SECTION m_FreeContextListLock;
	CRITICAL_SECTION  m_ConnectionListLock;						// m_ConnectionListLock;
	CRITICAL_SECTION  TMSection;
	/*****��¼�����б�******/
	CIOCPContext *m_pConnectionList;
	int m_nCurrentConnection;
	int  m_nPort;

	static BOOL AdminLogin;				//����Ա��½��־λ

	HANDLE m_hThreadList[MAX_THREAD+2];		//�߳�����
	UINT m_nSvrThreadNum;										//���ڱ������������ʱʵ�������ķ����߳���
	HANDLE m_hCompletion;
	SOCKET m_sListen;
	sockaddr_in addrLocal;

	BOOL m_bShutDown;				                 //����֪ͨ�����߳��˳�
	BOOL m_bServerStarted;		                 //��¼�������Ƿ�����	
	ServerState m_ServerState;                     //���ڱ���������ĵ�ǰ״̬

	CDBCommand  DBCommand;
	CAdminCommand AdminCommand;
	CAdminRecordset		AdminRecordset;
	CAreaRecordset  AreaRecordset;
	CDBRecordset  DBRecordset;
	CHRLightCommand  HRLightCommand;
	CHRLightRecordset  HRLightRecordset;
	CHRRoadCommand  HRRoadCommand;
	CHRRoadRecordset  HRRoadRecordset;
	CHRTerminalCommand  HRTerminalCommand;
	CHRTerminalRecordset  HRTerminalRecordset;
	CLightRecordset  LightRecordset;
	CRoadRecordset  RoadRecordset;
	CTerminalRecordset  TerminalRecordset;
	CUserRecordset  UserRecordset;
	CUserCommand UserCommand;
	CAreaCommand  AreaCommand;
	CTerminalCommand   TerminalCommand;
	CLightCommand  LightCommand;
	CRoadCommand  RoadCommand;


};
extern DWORD  mThreadID1;

