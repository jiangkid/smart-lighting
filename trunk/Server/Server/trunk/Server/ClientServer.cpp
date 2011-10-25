#include "StdAfx.h"
#include "ClientServer.h"
#include "ServerDlg.h"
CClientServer  _ClientServer;
CClientServer *ClientServer = &_ClientServer;
int y;
CClientServer::CClientServer(void)
{
}

CClientServer::~CClientServer(void)
{
}
/*****开始服务：状态变量初始化、socket创建与绑定、完成端口创建与关联*****/
BOOL CClientServer::Start(int nPort, int nMaxConnections, int nMaxFreeBuffers, int nMaxFreeContexts, int nInitialAccepts, int nInitialReads)
{
	if(m_bServerStarted)
	{
		return TRUE;
	}
	/*****参数设置*****/
	m_nPort = nPort;
	m_nMaxConnections = nMaxConnections;
	m_nMaxFreeBuffers = nMaxFreeBuffers;
	m_nMaxFreeContexts = nMaxFreeContexts;
	m_nInitialAccepts = nInitialAccepts;
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
	//加载扩展函数AcceptEx
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	DWORD dwBytes;
	::WSAIoctl(m_sListen,
				SIO_GET_EXTENSION_FUNCTION_POINTER,
				&GuidAcceptEx,sizeof(GuidAcceptEx),
				&m_lpfnAcceptEx,
				sizeof(m_lpfnAcceptEx),
				&dwBytes,
				NULL,
				NULL);
	//加载扩展函数GetAcceptExSockaddrs,该函数用来从AcceptEx取得数据
	GUID GuidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
	::WSAIoctl(m_sListen,
				SIO_GET_EXTENSION_FUNCTION_POINTER,
				&GuidGetAcceptExSockaddrs,
				sizeof(GuidGetAcceptExSockaddrs),
				&m_lpfnGetAcceptExSockaddrs,
				sizeof(m_lpfnGetAcceptExSockaddrs),
				&dwBytes,
				NULL,
				NULL);
	/*****创建完成端口，关联*****/
	m_hCompletion = CreateIoCompletionPort ( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	::CreateIoCompletionPort((HANDLE)m_sListen,m_hCompletion,(DWORD)0,0);
	/*****注册事件*****/
	::WSAEventSelect(m_sListen,  m_hAcceptEvent, FD_ACCEPT);
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
		int nIndex = ::WSAWaitForMultipleEvents(nEventCount,hWaitEvents,FALSE,300*1000,FALSE);
		//首先检查是否要停止服务
		if(pThis->m_bShutDown || nIndex == WSA_WAIT_FAILED)
		{
			::MessageBox(NULL,"用户通知退出或事件出错！","监听线程",MB_OK);
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
				::PostQueuedCompletionStatus(pThis->m_hCompletion,-1,0,NULL);
			}
			//等待IO处理线程退出
			::WaitForMultipleObjects(MAX_THREAD,&hWaitEvents[2],TRUE,5*1000);
			for (int i=2;i<MAX_THREAD+2;i++)
			{
				::CloseHandle(hWaitEvents[i]);
			}
			::CloseHandle(pThis->m_hCompletion);
			pThis->FreeBuffers();
			pThis->FreeContext();
			::ExitThread(0);
		}
		//1)定时检查所有未返回的AcceptEx IO 的连接建立了多少时间
		if (nIndex == WSA_WAIT_TIMEOUT)
		{
			::MessageBox(NULL,"事件等待超时！","提示",MB_OK);
			printf("WSA_WAIT_TIMEOUT is d%", ++y);
			pBuffer = pThis->m_pPendingAccepts;
			while (pBuffer != NULL)
			{
				int nSeconds;
				int nLen = sizeof(nSeconds);
				::getsockopt(pBuffer->sClient,SOL_SOCKET,SO_CONNECT_TIME,(char*)&nSeconds,&nLen);
				if (nSeconds!= -1 && nSeconds > 10*60)
				{
					::MessageBox(NULL,"关闭客户端的连接！","提示",MB_OK);
					closesocket(pBuffer->sClient);
					pBuffer->sClient = INVALID_SOCKET;
				}
				pBuffer = pBuffer->pNext;
			}
		}
		else
		{
			nIndex = nIndex - WAIT_OBJECT_0;
			WSANETWORKEVENTS Netevent;
			int nLimit = 0;
			if (nIndex = 0)   // 1 )m_hAcceptEvent事件对象受信，说明投递的Accept请求不够，需要增加
			{
				::WSAEnumNetworkEvents(pThis->m_sListen,hWaitEvents[nIndex],&Netevent);
				if (Netevent.lNetworkEvents & FD_ACCEPT)
				{
					nLimit = 50;
				}
			}
			else if(nIndex = 1) // 2）m_hAcceptEvent事件对象受信，说明投递的Accept请求不够，需要增加
			{
				nLimit = InterlockedExchange(&pThis->m_nRepostCount,0);
			}
			else if (nIndex > 1)
			{
				pThis->m_bShutDown = TRUE;
			}
			int i = 0;
			while (i++<nLimit && pThis->m_nPendingAcceptCount < pThis->m_nMaxAccepts)
			{
				pBuffer = pThis->AllocateBuffer(BUFFER_SIZE);
				if(pBuffer != NULL)
				{
					pThis->InsertPendingAccept(pBuffer);
					pThis->PostAccept(pBuffer);
				}
			}
		}
	}
	return TRUE;
}

