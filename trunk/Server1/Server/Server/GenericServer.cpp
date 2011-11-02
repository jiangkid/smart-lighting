#include "StdAfx.h"
#include "GenericServer.h"

#include "ServerDlg.h"
CGenericServer::CGenericServer(void)
{
	/*****列表*****/
	m_pFreeBufferList = NULL;				//用于在内存池中申请内存空间
	m_pFreeContextList = NULL;	
	m_pConnectionList = NULL;

	m_nFreeBufferCount = 0;
	m_nFreeContextCount = 0;
	m_nCurrentConnection = 0;

	/*****初始化临界区*****/
	::InitializeCriticalSection(&m_FreeContextListLock);
	::InitializeCriticalSection(&m_ConnectionListLock);

	/*****创立事件*****/
	m_nPort = 5005;
	m_hCompletion = NULL;			//完成端口设为空
	m_sListen = INVALID_SOCKET;	 //监听套接字设为无效套接字

	m_bShutDown = FALSE;			//服务器初始状态设置
	m_bServerStarted = FALSE;

	// 初始化WS2_32.dll
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	::WSAStartup(sockVersion, &wsaData);

	//将sysInfo.dwNumberOfProcessors*2和MAX_THREAD_NUM之间的较小值赋给m_nSvcThreadNum;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	m_nSvrThreadNum=sysInfo.dwNumberOfProcessors*2 < MAX_THREAD ? (sysInfo.dwNumberOfProcessors*2):MAX_THREAD;

}

CGenericServer::~CGenericServer(void)
{
	if(m_sListen != INVALID_SOCKET)								 //关闭监听套接字
	::closesocket(m_sListen);		
	::DeleteCriticalSection(&m_FreeContextListLock);//删除临界区
	::DeleteCriticalSection(&m_ConnectionListLock);		
	WSACleanup();
}

/*****socket创建与绑定、完成端口创建*****/
BOOL CGenericServer::Initialize_Socket(int  nPort)
{
	m_nPort = nPort;
	/*****套接字创建，绑定到本地端口，进入监听模式*****/
	m_sListen = socket(AF_INET,SOCK_STREAM,0);
	addrLocal.sin_family = AF_INET;
	addrLocal.sin_addr.S_un.S_addr = INADDR_ANY;
	addrLocal.sin_port=::htons(m_nPort);
	if(::bind(m_sListen,(sockaddr*)&addrLocal,sizeof(addrLocal)) == SOCKET_ERROR)
	{
		m_bServerStarted = FALSE;
		return FALSE;
	}
	::listen(m_sListen,300);
	m_hCompletion = CreateNewIoCompletionPort(0);
	if (m_hCompletion == NULL)
	{
		DWORD  CreIO_error = GetLastError();
		return  FALSE;
	}
	return TRUE;
}

void CGenericServer::InitializeBuffer(LPCIOCPBuffer pBuffer, IO_OPER  operation)
{
	ZeroMemory(pBuffer,sizeof(LPCIOCPBuffer));
	pBuffer->OverLapped.hEvent=NULL;
	pBuffer->OverLapped.Internal=0;
	pBuffer->OverLapped.InternalHigh=0;
	pBuffer->OverLapped.Offset=0;
	pBuffer->OverLapped.OffsetHigh=0;
	pBuffer->wbuf.buf=(char *) &(pBuffer->data);
	pBuffer->wbuf.len=BUFFER_SIZE;
	pBuffer->oper=operation;
	pBuffer->flags=0;
	return ;
}
/***************************************************************
函数功能：创建I/O完成端口
***************************************************************/
HANDLE CGenericServer::CreateNewIoCompletionPort(DWORD dwNumberOfConcurrentThreads)
{
	return (CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,dwNumberOfConcurrentThreads));
}

/***************************************************************
函数功能：将套接口与完成端口绑定
***************************************************************/
BOOL CGenericServer::AssociateWithIoCompletionPort(HANDLE hComPort,HANDLE hDevice,DWORD dwCompKey)
{
	return (CreateIoCompletionPort(hDevice,hComPort,dwCompKey,0)==hComPort);
}

