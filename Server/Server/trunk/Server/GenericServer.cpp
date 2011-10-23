#include "StdAfx.h"
#include "GenericServer.h"


CGenericServer::CGenericServer(void)
{
	/*****列表*****/
	m_pFreeBufferList = NULL;				//用于在内存池中申请内存空间
	m_pFreeContextList = NULL;	
	m_pPendingAccepts = NULL;			//用于建立列表
	m_pConnectionList = NULL;

	m_nFreeBufferCount = 0;
	m_nFreeContextCount = 0;
	m_nPendingAcceptCount = 0;
	m_nCurrentConnection = 0;

	/*****初始化临界区*****/
	::InitializeCriticalSection(&m_FreeBufferListLock);
	::InitializeCriticalSection(&m_FreeContextListLock);
	::InitializeCriticalSection(&m_PendingAcceptsLock);
	::InitializeCriticalSection(&m_ConnectionListLock);

	/*****创立事件*****/
	m_hAcceptEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hRepostEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_nRepostCount = 0;

	m_nPort = 5002;


	m_nInitialAccepts = 10;				//开始投递的Accept I/O的数量
	//m_nInitialReads = 4;
	m_nMaxAccepts = 100;			 //Accept （可能是buffer）的最大连接数   
	m_nMaxFreeBuffers = 200;		//空闲列表中buffer最大个数   
	m_nMaxFreeContexts = 100;
	m_nMaxConnections = 2000;	 //客户连接列表,context最大个数

	m_hListenThread = NULL;			//监听线程
	m_hCompletion = NULL;			//完成端口设为空
	m_sListen = INVALID_SOCKET;	 //监听套接字设为无效套接字
	m_lpfnAcceptEx = NULL;			 //Accept扩展函数地址
	//m_lpfnGetAcceptExSockaddrs = NULL;

	m_bShutDown = FALSE;			//服务器初始状态设置
	m_bServerStarted = FALSE;

	// 初始化WS2_32.dll
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	::WSAStartup(sockVersion, &wsaData);
}

CGenericServer::~CGenericServer(void)
{
	if(m_hListenThread != NULL)										//关闭监听线程
		::CloseHandle(m_hListenThread);
	if(m_sListen != INVALID_SOCKET)								 //关闭监听套接字
		::closesocket(m_sListen);
	::CloseHandle(m_hAcceptEvent);								 //关闭事件
	::CloseHandle(m_hRepostEvent);
	::DeleteCriticalSection(&m_FreeBufferListLock);			//删除临界区
	::DeleteCriticalSection(&m_FreeContextListLock);
	::DeleteCriticalSection(&m_PendingAcceptsLock);
	::DeleteCriticalSection(&m_ConnectionListLock);		
}



/***************申请缓冲区对象***************/
CIOCPBuffer *CGenericServer::AllocateBuffer(int nLen)
{
	CIOCPBuffer *pBuffer = NULL;
	if(nLen>BUFFER_SIZE)
	{
		return FALSE;
	}
	::EnterCriticalSection(&m_FreeBufferListLock);
	if(m_pFreeBufferList==NULL)
	{
		pBuffer =(CIOCPBuffer *) ::HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(CIOCPBuffer)+BUFFER_SIZE);
	}
	else
	{
		pBuffer = m_pFreeBufferList;			//从内存池中腾出一块区域
		m_pFreeBufferList = m_pFreeBufferList->pNext;
		pBuffer->pNext = NULL;
		m_nFreeBufferCount --;
	}
	::LeaveCriticalSection(&m_FreeBufferListLock);
	/*****初始化新的缓冲区对象*****/
	if(pBuffer!=NULL)
	{
		pBuffer->buff = (char*)(pBuffer + 1);
		pBuffer->nLen = nLen;
	}
	return pBuffer;
}

