#include "StdAfx.h"
#include "ClientServer.h"

CClientServer  _ClientServer;
CClientServer *ClientServer = &_ClientServer;

CClientServer::CClientServer(void)
{
}

CClientServer::~CClientServer(void)
{
}
/*****��ʼ����״̬������ʼ����socket������󶨡���ɶ˿ڴ��������*****/
BOOL CClientServer::Start(int nPort, int nMaxConnections, int nMaxFreeBuffers, int nMaxFreeContexts, int nInitialReads)
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
	::CreateIoCompletionPort(&m_sListen,m_hCompletion,(DWORD)0,0);
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
		int nIndex = ::WSAWaitForMultipleEvents(nEventCount,hWaitEvents,FALSE,60*1000,FALSE);
		//���ȼ���Ƿ�Ҫֹͣ����
		if(pThis->m_bShutDown || nIndex == WSA_WAIT_FAILED)
		{
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
	while (TRUE)
	{
		//�ڹ���������ɶ˿ڵ������׽����ϵȴ�IO���
		BOOL bOK=::GetQueuedCompletionStatus(pThis->m_hCompletion,&dwTrans,(LPDWORD)&dwKey,
			                           (LPOVERLAPPED*)&lpoverlapped,WSA_INFINITE);
		if (dwTrans=-1)//�û�֪ͨ�˳�
		{
			::ExitThread(0);
		}
		pBuffer = CONTAINING_RECORD(lpoverlapped,CIOCPBuffer,overlapped);
		int nError = NO_ERROR;
		if (!bOK)
		{
			SOCKET s;
			if (pBuffer->nOperation == OP_ACCEPT)
			{
				s = pThis->m_sListen;
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
			pThis->HandleIO(dwKey,pBuffer,dwTrans,nError);
		}
	}
	return TRUE;
}

