#include "StdAfx.h"
#include "ClientServer.h"
#include "ServerDlg.h"
CClientServer  _ClientServer;
CClientServer *ClientServer = &_ClientServer;
int y;
CClientServer::CClientServer(void)
{
	m_hListenThread = NULL;			//�����߳�
	m_evtThreadLanched=CreateEvent(NULL,FALSE,FALSE,NULL);
	m_evtSvrToStop=CreateEvent(NULL,FALSE,FALSE,NULL);
}

CClientServer::~CClientServer(void)
{
	CloseHandle(m_evtThreadLanched);
	CloseHandle(m_evtSvrToStop);
	if(m_hListenThread != NULL)										//�رռ����߳�
		::CloseHandle(m_hListenThread);
}
/*****��ʼ����,״̬������ʼ��*****/
BOOL CClientServer::Start(int nPort)
{
	Initialize_Socket(nPort);   //��ʼ���׽���
	for (UINT i = 0; i < m_nSvrThreadNum; ++i)
	{
		m_hThreadList[i] = NULL;
	}
	HANDLE	hAdminThread=CreateThread(NULL,0,_AdminThread,this,0,NULL);
	if (hAdminThread == NULL)
	{
		return  FALSE;
	}
	m_hThreadList[0] = hAdminThread;
	m_hListenThread=CreateThread(NULL,0,_ListenThreadProc,this,0,NULL);
	if (m_hListenThread == NULL)
	{
		DWORD  CreThread_error = GetLastError();
		return  FALSE;
	}
	m_hThreadList[1] =m_hListenThread;
	for (UINT i = 2; i < m_nSvrThreadNum+2; ++i)
	{
		m_hThreadList[i] = CreateThread(NULL,0,_WorkerThreadProc,this,0,NULL);
		if (m_hThreadList[i] == NULL)
		{
			return  FALSE;
		}
	}	
	return TRUE;
}

DWORD WINAPI  CClientServer::_ListenThreadProc(LPVOID lpParam)
{
	sockaddr_in addrRemote;
	memset(&addrRemote,0,sizeof(addrRemote));
	int Remote_len = sizeof(addrRemote);
	CClientServer *pThis = (CClientServer *)lpParam;
	CString tmp;
	char history[BUFFER_SIZE];
	int nResult;
	while(TRUE)
	{		
		ClientServer->m_sAccept=accept(ClientServer->m_sListen,NULL,NULL);
		getpeername(ClientServer->m_sAccept,(SOCKADDR *)&addrRemote,&Remote_len); 
		if (ClientServer->m_sAccept == INVALID_SOCKET )
		{
			int  error =  WSAGetLastError();
			return FALSE;
		}
		DWORD  accepterror = GetLastError();
		//
		::GetDlgItemText(H_ServerDlg,IDC_EDIT1,history,BUFFER_SIZE);
		tmp.Format("A guest joins us :%s\r\n",inet_ntoa(addrRemote.sin_addr));
		tmp +=(CString)history; 
		::SetDlgItemText(H_ServerDlg,IDC_EDIT1,tmp);	
		//Ϊ�½��յ��������������Ķ���
		ClientServer->pClient = ClientServer->AllocateContext(ClientServer->m_sAccept,addrRemote);
		if (ClientServer->pClient == NULL)
		{
			return FALSE;
		}
		else
			ClientServer->ConnListAdd(ClientServer->pClient);
		//Ͷ�ݳ�ʼ��IO����
		nResult = WSARecv(ClientServer->m_sAccept,
										&(ClientServer->pClient->pPerIOData->wbuf), 
										1, 
										NULL,
										&(ClientServer->pClient->pPerIOData->flags),
										&(ClientServer->pClient->pPerIOData->OverLapped),
										NULL);
		if ((nResult == SOCKET_ERROR) && (WSAGetLastError() != ERROR_IO_PENDING))
		{
			ClientServer->ConnListRemove(ClientServer->pClient);
			continue;
		}
	}
	return TRUE;
}

DWORD WINAPI  CClientServer::_WorkerThreadProc(LPVOID lpParam)
{
	CClientServer  *pIOCPServer = (CClientServer*)lpParam;
	HANDLE hIOCP = pIOCPServer ->m_hCompletion;
	BOOL bSucess = false;
	DWORD dwIOSize;
	LPCIOCPBuffer lpPerIOData;
	LPOVERLAPPED pOverLapped;
	LPCIOCPContext lpConnCtx;
	int nResult;
	char strTemp_send[BUFFER_SIZE];
	char strTemp_recv[BUFFER_SIZE];
	//char str1[BUFFER_SIZE] ;
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
			GetDlgItemText(H_ServerDlg,IDC_EDIT1,strTemp_send,BUFFER_SIZE);
			pIOCPServer->strSend+=(CString)strTemp_send;
			SetDlgItemText(H_ServerDlg,IDC_EDIT1,pIOCPServer->strSend);
			ClientServer->InitializeBuffer(lpPerIOData, SVR_IO_READ);
			nResult=WSARecv(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,&(lpPerIOData->flags),&(lpPerIOData->OverLapped),NULL);
			if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
			{
				pIOCPServer->ConnListRemove(lpConnCtx);
			}
			break;

		case SVR_IO_READ:
			pIOCPServer->strRecv = lpPerIOData->wbuf.buf;
			GetDlgItemText(H_ServerDlg,IDC_EDIT1,strTemp_recv,BUFFER_SIZE);			
			pIOCPServer->strRecv+=(CString)strTemp_recv;
			SetDlgItemText(H_ServerDlg,IDC_EDIT1,pIOCPServer->strRecv);

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
		default: break;
		}
	}
	return TRUE;
}

DWORD WINAPI CClientServer::_AdminThread(LPVOID pParam)
{
	CClientServer  *pIOCPServer= (CClientServer *)pParam;

	//֪ͨStratServer������Admin�߳�������,
	SetEvent(pIOCPServer->m_evtThreadLanched);

	//�ȴ�����ָֹͣ���ָ������������»ᱻ����
	//1.����StartServer����ʱ��ListenThreadδ����������
	//2.������ListenThread���й����г���
	//3.����StopServer����

	//����m_evtSvrToStop�¼����󱻴���
	WaitForSingleObject(pIOCPServer->m_evtSvrToStop,INFINITE);

	if(pIOCPServer->m_sListen!=INVALID_SOCKET)
		closesocket(pIOCPServer->m_sListen);//������ListenThread��accept���ó�������

	if(pIOCPServer->m_hCompletion)
	{
		//֪ͨService�߳̽���
		for(UINT i=0; i<pIOCPServer->m_nSvrThreadNum;i++)
			PostQueuedCompletionStatus(pIOCPServer->m_hCompletion,0,0,NULL);
	}
	//�ȴ�����Service�̺߳ͼ����߳̽���
	::WaitForMultipleObjects(pIOCPServer->m_nSvrThreadNum+1,&(pIOCPServer->m_hThreadList[1]),TRUE,WAIT4THREAD_MILLISECS*2);

	DWORD nExitCode;
	for(UINT j=1;j<pIOCPServer->m_nSvrThreadNum;j++)
	{
		GetExitCodeThread(pIOCPServer->m_hThreadList[j],&nExitCode);
		if(nExitCode==STILL_ACTIVE)
			TerminateThread(pIOCPServer->m_hThreadList[j],1);
	}
	pIOCPServer->ResetAll();
	return TRUE;
}


