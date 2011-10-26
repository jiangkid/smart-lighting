//////////////////////////////////////////////////
//Created at 2011/11/23 by ����
//////////////////////////////////////////////////

#include "StdAfx.h"
#include "GPRSServer.h"
#include "InitSock.h"
#include <process.h>

CGenericServer     _GenericServer;
CGenericServer     *g_pGenericServer = &_GenericServer;

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
BOOL CGPRSServer::StratServer()
{
	if (TRUE==m_bServerStarted)
	{
		return FALSE;
	}

	//���������߳�,���̸߳���������Ĺر�
	HANDLE hThread=CreateThread(NULL,0,AdminThread,this,0,NULL);
	if(hThread==NULL)
	{
		WSACleanup();
		return FALSE;
	}
	if(WaitForSingleObject(m_evtThreadLanched,WAIT4THREAD_MILLISECS)!=WAIT_OBJECT_0)
	{
		TerminateThread(hThread,1);
		CloseHandle(hThread);
		WSACleanup();
		return FALSE;
	}

	//��ȷ�������������Ĺ����߳̾�����浽m_hThreadList[0]
	m_hThreadList[0]=hThread;

	//���������߳�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	ResetEvent(m_evtThreadLanched);

	//���������߳�
	hThread=CreateThread(NULL,0,ListenThread,this,0,NULL);
	if(hThread==NULL)
	{
		//֪ͨ�Ѵ��������еĹ����߳���ֹ����
		SetEvent(m_evtSvrToStop);
		//�ȴ������߳̽���
		if(WaitForSingleObject(m_hThreadList[0],WAIT4THREAD_MILLISECS*3)!=WAIT_OBJECT_0)
			TerminateThread(m_hThreadList[0],1);

		CloseHandle(m_hThreadList[0]);
		WSACleanup();

		return FALSE;
	}

	//�ȴ������߳���������
	if(WaitForSingleObject(m_evtThreadLanched,WAIT4THREAD_MILLISECS)!=WAIT_OBJECT_0)
	{
		TerminateThread(hThread,1);
		CloseHandle(hThread);

		//֪ͨ�����߳̽���
		SetEvent(m_evtSvrToStop); 
		if(WaitForSingleObject(m_hThreadList[0],WAIT4THREAD_MILLISECS*3)!=WAIT_OBJECT_0)
			TerminateThread(m_hThreadList[0],1);
		CloseHandle(m_hThreadList[0]);
		WSACleanup();
		return FALSE;
	}

	//�������������ļ����߳̾�����浽m_hThreadList[1]
	m_hThreadList[1]=hThread;

	//���ķ�����״ֵ̬m_ServerStateΪSERVER_RUNNING
	m_bServerStarted=TRUE;

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

	WSACleanup();

	return TRUE;
}

//
//�����������߳�
//
DWORD WINAPI CGPRSServer::AdminThread(LPVOID pParam)
{
	CGPRSServer *pGPRSServer = (CGPRSServer *) pParam;

	//֪ͨStratServer������Admin�߳�������,
	SetEvent(pGPRSServer->m_evtThreadLanched);

	//�ȴ�����ָֹͣ���ָ������������»ᱻ����
	//1.����StartServer����ʱ��ListenThreadδ����������
	//2.������ListenThread���й����г���
	//3.����StopServer����

	//����m_evtSvrToStop�¼����󱻴���
	WaitForSingleObject(pGPRSServer->m_evtSvrToStop,INFINITE);

	//Do Clear Work
	if(pGPRSServer->m_sListen!=INVALID_SOCKET)
		closesocket(pGPRSServer->m_sListen);//������ListenThread��accept���ó�������

	if(pGPRSServer->m_hCompletion)
	{
		//֪ͨService�߳̽���
		for(UINT i=0; i<pGPRSServer->m_nSvcThreadNum;i++)
			PostQueuedCompletionStatus(pGPRSServer->m_hCompletion,0,0,NULL);
	}
	//�ȴ�����Service�̺߳ͼ����߳̽���
	WaitForMultipleObjects(pGPRSServer->m_nSvcThreadNum+1,&(pGPRSServer->m_hThreadList[1]),TRUE,WAIT4THREAD_MILLISECS*2);

	DWORD nExitCode;
	for(UINT j=1;j<pGPRSServer->m_nSvcThreadNum;j++)
	{
		GetExitCodeThread(pGPRSServer->m_hThreadList[j],&nExitCode);
		if(nExitCode==STILL_ACTIVE)
			TerminateThread(pGPRSServer->m_hThreadList[j],1);
	}
	return 0;
}

