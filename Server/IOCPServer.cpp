#include "StdAfx.h"
#include "IOCPServer.h"
#include "ServerDlg.h"

CIOCPServer _IOCPServer;
CIOCPServer  *g_pIOCPServer = &_IOCPServer;

CIOCPServer::CIOCPServer(void)
{
	m_evtThreadLanched=CreateEvent(NULL,FALSE,FALSE,NULL);
	m_evtSvrToStop=CreateEvent(NULL,FALSE,FALSE,NULL);

	//��ʼ���ٽ�������
	InitializeCriticalSection(&m_CriticalSection);

	//��λ�ڲ�����
	ResetAll(); 

	//��sysInfo.dwNumberOfProcessors*2��MAX_THREAD_NUM֮��Ľ�Сֵ����m_nSvcThreadNum;
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
//����������
//
BOOL CIOCPServer::SrartServer()
{
	if(m_ServerState==SERVER_RUNNING)
		return FALSE;

	//step.1 ��ʼ��Winsock
	WSAData wsaData;
	if(WSAStartup(WINSOCK_VERSION,&wsaData)!=0)
		return FALSE;
	ResetAll();

	//step.2 ���������̣߳����̸߳���������Ĺرչ���
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

	//��ȷ�������������Ĺ����߳̾�����浽m_hThreadList[0]
	m_hThreadList[0]=hThread;

	//Step.3���������߳�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
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
		ResetAll();
		return FALSE;
	}

	//�ȴ������߳���������
	if(WaitForSingleObject(m_evtThreadLanched,WAIT4THREAD_MILLISECS)!=WAIT_OBJECT_0)
	{
		TerminateThread(hThread,1);
		CloseHandle(hThread);
		SetEvent(m_evtSvrToStop); //֪ͨ�����߳̽���
		if(WaitForSingleObject(m_hThreadList[0],WAIT4THREAD_MILLISECS*3)!=WAIT_OBJECT_0)
			TerminateThread(m_hThreadList[0],1);
		CloseHandle(m_hThreadList[0]);
		WSACleanup();
		ResetAll();
		return FALSE;
	}

	//�������������ļ����߳̾�����浽m_hThreadList[1]
	m_hThreadList[1]=hThread;

	//���ķ�����״ֵ̬m_ServerStateΪSERVER_RUNNING
	m_ServerState=SERVER_RUNNING;

	return TRUE;
}

//
//ֹͣ������
//
BOOL CIOCPServer::StopServer()
{
	if(m_ServerState==SERVER_STOP)
		return FALSE;

	//�����¼�����m_evtSvrToStop��֪ͨ�����߳���ֹ������
	SetEvent(m_evtSvrToStop);

	//�ȴ������߳̽���
	WaitForSingleObject(m_hThreadList[0],WAIT4THREAD_MILLISECS*3);

	//�������߳�״̬��������߳��Դ�������״̬������ǿ����ֹ
	DWORD nExitCode;
	GetExitCodeThread(m_hThreadList[0],&nExitCode);
	if(nExitCode==STILL_ACTIVE)
		TerminateThread(m_hThreadList[0],1);
	ResetAll();
	WSACleanup();

	return TRUE;
}

//
//��ϵͳ�е����г�Ա����ȫ����λ�����ͷ�����̬ʱռ�õ���Դ
//���ڷ������ĳ�ʼ����������
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

	//��λ�¼�����m_evtThreadLanched��m_evtSvrToStop
	ResetEvent(m_evtThreadLanched);
	ResetEvent(m_evtSvrToStop);

	m_ptrConnCtxHead=NULL;
}

//
//����I/O��ɶ˿�
//
HANDLE CIOCPServer::CreateNewIoCompletionPort(DWORD dwNumberOfConcurrentThreads)
{
	return (CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,dwNumberOfConcurrentThreads));
}

//
//���׽ӿ�����ɶ˿ڰ�
//
BOOL CIOCPServer::AssociateWithIoCompletionPort(HANDLE hComPort,HANDLE hDevice,DWORD dwCompKey)
{
	return (CreateIoCompletionPort(hDevice,hComPort,dwCompKey,0)==hComPort);
}

//
//  ��Ҫ������������:
//  1.��sockAccept����ɶ˿�hIOCP��
//  2.Ϊ�����Ŀͻ������Ӵ���CONN_CTX����
//
LPCONN_CTX CIOCPServer::CreateConnCtx(SOCKET sockAccept,HANDLE hIOCP)
{
	LPCONN_CTX lpConnCtx=new CONN_CTX;
	if(lpConnCtx==NULL)
		return NULL;

	//��ֵ
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

	//���׽ӿ�����ɶ˿ڰ�
	if(!AssociateWithIoCompletionPort(m_hIOCP,(HANDLE) sockAccept,(DWORD) lpConnCtx))
	{
		delete lpConnCtx;
		lpConnCtx=NULL;
		return NULL;
	}
	return lpConnCtx;
}

//
//��������ϢlpConnCtx���뵽CGenericServerģ��ά�ֵ�ȫ�������б�
//
void CIOCPServer::ConnListAdd(LPCONN_CTX lpConnCtx)
{
	LPCONN_CTX pTemp;

	EnterCriticalSection(&m_CriticalSection);

	if(m_ptrConnCtxHead==NULL)
	{
		//����ĵ�һ����Ψһ���ڵ�
		lpConnCtx->pPrec=NULL;
		lpConnCtx->pNext=NULL;
		m_ptrConnCtxHead=lpConnCtx;
	}
	else
	{
		//�ӵ�����ͷ��
		pTemp=m_ptrConnCtxHead;
		m_ptrConnCtxHead=lpConnCtx;
		lpConnCtx->pPrec=NULL;
		lpConnCtx->pNext=pTemp;
		pTemp->pPrec=lpConnCtx;
	}
	LeaveCriticalSection(&m_CriticalSection);
}

