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
/*****��ʼ����״̬������ʼ����socket������󶨡���ɶ˿ڴ��������*****/
BOOL CClientServer::Start(int nPort, int nMaxConnections, int nMaxFreeBuffers, int nMaxFreeContexts, int nInitialAccepts, int nInitialReads)
{
	if(m_bServerStarted)
	{
		return TRUE;
	}
	/*****��������*****/
	m_nPort = nPort;
	m_nMaxConnections = nMaxConnections;
	m_nMaxFreeBuffers = nMaxFreeBuffers;
	m_nMaxFreeContexts = nMaxFreeContexts;
	m_nInitialAccepts = nInitialAccepts;
	m_nInitialReads = nInitialReads;
	/*****״̬����*****/
	m_bServerStarted = TRUE;
	m_bShutDown = FALSE;
	/*****�׽��ִ������󶨵����ض˿ڣ��������ģʽ*****/
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
	//������չ����AcceptEx
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
	//������չ����GetAcceptExSockaddrs,�ú���������AcceptExȡ������
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
	/*****������ɶ˿ڣ�����*****/
	m_hCompletion = CreateIoCompletionPort ( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	::CreateIoCompletionPort((HANDLE)m_sListen,m_hCompletion,(DWORD)0,0);
	/*****ע���¼�*****/
	::WSAEventSelect(m_sListen,  m_hAcceptEvent, FD_ACCEPT);
	//���������߳�
	HANDLE	hThread=CreateThread(NULL,0,_ListenThreadProc,this,0,NULL);
	return TRUE;
}

DWORD WINAPI  CClientServer::_ListenThreadProc(LPVOID lpParam)
{
	CClientServer *pThis = (CClientServer *)lpParam;
	//���ڼ����׽�����Ͷ�ݼ���AcceptI/O
	CIOCPBuffer *pBuffer;
	for(int i=0;i<pThis->m_nInitialAccepts;i++)//m_nInitialAccepts
	{
		pBuffer = pThis->AllocateBuffer(BUFFER_SIZE);
		if(pBuffer == NULL)
		{
			return -1;
		}
		pThis->InsertPendingAccept(pBuffer);// ��һ��I/O������������뵽m_pPendingAccepts����
		pThis->PostAccept(pBuffer);				 //Ͷ��IO
	}	
	//�����¼��������飬�Ա����������WSAWaitForMutipleEvents����
	HANDLE   hWaitEvents[2+MAX_THREAD];
	int nEventCount = 0;
	hWaitEvents[nEventCount++] = pThis->m_hAcceptEvent;
	hWaitEvents[nEventCount++] = pThis->m_hRepostEvent;
	//����ָ�������Ĺ����߳�����ɶ˿��ϴ���IO
	for(int i=0;i<MAX_THREAD;i++)
	{
		hWaitEvents[nEventCount++]  = ::CreateThread(NULL,0,_WorkerThreadProc, pThis,0,NULL);
	}
	//�����������ѭ���������¼����������е��¼�
	while(TRUE)
	{
		int nIndex = ::WSAWaitForMultipleEvents(nEventCount,hWaitEvents,FALSE,300*1000,FALSE);
		//���ȼ���Ƿ�Ҫֹͣ����
		if(pThis->m_bShutDown || nIndex == WSA_WAIT_FAILED)
		{
			::MessageBox(NULL,"�û�֪ͨ�˳����¼�����","�����߳�",MB_OK);
			//�ر���������
			pThis->CloseAllConnection();
			::Sleep(0);
			//�ر��׽���
			::closesocket(pThis->m_sListen);
			pThis->m_sListen = INVALID_SOCKET;
			::Sleep(0);
			//֪ͨ���е�IO�����߳��˳�
			for(int i=2;i<MAX_THREAD+2;++i)
			{
				::PostQueuedCompletionStatus(pThis->m_hCompletion,-1,0,NULL);
			}
			//�ȴ�IO�����߳��˳�
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
		//1)��ʱ�������δ���ص�AcceptEx IO �����ӽ����˶���ʱ��
		if (nIndex == WSA_WAIT_TIMEOUT)
		{
			::MessageBox(NULL,"�¼��ȴ���ʱ��","��ʾ",MB_OK);
			printf("WSA_WAIT_TIMEOUT is d%", ++y);
			pBuffer = pThis->m_pPendingAccepts;
			while (pBuffer != NULL)
			{
				int nSeconds;
				int nLen = sizeof(nSeconds);
				::getsockopt(pBuffer->sClient,SOL_SOCKET,SO_CONNECT_TIME,(char*)&nSeconds,&nLen);
				if (nSeconds!= -1 && nSeconds > 10*60)
				{
					::MessageBox(NULL,"�رտͻ��˵����ӣ�","��ʾ",MB_OK);
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
			if (nIndex = 0)   // 1 )m_hAcceptEvent�¼��������ţ�˵��Ͷ�ݵ�Accept���󲻹�����Ҫ����
			{
				::WSAEnumNetworkEvents(pThis->m_sListen,hWaitEvents[nIndex],&Netevent);
				if (Netevent.lNetworkEvents & FD_ACCEPT)
				{
					nLimit = 50;
				}
			}
			else if(nIndex = 1) // 2��m_hAcceptEvent�¼��������ţ�˵��Ͷ�ݵ�Accept���󲻹�����Ҫ����
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
	::MessageBox(NULL,"���빤���̣߳�","��ʾ",MB_OK);
	while (TRUE)
	{
		//�ڹ���������ɶ˿ڵ������׽����ϵȴ�IO���
		BOOL bOK=::GetQueuedCompletionStatus(pThis->m_hCompletion,&dwTrans,(LPDWORD)&dwKey,
			                           (LPOVERLAPPED*)&lpoverlapped,WSA_INFINITE);
		if (dwTrans==-1)//�û�֪ͨ�˳�
		{
			::MessageBox(NULL,"�˳������̣߳�","��ʾ",MB_OK);
			::ExitThread(0);			
		}
		::MessageBox(NULL,"��ý�������","��ʾ",MB_OK);
		pBuffer = CONTAINING_RECORD(lpoverlapped,CIOCPBuffer,overlapped);
		int nError = NO_ERROR;
		if (!bOK)
		{
			::MessageBox(NULL,"�ȴ���ɶ˿ڳ���","��ʾ",MB_OK);
			SOCKET s;
			if (pBuffer->nOperation == OP_ACCEPT)
			{
				s = pThis->m_sListen;
				::MessageBox(NULL,"�ȴ���ɶ˿ڳ���--���ܳ���","��ʾ",MB_OK);
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
��������HandleIO
���ܣ�������ɵ�IO����Ͷ���µ�IO�����ͷ���ɵĻ���������͹رյĿͻ������Ķ���
*******************************************************************************/
void CClientServer::HandleIO(DWORD dwKey, CIOCPBuffer *pBuffer, DWORD dwTrans, int nError)
{
	CIOCPContext *pContext = (CIOCPContext*)dwKey;
	::MessageBox(NULL,"����HandleIO������","��ʾ",MB_OK);
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
		::MessageBox(NULL,"���մ���","��ʾ",MB_OK);
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
				//���������ӵ���ɶ˿ڶ���
				::CreateIoCompletionPort((HANDLE)pClient->s,m_hCompletion,(DWORD)pClient,0);
				//֪ͨ�û�
				pBuffer->nLen = dwTrans;
				::MessageBox(NULL,"��ȡ���ܵ���Ϣ��","OP_ACCEPT",MB_OK);
				OnConnectionEstablished(pClient,pBuffer);

				//���µ�����Ͷ��һ��READ������Щ�ռ����׽��ֹرջ����ʱ�ͷ�
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
			/*else  //���������������ر�����
			{
				CloseAConnection(pClient);
				ReleaseContext(pClient);
			}*/
		}
		//��Դ���㣬�ر���ͻ�������
		/*else
		{
			::closesocket(pBuffer->sClient);
			pBuffer->sClient = INVALID_SOCKET;
		}
		//Accept������ɣ��ͷ�IO������
		ReleaseBuffer(pBuffer);
		//֪ͨ�����̼߳�����Ͷ��һ��Accept����
		::InterlockedIncrement(&m_nRepostCount);
		::SetEvent(m_hRepostEvent);*/
	}
}

void CClientServer::OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
	::MessageBox(NULL,"����OnConnectionEstablished��","OnConnectionEstablished",MB_OK);
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