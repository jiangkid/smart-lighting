#include "StdAfx.h"
#include "IOCPServer.h"
#include "ServerDlg.h"

CIOCPServer _IOCPServer;
CIOCPServer  *g_pIOCPServer = &_IOCPServer;

CIOCPServer::CIOCPServer(void)
{
	m_evtThreadLanched=CreateEvent(NULL,FALSE,FALSE,NULL);
	m_evtSvrToStop=CreateEvent(NULL,FALSE,FALSE,NULL);

	//初始化临界区对象
	InitializeCriticalSection(&m_CriticalSection);

	//复位内部对象
	ResetAll(); 

	//将sysInfo.dwNumberOfProcessors*2和MAX_THREAD_NUM之间的较小值赋给m_nSvcThreadNum;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	m_nSvcThreadNum=sysInfo.dwNumberOfProcessors*2 < MAX_THREAD ? (sysInfo.dwNumberOfProcessors*2):MAX_THREAD;
}

CIOCPServer::~CIOCPServer(void)
{
	CloseHandle(m_evtThreadLanched);
	CloseHandle(m_evtSvrToStop);
	DeleteCriticalSection(&m_CriticalSection);
}

//
//开启服务器
//
BOOL CIOCPServer::SrartServer()
{
	if(m_ServerState==SERVER_RUNNING)
		return FALSE;

	//step.1 初始化Winsock
	WSAData wsaData;
	if(WSAStartup(WINSOCK_VERSION,&wsaData)!=0)
		return FALSE;
	ResetAll();

	//step.2 启动管理线程，该线程负责服务器的关闭工作
	HANDLE hThread=CreateThread(NULL,0,AdminThread,this,0,NULL);
	if(hThread==NULL)
	{
		WSACleanup();
		ResetAll();
		return FALSE;
	}
	if(WaitForSingleObject(m_evtThreadLanched,WAIT4THREAD_MILLISECS)!=WAIT_OBJECT_0)
	{
		TerminateThread(hThread,1);
		CloseHandle(hThread);
		WSACleanup();
		ResetAll();
		return FALSE;
	}

	//将确定已正常启动的管理线程句柄保存到m_hThreadList[0]
	m_hThreadList[0]=hThread;

	//Step.3启动监听线程                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	ResetEvent(m_evtThreadLanched);

	//创建监听线程
	hThread=CreateThread(NULL,0,ListenThread,this,0,NULL);
	if(hThread==NULL)
	{
		//通知已处于运行中的管理线程终止服务
		SetEvent(m_evtSvrToStop);
		//等待管理线程结束
		if(WaitForSingleObject(m_hThreadList[0],WAIT4THREAD_MILLISECS*3)!=WAIT_OBJECT_0)
			TerminateThread(m_hThreadList[0],1);

		CloseHandle(m_hThreadList[0]);
		WSACleanup();
		ResetAll();
		return FALSE;
	}

	//等待监听线程正常结束
	if(WaitForSingleObject(m_evtThreadLanched,WAIT4THREAD_MILLISECS)!=WAIT_OBJECT_0)
	{
		TerminateThread(hThread,1);
		CloseHandle(hThread);
		SetEvent(m_evtSvrToStop); //通知管理线程结束
		if(WaitForSingleObject(m_hThreadList[0],WAIT4THREAD_MILLISECS*3)!=WAIT_OBJECT_0)
			TerminateThread(m_hThreadList[0],1);
		CloseHandle(m_hThreadList[0]);
		WSACleanup();
		ResetAll();
		return FALSE;
	}

	//将已正常启动的监听线程句柄保存到m_hThreadList[1]
	m_hThreadList[1]=hThread;

	//更改服务器状态值m_ServerState为SERVER_RUNNING
	m_ServerState=SERVER_RUNNING;

	return TRUE;
}