//
//��������ϢlpConnCtx��CGenericServerģ��ά�ֵ�ȫ�ֱ�����ɾ��
//�����ӽ���ʱ����
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
		if((pPrec==NULL)&&(pNext==NULL))  //[*]->NULL:����Ψһ�ڵ�
			m_ptrConnCtxHead=NULL;
		else if((pPrec==NULL) && (pNext!=NULL)) //[*]->[]->[]->....[]:�����׽ڵ�
		{
			pNext->pPrec=NULL;
			m_ptrConnCtxHead=pNext;
		}
		else if((pPrec!=NULL) && (pNext==NULL)) //[]->[]->...[*]:����ĩ�ڵ�
		{
			pPrec->pNext=NULL;
		}
		else if(pPrec && pNext)//[]->[*]->[]....[]:������м�ڵ�
		{
			pPrec->pNext=pNext;
			pNext->pPrec=pPrec;
		}

		//�ر����ӣ��ͷ���Դ
		if(lpConnCtx->sockAccept!=INVALID_SOCKET)
		{
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
#ifdef _DEBUG
			SOCKADDR_IN from;
			memset(&from,0,sizeof(from));
			int fromlen=sizeof(from);

			//����getpeername������ȡlpConnCtx��Ӧ�����ӵĿͻ��˵�IP��ַ
			getpeername(lpConnCtx->sockAccept,(SOCKADDR *)&from,&fromlen);

			//�������巢����ϢVM_USER_CLIENT
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
//��ɷ������˳�ʱ�ر����ӡ��ͷ���Դ�Ĺ�����
//��ȫ��������Ϣ�����е�ÿ���ڵ㣬�������ConnListRemove����
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
//�����������߳�
//
DWORD WINAPI CIOCPServer::AdminThread(LPVOID pParam)
{
	CIOCPServer *pIOCPServer = (CIOCPServer *) pParam;

	//֪ͨStratServer������Admin�߳�������,
	SetEvent(pIOCPServer->m_evtThreadLanched);

	//�ȴ�����ָֹͣ���ָ������������»ᱻ����
	//1.����StartServer����ʱ��ListenThreadδ����������
	//2.������ListenThread���й����г���
	//3.����StopServer����

	//����m_evtSvrToStop�¼����󱻴���
	WaitForSingleObject(pIOCPServer->m_evtSvrToStop,INFINITE);

	//Do Clear Work
	if(pIOCPServer->m_sdListen!=INVALID_SOCKET)
		closesocket(pIOCPServer->m_sdListen);//������ListenThread��accept���ó�������

	if(pIOCPServer->m_hIOCP)
	{
		//֪ͨService�߳̽���
		for(UINT i=0; i<pIOCPServer->m_nSvcThreadNum;i++)
			PostQueuedCompletionStatus(pIOCPServer->m_hIOCP,0,0,NULL);
	}
	//�ȴ�����Service�̺߳ͼ����߳̽���
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
//�����������߳�
//
DWORD WINAPI CIOCPServer::ListenThread(LPVOID pParam)
{
	CIOCPServer *pIOCPServer=(CIOCPServer *) pParam;

	//����I/O��ɶ˿�
	pIOCPServer->m_hIOCP=pIOCPServer->CreateNewIoCompletionPort(0);
	if(pIOCPServer->m_hIOCP==NULL)
		return -1;

	//������������߳�
	for(UINT i=0;i<pIOCPServer->m_nSvcThreadNum;i++)
	{
		HANDLE hThread=CreateThread(NULL,0,ServiceThread,pIOCPServer,0,NULL);
		if(hThread == NULL)
			return -1;

		else
			pIOCPServer->m_hThreadList[i+2]=hThread;
	}

	//���������˿ڣ�����SO_REUSEADDRѡ��ΪTRUE���󶨱��ض˿�5002��������Listen�������м���
	pIOCPServer->m_sdListen=socket(AF_INET,SOCK_STREAM,0);
	if(pIOCPServer->m_sdListen==INVALID_SOCKET)
		return -1;

	BOOL bReuseAddr=TRUE;

	//�ڼ����׽ӿ�������SO_REUSEADDRѡ��,Ϊ��,�׽��ֿ���һ�����ڱ������׽���ʹ�õĵ�ַ����һ��
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

	//֪ͨStartServer�����������߳�������
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

	while(1)    //ѭ�������������ӣ���Ͷ�ݳ�ʼI/O��������
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
		//Ͷ�ݳ�ʼI/O����
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
//�����߳�
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
//�����߳�
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
		//����GetQueuedCompletionStatus������ȡ�����Ϣ��
		//����õ�����ɼ�Ϊ�գ�����ֹ�ù����̣߳�
		//���GetQueuedCompletionStatus����false����I/O������Ϊ0��˵���ͻ��˶Ͽ������ӣ��������ConnListRemove���������
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
		//���������Ϣ�е�lpPerIOData->oper���ݣ�
		//�жϸ������Ϣ����Ӧ�ķ�����I/O�������ͣ�
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

		if(!bSuccess)  //����ʧ��
		{
			DWORD dwIOError=GetLastError();
			return dwIOError;
		}

		if (!bSuccess||(bSuccess&&(pIOCPServer->dwIOSize==0)))
		{
			pIOCPServer->ConnListRemove(pIOCPServer->lpConnCtx);
			continue;
		}

		//���������Ϣ�е�lpPerIOData->oper���ݣ�
		//�жϸ������Ϣ����Ӧ�ķ�����I/O�������ͣ�
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
//��������
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
//��������
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







