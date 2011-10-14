// GenericServer.cpp: implementation of the GenericServer class.
//
//////////////////////////////////////////////////////////////////////
#include  "stdafx.h"
#include "Server.h"
#include "GenericServer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GenericServer::GenericServer()
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

GenericServer::~GenericServer()
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

/*****��ʼ����״̬������ʼ����socket������󶨡���ɶ˿ڴ��������*****/
BOOL GenericServer::Start(int nPort, int nMaxConnections, int nMaxFreeBuffers, int nMaxFreeContexts, int nInitialReads)
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
	::CreateIoCompletionPort(m_sListen,m_hCompletion,(DWORD)0,0);
	/*****ע���¼�*****/
	::WSAEventSelect(  m_sListen,  m_nAcceptEvent, FD_ACCEPT);
}

void GenericServer::OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{

}

void GenericServer::OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{

}

void GenericServer::OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer,int nError)
{

}

void GenericServer::OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{

}

void GenericServer::OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{

}