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

GenericServer::~GenericServer()
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

/*****开始服务：状态变量初始化、socket创建与绑定、完成端口创建与关联*****/
BOOL GenericServer::Start(int nPort, int nMaxConnections, int nMaxFreeBuffers, int nMaxFreeContexts, int nInitialReads)
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
	::CreateIoCompletionPort(m_sListen,m_hCompletion,(DWORD)0,0);
	/*****注册事件*****/
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