#pragma once
#include "genericserver.h"
#include "UserRecordset.h"

class CGPRSServer :
	public CGenericServer
{
public:
	CGPRSServer(void);
	~CGPRSServer(void);

	BOOL StartServer(int nPort);
	BOOL StopServer();
	BOOL LogIn(LPCIOCPContext  pContext);
	static int listen;
	static SOCKET m_sAccept;
	CDBConnection *m_conn;

protected:
	HANDLE m_evtSvrToStop;                      //���ڹ����߳�ֹͣ������
	HANDLE m_evtThreadLanched;              //����֪ͨ�߳��ѳɹ�����

	CRITICAL_SECTION m_CriticalSection;     //�����̻߳�������ٽ���Դ
	UINT m_nSvcThreadNum;                       //���ڱ������������ʱʵ�������ķ����߳���
	HANDLE m_hThreadList[MAX_THREAD+2];   //�����̵߳���Ŀ,���������̺߳ͼ����߳�
	HANDLE m_hListenThread;
	
	CIOCPContext  *pClient ;
	static DWORD WINAPI ListenThread(LPVOID pParam);
	static DWORD WINAPI ServiceThread(LPVOID pParam);
	static DWORD WINAPI AdminThread(LPVOID pParam); 

	void SendHeartBeeatMsg();
};

//extern CGPRSServer     *pGprsServer;