//
//停止服务器
//
BOOL CIOCPServer::StopServer()
{
	if(m_ServerState==SERVER_STOP)
		return FALSE;

	//触发事件对象m_evtSvrToStop，通知管理线程终止服务器
	SetEvent(m_evtSvrToStop);

	//等待管理线程结束
	WaitForSingleObject(m_hThreadList[0],WAIT4THREAD_MILLISECS*3);

	//检查管理线程状态，如果该线程仍处于运行状态，则将其强行终止
	DWORD nExitCode;
	GetExitCodeThread(m_hThreadList[0],&nExitCode);
	if(nExitCode==STILL_ACTIVE)
		TerminateThread(m_hThreadList[0],1);
	ResetAll();
	WSACleanup();

	return TRUE;
}

//
//将系统中的所有成员变量全部复位，并释放运行态时占用的资源
//用于服务器的初始化或者重启
//
void CIOCPServer::ResetAll()
{
	m_ServerState=SERVER_STOP;
	m_sdListen=INVALID_SOCKET;
	for(int i=0; i<MAX_THREAD+2; i++)
	{
		if(m_hThreadList[i])
			CloseHandle(m_hThreadList[i]);
		m_hThreadList[i]=NULL;
	}
	if(m_hIOCP)
	{
		CloseHandle(m_hIOCP);
		m_hIOCP=NULL;
	}
	if(m_ptrConnCtxHead)
		ConnListClear();

	//复位事件对象m_evtThreadLanched和m_evtSvrToStop
	ResetEvent(m_evtThreadLanched);
	ResetEvent(m_evtSvrToStop);

	m_ptrConnCtxHead=NULL;
}

//
//创建I/O完成端口
//
HANDLE CIOCPServer::CreateNewIoCompletionPort(DWORD dwNumberOfConcurrentThreads)
{
	return (CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,dwNumberOfConcurrentThreads));
}

//
//将套接口与完成端口绑定
//
BOOL CIOCPServer::AssociateWithIoCompletionPort(HANDLE hComPort,HANDLE hDevice,DWORD dwCompKey)
{
	return (CreateIoCompletionPort(hDevice,hComPort,dwCompKey,0)==hComPort);
}

//
//  主要完成以下两项工作:
//  1.将sockAccept与完成端口hIOCP绑定
//  2.为新来的客户端连接创建CONN_CTX数据
//
LPCONN_CTX CIOCPServer::CreateConnCtx(SOCKET sockAccept,HANDLE hIOCP)
{
	LPCONN_CTX lpConnCtx=new CONN_CTX;
	if(lpConnCtx==NULL)
		return NULL;

	//赋值
	lpConnCtx->pNext=NULL;
	lpConnCtx->pPrec=NULL;
	lpConnCtx->sockAccept=sockAccept;
	ZeroMemory(lpConnCtx->pPerIOData,sizeof(PER_IO_DATA));

	lpConnCtx->pPerIOData->OverLapped.hEvent=NULL;
	lpConnCtx->pPerIOData->OverLapped.Internal=0;
	lpConnCtx->pPerIOData->OverLapped.InternalHigh=0;
	lpConnCtx->pPerIOData->OverLapped.Offset=0;
	lpConnCtx->pPerIOData->OverLapped.OffsetHigh=0;
	lpConnCtx->pPerIOData->wbuf.buf=(char *)lpConnCtx->pPerIOData->data;
	lpConnCtx->pPerIOData->wbuf.len=BUFFER_SIZE;
	lpConnCtx->pPerIOData->oper=SVR_IO_READ;
	lpConnCtx->pPerIOData->flags=0;

	//将套接口与完成端口绑定
	if(!AssociateWithIoCompletionPort(m_hIOCP,(HANDLE) sockAccept,(DWORD) lpConnCtx))
	{
		delete lpConnCtx;
		lpConnCtx=NULL;
		return NULL;
	}
	return lpConnCtx;
}

//
//将连接信息lpConnCtx加入到CGenericServer模块维持的全局链接列表
//
void CIOCPServer::ConnListAdd(LPCONN_CTX lpConnCtx)
{
	LPCONN_CTX pTemp;

	EnterCriticalSection(&m_CriticalSection);

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
	LeaveCriticalSection(&m_CriticalSection);
}

