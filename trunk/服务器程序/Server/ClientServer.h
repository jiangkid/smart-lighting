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
	CString DataCheck(CHAR * buffer,DWORD length);   //��������������ж�
	BOOL CreateUser(CHAR * buffer);
	BOOL DeleteUser(CHAR * buffer);
	CString  GetLedStatus(CHAR* buffer);
	BOOL ChangePassword(CHAR* buffer);
	BOOL InitializeClient(CHAR* buffer,CString &x,CString &y,CString &z);
	BOOL AddID(unsigned long int &buffer);

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