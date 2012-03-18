#include "StdAfx.h"
#include "GPRSServer.h"
#include <process.h>
#include "ServerDlg.h"

CGPRSServer::CGPRSServer(void)
{
	m_conn =new CDBConnection;
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
	delete m_conn;
}
/********************************************
   �������ܣ��û���¼��֤
********************************************/
 BOOL CGPRSServer::LogIn(LPCIOCPContext  pContext)
 {
	CString str;
	LPTSTR p ;
	CHAR AreaID[2];
	p = AreaID;
	UCHAR data4 = pContext->pPerIOData->data[4];
	//p = pContext->pPerIOData->data[5];
	if ((pContext->pPerIOData->data[0]==0x2F)&&(pContext->pPerIOData->data[1]==0x2F)&&(pContext->pPerIOData->data[2]==0x2F)&&(data4==0xCC))
	{
		ZeroMemory(pContext->Identify,10);
 		wsprintf(p, "%.2X", pContext->pPerIOData->data[3]);
    	memcpy(pContext->Identify,p,2);		
		return TRUE;
	}
	else
		return FALSE;
 }

int CGPRSServer::listen = 0;
SOCKET CGPRSServer::m_sAccept = 0;
//
//����������
//
BOOL CGPRSServer::StartServer(int nPort)
{
// 	char buffer[16] ={'F','1','3','2','3','0','3','0','3','1','3','1','3','3','3','3'};
// 	int length = 16;
// 	char *temp;
// 	temp = Translation_ID(buffer,length);
	m_bServerStarted = TRUE;
	Initialize_Socket(nPort);   //��ʼ���׽���
	//�����ݿ�����
	if(!m_conn->Open())
	{
		::AfxMessageBox("���ݿ��ʧ��");
		return FALSE;
	}
	AdminRecordset.m_cnn = m_conn;
	AdminCommand.m_cnn = m_conn;
	UserRecordset.m_cnn = m_conn;
	UserCommand.m_cnn = m_conn;
	AreaCommand.m_cnn = m_conn;
	AreaRecordset.m_cnn = m_conn;
	TerminalCommand.m_cnn = m_conn;
	TerminalRecordset.m_cnn = m_conn;
	LightCommand.m_cnn = m_conn;
	LightRecordset.m_cnn = m_conn;
	RoadCommand.m_cnn = m_conn;
	RoadRecordset.m_cnn = m_conn;
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
	if (this->DestroyQueue(QlistGtC) != TRUE)
	{
		return FALSE;
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
		::GetDlgItemText(H_ServerDlg, IDC_EDIT1, history, BUFFER_SIZE);
		tmp.Format("A GPRS joins us :%s\r\n", inet_ntoa(addrRemote.sin_addr));
		tmp +=(CString)history; 
		::SetDlgItemText(H_ServerDlg, IDC_EDIT1, tmp);	
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
						&(pGPRSServer->pClient->pPerIOData->wbuf), 	1, 
						NULL,	&(pGPRSServer->pClient->pPerIOData->flags),
						&(pGPRSServer->pClient->pPerIOData->OverLapped),	NULL);
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
	int IDlength = 16;
	CString buffer1;
	CString temp1;
	sockaddr_in addrAccept;
	char *recv, *IDchar;
	char strTemp_send[BUFFER_SIZE];
	char strTemp_recv[BUFFER_SIZE];
/*	char strTemp_ID[21];*/
	char AreaID[2];
	char strTemp_IDGTC[29];		//���� 
	UCHAR strTemp_IDG[8];			//8ΪID
	char  strTemp_IDC[16];			//16λID
	char * strTemp_ID;
	U8 * strSendID;
	ConTrlInfo *m_ControlCode;
	char strGPRSData[21];
	U8 current[2];                   //���� 2��byte
	LPGPRSRecvInfo  pGPRSRecvInfo;

	ZeroMemory(&strTemp_recv,sizeof(strTemp_recv));
	ZeroMemory(&strTemp_send,sizeof(strTemp_send));

	while (1)
	{
		//Sleep(10000);
		if  ((QlistCtG->front != QlistCtG->rear))
		{
			length = 0;
			recv = pIOCPServer->OutQueue(QlistCtG,length);
			buffer1 = recv;
			if (buffer1 == "error")
			{
				continue;
			}
			m_ControlCode = (ConTrlInfo *)recv;
			for(int i = 0; i<2; ++i)
			{
				AreaID[i] = m_ControlCode->m_ID[i];
			}
			strSendID = pIOCPServer->GPRSTranslationID(m_ControlCode->m_ID,16);
			ZeroMemory(strGPRSData,21);
			memcpy(strGPRSData,m_ControlCode->m_First,4);
			memcpy(strGPRSData+4,strSendID,8);
			memcpy(strGPRSData+12,m_ControlCode->m_OrderType,1);
			memcpy(strGPRSData+13,m_ControlCode->m_OrderObject,1);
			memcpy(strGPRSData+14,m_ControlCode->m_ActiveType,1);
			memcpy(strGPRSData+15,m_ControlCode->m_CheckData,4);
			memcpy(strGPRSData+19,m_ControlCode->m_EndBuffer,2);
			lpConnCtx = pIOCPServer->LocateGPRSElement(AreaID,sizeof(AreaID));//(LPTSTR)
			if (lpConnCtx == NULL)
			{
				continue;
			}
			pIOCPServer->InitializeBuffer(lpPerIOData,SVR_IO_WRITE );
			lpConnCtx ->pPerIOData->wbuf.len = 21;
			lpConnCtx ->pPerIOData->wbuf.buf = strGPRSData;
			lpConnCtx ->pPerIOData->oper = SVR_IO_WRITE;
			lpConnCtx ->pPerIOData->flags = 0;
			if (lpConnCtx ->sockAccept != INVALID_SOCKET)
			{
				nResult=WSASend(lpConnCtx ->sockAccept, 
					&(lpConnCtx ->pPerIOData->wbuf), 
					1, 
					NULL,
					lpConnCtx ->pPerIOData->flags,
					&(lpConnCtx ->pPerIOData->OverLapped),
					NULL);
				if((nResult==SOCKET_ERROR) && (WSAGetLastError()!=ERROR_IO_PENDING))
				{
					//MessageBox(NULL, "GPRS��������ʧ��", "��ʾ", MB_OK);
				}
			}
			buffer1.Empty();
			buffer1.Format(("Server Send Message to GPRS  %s   : "), inet_ntoa(lpConnCtx->addrAccept.sin_addr));
			pIOCPServer->ShowMessage(strGPRSData, 21, buffer1, 3);
		}	

		/*********************IOCP ���ݴ���*************************/
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
 		if(dwIOSize == -1)  //�û�֪ͨ�˳�
 		{
 			::ExitThread(0);
 		}
		//ZeroMemory(lpPerIOData,sizeof(CIOCPBuffer));
 		lpPerIOData = (LPCIOCPBuffer)(pOverLapped);
		
 		if (!bSucess||(bSucess&&(dwIOSize==0)))
 		{
			if ((lpConnCtx->sockAccept != INVALID_SOCKET)&&(lpConnCtx->LogIn == TRUE || lpConnCtx->LogIn == FALSE))
 			{
 				addrAccept = lpConnCtx->addrAccept;
 				pIOCPServer->ConnListRemove(lpConnCtx);
 				GetDlgItemText(H_ServerDlg,IDC_EDIT2,strTemp_send,BUFFER_SIZE);
 				pIOCPServer->strSend.Format("GPRS���˳� :%s\r\n",inet_ntoa(addrAccept.sin_addr));
 				pIOCPServer->strSend+=(CString)strTemp_send;
 				SetDlgItemText(H_ServerDlg,IDC_EDIT2,pIOCPServer->strSend);
 			}			
 			continue;
 		}
		/***************��¼�ж�****************/
		if (lpConnCtx->LogIn == FALSE)
		{
			if (lpPerIOData->oper == SVR_IO_READ)
			{
				if ((pIOCPServer->LogIn(lpConnCtx)) == TRUE)  //��¼�ɹ�
				{
					lpConnCtx->LogIn = TRUE;
					pIOCPServer->InitializeBuffer(lpPerIOData, SVR_IO_READ);
					nResult=WSARecv(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,&(lpPerIOData->flags),&(lpPerIOData->OverLapped),NULL);
					if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
					{
						pIOCPServer->ConnListRemove(lpConnCtx);
					}
					ZeroMemory(lpPerIOData->data,sizeof(lpPerIOData->data));
					continue;
				}
				else 
				{
					pIOCPServer->ConnListRemove(lpConnCtx); //�˴���bug�����ͻ��˵�½�����߳�ͨ�Ų������󣬻����  
					continue;
				}
			} 
			else
			{
				pIOCPServer->ConnListRemove(lpConnCtx);
				continue;
			}	
		}	
 		switch(lpPerIOData->oper)
 		{
 		case SVR_IO_WRITE:
 			pIOCPServer->InitializeBuffer(lpPerIOData, SVR_IO_READ);
 			nResult=WSARecv(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,&(lpPerIOData->flags),&(lpPerIOData->OverLapped),NULL);
 			if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
 			{
 				pIOCPServer->ConnListRemove(lpConnCtx);
 			}
 			break;
 
 		case SVR_IO_READ:
 			if (dwIOSize != 21)
 			{
				pIOCPServer->InitializeBuffer(lpPerIOData,SVR_IO_WRITE );
				lpConnCtx ->pPerIOData->wbuf.len = 5;
				lpConnCtx ->pPerIOData->wbuf.buf = "error";
				lpConnCtx ->pPerIOData->oper = SVR_IO_WRITE;
				lpConnCtx ->pPerIOData->flags = 0;
				WSASend(lpConnCtx ->sockAccept, 
						&(lpConnCtx ->pPerIOData->wbuf), 
						1, 
						NULL,
						lpConnCtx ->pPerIOData->flags,
						&(lpConnCtx ->pPerIOData->OverLapped),
						NULL);
				continue;
 			}

			buffer1.Empty();
			buffer1.Format(("GPRS %s send Message to Server  : "), inet_ntoa(lpConnCtx->addrAccept.sin_addr));
			pIOCPServer->ShowMessage(lpPerIOData->data, dwIOSize, buffer1, 4);//��ʾGPRS���͵�����

			ZeroMemory(strGPRSData,sizeof(strGPRSData));
			memcpy(strGPRSData,lpPerIOData->data,21);
			pGPRSRecvInfo = (LPGPRSRecvInfo)strGPRSData;
			ZeroMemory(strTemp_IDC,sizeof(strTemp_IDC));
			strTemp_ID = pIOCPServer->CharToCString(pGPRSRecvInfo->m_ID,8);
			strcpy(strTemp_IDC,strTemp_ID);
			if (pGPRSRecvInfo->m_ActiveType[0] == 0xBD)  //��������Ϊ��ѯ
			{
				switch(pGPRSRecvInfo->m_OrderObject[0])	//Ŀ��ʶ��
				{
				case  0x04:			//һ·�µƵ���Ϣ����(����״̬�͵�������ͬһ����������)
					temp1.Empty();
					ZeroMemory(strTemp_send,sizeof(strTemp_send));
					memcpy(strTemp_send,strTemp_IDC,16);
					strTemp_send[16]=0;
					temp1=strTemp_send;
					pIOCPServer->LightRecordset.UpdateLightInfo(temp1,pGPRSRecvInfo->m_OrderType[0],pGPRSRecvInfo->m_Data);
					pIOCPServer->LightRecordset.Close();
					break;
				case  0x06:							//һ���ն��£���·�ĵ�������
															//CRoadRecordset::UpdateRoadInfo(CString roadID,U8 roadNum,CString currentValue)
					temp1.Empty();
					memcpy(strTemp_IDC+4,pIOCPServer->CharToCString(&pGPRSRecvInfo->m_Data[0],1),2);
					strTemp_IDC[6]=0;
					temp1 = strTemp_IDC; 
					current[0] = pGPRSRecvInfo->m_Data[2];
					current[1] = pGPRSRecvInfo->m_Data[3];
					pIOCPServer->RoadRecordset.UpdateRoadCurrent(temp1,pGPRSRecvInfo->m_Data[1],current);
					pIOCPServer->RoadRecordset.Close();
					break;
				case  0x01:												
					//(1)�����ƻ��ߵ���·�Ĳ�ѯ��Ϣ����
					if (pGPRSRecvInfo->m_OrderType[0] >0xA0)   //�����Ƶ���Ϣ����(W11��ʾ��ȷ���ص�����W01��ʾ��ȷ����״̬)
					{
						temp1.Empty();
						ZeroMemory(strTemp_send,sizeof(strTemp_send));
						memcpy(strTemp_send,strTemp_IDC,16);
						strTemp_send[16]=0;
						temp1=strTemp_send;
						if (pIOCPServer->LightRecordset.UpdateoneLightInfo(temp1,pGPRSRecvInfo->m_OrderType[0],pGPRSRecvInfo->m_Data))
						{
							if (pGPRSRecvInfo->m_OrderType[0] == 0xAE)
							{
								buffer1.Empty();
								buffer1 = "W11";
							}
							else
							{
								buffer1.Empty();
								buffer1 = "W01";
							}
						}
						else
						{
							if (pGPRSRecvInfo->m_OrderType[0] == 0xAE)
							{
								buffer1.Empty();
								buffer1 = "W10";
							}
							else
							{
								buffer1.Empty();
								buffer1 = "W00";
							}
						}
						pIOCPServer->LightRecordset.Close();
						pIOCPServer->GPRSInQueue(buffer1,pGPRSRecvInfo);
					}
					//(2)����·�Ĳ�ѯ״̬����
					else if (pGPRSRecvInfo->m_OrderType[0] == 0x32)    
					{
						temp1.Empty();
						strTemp_IDC[4]=0;
						temp1 = strTemp_IDC;
						current[0] = pGPRSRecvInfo->m_Data[3];
						pIOCPServer->RoadRecordset.UpdateRoadsStatus(temp1,current[0]);
						pIOCPServer->RoadRecordset.Close();
						buffer1.Empty();
						buffer1 = "W41";
						pIOCPServer->GPRSInQueue(buffer1,pGPRSRecvInfo);
					}
					break;
				}
			}
			else//��������Ϊ����
			{
				buffer1.Empty();
				buffer1 = "N00";
				pIOCPServer->GPRSInQueue(buffer1,pGPRSRecvInfo);
			}
 			pIOCPServer->InitializeBuffer(lpPerIOData, SVR_IO_READ);
 			nResult=WSARecv(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,&(lpPerIOData->flags),&(lpPerIOData->OverLapped),NULL);
 			if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
 			{
 				pIOCPServer->ConnListRemove(lpConnCtx);
 			}
 			break;
 		default: break;
 		}
  	}
	delete lpPerIOData;
	CoUninitialize();
	return TRUE;
}
/*****************************************************************************
�������ܣ����յ���GPRS���ݣ�ת���ɿͻ�����Ҫ�����ͣ�ID16λ����Ȼ������
ͨ���������Ŀͻ��˲��ַ�����
******************************************************************************/
bool   CGPRSServer::GPRSInQueue(CString buffer, LPGPRSRecvInfo temp)
{
		char strTemp_IDGTC[32];		//���� 
		char * IDchar;
		ZeroMemory(strTemp_IDGTC,sizeof(strTemp_IDGTC));
		//LPConTrlInfo  message = (LPConTrlInfo)malloc(sizeof(ConTrlInfo));
		//ZeroMemory(message,sizeof(ConTrlInfo));
		IDchar = CharToCString(temp->m_ID,8);
		//memcpy(message->m_ID,IDchar,16);
		memcpy(strTemp_IDGTC,buffer,3);
		memcpy(strTemp_IDGTC+3, temp->m_First,4);
		memcpy(strTemp_IDGTC+7,IDchar,16);
		memcpy(strTemp_IDGTC+23,temp->m_OrderObject,1);
		memcpy(strTemp_IDGTC+24,temp->m_OrderType,1);
		memcpy(strTemp_IDGTC+25,temp->m_ActiveType,1);
		memcpy(strTemp_IDGTC+26,temp->m_Data,4);
		memcpy(strTemp_IDGTC+30,temp->m_EndBuffer,2);
		if (InQueue(QlistGtC,strTemp_IDGTC,sizeof(strTemp_IDGTC)))
		{
			return true;
		}
		return false;		
}