//
//将连接信息lpConnCtx从CGenericServer模块维持的全局变量中删除
//在连接结束时调用
//
void CIOCPServer::ConnListRemove(LPCONN_CTX lpConnCtx)
{
	LPCONN_CTX pPrec;
	LPCONN_CTX pNext;

	EnterCriticalSection(&m_CriticalSection);
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
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
#ifdef _DEBUG
			SOCKADDR_IN from;
			memset(&from,0,sizeof(from));
			int fromlen=sizeof(from);

			//调用getpeername函数获取lpConnCtx对应的连接的客户端的IP地址
			getpeername(lpConnCtx->sockAccept,(SOCKADDR *)&from,&fromlen);

			//向主窗体发送消息VM_USER_CLIENT
			AfxGetMainWnd()->PostMessage(WM_USER_CLIENT,from.sin_addr.s_addr,CLIENT_DISCONNECT);
#endif // _DEBUG
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
			closesocket(lpConnCtx->sockAccept);
		}
		delete lpConnCtx;
		lpConnCtx=NULL;
	}
	LeaveCriticalSection(&m_CriticalSection);
	return;
}

//
//完成服务器退出时关闭连接、释放资源的工作。
//对全局连接信息链表中的每个节点，逐个调用ConnListRemove函数
//
void CIOCPServer::ConnListClear()
{
	LPCONN_CTX pTemp1,pTemp2;

	EnterCriticalSection(&m_CriticalSection);
	pTemp1=m_ptrConnCtxHead;
	while(pTemp1)
	{
		pTemp2=pTemp1->pNext;
		ConnListRemove(pTemp1);
		pTemp1=pTemp2;
	}
	LeaveCriticalSection(&m_CriticalSection);
	return;
}

//
//服务器管理线程
//
DWORD WINAPI CIOCPServer::AdminThread(LPVOID pParam)
{
	CIOCPServer *pIOCPServer = (CIOCPServer *) pParam;

	//通知StratServer函数，Admin线程已启动,
	SetEvent(pIOCPServer->m_evtThreadLanched);

	//等待服务停止指令，该指令在三种情况下会被触发
	//1.调用StartServer函数时，ListenThread未能正常启动
	//2.服务器ListenThread运行过程中出错
	//3.调用StopServer函数

	//阻塞m_evtSvrToStop事件对象被触发
	WaitForSingleObject(pIOCPServer->m_evtSvrToStop,INFINITE);

	//Do Clear Work
	if(pIOCPServer->m_sdListen!=INVALID_SOCKET)
		closesocket(pIOCPServer->m_sdListen);//将导致ListenThread的accept调用出错并结束

	if(pIOCPServer->m_hIOCP)
	{
		//通知Service线程结束
		for(UINT i=0; i<pIOCPServer->m_nSvcThreadNum;i++)
			PostQueuedCompletionStatus(pIOCPServer->m_hIOCP,0,0,NULL);
	}
	//等待所有Service线程和监听线程结束
	WaitForMultipleObjects(pIOCPServer->m_nSvcThreadNum+1,&(pIOCPServer->m_hThreadList[1]),TRUE,WAIT4THREAD_MILLISECS*2);

	DWORD nExitCode;
	for(UINT j=1;j<pIOCPServer->m_nSvcThreadNum;j++)
	{
		GetExitCodeThread(pIOCPServer->m_hThreadList[j],&nExitCode);
		if(nExitCode==STILL_ACTIVE)
			TerminateThread(pIOCPServer->m_hThreadList[j],1);
	}

	pIOCPServer->ResetAll();
	return 0;
}