DWORD WINAPI  CClientServer::_WorkerThreadProc(LPVOID lpParam)
{
	CClientServer *pThis = (CClientServer*)lpParam;
	CIOCPBuffer *pBuffer;
	DWORD dwKey;
	DWORD dwTrans;
	LPOVERLAPPED lpoverlapped;
	::MessageBox(NULL,"进入工作线程！","提示",MB_OK);
	while (TRUE)
	{
		//在关联到此完成端口的所有套接字上等待IO完成
		BOOL bOK=::GetQueuedCompletionStatus(pThis->m_hCompletion,&dwTrans,(LPDWORD)&dwKey,
			                           (LPOVERLAPPED*)&lpoverlapped,WSA_INFINITE);
		if (dwTrans==-1)//用户通知退出
		{
			::MessageBox(NULL,"退出工作线程！","提示",MB_OK);
			::ExitThread(0);			
		}
		::MessageBox(NULL,"获得接受请求！","提示",MB_OK);
		pBuffer = CONTAINING_RECORD(lpoverlapped,CIOCPBuffer,overlapped);
		int nError = NO_ERROR;
		if (!bOK)
		{
			::MessageBox(NULL,"等待完成端口出错！","提示",MB_OK);
			SOCKET s;
			if (pBuffer->nOperation == OP_ACCEPT)
			{
				s = pThis->m_sListen;
				::MessageBox(NULL,"等待完成端口出错--接受出错！","提示",MB_OK);
			}
			else
			{
				s = ((CIOCPContext*)dwKey)->s;
			}
			DWORD dwFlags = 0;
			if (!::WSAGetOverlappedResult(s,&pBuffer->overlapped,&dwTrans,FALSE,&dwFlags))
			{
				nError = ::WSAGetLastError();
			}			
		}
		pThis->HandleIO(dwKey,pBuffer,dwTrans,nError);
	}
	return TRUE;
}

