#include "StdAfx.h"
#include "GPRSServer.h"
//#include "InitSock.h"
#include <process.h>
#include "ServerDlg.h"

//////////////////////测试/////////////////////////////
#include "HRLightCommand.h" 
#include "AdminRecordset.h"
#include "HRRoadCommand.h"
#include "HRTerminalCommand.h"
#include"UserCommand.h "
#include "LightRecordset.h"
#include "WarningInfoCommand.h"
#include "WarningInfoRecordset.h"
#include "WarningInfoCommand.h"
#include "TerminalRecordset.h"
#include "RoadRecordset.h"
#include "LightRecordset.h"
#include "HROperation.h"
#include "HROperationRecordset.h"
#include "HRLightRecordset.h"
#include "AdminCommand.h"
#include "AreaCommand.h"
#include "AreaRecordset.h"
//////////////////////////////////////////////////////

//CGPRSServer    _GPRSServer;
//CGPRSServer   *pGprsServer = &_GPRSServer;

CGPRSServer::CGPRSServer(void)
{
	m_conn =new CDBConnection;
	m_evtThreadLanched=CreateEvent(NULL,FALSE,FALSE,NULL);
	m_evtSvrToStop=CreateEvent(NULL,FALSE,FALSE,NULL);

	//初始化临界区对象
	InitializeCriticalSection(&m_CriticalSection);

	//将sysInfo.dwNumberOfProcessors*2和MAX_THREAD_NUM之间的较小值赋给m_nSvcThreadNum;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	m_nSvcThreadNum=sysInfo.dwNumberOfProcessors*2 < MAX_THREAD ? (sysInfo.dwNumberOfProcessors*2):MAX_THREAD;
}

CGPRSServer::~CGPRSServer(void)
{
	CloseHandle(m_evtThreadLanched);
	CloseHandle(m_evtSvrToStop);
	DeleteCriticalSection(&m_CriticalSection);
	delete m_conn;
}
int CGPRSServer::listen = 0;
SOCKET CGPRSServer::m_sAccept = 0;
//
//开启服务器
//
BOOL CGPRSServer::StartServer(int nPort)
{
// 	char buffer[16] ={'F','1','3','2','3','0','3','0','3','1','3','1','3','3','3','3'};
// 	int length = 16;
// 	char *temp;
// 	temp = Translation_ID(buffer,length);
	m_bServerStarted = TRUE;
	Initialize_Socket(nPort);   //初始化套接字
	//打开数据库连接
	if(!m_conn->Open())
	{
		::AfxMessageBox("数据库打开失败");
		return FALSE;
	}
	for (UINT i = 0; i < m_nSvrThreadNum; ++i)
	{
		m_hThreadList[i] = NULL;
	}
	HANDLE	hAdminThread=CreateThread(NULL,0,AdminThread,this,0,NULL);
	if (hAdminThread == NULL)
	{
		return  FALSE;
	}
	m_hThreadList[0] = hAdminThread;
	m_hListenThread=CreateThread(NULL,0,ListenThread,this,0,NULL);
	if (m_hListenThread == NULL)
	{
		DWORD  CreThread_error = GetLastError();
		return  FALSE;
	}
	m_hThreadList[1] =m_hListenThread;//m_nSvrThreadNum+2
	for (UINT i = 2; i <3 ; ++i)
	{
		m_hThreadList[i] = CreateThread(NULL,0,ServiceThread,this,0,NULL);
		if (m_hThreadList[i] == NULL)
		{
			return  FALSE;
		}
	}	
	return TRUE;
}

//
//停止服务器
//
BOOL CGPRSServer::StopServer()
{
	if(m_bServerStarted==FALSE)
		return FALSE;

	//触发事件对象m_evtSvrToStop，通知管理线程终止服务器
	SetEvent(m_evtSvrToStop);

	//等待管理线程结束
	WaitForSingleObject(m_hThreadList[0],WAIT4THREAD_MILLISECS*3);

	//检查管理线程状态，如果该线程仍处于运行状态，则将其强行终止
	DWORD nExitCode;
	GetExitCodeThread(m_hThreadList[0],&nExitCode);
	if(nExitCode==STILL_ACTIVE)
	{
		TerminateThread(m_hThreadList[0],1);
	}
	if (this->DestroyQueue(QlistGtC) != TRUE)
	{
		return FALSE;
	}
	return TRUE;
}

