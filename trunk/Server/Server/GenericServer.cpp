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
