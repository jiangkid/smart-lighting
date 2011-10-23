//////////////////////////////////////////////////
//Created at 2011/11/23 by 丁亮
//////////////////////////////////////////////////

#include "StdAfx.h"
#include "GPRSServer.h"
#include "InitSock.h"
#include <process.h>

CGenericServer     _GenericServer;
CGenericServer     *g_pGenericServer = &_GenericServer;

CGPRSServer::CGPRSServer(void)
{
	m_evtThreadLanched=CreateEvent(NULL,FALSE,FALSE,NULL);
	m_evtSvrToStop=CreateEvent(NULL,FALSE,FALSE,NULL);

	//初始化临界区对象
	InitializeCriticalSection(&m_CriticalSection);

	//将sysInfo.dwNumberOfProcessors*2和MAX_THREAD_NUM之间的较小值赋给m_nSvcThreadNum;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	m_nSvcThreadNum=sysInfo.dwNumberOfProcessors*2 < MAX_THREAD ? (sysInfo.dwNumberOfProcessors*2):MAX_THREAD;
}

CGPRSServer::~CGPRSServer(void)
{
	CloseHandle(m_evtThreadLanched);
	CloseHandle(m_evtSvrToStop);
	DeleteCriticalSection(&m_CriticalSection);
}

//
//开启服务器
//
BOOL CGPRSServer::StratServer()
{
	if (TRUE==m_bServerStarted)
	{
		return FALSE;
	}

	//初始化 Winsock库
	CInitSock theSock;

	//启动管理线程,该线程负责服务器的关闭
	HANDLE hThread=CreateThread(NULL,0,AdminThread,this,0,NULL);
	if(hThread==NULL)
	{
		WSACleanup();
		return FALSE;
	}
	if(WaitForSingleObject(m_evtThreadLanched,WAIT4THREAD_MILLISECS)!=WAIT_OBJECT_0)
	{
		TerminateThread(hThread,1);
		CloseHandle(hThread);
		WSACleanup();
		return FALSE;
	}

	//将确定已正常启动的管理线程句柄保存到m_hThreadList[0]
	m_hThreadList[0]=hThread;

	//启动监听线程                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	ResetEvent(m_evtThreadLanched);

	//创建监听线程
	hThread=CreateThread(NULL,0,ListenThread,this,0,NULL);
	if(hThread==NULL)
	{
		//通知已处于运行中的管理线程终止服务
		SetEvent(m_evtSvrToStop);
		//等待管理线程结束
		if(WaitForSingleObject(m_hThreadList[0],WAIT4THREAD_MILLISECS*3)!=WAIT_OBJECT_0)
			TerminateThread(m_hThreadList[0],1);

		CloseHandle(m_hThreadList[0]);
		WSACleanup();

		return FALSE;
	}

	//等待监听线程正常结束
	if(WaitForSingleObject(m_evtThreadLanched,WAIT4THREAD_MILLISECS)!=WAIT_OBJECT_0)
	{
		TerminateThread(hThread,1);
		CloseHandle(hThread);

		//通知管理线程结束
		SetEvent(m_evtSvrToStop); 
		if(WaitForSingleObject(m_hThreadList[0],WAIT4THREAD_MILLISECS*3)!=WAIT_OBJECT_0)
			TerminateThread(m_hThreadList[0],1);
		CloseHandle(m_hThreadList[0]);
		WSACleanup();
		return FALSE;
	}

	//将已正常启动的监听线程句柄保存到m_hThreadList[1]
	m_hThreadList[1]=hThread;

	//更改服务器状态值m_ServerState为SERVER_RUNNING
	m_bServerStarted=TRUE;

	return TRUE;
}

//
//停止服务器
//
BOOL CGPRSServer::StopServer()
{
	if(m_bServerStarted==FALSE)
		return FALSE;

	//触发事件对象m_evtSvrToStop，通知管理线程终止服务器
	SetEvent(m_evtSvrToStop);

	//等待管理线程结束
	WaitForSingleObject(m_hThreadList[0],WAIT4THREAD_MILLISECS*3);

	//检查管理线程状态，如果该线程仍处于运行状态，则将其强行终止
	DWORD nExitCode;
	GetExitCodeThread(m_hThreadList[0],&nExitCode);
	if(nExitCode==STILL_ACTIVE)
	{
		TerminateThread(m_hThreadList[0],1);
	}

	WSACleanup();

	return TRUE;
}