//
//服务器管理线程
//
DWORD WINAPI CGPRSServer::AdminThread(LPVOID pParam)
{
	CGPRSServer *pGPRSServer = (CGPRSServer *) pParam;
	WaitForSingleObject(pGPRSServer->m_evtSvrToStop,INFINITE);		//阻塞m_evtSvrToStop事件对象被触发

	if(pGPRSServer->m_sListen!=INVALID_SOCKET)
		closesocket(pGPRSServer->m_sListen);											//将导致ListenThread的accept调用出错并结束

	if(pGPRSServer->m_hCompletion)
	{
		//通知Service线程结束
		for(UINT i=0; i<pGPRSServer->m_nSvcThreadNum;i++)
			PostQueuedCompletionStatus(pGPRSServer->m_hCompletion,0,0,NULL);
	}
	//等待所有Service线程和监听线程结束
	WaitForMultipleObjects(pGPRSServer->m_nSvcThreadNum+1,&(pGPRSServer->m_hThreadList[1]),TRUE,WAIT4THREAD_MILLISECS*2);

	DWORD nExitCode;
	for(UINT j=1;j<pGPRSServer->m_nSvcThreadNum+2;j++)
	{
		GetExitCodeThread(pGPRSServer->m_hThreadList[j],&nExitCode);
		if(nExitCode==STILL_ACTIVE)
			TerminateThread(pGPRSServer->m_hThreadList[j],1);
	}
	return TRUE;
}

//
//服务器监听线程
//
DWORD WINAPI CGPRSServer::ListenThread(LPVOID pParam)
{
	CGPRSServer *pGPRSServer=(CGPRSServer *) pParam;
	sockaddr_in addrRemote;
	memset(&addrRemote,0,sizeof(addrRemote));
	int Remote_len = sizeof(addrRemote);
	CString tmp, temp;
	CString buffer1;
	char history[BUFFER_SIZE];
	int nResult;
	while(TRUE)
	{	
		pGPRSServer->m_sAccept=accept(pGPRSServer->m_sListen,NULL,NULL);
		getpeername(pGPRSServer->m_sAccept,(SOCKADDR *)&addrRemote,&Remote_len); 
		if (pGPRSServer->m_sAccept == INVALID_SOCKET )
		{
			return FALSE;
		}
		::GetDlgItemText(H_ServerDlg,IDC_EDIT2,history,BUFFER_SIZE);
		tmp.Format("A guest joins us :%s\r\n",inet_ntoa(addrRemote.sin_addr));
		tmp +=(CString)history; 
		::SetDlgItemText(H_ServerDlg,IDC_EDIT2,tmp);	
		//为新接收的连接申请上下文对象
		pGPRSServer->pClient = pGPRSServer->AllocateContext(pGPRSServer->m_sAccept,addrRemote);
		if (pGPRSServer->pClient == NULL)
		{
			return FALSE;
		}
		else
			pGPRSServer->ConnListAdd(pGPRSServer->pClient);
		//投递初始的IO操作
		nResult = WSARecv(pGPRSServer->m_sAccept,
			&(pGPRSServer->pClient->pPerIOData->wbuf), 
			1, 
			NULL,
			&(pGPRSServer->pClient->pPerIOData->flags),
			&(pGPRSServer->pClient->pPerIOData->OverLapped),
			NULL);
		if ((nResult == SOCKET_ERROR) && (WSAGetLastError() != ERROR_IO_PENDING))
		{
			pGPRSServer->ConnListRemove(pGPRSServer->pClient);
			continue;
		}
	listen = 1;
	}
	return 0;
}