/***************释放缓冲区对象***************/
void CGenericServer::ReleaseBuffer(CIOCPBuffer *pBuffer)
{
	::EnterCriticalSection(&m_FreeBufferListLock);
	if(m_nFreeBufferCount<m_nMaxFreeBuffers)
	{
		memset(pBuffer,0,sizeof(CIOCPBuffer)+BUFFER_SIZE);
		pBuffer->pNext = m_pFreeBufferList;
		m_pFreeBufferList = pBuffer;
		m_nFreeBufferCount ++;
	}
	::LeaveCriticalSection(&m_FreeBufferListLock);
}
/***************申请套接字上下文***************/
CIOCPContext *CGenericServer::AllocateContext(SOCKET s)
{
	CIOCPContext *pContext = NULL;

	::EnterCriticalSection(&m_FreeContextListLock);
	if(m_pFreeBufferList==NULL)
	{
		pContext =(CIOCPContext *) ::HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(CIOCPContext)+BUFFER_SIZE);
		::InitializeCriticalSection(&pContext->Lock);
	}
	else
	{
		pContext = m_pFreeContextList;			//从内存池中腾出一块区域
		m_pFreeContextList = m_pFreeContextList->pNext;
		pContext->pNext = NULL;
		m_nFreeContextCount --;
	}
	::LeaveCriticalSection(&m_FreeContextListLock);
	/*****初始化新的缓冲区对象*****/
	if(pContext!=NULL)
	{
		pContext->s = s;
	}
	return pContext;
}
/***************释放套接字上下文***************/
void CGenericServer::ReleaseContext(CIOCPContext *pContext)
{
	if(pContext->s != INVALID_SOCKET)
		::closesocket(pContext->s);
	//首先释放（如果有的话）此套节字上的没有按顺序完成的读I/O的缓冲区
	CIOCPBuffer *pNext;
	while(pContext->pOutOfOrderReads !=  NULL)
	{
		pNext = pContext->pOutOfOrderReads->pNext;
		ReleaseBuffer(pContext->pOutOfOrderReads);
		pContext->pOutOfOrderReads = pNext;
	}
	::EnterCriticalSection(&m_FreeContextListLock);
	if(m_nFreeContextCount <= m_nMaxFreeContexts) // 添加到空闲列表
	{
		// 先将关键代码段变量保存到一个临时变量中
		CRITICAL_SECTION cstmp = pContext->Lock;
		// 将要释放的上下文对象初始化为0
		memset(pContext, 0, sizeof(CIOCPContext));

		// 再放会关键代码段变量，将要释放的上下文对象添加到空闲列表的表头
		pContext->Lock = cstmp;
		pContext->pNext = m_pFreeContextList;
		m_pFreeContextList = pContext;

		// 更新计数
		m_nFreeContextCount ++;
	}
	else
	{
		::DeleteCriticalSection(&pContext->Lock);
		::HeapFree(::GetProcessHeap, 0, pContext);
	}
	::LeaveCriticalSection(&m_FreeContextListLock);
}
/***************释放空闲缓冲区对象列表***************/
void CGenericServer::FreeBuffers()
{
	::EnterCriticalSection(&m_FreeBufferListLock);
	CIOCPBuffer *pFreeBuffer = m_pFreeBufferList;
	CIOCPBuffer *pNextBuffer;
	while(pFreeBuffer  != NULL)   //遍历
	{
		pNextBuffer = pFreeBuffer->pNext;
		if(!::HeapFree(::GetProcessHeap(), 0, pFreeBuffer))
		{
			break;
		}
		pFreeBuffer = pNextBuffer;
	}
	m_pFreeBufferList = NULL;
	m_nFreeBufferCount = 0;
	::LeaveCriticalSection(&m_FreeBufferListLock);
}
/***************释放空闲上下文对象列表***************/
void CGenericServer::FreeContext()
{
	::EnterCriticalSection(&m_FreeContextListLock);
	CIOCPContext *pFreeContext =  m_pFreeContextList;
	CIOCPContext *pNextContext;
	while(pFreeContext != NULL)
	{
		pNextContext = pFreeContext->pNext;
		::DeleteCriticalSection(&pFreeContext->Lock);
		if(!::HeapFree(::GetProcessHeap(), 0, pFreeContext))
		{
			break;
		}
		pFreeContext = pNextContext;
	}
	m_pFreeContextList = NULL;
	m_nFreeContextCount = 0;
	::LeaveCriticalSection(&m_FreeContextListLock);
}
/***************向连接列表中添加一个连接***************/
BOOL CGenericServer::AddAConnection(CIOCPContext *pContext)
{
	::EnterCriticalSection(&m_ConnectionListLock);
	if(m_nCurrentConnection <= m_nMaxConnections)
	{
		// 添加到表头
		pContext->pNext = m_pConnectionList;
		m_pConnectionList = pContext;
		// 更新计数
		m_nCurrentConnection ++;

		::LeaveCriticalSection(&m_ConnectionListLock);
		return TRUE;
	}
	::LeaveCriticalSection(&m_ConnectionListLock);
	return FALSE;
}
/***************从列表中移除要关闭的一个连接***************/
void CGenericServer::CloseAConnection(CIOCPContext *pContext)
{
	::EnterCriticalSection(&m_ConnectionListLock);
	CIOCPContext* pTest = m_pConnectionList;
	if(pTest == pContext)
	{
		m_pConnectionList =  pContext->pNext;
		m_nCurrentConnection --;
	}
	else
	{
		while(pTest != NULL && pTest->pNext !=  pContext)
			pTest = pTest->pNext;
		if(pTest != NULL)
		{
			pTest->pNext =  pContext->pNext;
			m_nCurrentConnection --;
		}
	}
	::LeaveCriticalSection(&m_ConnectionListLock);
	// 然后关闭客户套节字
	::EnterCriticalSection(&pContext->Lock);

	if(pContext->s != INVALID_SOCKET)
	{
		::closesocket(pContext->s);	
		pContext->s = INVALID_SOCKET;
	}
	pContext->bClosing = TRUE;

	::LeaveCriticalSection(&pContext->Lock);
}
/***************关闭所有连接***************/
void CGenericServer::CloseAllConnection()
{
	// 遍历整个连接列表，关闭所有的客户套节字

	::EnterCriticalSection(&m_ConnectionListLock);

	CIOCPContext *pContext = m_pConnectionList;
	while(pContext != NULL)
	{	
		::EnterCriticalSection(&pContext->Lock);
		if(pContext->s != INVALID_SOCKET)
		{
			::closesocket(pContext->s);
			pContext->s = INVALID_SOCKET;
		}
		pContext->bClosing = TRUE;
		::LeaveCriticalSection(&pContext->Lock);			
		pContext = pContext->pNext;
	}
	m_pConnectionList = NULL;
	m_nCurrentConnection = 0;
	::LeaveCriticalSection(&m_ConnectionListLock);
}
/***************插入未决的接受请求***************/
BOOL CGenericServer::InsertPendingAccept(CIOCPBuffer *pBuffer)
{
	// 将一个I/O缓冲区对象插入到m_pPendingAccepts表中
	::EnterCriticalSection(&m_PendingAcceptsLock);
	if(m_pPendingAccepts == NULL)
		m_pPendingAccepts = pBuffer;
	else
	{
		pBuffer->pNext = m_pPendingAccepts;
		m_pPendingAccepts = pBuffer;
	}
	m_nPendingAcceptCount ++;
	::LeaveCriticalSection(&m_PendingAcceptsLock);
	return TRUE;
}
/***************移除未决的接受请求***************/
BOOL CGenericServer::RemovePendingAccept(CIOCPBuffer *pBuffer)
{
	BOOL bResult = FALSE;

	// 遍历m_pPendingAccepts表，从中移除pBuffer所指向的缓冲区对象
	::EnterCriticalSection(&m_PendingAcceptsLock);

	CIOCPBuffer *pTest = m_pPendingAccepts;
	if(pTest == pBuffer)	// 如果是表头元素
	{
		m_pPendingAccepts = pBuffer->pNext;
		bResult = TRUE;
	}
	else					// 不是表头元素的话，就要遍历这个表来查找了
	{
		while(pTest != NULL && pTest->pNext != pBuffer)
			pTest = pTest->pNext;
		if(pTest != NULL)
		{
			pTest->pNext = pBuffer->pNext;
			bResult = TRUE;
		}
	}
	// 更新计数
	if(bResult)
		m_nPendingAcceptCount --;

	::LeaveCriticalSection(&m_PendingAcceptsLock);

	return  bResult;
}

