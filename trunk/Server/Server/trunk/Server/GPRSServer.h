///////////////////////////////////////////
//Created at 2011/11/23 by 丁亮
///////////////////////////////////////////

#pragma once
#include "genericserver.h"

#define WAIT4THREAD_MILLISECS 3000         //表示线程A在等待线程B触发某事件对象时，最多只等待3秒钟

class CGPRSServer :
	public CGenericServer
{
public:
	CGPRSServer(void);
	~CGPRSServer(void);

	BOOL StratServer();
	BOOL StopServer();

protected:
	HANDLE m_evtSvrToStop;                      //用于管理线程停止服务器
	HANDLE m_evtThreadLanched;              //用于通知线程已成功启动

	CRITICAL_SECTION m_CriticalSection;     //用于线程互斥访问临界资源
	UINT m_nSvcThreadNum;                       //用于保存服务器运行时实际启动的服务线程数
	HANDLE m_hThreadList[MAX_SERVICETHREAD_NUM+2];   //服务线程的数目,包括管理线程和监听线程

	static DWORD WINAPI ListenThread(LPVOID pParam);
	static DWORD WINAPI ServiceThread(LPVOID pParam);
	static DWORD WINAPI AdminThread(LPVOID pParam);

};

extern CGenericServer     *g_pGenericServer;
