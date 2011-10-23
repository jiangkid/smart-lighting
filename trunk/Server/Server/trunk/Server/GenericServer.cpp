#include "StdAfx.h"
#include "GenericServer.h"


CGenericServer::CGenericServer(void)
{
	/*****�б�*****/
	m_pFreeBufferList = NULL;				//�������ڴ���������ڴ�ռ�
	m_pFreeContextList = NULL;	
	m_pPendingAccepts = NULL;			//���ڽ����б�
	m_pConnectionList = NULL;

	m_nFreeBufferCount = 0;
	m_nFreeContextCount = 0;
	m_nPendingAcceptCount = 0;
	m_nCurrentConnection = 0;

	/*****��ʼ���ٽ���*****/
	::InitializeCriticalSection(&m_FreeBufferListLock);
	::InitializeCriticalSection(&m_FreeContextListLock);
	::InitializeCriticalSection(&m_PendingAcceptsLock);
	::InitializeCriticalSection(&m_ConnectionListLock);

	/*****�����¼�*****/
	m_hAcceptEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hRepostEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_nRepostCount = 0;

	m_nPort = 5002;


	m_nInitialAccepts = 10;				//��ʼͶ�ݵ�Accept I/O������
	//m_nInitialReads = 4;
	m_nMaxAccepts = 100;			 //Accept ��������buffer�������������   
	m_nMaxFreeBuffers = 200;		//�����б���buffer������   
	m_nMaxFreeContexts = 100;
	m_nMaxConnections = 2000;	 //�ͻ������б�,context������

	m_hListenThread = NULL;			//�����߳�
	m_hCompletion = NULL;			//��ɶ˿���Ϊ��
	m_sListen = INVALID_SOCKET;	 //�����׽�����Ϊ��Ч�׽���
	m_lpfnAcceptEx = NULL;			 //Accept��չ������ַ
	//m_lpfnGetAcceptExSockaddrs = NULL;

	m_bShutDown = FALSE;			//��������ʼ״̬����
	m_bServerStarted = FALSE;

	// ��ʼ��WS2_32.dll
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	::WSAStartup(sockVersion, &wsaData);
}

CGenericServer::~CGenericServer(void)
{
	if(m_hListenThread != NULL)										//�رռ����߳�
		::CloseHandle(m_hListenThread);
	if(m_sListen != INVALID_SOCKET)								 //�رռ����׽���
		::closesocket(m_sListen);
	::CloseHandle(m_hAcceptEvent);								 //�ر��¼�
	::CloseHandle(m_hRepostEvent);
	::DeleteCriticalSection(&m_FreeBufferListLock);			//ɾ���ٽ���
	::DeleteCriticalSection(&m_FreeContextListLock);
	::DeleteCriticalSection(&m_PendingAcceptsLock);
	::DeleteCriticalSection(&m_ConnectionListLock);		
}



/***************���뻺��������***************/
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
		pBuffer = m_pFreeBufferList;			//���ڴ�����ڳ�һ������
		m_pFreeBufferList = m_pFreeBufferList->pNext;
		pBuffer->pNext = NULL;
		m_nFreeBufferCount --;
	}
	::LeaveCriticalSection(&m_FreeBufferListLock);
	/*****��ʼ���µĻ���������*****/
	if(pBuffer!=NULL)
	{
		pBuffer->buff = (char*)(pBuffer + 1);
		pBuffer->nLen = nLen;
	}
	return pBuffer;
}

