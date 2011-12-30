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
	while(!listen);
	CGPRSServer  *pIOCPServer = (CGPRSServer*)pParam;
	HANDLE hIOCP = pIOCPServer ->m_hCompletion;

/////////////////测试部分/////////////////////////////////////////////////
	CUserRecordset m_UserRS;
	CUserCommand m_UserCmd;
	CHRLightCommand m_HRLightCmd;
	CAdminRecordset m_AdminRst;
	CHRRoadCommand m_HRRoadCmd;
	CHRTerminalCommand m_HRTerminalCmd;
	CLightRecordset m_LightRs;
	CWarningInfoRecordset m_WarningInfoRs;
	CWarningInfoCommand m_WarningInfoCmd;
	CTerminalRecordset m_TerminalRs;
	CRoadRecordset m_RoadRs;
	CHROperationCommand m_HROperationCmd;
	CHROperationRecordset m_HROperationRs;
	CHRLightRecordset m_HRLightRs;
	CAdminCommand m_AdminCmd;
	CAreaCommand m_AreaCmd;

	m_HRLightCmd.m_cnn=pIOCPServer->m_conn;
	m_AdminRst.m_cnn=pIOCPServer->m_conn;
	m_HRRoadCmd.m_cnn=pIOCPServer->m_conn;
	m_HRTerminalCmd.m_cnn=pIOCPServer->m_conn;
	m_UserCmd.m_cnn=pIOCPServer->m_conn;
	m_LightRs.m_cnn=pIOCPServer->m_conn;
	m_UserRS.m_cnn=pIOCPServer->m_conn;
	m_WarningInfoRs.m_cnn=pIOCPServer->m_conn;
	m_WarningInfoCmd.m_cnn=pIOCPServer->m_conn;
	m_TerminalRs.m_cnn=pIOCPServer->m_conn;
	m_RoadRs.m_cnn=pIOCPServer->m_conn;
	m_HROperationCmd.m_cnn=pIOCPServer->m_conn;
	m_HROperationRs.m_cnn=pIOCPServer->m_conn;
	m_HRLightRs.m_cnn=pIOCPServer->m_conn;
	m_AdminCmd.m_cnn=pIOCPServer->m_conn;
	m_AreaCmd.m_cnn=pIOCPServer->m_conn;
//////////////////////////////////////////////////////////////////////////////

	
	BOOL bSucess = false;
	DWORD dwIOSize;
	LPCIOCPBuffer lpPerIOData = new CIOCPBuffer;
	ZeroMemory(lpPerIOData,sizeof(CIOCPBuffer));
	LPOVERLAPPED pOverLapped;
	LPCIOCPContext lpConnCtx;
	int nResult;
	CString buffer1;
	CString temp1;


	//////////////////////////测试/////////////////////////////////
	CString tempArea;
	CString tempName = "丁亮";
	CString tempPsw;
