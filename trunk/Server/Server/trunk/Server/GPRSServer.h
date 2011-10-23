///////////////////////////////////////////
//Created at 2011/11/23 by ����
///////////////////////////////////////////

#pragma once
#include "genericserver.h"

#define WAIT4THREAD_MILLISECS 3000         //��ʾ�߳�A�ڵȴ��߳�B����ĳ�¼�����ʱ�����ֻ�ȴ�3����

class CGPRSServer :
	public CGenericServer
{
public:
	CGPRSServer(void);
	~CGPRSServer(void);

	BOOL StratServer();
	BOOL StopServer();

protected:
	HANDLE m_evtSvrToStop;                      //���ڹ����߳�ֹͣ������
	HANDLE m_evtThreadLanched;              //����֪ͨ�߳��ѳɹ�����

	CRITICAL_SECTION m_CriticalSection;     //�����̻߳�������ٽ���Դ
	UINT m_nSvcThreadNum;                       //���ڱ������������ʱʵ�������ķ����߳���
	HANDLE m_hThreadList[MAX_SERVICETHREAD_NUM+2];   //�����̵߳���Ŀ,���������̺߳ͼ����߳�

	static DWORD WINAPI ListenThread(LPVOID pParam);
	static DWORD WINAPI ServiceThread(LPVOID pParam);
	static DWORD WINAPI AdminThread(LPVOID pParam);

};

extern CGenericServer     *g_pGenericServer;
