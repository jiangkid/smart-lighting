#pragma once
#include "genericserver.h"
//#include "ServerDlg.h"

#define GET_USERNAME	0x29
#define GET_GPRSID		0x30
#define GET_TERMINALID	0x31
#define GET_ROADID		0x32
#define GET_LIGHTID		0x33
#define BIND_GPRSID		0x39
#define BIND_TERMINALID 0x38
#define BIND_ROADID		0x37
#define BIND_LIGHTID	0x36


class CClientServer :public CGenericServer
{
public:
	CClientServer(void);
	~CClientServer(void);
	/*****��ʼ����״̬������ʼ����socket������󶨡���ɶ˿ڴ��������*****/
	BOOL Start(int nPort);
	BOOL StopServer();

	/****ͨ��Э�����ݴ�����****/
	CString  FieldSet(CHAR * buffer);
	BOOL LogIn(LPCIOCPContext  pContext);   //��¼��⺯��
	CString DataCheck(CHAR * buffer);   //��������������ж�
	BOOL CreateUser(CHAR * buffer);
	BOOL DeleteUser(CHAR * buffer);
	CString  GetLedStatus(CHAR* buffer);
	BOOL ChangePassword(CHAR* buffer);
	BOOL InitializeClient(CHAR* buffer,CString &x,CString &y,CString &z);
	BOOL AddID(char *buffer);

	HANDLE m_evtSvrToStop;                      //���ڹ����߳�ֹͣ������
	HANDLE m_evtThreadLanched;              //����֪ͨ�߳��ѳɹ�����
	HANDLE m_hListenThread;

	CDBConnection *m_conn;						 //���ݿ������
	
	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);
	static DWORD WINAPI _AdminThread(LPVOID pParam);
	
	SOCKET m_sAccept;
	CIOCPContext  *pClient ;

protected:
	static int listen;
};

//extern CClientServer *ClientServer;