//
//服务器监听线程
//
DWORD WINAPI CIOCPServer::ListenThread(LPVOID pParam)
{
	CIOCPServer *pIOCPServer=(CIOCPServer *) pParam;

	//创建I/O完成端口
	pIOCPServer->m_hIOCP=pIOCPServer->CreateNewIoCompletionPort(0);
	if(pIOCPServer->m_hIOCP==NULL)
		return -1;

	//创建多个工作线程
	for(UINT i=0;i<pIOCPServer->m_nSvcThreadNum;i++)
	{
		HANDLE hThread=CreateThread(NULL,0,ServiceThread,pIOCPServer,0,NULL);
		if(hThread == NULL)
			return -1;

		else
			pIOCPServer->m_hThreadList[i+2]=hThread;
	}

	//创建监听端口，设置SO_REUSEADDR选项为TRUE，绑定本地端口5002，并调用Listen函数进行监听
	pIOCPServer->m_sdListen=socket(AF_INET,SOCK_STREAM,0);
	if(pIOCPServer->m_sdListen==INVALID_SOCKET)
		return -1;

	BOOL bReuseAddr=TRUE;

	//在监听套接口上启用SO_REUSEADDR选项,为真,套接字可与一个正在被其他套接字使用的地址绑定在一起
	if(setsockopt(pIOCPServer->m_sdListen,SOL_SOCKET,SO_REUSEADDR,(char *)&bReuseAddr,sizeof(bReuseAddr))==SOCKET_ERROR)
		return -1;

	SOCKADDR_IN local;
	memset(&local,0,sizeof(local));
	local.sin_family=AF_INET;
	local.sin_port=htons(5002);
	local.sin_addr.S_un.S_addr=INADDR_ANY;
	if(bind(pIOCPServer->m_sdListen,(SOCKADDR *)&local,sizeof(local))==SOCKET_ERROR)
		return -1;

	if(listen(pIOCPServer->m_sdListen,5)==SOCKET_ERROR)
		return -1;

	//通知StartServer函数，监听线程已启动
	SetEvent(pIOCPServer->m_evtThreadLanched);

	SOCKET sockAccept;
	LPCONN_CTX lpConnCtx;
	int nResult;
 //   SOCKADDR_IN acceptAddr;
//	char tmp;

///////////////////////////////////////////////
#ifdef _DEBUG
		SOCKADDR_IN from;
#endif // _DEBUG
//////////////////////////////////////////////

	while(1)    //循环接受外来连接，并投递初始I/O操作请求
	{
		sockAccept=accept(pIOCPServer->m_sdListen,NULL,NULL);

//		tmp.Format("%s\r\n",inet_ntoa(acceptAddr.sin_addr));
//		SetDlgItemText(show,IDC_EDIT_RECV, tmp);
		if(sockAccept==INVALID_SOCKET)
		{
			SetEvent(pIOCPServer->m_evtSvrToStop);
			return -1;
		}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
		memset(&from,0,sizeof(from));
		int fromlen=sizeof(from);
		getpeername(sockAccept,(SOCKADDR *)&from,&fromlen); 
		AfxGetMainWnd()->PostMessage(WM_USER_CLIENT,from.sin_addr.s_addr,CLIENT_CONN_REQ);
//		AfxGetMainWnd()->PostMessage(WM_USER_CLIENT,0,0);
#endif
//		if(pIOCPServer->m_bSvrPaused)
//		{
//			closesocket(sockAccept);
#ifdef _DEMO
			AfxGetMainWnd()->PostMessage(WM_USER_CLIENT,from.sin_addr.s_addr,CLIENT_REJECT);

#endif
//			continue;
//		}

		lpConnCtx=pIOCPServer->CreateConnCtx(sockAccept,pIOCPServer->m_hIOCP);
		if(lpConnCtx==NULL)
		{
			SetEvent(pIOCPServer->m_evtSvrToStop);  
			return -1;
		}
		else
			pIOCPServer->ConnListAdd(lpConnCtx);

#ifdef _DEMO
		AfxGetMainWnd()->PostMessage(WM_USER_CLIENT,from.sin_addr.s_addr,CLIENT_ACCEPT);
#endif
		//投递初始I/O操作
		nResult=WSARecv(sockAccept,&(lpConnCtx->pPerIOData->wbuf),1,NULL,&(lpConnCtx->pPerIOData->flags),&(lpConnCtx->pPerIOData->OverLapped),NULL);
		if(nResult==SOCKET_ERROR && WSAGetLastError()!=ERROR_IO_PENDING)
		{
			pIOCPServer->ConnListRemove(lpConnCtx);
			continue;
		}
	}
	return 0;
}

