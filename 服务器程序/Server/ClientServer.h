#pragma once
#include "genericserver.h"
//#include "ServerDlg.h"

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
	
	HANDLE m_evtSvrToStop;                      //���ڹ����߳�ֹͣ������
	HANDLE m_evtThreadLanched;              //����֪ͨ�߳��ѳɹ�����
	HANDLE m_hListenThread;

	CDBConnection *m_conn;						 //���ݿ������
	
	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);
	static DWORD WINAPI _AdminThread(LPVOID pParam);
	
	SOCKET m_sAccept;
	CIOCPContext  *pClient ;
};

//extern CClientServer *ClientServer;