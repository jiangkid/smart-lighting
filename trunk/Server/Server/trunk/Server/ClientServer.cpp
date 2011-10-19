#include "StdAfx.h"
#include "ClientServer.h"

CClientServer::CClientServer(void)
{
}

CClientServer::~CClientServer(void)
{
}
/*****开始服务：状态变量初始化、socket创建与绑定、完成端口创建与关联*****/
BOOL CClientServer::Start(int nPort, int nMaxConnections, int nMaxFreeBuffers, int nMaxFreeContexts, int nInitialReads)
{
	if(m_bServerStarted)
	{
		return FALSE;
	}
	/*****参数设置*****/
	m_nPort = nPort;
	m_nMaxConnections = nMaxConnections;
	m_nMaxFreeBuffers = nMaxFreeBuffers;
	m_nMaxFreeContexts = nMaxFreeContexts;
	m_nInitialReads = nInitialReads;
	/*****状态设置*****/
	m_bServerStarted = TRUE;
	m_bShutDown = FALSE;
	/*****套接字创建，绑定到本地端口，进入监听模式*****/
	m_sListen = ::WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	sockaddr_in addrLocal;
	addrLocal.sin_family = AF_INET;
	addrLocal.sin_addr.S_un.S_addr = INADDR_ANY;
	addrLocal.sin_port=::htons(m_nPort);
	if(::bind(m_sListen,(sockaddr*)&addrLocal,sizeof(addrLocal)) == SOCKET_ERROR)
	{
		m_bServerStarted = FALSE;
		return FALSE;
	}
	::listen(m_sListen,300);
	/*****创建完成端口，关联*****/
	m_hCompletion = CreateIoCompletionPort ( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	::CreateIoCompletionPort(&m_sListen,m_hCompletion,(DWORD)0,0);
	/*****注册事件*****/
	::WSAEventSelect(  m_sListen,  m_hAcceptEvent, FD_ACCEPT);
	//创建监听线程
	HANDLE	hThread=CreateThread(NULL,0,_ListenThreadProc,this,0,NULL);
	return TRUE;
}

DWORD WINAPI  CClientServer::_ListenThreadProc(LPVOID lpParam)
{
	CClientServer *pThis = (CClientServer *)lpParam;
	//先在监听套接字上投递几个AcceptI/O
	CIOCPBuffer *pBuffer;
	for(int i=0;i<pThis->m_nInitialAccepts;i++)//m_nInitialAccepts
	{
		pBuffer = pThis->AllocateBuffer(BUFFER_SIZE);
		if(pBuffer == NULL)
		{
			return -1;
		}
		pThis->InsertPendingAccept(pBuffer);// 将一个I/O缓冲区对象插入到m_pPendingAccepts表中
		pThis->PostAccept(pBuffer);				 //投递IO
	}	
	//构建事件对象数组，以便在上面调用WSAWaitForMutipleEvents函数
	HANDLE   hWaitEvents[2+MAX_THREAD];
	int nEventCount = 0;
	hWaitEvents[nEventCount++] = pThis->m_hAcceptEvent;
	hWaitEvents[nEventCount++] = pThis->m_hRepostEvent;
	//创建指定数量的工作线程在完成端口上处理IO
	for(int i=0;i<MAX_THREAD;i++)
	{
		hWaitEvents[nEventCount++]  = ::CreateThread(NULL,0,_WorkerThreadProc, pThis,0,NULL);
	}
	//下面进入无线循环，处理事件对象数组中的事件
	while(TRUE)
	{
		int nIndex = ::WSAWaitForMultipleEvents(nEventCount,hWaitEvents,FALSE,60*1000,FALSE);
		//首先检查是否要停止服务
		if(pThis->m_bShutDown || nIndex == WSA_WAIT_FAILED)
		{
			//关闭所有连接
			pThis->CloseAllConnection();
			::Sleep(0);
			//关闭套接字
			::closesocket(pThis->m_sListen);
			pThis->m_sListen = INVALID_SOCKET;
			::Sleep(0);
			//通知所有的IO处理线程退出
			for(int i=2;i<MAX_THREAD+2;++i)
			{

			}
		}
	}


	return TRUE;
}

DWORD WINAPI  CClientServer::_WorkerThreadProc(LPVOID lpParam)
{



	return TRUE;
}