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
	HANDLE m_evtSvrToStop;                      //用于管理线程停止服务器
	HANDLE m_evtThreadLanched;              //用于通知线程已成功启动

	CRITICAL_SECTION m_CriticalSection;     //用于线程互斥访问临界资源
	UINT m_nSvcThreadNum;                       //用于保存服务器运行时实际启动的服务线程数
	HANDLE m_hThreadList[MAX_THREAD+2];   //服务线程的数目,包括管理线程和监听线程
	HANDLE m_hListenThread;
	
	CIOCPContext  *pClient ;
	static DWORD WINAPI ListenThread(LPVOID pParam);
	static DWORD WINAPI ServiceThread(LPVOID pParam);
	static DWORD WINAPI AdminThread(LPVOID pParam); 

	void SendHeartBeeatMsg();
};

//extern CGPRSServer     *pGprsServer;