/***************创建套接字上下文并关联***************/
LPCIOCPContext CGenericServer::AllocateContext(SOCKET s, SOCKADDR_IN  addraccept)
{
	CIOCPContext *pContext = new CIOCPContext;
	if (pContext == NULL)
	{
		return NULL;
	}
	::EnterCriticalSection(&m_FreeContextListLock);
	pContext->pNext=NULL;
	pContext->pPrec=NULL;
	pContext->sockAccept=s;
	ZeroMemory(pContext->pPerIOData,sizeof(CIOCPBuffer));

	pContext->pPerIOData->OverLapped.hEvent=NULL;
	pContext->pPerIOData->OverLapped.Internal=0;
	pContext->pPerIOData->OverLapped.InternalHigh=0;
	pContext->pPerIOData->OverLapped.Offset=0;
	pContext->pPerIOData->OverLapped.OffsetHigh=0;
	pContext->pPerIOData->wbuf.buf=(char *)pContext->pPerIOData->data;
	pContext->pPerIOData->wbuf.len=BUFFER_SIZE;
	pContext->pPerIOData->oper=SVR_IO_READ;
	pContext->addrAccept = addraccept;
	pContext->pPerIOData->flags=0;

	//将套接口与完成端口绑定
	if(!AssociateWithIoCompletionPort(m_hCompletion,(HANDLE) s,(DWORD) pContext))
	{
		delete pContext;
		pContext =NULL;
		return NULL;
	}
	::LeaveCriticalSection(&m_FreeContextListLock);
	if(pContext!=NULL)
	{
		pContext->sockAccept = s;
	}
	return pContext;
}
/***************释放套接字上下文***************/
void CGenericServer::ReleaseContext(CIOCPContext *pContext)
{

}

/***************向连接列表中添加一个连接***************/
void CGenericServer::ConnListAdd(LPCIOCPContext lpConnCtx)
{
	LPCIOCPContext  pTemp;
	EnterCriticalSection(&m_ConnectionListLock);
	if(m_ptrConnCtxHead==NULL)
	{
		//链表的第一个（唯一）节点
		lpConnCtx->pPrec=NULL;
		lpConnCtx->pNext=NULL;
		m_ptrConnCtxHead=lpConnCtx;
	}
	else
	{
		//加到链表头部
		pTemp=m_ptrConnCtxHead;
		m_ptrConnCtxHead=lpConnCtx;
		lpConnCtx->pPrec=NULL;
		lpConnCtx->pNext=pTemp;
		pTemp->pPrec=lpConnCtx;
	}
	LeaveCriticalSection(&m_ConnectionListLock);
}

//将连接信息lpConnCtx从CGenericServer模块维持的全局变量中删除
//在连接结束时调用
//
void CGenericServer::ConnListRemove(LPCIOCPContext  lpConnCtx)
{
	LPCIOCPContext    pPrec;
	LPCIOCPContext    pNext;

	EnterCriticalSection(&m_ConnectionListLock);
	if(lpConnCtx)
	{
		pPrec=lpConnCtx->pPrec;
		pNext=lpConnCtx->pNext;
		if((pPrec==NULL)&&(pNext==NULL))  //[*]->NULL:链表唯一节点
			m_ptrConnCtxHead=NULL;
		else if((pPrec==NULL) && (pNext!=NULL)) //[*]->[]->[]->....[]:链表首节点
		{
			pNext->pPrec=NULL;
			m_ptrConnCtxHead=pNext;
		}
		else if((pPrec!=NULL) && (pNext==NULL)) //[]->[]->...[*]:链表末节点
		{
			pPrec->pNext=NULL;
		}
		else if(pPrec && pNext)//[]->[*]->[]....[]:链表的中间节点
		{
			pPrec->pNext=pNext;
			pNext->pPrec=pPrec;
		}

		//关闭连接，释放资源
		if(lpConnCtx->sockAccept!=INVALID_SOCKET)
		{
			closesocket(lpConnCtx->sockAccept);
		}
		delete lpConnCtx;
		lpConnCtx=NULL;
	}
	LeaveCriticalSection(&m_ConnectionListLock);
	return;
}

//完成服务器退出时关闭连接、释放资源的工作。
//对全局连接信息链表中的每个节点，逐个调用ConnListRemove函数

void CGenericServer::ConnListClear()
{
	LPCIOCPContext pTemp1,pTemp2;

	EnterCriticalSection(&m_ConnectionListLock);
	pTemp1=m_ptrConnCtxHead;
	while(pTemp1)
	{
		pTemp2=pTemp1->pNext;
		ConnListRemove(pTemp1);
		pTemp1=pTemp2;
	}
	LeaveCriticalSection(&m_ConnectionListLock);
	return;
}

 void CGenericServer::ResetAll()
{
	m_ServerState=SERVER_STOP;
	m_sListen=INVALID_SOCKET;
	for(int i=0; i<MAX_THREAD+2; i++)
	{
		if(m_hThreadList[i])
			CloseHandle(m_hThreadList[i]);
		m_hThreadList[i]=NULL;
	}
	if(m_hCompletion)
	{
		CloseHandle(m_hCompletion);
		m_hCompletion=NULL;
	}
	if(m_ptrConnCtxHead)
		ConnListClear();
	m_ptrConnCtxHead=NULL;
	return ;
}