//
//�����������߳�
//
DWORD WINAPI CGPRSServer::ListenThread(LPVOID pParam)
{
	CGPRSServer *pGPRSServer=(CGPRSServer *) pParam;

	//����I/O��ɶ˿�
	pGPRSServer->m_hCompletion=pGPRSServer->CreateNewIoCompletionPort(0);
	if(pGPRSServer->m_hCompletion==NULL)
		return -1;

	//������������߳�
	for(UINT i=0;i<pGPRSServer->m_nSvcThreadNum;i++)
	{
		HANDLE hThread=CreateThread(NULL,0,ServiceThread,pGPRSServer,0,NULL);
		if(hThread == NULL)
			return -1;

		else
			pGPRSServer->m_hThreadList[i+2]=hThread;
	}

	//���������˿ڣ�����SO_REUSEADDRѡ��ΪTRUE���󶨱��ض˿�5002��������Listen�������м���
	pGPRSServer->m_sListen=socket(AF_INET,SOCK_STREAM,0);
	if(pGPRSServer->m_sListen==INVALID_SOCKET)
		return -1;

	BOOL bReuseAddr=TRUE;

	//�ڼ����׽ӿ�������SO_REUSEADDRѡ��,Ϊ��,�׽��ֿ���һ�����ڱ������׽���ʹ�õĵ�ַ����һ��
	if(setsockopt(pGPRSServer->m_sListen,SOL_SOCKET,SO_REUSEADDR,(char *)&bReuseAddr,sizeof(bReuseAddr))==SOCKET_ERROR)
		return -1;

	SOCKADDR_IN local;
	memset(&local,0,sizeof(local));
	local.sin_family=AF_INET;
	local.sin_port=htons(5002);
	local.sin_addr.S_un.S_addr=INADDR_ANY;
	if(bind(pGPRSServer->m_sListen,(SOCKADDR *)&local,sizeof(local))==SOCKET_ERROR)
		return -1;

	if(listen(pGPRSServer->m_sListen,5)==SOCKET_ERROR)
		return -1;

	//֪ͨStartServer�����������߳�������
	SetEvent(pGPRSServer->m_evtThreadLanched);
/*
	SOCKET sockAccept;
	LPCONN_CTX lpConnCtx;
	int nResult;

#ifdef _DEMO
	SOCKADDR_IN from;
#endif
	while(1)    //ѭ�������������ӣ���Ͷ�ݳ�ʼI/O��������
	{
		sockAccept=accept(pGenericServer->m_sdListen,NULL,NULL);
		if(sockAccept==INVALID_SOCKET)
		{
			SetEvent(pGenericServer->m_evtSvrToStop);
			return -1;
		}
#ifdef _DEMO
		memset(&from,0,sizeof(from));
		int fromlen=sizeof(from);
		getpeername(sockAccept,(SOCKADDR *)&from,&fromlen);
		AfxGetMainWnd()->PostMessage(WM_USER_CLIENT,from.sin_addr.s_addr,CLIENT_CONN_REQ);
#endif
		if(pGenericServer->m_bSvrPaused)
		{
			closesocket(sockAccept);
#ifdef _DEMO
			AfxGetMainWnd()->PostMessage(WM_USER_CLIENT,from.sin_addr.s_addr,CLIENT_REJECT);

#endif
			continue;
		}

		lpConnCtx=pGenericServer->CreateConnCtx(sockAccept,pGenericServer->m_hIOCP);
		if(lpConnCtx==NULL)
		{
			SetEvent(pGenericServer->m_evtSvrToStop);
			return -1;
		}
		else
			pGenericServer->ConnListAdd(lpConnCtx);

#ifdef _DEMO
		AfxGetMainWnd()->PostMessage(WM_USER_CLIENT,from.sin_addr.s_addr,CLIENT_ACCEPT);
#endif
		//Ͷ�ݳ�ʼI/O����
		nResult=WSARecv(sockAccept,
			&(lpConnCtx->pPerIOData->wbuf),
			1,
			NULL,
			&(lpConnCtx->pPerIOData->flags),
			&(lpConnCtx->pPerIOData->OverLapped),
			NULL
			);
		if(nResult==SOCKET_ERROR && WSAGetLastError()!=ERROR_IO_PENDING)
		{
			pGenericServer->ConnListRemove(lpConnCtx);
			continue;
		}
	}*/
	return 0;
}
DWORD WINAPI CGPRSServer::ServiceThread(LPVOID pParam)
{

	return 0;
}