/***************�ͷŻ���������***************/
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
/***************�����׽���������***************/
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
		pContext = m_pFreeContextList;			//���ڴ�����ڳ�һ������
		m_pFreeContextList = m_pFreeContextList->pNext;
		pContext->pNext = NULL;
		m_nFreeContextCount --;
	}
	::LeaveCriticalSection(&m_FreeContextListLock);
	/*****��ʼ���µĻ���������*****/
	if(pContext!=NULL)
	{
		pContext->s = s;
	}
	return pContext;
}
/***************�ͷ��׽���������***************/
void CGenericServer::ReleaseContext(CIOCPContext *pContext)
{
	if(pContext->s != INVALID_SOCKET)
		::closesocket(pContext->s);
	//�����ͷţ�����еĻ������׽����ϵ�û�а�˳����ɵĶ�I/O�Ļ�����
	CIOCPBuffer *pNext;
	while(pContext->pOutOfOrderReads !=  NULL)
	{
		pNext = pContext->pOutOfOrderReads->pNext;
		ReleaseBuffer(pContext->pOutOfOrderReads);
		pContext->pOutOfOrderReads = pNext;
	}
	::EnterCriticalSection(&m_FreeContextListLock);
	if(m_nFreeContextCount <= m_nMaxFreeContexts) // ��ӵ������б�
	{
		// �Ƚ��ؼ�����α������浽һ����ʱ������
		CRITICAL_SECTION cstmp = pContext->Lock;
		// ��Ҫ�ͷŵ������Ķ����ʼ��Ϊ0
		memset(pContext, 0, sizeof(CIOCPContext));

		// �ٷŻ�ؼ�����α�������Ҫ�ͷŵ������Ķ�����ӵ������б�ı�ͷ
		pContext->Lock = cstmp;
		pContext->pNext = m_pFreeContextList;
		m_pFreeContextList = pContext;

		// ���¼���
		m_nFreeContextCount ++;
	}
	else
	{
		::DeleteCriticalSection(&pContext->Lock);
		::HeapFree(::GetProcessHeap, 0, pContext);
	}
	::LeaveCriticalSection(&m_FreeContextListLock);
}
/***************�ͷſ��л����������б�***************/
void CGenericServer::FreeBuffers()
{
	::EnterCriticalSection(&m_FreeBufferListLock);
	CIOCPBuffer *pFreeBuffer = m_pFreeBufferList;
	CIOCPBuffer *pNextBuffer;
	while(pFreeBuffer  != NULL)   //����
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
/***************�ͷſ��������Ķ����б�***************/
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
/***************�������б������һ������***************/
BOOL CGenericServer::AddAConnection(CIOCPContext *pContext)
{
	::EnterCriticalSection(&m_ConnectionListLock);
	if(m_nCurrentConnection <= m_nMaxConnections)
	{
		// ��ӵ���ͷ
		pContext->pNext = m_pConnectionList;
		m_pConnectionList = pContext;
		// ���¼���
		m_nCurrentConnection ++;

		::LeaveCriticalSection(&m_ConnectionListLock);
		return TRUE;
	}
	::LeaveCriticalSection(&m_ConnectionListLock);
	return FALSE;
}
/***************���б����Ƴ�Ҫ�رյ�һ������***************/
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
	// Ȼ��رտͻ��׽���
	::EnterCriticalSection(&pContext->Lock);

	if(pContext->s != INVALID_SOCKET)
	{
		::closesocket(pContext->s);	
		pContext->s = INVALID_SOCKET;
	}
	pContext->bClosing = TRUE;

	::LeaveCriticalSection(&pContext->Lock);
}
/***************�ر���������***************/
void CGenericServer::CloseAllConnection()
{
	// �������������б��ر����еĿͻ��׽���

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
/***************����δ���Ľ�������***************/
BOOL CGenericServer::InsertPendingAccept(CIOCPBuffer *pBuffer)
{
	// ��һ��I/O������������뵽m_pPendingAccepts����
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
/***************�Ƴ�δ���Ľ�������***************/
BOOL CGenericServer::RemovePendingAccept(CIOCPBuffer *pBuffer)
{
	BOOL bResult = FALSE;

	// ����m_pPendingAccepts�������Ƴ�pBuffer��ָ��Ļ���������
	::EnterCriticalSection(&m_PendingAcceptsLock);

	CIOCPBuffer *pTest = m_pPendingAccepts;
	if(pTest == pBuffer)	// ����Ǳ�ͷԪ��
	{
		m_pPendingAccepts = pBuffer->pNext;
		bResult = TRUE;
	}
	else					// ���Ǳ�ͷԪ�صĻ�����Ҫ�����������������
	{
		while(pTest != NULL && pTest->pNext != pBuffer)
			pTest = pTest->pNext;
		if(pTest != NULL)
		{
			pTest->pNext = pBuffer->pNext;
			bResult = TRUE;
		}
	}
	// ���¼���
	if(bResult)
		m_nPendingAcceptCount --;

	::LeaveCriticalSection(&m_PendingAcceptsLock);

	return  bResult;
}

/***************ȡ����һ��Ҫ��ȡ��***************/
CIOCPBuffer *CGenericServer::GetNextReadBuffer(CIOCPContext *pContext, CIOCPBuffer *pBuffer )
{
	if(pBuffer != NULL)
	{
		// �����Ҫ������һ�����к���ȣ������黺����
		if(pBuffer->nSequenceNumber == pContext->nCurrentSequence)
		{
			return pBuffer;
		}

		// �������ȣ���˵��û�а�˳��������ݣ�����黺�������浽���ӵ�pOutOfOrderReads�б���

		// �б��еĻ������ǰ��������кŴ�С�����˳�����е�

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

		if(pPre == NULL) // Ӧ�ð�pBuffer���뵽pOutOfOrderReads�еı�ͷ  pBuffer->nSequenceNumber < ptr->nSequenceNumber
		{
			pBuffer->pNext = pContext->pOutOfOrderReads;
			pContext->pOutOfOrderReads = pBuffer;
		}
		else			// Ӧ�ò��뵽����м�
		{
			pBuffer->pNext = pPre->pNext;  //?????
			pPre->pNext = pBuffer->pNext;
		}
	}

	// ����ͷԪ�ص����кţ������Ҫ�������к�һ�£��ͽ����ӱ����Ƴ������ظ��û�
	CIOCPBuffer *ptr = pContext->pOutOfOrderReads;
	if(ptr != NULL && (ptr->nSequenceNumber == pContext->nCurrentSequence))
	{
		pContext->pOutOfOrderReads = ptr->pNext;
		return ptr;
	}
	return NULL;
}
/***************Ͷ��IO������IO������IO***************/
BOOL CGenericServer::PostAccept(CIOCPBuffer *pBuffer)
{
	// ����I/O����
	pBuffer->nOperation = OP_ACCEPT;

	// Ͷ�ݴ��ص�I/O  
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
	// ����I/O����
	pBuffer->nOperation = OP_READ;	

	::EnterCriticalSection(&pContext->Lock);

	// �������к�
	pBuffer->nSequenceNumber = pContext->nReadSequence;

	// Ͷ�ݴ��ص�I/O
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

	// �����׽����ϵ��ص�I/O�����Ͷ����кż���

	pContext->nOutstandingRecv ++;
	pContext->nReadSequence ++;

	::LeaveCriticalSection(&pContext->Lock);

	return TRUE;
}

BOOL CGenericServer::PostRecv(CIOCPContext *pContext,CIOCPBuffer *pBuffer)
{
	// ����Ͷ�ݵķ��͵���������ֹ�û����������ݶ������գ����·������׳��������Ͳ���
	if(pContext->nOutstandingSend > m_nMaxSends)
		return FALSE;

	// ����I/O���ͣ������׽����ϵ��ص�I/O����
	pBuffer->nOperation = OP_WRITE;

	// Ͷ�ݴ��ص�I/O
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

	// �����׽����ϵ��ص�I/O����
	::EnterCriticalSection(&pContext->Lock);
	pContext->nOutstandingSend ++;
	::LeaveCriticalSection(&pContext->Lock);

	return TRUE;
}

/******************************************************************************
��������HandleIO
���ܣ�������ɵ�IO����Ͷ���µ�IO�����ͷ���ɵĻ���������͹رյĿͻ������Ķ���
*******************************************************************************/
void CGenericServer::HandleIO(DWORD dwKey, CIOCPBuffer *pBuffer, DWORD dwTrans, int nError)
{
	CIOCPContext *pContext = (CIOCPContext*)dwKey;
	
	if (pContext != NULL)  
	{
		/******(1)�Ƿ��ͺͽ������󣬼����׽����ϵ�δ����IO����******/
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
	
		/******��2������׽����Ƿ��Ѿ������ǹر�******/
		if (pContext->bClosing)  
		{
			//�׽����Ѿ����ر�
			if (pContext->nOutstandingRecv == 0 && pContext->nOutstandingSend)
			{
				ReleaseContext(pContext);
			}
			//�ͷ��ѹر��׽��ֵ�δ��IO
			ReleaseBuffer(pBuffer);
			return;
		}
	}
	else  //��������
	{
		RemovePendingAccept(pBuffer);
	}
	/******��3������׽����Ϸ����Ĵ�������еĻ���֪ͨ�û���Ȼ��ر��׽���******/
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
			//�ͻ��˳����ͷ�IO������
			if (pBuffer->sClient != INVALID_SOCKET)
			{
				::closesocket(pBuffer->sClient);
				pBuffer->sClient = INVALID_SOCKET;
			}
		}
		ReleaseBuffer(pBuffer);
		return;
	}
	/******��ʼ����******/
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
				//���������ӵ���ɶ˿ڶ���
				::CreateIoCompletionPort((HANDLE)pClient->s,m_hCompletion,(DWORD)pClient,0);
				//֪ͨ�û�
				pBuffer->nLen = dwTrans;
				OnConnectionEstablished(pClient,pBuffer);
				//���µ�����Ͷ��һ��READ������Щ�ռ����׽��ֹرջ����ʱ�ͷ�
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
			else  //���������������ر�����
			{
				CloseAConnection(pClient);
				ReleaseContext(pClient);
			}
		}
		//��Դ���㣬�ر���ͻ�������
		else
		{
			::closesocket(pBuffer->sClient);
			pBuffer->sClient = INVALID_SOCKET;
		}
		//Accept������ɣ��ͷ�IO������
		ReleaseBuffer(pBuffer);
		//֪ͨ�����̼߳�����Ͷ��һ��Accept����
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
//����I/O��ɶ˿�
//
HANDLE CGenericServer::CreateNewIoCompletionPort(DWORD dwNumberOfConcurrentThreads)
{
	return (CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,dwNumberOfConcurrentThreads));
}

//
//���׽ӿ�����ɶ˿ڰ�
//
BOOL CGenericServer::AssociateWithIoCompletionPort(HANDLE hComPort,HANDLE hDevice,DWORD dwCompKey)
{
	return (CreateIoCompletionPort(hDevice,hComPort,dwCompKey,0)==hComPort);
}