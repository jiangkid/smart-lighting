#include "StdAfx.h"
#include "GPRSServer.h"
//#include "InitSock.h"
#include <process.h>
#include "ServerDlg.h"
#include "ServerDataSet.h"
CGPRSServer     _GPRSServer;
CGPRSServer     *pGprsServer = &_GPRSServer;

CGPRSServer::CGPRSServer(void)
{
	m_evtThreadLanched=CreateEvent(NULL,FALSE,FALSE,NULL);
	m_evtSvrToStop=CreateEvent(NULL,FALSE,FALSE,NULL);

	//��ʼ���ٽ�������
	InitializeCriticalSection(&m_CriticalSection);

	//��sysInfo.dwNumberOfProcessors*2��MAX_THREAD_NUM֮��Ľ�Сֵ����m_nSvcThreadNum;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	m_nSvcThreadNum=sysInfo.dwNumberOfProcessors*2 < MAX_THREAD ? (sysInfo.dwNumberOfProcessors*2):MAX_THREAD;
}

CGPRSServer::~CGPRSServer(void)
{
	CloseHandle(m_evtThreadLanched);
	CloseHandle(m_evtSvrToStop);
	DeleteCriticalSection(&m_CriticalSection);
}

//
//����������
//
BOOL CGPRSServer::StartServer(int nPort)
{
	m_bServerStarted = TRUE;
	Initialize_Socket(nPort);   //��ʼ���׽���
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
	m_hThreadList[1] =m_hListenThread;
	for (UINT i = 2; i < m_nSvrThreadNum+2; ++i)
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
//ֹͣ������
//
BOOL CGPRSServer::StopServer()
{
	if(m_bServerStarted==FALSE)
		return FALSE;

	//�����¼�����m_evtSvrToStop��֪ͨ�����߳���ֹ������
	SetEvent(m_evtSvrToStop);

	//�ȴ������߳̽���
	WaitForSingleObject(m_hThreadList[0],WAIT4THREAD_MILLISECS*3);

	//�������߳�״̬��������߳��Դ�������״̬������ǿ����ֹ
	DWORD nExitCode;
	GetExitCodeThread(m_hThreadList[0],&nExitCode);
	if(nExitCode==STILL_ACTIVE)
	{
		TerminateThread(m_hThreadList[0],1);
	}
	return TRUE;
}

//
//�����������߳�
//
DWORD WINAPI CGPRSServer::AdminThread(LPVOID pParam)
{
	CGPRSServer *pGPRSServer = (CGPRSServer *) pParam;
	WaitForSingleObject(pGPRSServer->m_evtSvrToStop,INFINITE);		//����m_evtSvrToStop�¼����󱻴���

	if(pGPRSServer->m_sListen!=INVALID_SOCKET)
		closesocket(pGPRSServer->m_sListen);											//������ListenThread��accept���ó�������

	if(pGPRSServer->m_hCompletion)
	{
		//֪ͨService�߳̽���
		for(UINT i=0; i<pGPRSServer->m_nSvcThreadNum;i++)
			PostQueuedCompletionStatus(pGPRSServer->m_hCompletion,0,0,NULL);
	}
	//�ȴ�����Service�̺߳ͼ����߳̽���
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
//�����������߳�
//
DWORD WINAPI CGPRSServer::ListenThread(LPVOID pParam)
{
	CGPRSServer *pGPRSServer=(CGPRSServer *) pParam;
	sockaddr_in addrRemote;
	memset(&addrRemote,0,sizeof(addrRemote));
	int Remote_len = sizeof(addrRemote);
	CString tmp;
	char history[BUFFER_SIZE];
	int nResult;
	while(TRUE)
	{		
		pGPRSServer->m_sAccept=accept(pGPRSServer->m_sListen,NULL,NULL);
		getpeername(pGPRSServer->m_sAccept,(SOCKADDR *)&addrRemote,&Remote_len); 
		if (pGPRSServer->m_sAccept == INVALID_SOCKET )
		{
			int  error =  WSAGetLastError();
			return FALSE;
		}
		::GetDlgItemText(H_ServerDlg,IDC_EDIT2,history,BUFFER_SIZE);
		tmp.Format("A guest joins us :%s\r\n",inet_ntoa(addrRemote.sin_addr));
		tmp +=(CString)history; 
		::SetDlgItemText(H_ServerDlg,IDC_EDIT2,tmp);	
		//Ϊ�½��յ��������������Ķ���
		pGPRSServer->pClient = pGPRSServer->AllocateContext(pGPRSServer->m_sAccept,addrRemote);
		if (pGPRSServer->pClient == NULL)
		{
			return FALSE;
		}
		else
			pGPRSServer->ConnListAdd(pGPRSServer->pClient);
		//Ͷ�ݳ�ʼ��IO����
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
	}
	return 0;
}

//
//�����������߳�
//
DWORD WINAPI CGPRSServer::ServiceThread(LPVOID pParam)
{
	CGPRSServer  *pIOCPServer = (CGPRSServer*)pParam;
	HANDLE hIOCP = pIOCPServer ->m_hCompletion;
	BOOL bSucess = false;
	DWORD dwIOSize;
	LPCIOCPBuffer lpPerIOData;
	LPOVERLAPPED pOverLapped;
	LPCIOCPContext lpConnCtx;
	int nResult;
	char strTemp_send[BUFFER_SIZE];
	char strTemp_recv[BUFFER_SIZE];
	while (1)
	{
		bSucess = GetQueuedCompletionStatus(hIOCP,
			&dwIOSize,
			(LPDWORD)&lpConnCtx,
			&pOverLapped,
			INFINITE);
		if (lpConnCtx == NULL)
		{
			return -1;
		}
		if(dwIOSize == -1)  //�û�֪ͨ�˳�
		{
			::ExitThread(0);
		}
		lpPerIOData = (LPCIOCPBuffer)(pOverLapped);

		if (!bSucess||(bSucess&&(dwIOSize==0)))
		{
			pIOCPServer->ConnListRemove(lpConnCtx);
			::MessageBox(NULL,"���ӶϿ���","ACCEPT",MB_OK);
			continue;
		}
		switch(lpPerIOData->oper)
		{
		case SVR_IO_WRITE:
			pIOCPServer->strSend = lpPerIOData->wbuf.buf;
			GetDlgItemText(H_ServerDlg,IDC_EDIT2,strTemp_send,BUFFER_SIZE);
			pIOCPServer->strSend+=(CString)strTemp_send;
			SetDlgItemText(H_ServerDlg,IDC_EDIT2,pIOCPServer->strSend);
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
			pIOCPServer->strRecv+=(CString)strTemp_recv;
			SetDlgItemText(H_ServerDlg,IDC_EDIT2,pIOCPServer->strRecv);

			lpPerIOData->wbuf.buf = (char*)LPCTSTR(g_pServerDataSet->GetUserName());
			//lpPerIOData->wbuf.buf
			lpPerIOData->wbuf.len=18;
			lpPerIOData->oper=SVR_IO_WRITE;
			lpPerIOData->flags=0;
			nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
			if(nResult==SOCKET_ERROR && WSAGetLastError()!=ERROR_IO_PENDING)
			{
				pIOCPServer->ConnListRemove(lpConnCtx);
			}
			break;
		default: break;
		}
	}
	return TRUE;
}