/***************取得下一个要读取的***************/
CIOCPBuffer *CGenericServer::GetNextReadBuffer(CIOCPContext *pContext, CIOCPBuffer *pBuffer )
{
	if(pBuffer != NULL)
	{
		// 如果与要读的下一个序列号相等，则读这块缓冲区
		if(pBuffer->nSequenceNumber == pContext->nCurrentSequence)
		{
			return pBuffer;
		}

		// 如果不相等，则说明没有按顺序接收数据，将这块缓冲区保存到连接的pOutOfOrderReads列表中

		// 列表中的缓冲区是按照其序列号从小到大的顺序排列的

		pBuffer->pNext = NULL;

		CIOCPBuffer *ptr = pContext->pOutOfOrderReads;
		CIOCPBuffer *pPre = NULL;
		while(ptr != NULL)
		{
			if(pBuffer->nSequenceNumber < ptr->nSequenceNumber)
				break;

			pPre = ptr;
			ptr = ptr->pNext;
		}

		if(pPre == NULL) // 应该把pBuffer插入到pOutOfOrderReads中的表头  pBuffer->nSequenceNumber < ptr->nSequenceNumber
		{
			pBuffer->pNext = pContext->pOutOfOrderReads;
			pContext->pOutOfOrderReads = pBuffer;
		}
		else			// 应该插入到表的中间
		{
			pBuffer->pNext = pPre->pNext;  //?????
			pPre->pNext = pBuffer->pNext;
		}
	}

	// 检查表头元素的序列号，如果与要读的序列号一致，就将它从表中移除，返回给用户
	CIOCPBuffer *ptr = pContext->pOutOfOrderReads;
	if(ptr != NULL && (ptr->nSequenceNumber == pContext->nCurrentSequence))
	{
		pContext->pOutOfOrderReads = ptr->pNext;
		return ptr;
	}
	return NULL;
}
/***************投递IO、发送IO、接收IO***************/
BOOL CGenericServer::PostAccept(CIOCPBuffer *pBuffer)
{
	// 设置I/O类型
	pBuffer->nOperation = OP_ACCEPT;

	// 投递此重叠I/O  
	DWORD dwBytes;
	pBuffer->sClient = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	BOOL b = m_lpfnAcceptEx(m_sListen, 
		pBuffer->sClient,
		pBuffer->buff, 
		0,
		sizeof(sockaddr_in) + 16, 
		sizeof(sockaddr_in) + 16, 
		&dwBytes, 
		&pBuffer->overlapped);
	if(!b && ::WSAGetLastError() != WSA_IO_PENDING)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CGenericServer::PostSend(CIOCPContext *pContext,CIOCPBuffer *pBuffer)
{
	// 设置I/O类型
	pBuffer->nOperation = OP_READ;	

	::EnterCriticalSection(&pContext->Lock);

	// 设置序列号
	pBuffer->nSequenceNumber = pContext->nReadSequence;

	// 投递此重叠I/O
	DWORD dwBytes;
	DWORD dwFlags = 0;
	WSABUF buf;
	buf.buf = pBuffer->buff;
	buf.len = pBuffer->nLen;
	if(::WSARecv(pContext->s, &buf, 1, &dwBytes, &dwFlags, &pBuffer->overlapped, NULL) != NO_ERROR)
	{
		if(::WSAGetLastError() != WSA_IO_PENDING)
		{
			::LeaveCriticalSection(&pContext->Lock);
			return FALSE;
		}
	}

	// 增加套节字上的重叠I/O计数和读序列号计数

	pContext->nOutstandingRecv ++;
	pContext->nReadSequence ++;

	::LeaveCriticalSection(&pContext->Lock);

	return TRUE;
}

BOOL CGenericServer::PostRecv(CIOCPContext *pContext,CIOCPBuffer *pBuffer)
{
	// 跟踪投递的发送的数量，防止用户仅发送数据而不接收，导致服务器抛出大量发送操作
	if(pContext->nOutstandingSend > m_nMaxSends)
		return FALSE;

	// 设置I/O类型，增加套节字上的重叠I/O计数
	pBuffer->nOperation = OP_WRITE;

	// 投递此重叠I/O
	DWORD dwBytes;
	DWORD dwFlags = 0;
	WSABUF buf;
	buf.buf = pBuffer->buff;
	buf.len = pBuffer->nLen;
	if(::WSASend(pContext->s, 
		&buf, 1, &dwBytes, dwFlags, &pBuffer->overlapped, NULL) != NO_ERROR)
	{
		if(::WSAGetLastError() != WSA_IO_PENDING)
			return FALSE;
	}	

	// 增加套节字上的重叠I/O计数
	::EnterCriticalSection(&pContext->Lock);
	pContext->nOutstandingSend ++;
	::LeaveCriticalSection(&pContext->Lock);

	return TRUE;
}

/******************************************************************************
函数名：HandleIO
功能：处理完成的IO请求，投递新的IO请求，释放完成的缓冲区对象和关闭的客户上下文对象
*******************************************************************************/
void CGenericServer::HandleIO(DWORD dwKey, CIOCPBuffer *pBuffer, DWORD dwTrans, int nError)
{
	CIOCPContext *pContext = (CIOCPContext*)dwKey;
	
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
		CIOCPContext *pClient = AllocateContext(pBuffer->sClient);
		if (pClient != NULL)
		{
			if (AddAConnection(pClient))
			{
				int nLocalLen,nRemoteLen;
				LPSOCKADDR pLocalAddr, pRemoteAddr;
				m_lpfnGetAcceptExSockaddrs(pBuffer->buff,0,
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
				OnConnectionEstablished(pClient,pBuffer);
				//向新的连接投递一个READ请求，这些空间在套接字关闭或出错时释放
				for (int i=0;i<5;i++)
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
				}
			}
			else  //连接数量已满，关闭连接
			{
				CloseAConnection(pClient);
				ReleaseContext(pClient);
			}
		}
		//资源不足，关闭与客户的连接
		else
		{
			::closesocket(pBuffer->sClient);
			pBuffer->sClient = INVALID_SOCKET;
		}
		//Accept请求完成，释放IO缓冲区
		ReleaseBuffer(pBuffer);
		//通知监听线程继续再投递一个Accept请求
		::InterlockedIncrement(&m_nRepostCount);
		::SetEvent(m_hRepostEvent);
	}
}

void CGenericServer::OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{

}

void CGenericServer::OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{

}

void CGenericServer::OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer,int nError)
{

}

void CGenericServer::OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{

}

void CGenericServer::OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{

}

//
//创建I/O完成端口
//
HANDLE CGenericServer::CreateNewIoCompletionPort(DWORD dwNumberOfConcurrentThreads)
{
	return (CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,dwNumberOfConcurrentThreads));
}

//
//将套接口与完成端口绑定
//
BOOL CGenericServer::AssociateWithIoCompletionPort(HANDLE hComPort,HANDLE hDevice,DWORD dwCompKey)
{
	return (CreateIoCompletionPort(hDevice,hComPort,dwCompKey,0)==hComPort);
}