/******************************************************************************
函数名：HandleIO
功能：处理完成的IO请求，投递新的IO请求，释放完成的缓冲区对象和关闭的客户上下文对象
*******************************************************************************/
void CClientServer::HandleIO(DWORD dwKey, CIOCPBuffer *pBuffer, DWORD dwTrans, int nError)
{
	CIOCPContext *pContext = (CIOCPContext*)dwKey;
	::MessageBox(NULL,"进入HandleIO函数！","提示",MB_OK);
	if (pContext != NULL)  
	{
		/******(1)是发送和接收请求，减少套接字上的未决的IO计数******/
		::EnterCriticalSection(&pContext->Lock);
		if (pBuffer->nOperation == OP_READ)
		{
			pContext->nOutstandingRecv--;
		}
		else if (pBuffer->nOperation == OP_WRITE)
		{
			pContext->nOutstandingSend--;
		}
		::LeaveCriticalSection(&pContext->Lock);
	
		/******（2）检查套接字是否已经被我们关闭******/
		if (pContext->bClosing)  
		{
			//套接字已经被关闭
			if (pContext->nOutstandingRecv == 0 && pContext->nOutstandingSend)
			{
				ReleaseContext(pContext);
			}
			//释放已关闭套接字的未决IO
			ReleaseBuffer(pBuffer);
			return;
		}
	}
	else  //连接请求
	{
		RemovePendingAccept(pBuffer);
	}
	/******（3）检查套接字上发生的错误，如果有的话，通知用户，然后关闭套接字******/
	if (nError != NO_ERROR)
	{
		if (pBuffer->nOperation != OP_ACCEPT)
		{
			OnConnectionError(pContext,pBuffer,nError);
			CloseAConnection(pContext);
			if (pContext->nOutstandingRecv == 0 && pContext->nOutstandingSend  == 0)
			{
				ReleaseContext(pContext);
			}
		}
		else
		{
			//客户端出错，释放IO缓冲区
			if (pBuffer->sClient != INVALID_SOCKET)
			{
				::closesocket(pBuffer->sClient);
				pBuffer->sClient = INVALID_SOCKET;
			}
		}
		ReleaseBuffer(pBuffer);
		return;
	}
	/******开始处理******/
	if (pBuffer->nOperation == OP_ACCEPT)
	{
		::MessageBox(NULL,"接收处理！","提示",MB_OK);
		CIOCPContext *pClient = AllocateContext(pBuffer->sClient);
		if (pClient != NULL)
		{
			if (AddAConnection(pClient))
			{
				int nLocalLen,nRemoteLen;
				LPSOCKADDR pLocalAddr, pRemoteAddr;
				m_lpfnGetAcceptExSockaddrs(pBuffer->buff,
																0,
																sizeof(sockaddr_in*)+16,
																sizeof(sockaddr_in*)+16,
																(SOCKADDR**)&pLocalAddr,
																&nLocalLen,
																(SOCKADDR**)&pRemoteAddr,
																&nRemoteLen);
				memcpy(&pClient->addrLocal,pLocalAddr,nLocalLen);
				memcpy(&pClient->addrRemote,pRemoteAddr,nRemoteLen);
				//关联新连接到完成端口对象
				::CreateIoCompletionPort((HANDLE)pClient->s,m_hCompletion,(DWORD)pClient,0);
				//通知用户
				pBuffer->nLen = dwTrans;
				::MessageBox(NULL,"获取接受的信息！","OP_ACCEPT",MB_OK);
				OnConnectionEstablished(pClient,pBuffer);

				//向新的连接投递一个READ请求，这些空间在套接字关闭或出错时释放
			/*	for (int i=0;i<5;i++)
				{
					CIOCPBuffer *p = AllocateBuffer(BUFFER_SIZE);
					if (p != NULL)
					{
						if (!PostRecv(pClient,p))
						{
							CloseAConnection(pClient);
							break;
						}
					}
				}*/
			}
			/*else  //连接数量已满，关闭连接
			{
				CloseAConnection(pClient);
				ReleaseContext(pClient);
			}*/
		}
		//资源不足，关闭与客户的连接
		/*else
		{
			::closesocket(pBuffer->sClient);
			pBuffer->sClient = INVALID_SOCKET;
		}
		//Accept请求完成，释放IO缓冲区
		ReleaseBuffer(pBuffer);
		//通知监听线程继续再投递一个Accept请求
		::InterlockedIncrement(&m_nRepostCount);
		::SetEvent(m_hRepostEvent);*/
	}
}

void CClientServer::OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
	::MessageBox(NULL,"进入OnConnectionEstablished！","OnConnectionEstablished",MB_OK);
	CString tmp;
	tmp.Format("%s\r\n",pContext->addrRemote);
	printf("%s",tmp);
	ServerDlg.Edit(tmp);
	return;
}

void CClientServer::OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
}

void CClientServer::OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer,int nError)
{
}

void CClientServer::OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
}

void CClientServer::OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
}