//
//服务线程
//
DWORD WINAPI CIOCPServer::ServiceThread(LPVOID pParam)
{
	CIOCPServer *pIOCPServer = (CIOCPServer*)pParam;
	HANDLE hIOCP = pIOCPServer ->m_hIOCP;
	BOOL bSucess = false;
	DWORD dwIOSize;
	LPPER_IO_DATA lpPerIOData;
	LPOVERLAPPED pOverLapped;
	LPCONN_CTX lpConnCtx;
	int nResult;
	char strTemp_send[BUFFER_SIZE];
	char strTemp_recv[BUFFER_SIZE];
	char *str1 = "abc";

	while (1)
	{
		bSucess = GetQueuedCompletionStatus(hIOCP,&dwIOSize,(LPDWORD)&lpConnCtx,&pOverLapped,INFINITE);
		if (lpConnCtx == NULL)
		{
			return -1;
		}
		lpPerIOData = (LPPER_IO_DATA)(pOverLapped);

		if (!bSucess||(bSucess&&(dwIOSize==0)))
		{
			pIOCPServer->ConnListRemove(lpConnCtx);
			continue;
		}
		switch(lpPerIOData->oper)
		{
		case SVR_IO_WRITE:
//			pIOCPServer->strSend = lpPerIOData->wbuf.buf;
//			GetDlgItemText(show,IDC_EDIT_RECV,strTemp_send,BUFFER_SIZE);
//			pIOCPServer->strSend+=(CString)strTemp_send;
//			SetDlgItemText(show,IDC_EDIT_SEND,pIOCPServer->strSend);
			
			ZeroMemory(lpPerIOData,sizeof(PER_IO_DATA));
			lpPerIOData->OverLapped.hEvent=NULL;
			lpPerIOData->OverLapped.Internal=0;
			lpPerIOData->OverLapped.InternalHigh=0;
			lpPerIOData->OverLapped.Offset=0;
			lpPerIOData->OverLapped.OffsetHigh=0;
			lpPerIOData->wbuf.buf=(char *) &(lpPerIOData->data);
//			lpPerIOData->wbuf.buf= "hello,the world!";
			lpPerIOData->wbuf.len=BUFFER_SIZE;
			lpPerIOData->oper=SVR_IO_READ;
			lpPerIOData->flags=0;

			nResult=WSARecv(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,&(lpPerIOData->flags),&(lpPerIOData->OverLapped),NULL);
			
			if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
			{
				pIOCPServer->ConnListRemove(lpConnCtx);
			}
			pIOCPServer->strSend = lpPerIOData->wbuf.buf;
			GetDlgItemText(show,IDC_EDIT_SEND,strTemp_send,BUFFER_SIZE);
			pIOCPServer->strSend+=(CString)strTemp_send;
			SetDlgItemText(show,IDC_EDIT_SEND,pIOCPServer->strSend);
			break;
		
		case SVR_IO_READ:
			pIOCPServer->strRecv = lpPerIOData->wbuf.buf;
			GetDlgItemText(show,IDC_EDIT_RECV,strTemp_recv,BUFFER_SIZE);
			pIOCPServer->strRecv+=(CString)strTemp_recv;

			SetDlgItemText(show,IDC_EDIT_RECV,pIOCPServer->strRecv);
			
			if(!strcmp(lpPerIOData->wbuf.buf,str1))
			{
				memset(lpPerIOData->wbuf.buf,0,sizeof(lpPerIOData->wbuf.buf));
				lpPerIOData->wbuf.buf = "hello the world!\n";
			}

			else
				lpPerIOData->wbuf.buf = "China!\n";
			lpPerIOData->wbuf.len=18;
			lpPerIOData->oper=SVR_IO_WRITE;
			lpPerIOData->flags=0;

			nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
			if(nResult==SOCKET_ERROR && WSAGetLastError()!=ERROR_IO_PENDING)
			{
				pIOCPServer->ConnListRemove(lpConnCtx);
			}
			break;
		default: ;
		}
	}
	return 0;
}

