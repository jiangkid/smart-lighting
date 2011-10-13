// GenericServer.cpp: implementation of the GenericServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
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
}

GenericServer::~GenericServer()
{

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