DWORD WINAPI CGPRSServer::ServiceThread(LPVOID pParam)
{	
	::CoInitialize(NULL);
	while(!listen)
	{
		Sleep(100);
	}
	CGPRSServer  *pIOCPServer = (CGPRSServer*)pParam;
	HANDLE hIOCP = pIOCPServer ->m_hCompletion;	
	BOOL bSucess = false;
	DWORD dwIOSize;
	LPCIOCPBuffer lpPerIOData = new CIOCPBuffer;
	ZeroMemory(lpPerIOData,sizeof(CIOCPBuffer));
	LPOVERLAPPED pOverLapped;
	LPCIOCPContext lpConnCtx;
	int nResult = 0;
	int length = 0;
	CString buffer1;
	CString temp1;
	sockaddr_in addrAccept;
	char *recv;
	char strTemp_send[BUFFER_SIZE];
	char strTemp_recv[BUFFER_SIZE];
	ZeroMemory(&strTemp_recv,sizeof(strTemp_recv));
	ZeroMemory(&strTemp_send,sizeof(strTemp_send));

	while (1)
	{
		//发送三次心跳消息,若远程终端没有响应,则服务端移除此连接		
		if  (QlistCtG->front != QlistCtG->rear)
		{
			length = 0;
			recv = pIOCPServer->OutQueue(QlistCtG,length);
			if (buffer1 == "error")
			{
				continue;
			}
			
			pIOCPServer->InitializeBuffer(lpPerIOData,SVR_IO_WRITE );
			pIOCPServer->pClient->pPerIOData->wbuf.buf = recv;
			pIOCPServer->pClient->pPerIOData->wbuf.len = length;
			pIOCPServer->pClient->pPerIOData->oper = SVR_IO_WRITE;
			pIOCPServer->pClient->pPerIOData->flags = 0;
			nResult=WSASend(pIOCPServer->m_sAccept, 
				&(pIOCPServer->pClient->pPerIOData->wbuf), 
				1, 
				NULL,
				pIOCPServer->pClient->pPerIOData->flags,
				&(pIOCPServer->pClient->pPerIOData->OverLapped),
				NULL);
			if((nResult==SOCKET_ERROR) && (WSAGetLastError()!=ERROR_IO_PENDING))
			{
				MessageBox(NULL, "GPRS发送数据失败", "提示", MB_OK);
			}
			GetDlgItemText(H_ServerDlg,IDC_EDIT2,strTemp_send,BUFFER_SIZE);
			buffer1 += "\r\n";
			buffer1 += strTemp_send;
			SetDlgItemText(H_ServerDlg,IDC_EDIT2, buffer1);
		}		
 		Sleep(100);
 		bSucess = GetQueuedCompletionStatus(hIOCP,
 			&dwIOSize,
 			(LPDWORD)&lpConnCtx,
 			&pOverLapped,
 			500);    
 		if ((!bSucess) &&(GetLastError() ==  WAIT_TIMEOUT))
 		{
 			continue;
 		}
 		if (lpConnCtx == NULL)
 		{
 			return -1;
 		}
 		if(dwIOSize == -1)  //用户通知退出
 		{
 			::ExitThread(0);
 		}
 		lpPerIOData = (LPCIOCPBuffer)(pOverLapped);
 
 		if (!bSucess||(bSucess&&(dwIOSize==0)))
 		{
			/***************此处存在bug****************/
 			if ((lpConnCtx->sockAccept != INVALID_SOCKET)&&((lpConnCtx->pPerIOData->oper == SVR_IO_READ)||(lpConnCtx->pPerIOData->oper == SVR_IO_WRITE)))
 			{
 				addrAccept = lpConnCtx->addrAccept;
 				pIOCPServer->ConnListRemove(lpConnCtx);
 				GetDlgItemText(H_ServerDlg,IDC_EDIT2,strTemp_send,BUFFER_SIZE);
 				pIOCPServer->strSend.Format("客户端退出 :%s\r\n",inet_ntoa(addrAccept.sin_addr));
 				pIOCPServer->strSend+=(CString)strTemp_send;
 				SetDlgItemText(H_ServerDlg,IDC_EDIT2,pIOCPServer->strSend);
 			}			
 			continue;
 		}
 		switch(lpPerIOData->oper)
 		{
 		case SVR_IO_WRITE:
 			/*pIOCPServer->strSend = lpPerIOData->wbuf.buf;
 			GetDlgItemText(H_ServerDlg,IDC_EDIT2,strTemp_send,BUFFER_SIZE);
 			pIOCPServer->strSend+=(CString)strTemp_send;
 			SetDlgItemText(H_ServerDlg,IDC_EDIT2,pIOCPServer->strSend);*/
 			pIOCPServer->InitializeBuffer(lpPerIOData, SVR_IO_READ);
 			nResult=WSARecv(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,&(lpPerIOData->flags),&(lpPerIOData->OverLapped),NULL);
 			if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
 			{
 				pIOCPServer->ConnListRemove(lpConnCtx);
 			}

 			break;
 
 		case SVR_IO_READ:
 			pIOCPServer->strRecv = lpPerIOData->wbuf.buf;
 			GetDlgItemText(H_ServerDlg,IDC_EDIT2,strTemp_recv,BUFFER_SIZE);			
 			pIOCPServer->strRecv += "\r\n";
 			pIOCPServer->strRecv+=(CString)strTemp_recv;
 			SetDlgItemText(H_ServerDlg,IDC_EDIT2,pIOCPServer->strRecv);
 			
 			pIOCPServer->InQueue(QlistGtC,lpPerIOData->wbuf.buf,dwIOSize);
 			pIOCPServer->InitializeBuffer(lpPerIOData, SVR_IO_READ);
 			nResult=WSARecv(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,&(lpPerIOData->flags),&(lpPerIOData->OverLapped),NULL);
 			if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
 			{
 				pIOCPServer->ConnListRemove(lpConnCtx);
 			}
 			/*lpPerIOData->wbuf.buf = "China!\n";
 			lpPerIOData->wbuf.len=18;
 			lpPerIOData->oper=SVR_IO_WRITE;
 			lpPerIOData->flags=0;
 			nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
 			if(nResult==SOCKET_ERROR && WSAGetLastError()!=ERROR_IO_PENDING)
 			{
 				pIOCPServer->ConnListRemove(lpConnCtx);
 			}*/
 			break;
 		default: break;
 		}
  	}
	delete lpPerIOData;
	CoUninitialize();
	return TRUE;
}


