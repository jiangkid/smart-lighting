#include "StdAfx.h"
#include "ClientServer.h"

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
		return FALSE;
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
	/*****������ɶ˿ڣ�����*****/
	m_hCompletion = CreateIoCompletionPort ( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	::CreateIoCompletionPort(&m_sListen,m_hCompletion,(DWORD)0,0);
	/*****ע���¼�*****/
	::WSAEventSelect(  m_sListen,  m_hAcceptEvent, FD_ACCEPT);
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

			}
		}
	}


	return TRUE;
}

DWORD WINAPI  CClientServer::_WorkerThreadProc(LPVOID lpParam)
{



	return TRUE;
}