/*
//
//服务线程
//
DWORD WINAPI CIOCPServer::ServiceThread(LPVOID pParam)
{
	CIOCPServer *pIOCPServer=(CIOCPServer *) pParam;
	HANDLE hIOCP=pIOCPServer->m_hIOCP;
 
	BOOL bSuccess=false;
	DWORD dwIOSize;
	LPPER_IO_DATA lpPerIOData;

	LPOVERLAPPED pOverLapped;
	LPCONN_CTX lpConnCtx;
	int nResult;

	CString str;
	char  strTemp[BUFFER_SIZE];

	while(1)
	{
		//调用GetQueuedCompletionStatus函数获取完成信息。
		//如果得到的完成键为空，则终止该工作线程；
		//如果GetQueuedCompletionStatus返回false或者I/O数据量为0（说明客户端断开了连接），则调用ConnListRemove清除该连接
		bSuccess=GetQueuedCompletionStatus(hIOCP,&dwIOSize,(LPDWORD)&lpConnCtx,&pOverLapped,INFINITE);
//#ifdef _DEBUG
		if(!bSuccess)
			printf("%d\n",GetLastError());
//#endif
		if(lpConnCtx==NULL)
			return -1;

		lpPerIOData=(LPPER_IO_DATA)(pOverLapped);

		if(!bSuccess||(bSuccess&&(dwIOSize==0)))
		{
			pIOCPServer->ConnListRemove(lpConnCtx);
			continue;
		}
#ifdef _DEBUG
		printf("Different way to obtain PER_IO_DATA\n");
		printf("The two one must be equal -A:%x\tB:%x\n",lpConnCtx->pPerIOData,lpPerIOData);
#endif
		//根据完成信息中的lpPerIOData->oper数据，
		//判断该完成信息所对应的服务器I/O操作类型：
		switch(lpPerIOData->oper)
		{
			case SVR_IO_WRITE: 
#ifdef _DEBUG
				printf("Socket %d send: %s\n",lpConnCtx->sockAccept,lpPerIOData->wbuf.buf);
#endif
//				 lpPerIOData->wbuf.buf="hello\n";
//				lpPerIOData->wbuf.buf;
//				SetDlgItemText(show,IDC_EDIT_RECV,lpPerIOData->wbuf.buf);
				ZeroMemory(lpPerIOData,sizeof(PER_IO_DATA));
				lpPerIOData->OverLapped.hEvent=NULL;
				lpPerIOData->OverLapped.Internal=0;
				lpPerIOData->OverLapped.InternalHigh=0;
				lpPerIOData->OverLapped.Offset=0;
				lpPerIOData->OverLapped.OffsetHigh=0;
				lpPerIOData->wbuf.buf=(char *) &(lpPerIOData->data);
				lpPerIOData->wbuf.len=BUFFER_SIZE;
				lpPerIOData->oper=SVR_IO_READ;
				lpPerIOData->flags=0;

				nResult=WSARecv(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,&(lpPerIOData->flags),&(lpPerIOData->OverLapped),NULL);

				if(nResult==SOCKET_ERROR && WSAGetLastError()!=ERROR_IO_PENDING)
				{
#ifdef _DEBUG
					printf("WSARecv: %d\n",WSAGetLastError());
#endif
					pIOCPServer->ConnListRemove(lpConnCtx);
				}
//				SetDlgItemText(show,IDC_EDIT_RECV,lpPerIOData->wbuf.buf);
				break;
			case SVR_IO_READ: 
#ifdef _DEBUG
				printf("Socket %d recv: %s\n",lpConnCtx->sockAccept,lpPerIOData->wbuf.buf);
#endif
				str = lpPerIOData->wbuf.buf;
				//////////////////////////////////////////////////////////////
//				if (str = "1")
	//			{
	//				pIOCPServer->strSend = "hello\n";
	//			}
	//			else
	//				pIOCPServer->strSend = "the world\n";
				//////////////////////////////////////////////////////////////
				str+="\n";
				GetDlgItemText(show,IDC_EDIT_RECV,strTemp,sizeof(strTemp));
				str+=(CString)strTemp;
				
    			SetDlgItemText(show,IDC_EDIT_RECV,str);

				lpPerIOData->wbuf.len=sizeof()+2;
				lpPerIOData->oper=SVR_IO_WRITE;
				lpPerIOData->flags=0;

				lpPerIOData->wbuf.buf = str;
			
				nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
			    if(nResult==SOCKET_ERROR && WSAGetLastError()!=ERROR_IO_PENDING)
			    {
#ifdef _DEBUG
					printf("WSASend: %d\n",WSAGetLastError());
#endif
					pIOCPServer->ConnListRemove(lpConnCtx);
				}
	//			SetDlgItemText(show,IDC_EDIT_RECV,lpPerIOData->wbuf.buf);
				break;
			default:;

		}
	}
	return 0;
}
*/
/*
DWORD WINAPI CIOCPServer::ServiceThread(LPVOID pParam)
{
	CIOCPServer *pIOCPServer=(CIOCPServer *) pParam;
	HANDLE hIOCP=pIOCPServer->m_hIOCP;
 
	BOOL bSuccess=false;

	while(1)
	{
		bSuccess=GetQueuedCompletionStatus(hIOCP,&(pIOCPServer->dwIOSize),(LPDWORD)&(pIOCPServer->lpConnCtx),&(pIOCPServer->pOverLapped),INFINITE);
		pIOCPServer->	lpPerIOData = (LPPER_IO_DATA)(pIOCPServer->pOverLapped);

		if(pIOCPServer->lpConnCtx==NULL)
			return -1;

		if(!bSuccess)  //调用失败
		{
			DWORD dwIOError=GetLastError();
			return dwIOError;
		}

		if (!bSuccess||(bSuccess&&(pIOCPServer->dwIOSize==0)))
		{
			pIOCPServer->ConnListRemove(pIOCPServer->lpConnCtx);
			continue;
		}

		//根据完成信息中的lpPerIOData->oper数据，
		//判断该完成信息所对应的服务器I/O操作类型：
		switch(pIOCPServer->lpPerIOData->oper)
		{ 
			case SVR_IO_WRITE: 
				pIOCPServer->Recv();
				break;

			case SVR_IO_READ: 
				pIOCPServer->Send(pIOCPServer->StrSend);
				break;

			default:;
		}
	}
	return 0;
}
*/