//	CString tempArea;
	CString tempAreaName="杭州";
	CString tempWarningInfo;
	CString tempTerminalName;
	CString tempRoadName;
	int tempGroupNum;
	CString str_tempGroupNum;
	CString temLightName;
	CString tempLightStatus;
	CString tempOprationRecord;
	CString tempLightRecord;
	////////////////////////////////////////////////////////////////


	sockaddr_in addrAccept;
	char strTemp_send[BUFFER_SIZE];
	char strTemp_recv[BUFFER_SIZE];
	ZeroMemory(&strTemp_recv,sizeof(strTemp_recv));
	ZeroMemory(&strTemp_send,sizeof(strTemp_send));

	while (1)
	{
		//发送三次心跳消息,若远程终端没有响应,则服务端移除此连接

		/*
		if  (QlistCtG->front != QlistCtG->rear)
		{
			buffer1 = pIOCPServer->OutQueue(QlistCtG);
			if (buffer1 == "error")
			{
				continue;
			}
			pIOCPServer->InitializeBuffer(lpPerIOData,SVR_IO_WRITE );
			pIOCPServer->pClient->pPerIOData->wbuf.buf = (LPTSTR)(LPCTSTR)buffer1;
			pIOCPServer->pClient->pPerIOData->wbuf.len = buffer1.GetLength();
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
			
		}*/
/*		m_UserRS.m_cnn=pIOCPServer->m_conn;*/

		/////////////////////////测试通过部分//////////////////////////////////////////////
//		m_HRLightCmd.AddHistoryRecordOfLight("1990",0);
//		tempArea=m_UserRS.GetAreaNameByUserName(tempName);
//		m_UserRS.Close();
//		tempPsw=m_AdminRst.GetPswByAdminName(tempName);
//		m_HRRoadCmd.AddHistoryRecordOfRoad("1990",0);
//		m_HRTerminalCmd.AddHistoryRecordOfTerminal(10,"1","1","1","1","1","1","1","1","1","1","1","1");
//		m_UserCmd.ChangeUserName("dingliang","丁亮");
//		m_UserCmd.DeleteUser("丁亮");
//		m_UserCmd.NewUser("张三","111111","杭州电子科技大学");
//		m_UserCmd.ChangeUserArea("dl","杭电");
//		m_UserCmd.ChangeUserPsw("张三","123456789");
//		tempArea=m_UserRS.GetAreaNameByUserName(tempName);
//		tempPsw=m_UserRS.GetPswByUserName(tempName);
//		m_UserRS.SetAreaName("丁亮",tempAreaName);
//		m_WarningInfoCmd.AddWarningIfo(1001,"12/24/2011 12:00:00","1","灯坏了");
//		tempWarningInfo=m_WarningInfoRs.GetWaringInfo("2011-12-14 12:00:00","2011-12-29 12:00:00");
//		m_TerminalRs.SetTerminalName(1,"丁亮");
//		tempTerminalName=m_TerminalRs.GetTerminalName(1);
//		m_RoadRs.SetRoadName(1,"杭州电子科技大学");
//		tempRoadName=m_RoadRs.GetRoadName(1);
//		tempGroupNum=m_LightRs.GetGroupNum(3);
//		str_tempGroupNum.Format("%d",tempGroupNum);
//		temLightName=m_LightRs.GetLightName(2);
//		tempLightStatus=m_LightRs.GetLightStatus(3);
//		m_LightRs.SetGoupNum(1,1000);
//		m_LightRs.SetLightName(0,"1000号灯");
//		m_LightRs.SetLightStatus(3,1);
//		m_HROperationCmd.AddOperationRecord("2011-12-26 12:00:00","关闭1000号灯");
//		tempOprationRecord=m_HROperationRs.GetOpertionRecord("2011-12-14 12:00:00","2011-12-29 12:00:00");
//		 tempLightRecord=m_HRLightRs.GetLightRecord("2号灯");
//		m_AdminCmd.ChangeAdminPsw("丁亮","123456789");
//		m_AdminCmd.DeleteAdmin("吴红生");
//		m_AdminCmd.NewAdmin("dingliang","1234567890");
		m_AreaCmd.AddArea("10000000","下沙","一号路，二号路，三号路");
///////////////////////////////////////////////////////////////////////////////////////////////////

 		pIOCPServer->InitializeBuffer(lpPerIOData,SVR_IO_WRITE );
 		pIOCPServer->pClient->pPerIOData->wbuf.buf = (LPTSTR)(LPCTSTR)tempLightRecord;
 		pIOCPServer->pClient->pPerIOData->wbuf.len =tempLightRecord.GetLength();
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
		//////////////////////////////////////////测试部分///////////////////////////////
		m_AdminRst.Close();
		m_UserRS.Close();
		m_TerminalRs.Close();
		m_RoadRs.Close();
		m_LightRs.Close();
		
		////////////////////////////////////////////////////////////////////////////////////
		
// 		Sleep(100);
// 		bSucess = GetQueuedCompletionStatus(hIOCP,
// 			&dwIOSize,
// 			(LPDWORD)&lpConnCtx,
// 			&pOverLapped,
// 			500);    
// 		if ((!bSucess) &&(GetLastError() ==  WAIT_TIMEOUT))
// 		{
// 			continue;
// 		}
// 		if (lpConnCtx == NULL)
// 		{
// 			return -1;
// 		}
// 		if(dwIOSize == -1)  //用户通知退出
// 		{
// 			::ExitThread(0);
// 		}
// 		lpPerIOData = (LPCIOCPBuffer)(pOverLapped);
// 
// 		if (!bSucess||(bSucess&&(dwIOSize==0)))
// 		{
// 			if ((lpConnCtx->sockAccept != INVALID_SOCKET)/*&&(lpConnCtx->LogIn == TRUE || lpConnCtx->LogIn == FALSE)*/)
// 			{
// 				addrAccept = lpConnCtx->addrAccept;
// 				pIOCPServer->ConnListRemove(lpConnCtx);
// 				GetDlgItemText(H_ServerDlg,IDC_EDIT2,strTemp_send,BUFFER_SIZE);
// 				pIOCPServer->strSend.Format("客户端退出 :%s\r\n",inet_ntoa(addrAccept.sin_addr));
// 				pIOCPServer->strSend+=(CString)strTemp_send;
// 				SetDlgItemText(H_ServerDlg,IDC_EDIT2,pIOCPServer->strSend);
// 			}			
// 			continue;
// 		}
// 		switch(lpPerIOData->oper)
// 		{
// 		case SVR_IO_WRITE:
// 			/*pIOCPServer->strSend = lpPerIOData->wbuf.buf;
// 			GetDlgItemText(H_ServerDlg,IDC_EDIT2,strTemp_send,BUFFER_SIZE);
// 			pIOCPServer->strSend+=(CString)strTemp_send;
// 			SetDlgItemText(H_ServerDlg,IDC_EDIT2,pIOCPServer->strSend);*/
// 			pIOCPServer->InitializeBuffer(lpPerIOData, SVR_IO_READ);
// 			nResult=WSARecv(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,&(lpPerIOData->flags),&(lpPerIOData->OverLapped),NULL);
// 			if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
// 			{
// 				pIOCPServer->ConnListRemove(lpConnCtx);
// 			}
// 			break;
// 
// 		case SVR_IO_READ:
// 			pIOCPServer->strRecv = lpPerIOData->wbuf.buf;
// 			GetDlgItemText(H_ServerDlg,IDC_EDIT2,strTemp_recv,BUFFER_SIZE);			
// 			pIOCPServer->strRecv += "\r\n";
// 			pIOCPServer->strRecv+=(CString)strTemp_recv;
// 			SetDlgItemText(H_ServerDlg,IDC_EDIT2,pIOCPServer->strRecv);
// 			
// 			pIOCPServer->InQueue(QlistGtC,lpPerIOData->wbuf.buf);
// 			pIOCPServer->InitializeBuffer(lpPerIOData, SVR_IO_READ);
// 			nResult=WSARecv(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,&(lpPerIOData->flags),&(lpPerIOData->OverLapped),NULL);
// 			if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
// 			{
// 				pIOCPServer->ConnListRemove(lpConnCtx);
// 			}
// 			/*lpPerIOData->wbuf.buf = "China!\n";
// 			lpPerIOData->wbuf.len=18;
// 			lpPerIOData->oper=SVR_IO_WRITE;
// 			lpPerIOData->flags=0;
// 			nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
// 			if(nResult==SOCKET_ERROR && WSAGetLastError()!=ERROR_IO_PENDING)
// 			{
// 				pIOCPServer->ConnListRemove(lpConnCtx);
// 			}*/
// 			break;
// 		default: break;
// 		}
  	}
	delete lpPerIOData;
	CoUninitialize();
	return TRUE;
}

//
//发送心跳消息，发送三次，若在前两次收到GPRS端的连接，则继续，若未收到回复，则断开此链接
//
void CGPRSServer::SendHeartBeeatMsg()
{

}

