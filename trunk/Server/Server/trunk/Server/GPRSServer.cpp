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

//
//服务器管理线程
//
DWORD WINAPI CGPRSServer::AdminThread(LPVOID pParam)
{
	CGPRSServer *pGPRSServer = (CGPRSServer *) pParam;

	//通知StratServer函数，Admin线程已启动,
	SetEvent(pGPRSServer->m_evtThreadLanched);

	//等待服务停止指令，该指令在三种情况下会被触发
	//1.调用StartServer函数时，ListenThread未能正常启动
	//2.服务器ListenThread运行过程中出错
	//3.调用StopServer函数

	//阻塞m_evtSvrToStop事件对象被触发
	WaitForSingleObject(pGPRSServer->m_evtSvrToStop,INFINITE);

	//Do Clear Work
	if(pGPRSServer->m_sListen!=INVALID_SOCKET)
		closesocket(pGPRSServer->m_sListen);//将导致ListenThread的accept调用出错并结束

	if(pGPRSServer->m_hCompletion)
	{
		//通知Service线程结束
		for(UINT i=0; i<pGPRSServer->m_nSvcThreadNum;i++)
			PostQueuedCompletionStatus(pGPRSServer->m_hCompletion,0,0,NULL);
	}
	//等待所有Service线程和监听线程结束
	WaitForMultipleObjects(pGPRSServer->m_nSvcThreadNum+1,&(pGPRSServer->m_hThreadList[1]),TRUE,WAIT4THREAD_MILLISECS*2);

	DWORD nExitCode;
	for(UINT j=1;j<pGPRSServer->m_nSvcThreadNum;j++)
	{
		GetExitCodeThread(pGPRSServer->m_hThreadList[j],&nExitCode);
		if(nExitCode==STILL_ACTIVE)
			TerminateThread(pGPRSServer->m_hThreadList[j],1);
	}
	return 0;
}

//
//服务器监听线程
//
DWORD WINAPI CGPRSServer::ListenThread(LPVOID pParam)
{
	CGPRSServer *pGPRSServer=(CGPRSServer *) pParam;

	//创建I/O完成端口
	pGPRSServer->m_hCompletion=pGPRSServer->CreateNewIoCompletionPort(0);
	if(pGPRSServer->m_hCompletion==NULL)
		return -1;

	//创建多个工作线程
	for(UINT i=0;i<pGPRSServer->m_nSvcThreadNum;i++)
	{
		HANDLE hThread=CreateThread(NULL,0,ServiceThread,pGPRSServer,0,NULL);
		if(hThread == NULL)
			return -1;

		else
			pGPRSServer->m_hThreadList[i+2]=hThread;
	}

	//创建监听端口，设置SO_REUSEADDR选项为TRUE，绑定本地端口5002，并调用Listen函数进行监听
	pGPRSServer->m_sListen=socket(AF_INET,SOCK_STREAM,0);
	if(pGPRSServer->m_sListen==INVALID_SOCKET)
		return -1;

	BOOL bReuseAddr=TRUE;

	//在监听套接口上启用SO_REUSEADDR选项,为真,套接字可与一个正在被其他套接字使用的地址绑定在一起
	if(setsockopt(pGPRSServer->m_sListen,SOL_SOCKET,SO_REUSEADDR,(char *)&bReuseAddr,sizeof(bReuseAddr))==SOCKET_ERROR)
		return -1;

	SOCKADDR_IN local;
	memset(&local,0,sizeof(local));
	local.sin_family=AF_INET;
	local.sin_port=htons(5002);
	local.sin_addr.S_un.S_addr=INADDR_ANY;
	if(bind(pGPRSServer->m_sListen,(SOCKADDR *)&local,sizeof(local))==SOCKET_ERROR)
		return -1;

	if(listen(pGPRSServer->m_sListen,5)==SOCKET_ERROR)
		return -1;

	//通知StartServer函数，监听线程已启动
	SetEvent(pGPRSServer->m_evtThreadLanched);
/*
	SOCKET sockAccept;
	LPCONN_CTX lpConnCtx;
	int nResult;

#ifdef _DEMO
	SOCKADDR_IN from;
#endif
	while(1)    //循环接受外来连接，并投递初始I/O操作请求
	{
		sockAccept=accept(pGenericServer->m_sdListen,NULL,NULL);
		if(sockAccept==INVALID_SOCKET)
		{
			SetEvent(pGenericServer->m_evtSvrToStop);
			return -1;
		}
#ifdef _DEMO
		memset(&from,0,sizeof(from));
		int fromlen=sizeof(from);
		getpeername(sockAccept,(SOCKADDR *)&from,&fromlen);
		AfxGetMainWnd()->PostMessage(WM_USER_CLIENT,from.sin_addr.s_addr,CLIENT_CONN_REQ);
#endif
		if(pGenericServer->m_bSvrPaused)
		{
			closesocket(sockAccept);
#ifdef _DEMO
			AfxGetMainWnd()->PostMessage(WM_USER_CLIENT,from.sin_addr.s_addr,CLIENT_REJECT);

#endif
			continue;
		}

		lpConnCtx=pGenericServer->CreateConnCtx(sockAccept,pGenericServer->m_hIOCP);
		if(lpConnCtx==NULL)
		{
			SetEvent(pGenericServer->m_evtSvrToStop);
			return -1;
		}
		else
			pGenericServer->ConnListAdd(lpConnCtx);

#ifdef _DEMO
		AfxGetMainWnd()->PostMessage(WM_USER_CLIENT,from.sin_addr.s_addr,CLIENT_ACCEPT);
#endif
		//投递初始I/O操作
		nResult=WSARecv(sockAccept,
			&(lpConnCtx->pPerIOData->wbuf),
			1,
			NULL,
			&(lpConnCtx->pPerIOData->flags),
			&(lpConnCtx->pPerIOData->OverLapped),
			NULL
			);
		if(nResult==SOCKET_ERROR && WSAGetLastError()!=ERROR_IO_PENDING)
		{
			pGenericServer->ConnListRemove(lpConnCtx);
			continue;
		}
	}*/
	return 0;
}
DWORD WINAPI CGPRSServer::ServiceThread(LPVOID pParam)
{

	return 0;
}