//
//接收数据
//
/*
char *CIOCPServer::Recv()
{
	int nResult;
	ZeroMemory(lpPerIOData,sizeof(PER_IO_DATA));
	lpPerIOData->OverLapped.hEvent=NULL;
	lpPerIOData->OverLapped.Internal=0;
	lpPerIOData->OverLapped.InternalHigh=0;
	lpPerIOData->OverLapped.Offset=0;
	lpPerIOData->OverLapped.OffsetHigh=0;
	lpPerIOData->wbuf.buf=(char *) &(lpPerIOData->data);
	lpPerIOData->wbuf.len=BUFFER_SIZE;
	lpPerIOData->oper=SVR_IO_READ;
	lpPerIOData->flags=0;

	nResult=WSARecv(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,&(lpPerIOData->flags),&(lpPerIOData->OverLapped),NULL);

	if(nResult==SOCKET_ERROR && WSAGetLastError()!=ERROR_IO_PENDING)
	{
		ConnListRemove(lpConnCtx);
		return 0;
	}
	else
	{
		return lpPerIOData->wbuf.buf;
	}

}

//
//发送数据
//
void CIOCPServer::Send(char *str)
{
	int nResult;
	lpPerIOData->wbuf.len=dwIOSize;
	lpPerIOData->oper=SVR_IO_WRITE;
	lpPerIOData->flags=0;
	lpPerIOData->wbuf.buf=str;

	nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
	if(nResult==SOCKET_ERROR && WSAGetLastError()!=ERROR_IO_PENDING)
	{
		ConnListRemove(lpConnCtx);
	}	
}

*/







