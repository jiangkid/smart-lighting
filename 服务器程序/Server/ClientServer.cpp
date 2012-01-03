#include "StdAfx.h"
#include "ServerDlg.h"
#include "ClientServer.h"

CClientServer::CClientServer(void)
{
	m_hListenThread = NULL;			//�����߳�
	m_evtThreadLanched=CreateEvent(NULL,FALSE,FALSE,NULL);
	m_evtSvrToStop=CreateEvent(NULL,FALSE,FALSE,NULL);
	m_conn = new CDBConnection;
}

CClientServer::~CClientServer(void)
{
	CloseHandle(m_evtThreadLanched);
	CloseHandle(m_evtSvrToStop);
	if(m_hListenThread != NULL)										//�رռ����߳�
	::CloseHandle(m_hListenThread);
	delete m_conn;
}
/**************************************************
�������ܣ�������Ϣ�ж�
***************************************************/
 CString CClientServer::DataCheck(CHAR * buffer)
 {
	CString temp;
	switch (buffer[0])
	{
	case 'C': //�������û�
		if (this->CreateUser(buffer) == TRUE)
		{
			temp = "C0";
		}
		else temp = "C1";
		break;
	case 'D':
		this->DeleteUser(buffer);				//ɾ���û�
		break;
	case 'G':
		temp = this->GetLedStatus(buffer);
		break;
	case 'H':									//��ȡ��ʷ��Ϣ
		break;
	case 'L':
		if (InitializeClient(buffer)==TRUE)
		{
		}
		break;
	case 'M':
		if (this->ChangePassword(buffer) == TRUE)
		{
			temp = "M0";
		}
		else temp = "M1";
		break;
	case 'N':  
		this->InQueue(QlistCtG,buffer);			//��������
		break;
	case 'S':									//��������
		temp = this->FieldSet(buffer);
		break;
	default:
		temp ="";
		break;
	}
	return temp;
 }
/********************************************
   �������ܣ��û���¼��֤
********************************************/
 BOOL CClientServer::LogIn(LPCIOCPContext  pContext)
 {
	 CString  temp1,temp2,temp3;
	 CString temp;
	 CHAR  history[BUFFER_SIZE];
	 CHAR *buffer = pContext->pPerIOData->wbuf.buf; 
	 int  i=0;
	  if (buffer[0] != 'I')
	  {
			return FALSE;
	  }
	  else
	 {
		 for (i =1;i<3; ++i)
		 {
			temp += buffer[i];
		 }
		for (i =3;buffer[i] != '+'; ++i)
		{
			temp1 += buffer[i];
		}
		++i;
		for (int y=i; y <(i+6); ++y)
		{
			temp2 += buffer[y];
		}
	 }
	if ((temp == "00") || (temp == "01"))
	{
		if (temp == "00")
		{
			temp3 = AdminRecordset.GetPswByAdminName(temp1);
			AdminRecordset.Close();
		}
		else
		{
			temp3 = UserRecordset.GetPswByUserName(temp1);
			UserRecordset.Close();
		}
	}
	else
	{
		return FALSE;
	}
	if (::strcmp(temp2,temp3) != 0)
	{
		return FALSE;
	}
	  switch(temp[1])
	  {
	  case '0':
				pContext->ID = TRUE;
				break;
	  case '1':
				pContext->ID =FALSE;
				break;
	  default:
		  	  break;
	  }
	/* GetDlgItemText(H_ServerDlg,IDC_EDIT1,history,BUFFER_SIZE);			
	 temp1+="\r\n";
	 temp1+=(CString)history;
	 temp3+=temp1;
	 SetDlgItemText(H_ServerDlg,IDC_EDIT1,temp3);*/
	 return TRUE;
 }
 /**************************************************
 �������ܣ���ʼ��
 ***************************************************/
 BOOL CClientServer::InitializeClient(CHAR* buffer)
 {
	CString User,AreaName,AreaID,TermialName,TermialID,RoadName,RoadID;
	for (int i=1;buffer[i]!='#';++i)
	{
		User +=buffer[i];
	}
	AreaName = UserRecordset.GetAreaNameByUserName(User);
	TermialID = AreaRecordset.GetTerminalsByAreaName(AreaName);
	TermialName = AreaRecordset.GetTerminalsByAreaName(AreaName);
	
	return TRUE;
 }
/**************************************************
�������ܣ��������û�
***************************************************/
BOOL CClientServer::CreateUser(CHAR * buffer)
{
	CString temp1,temp2;
	int i = 0, y =0, x = 0;
	for (i=2;buffer[i] != '+'; ++i)
	{
		temp1 += buffer[i];
	}
	++i;
	for ( y = i; buffer[y] != '#';++y)
	{
		temp2 += buffer[y];
	}
	/* ++y;,temp3
	 for (x=y; buffer[x]!='#';++x)
	{
		temp3 +=buffer[x];
	}*/
	switch (buffer[1])
	{
	case '0':
		break;
	case '1':
		if (UserCommand.NewUser(temp1,temp2) == FALSE)
		{
			return FALSE;
		}
		break;
	default:
		break;
	}
	return TRUE;
}
/**************************************************
�������ܣ�ɾ���û�
***************************************************/
BOOL CClientServer::DeleteUser(CHAR * buffer)
{
	CString temp1,temp2;
	int i = 0;
	for (i=1;buffer[i] != '+'; ++i)
	{
		temp1 = buffer[i];
	}
	++i; 
	for (int y = i; y<(i+6);y++)
	{
		temp2 = buffer[y];
	}
	return TRUE;
}
 /********************************************
�������ܣ��޸�����
 ********************************************/
BOOL CClientServer::ChangePassword(CHAR* buffer)
{
	CString temp1,temp2;
	int i = 0, y =0, x = 0;
	for (i=2;buffer[i] != '+'; ++i)
	{
		temp1 += buffer[i];
	}
	++i;
	for ( y = i; buffer[y] != '#';++y)
	{
		temp2 += buffer[y];
	}
	++y;
	switch (buffer[1])
	{
	case '0':
		/*if (AdminCommand.ChangeUserPsw(temp1,temp2) == FALSE)
		{
			return FALSE;
		}*/
		break;
	case '1':
		if (UserCommand.ChangeUserPsw(temp1,temp2) == FALSE)
		{
			return FALSE;
		}
		break;
	default:
		break;
	}
	return TRUE;
}
 /********************************************
�������ܣ���������
 ********************************************/
CString  CClientServer::FieldSet(CHAR * buffer)
{
	CString temp,temp1,temp2;
	int i = 0;
	int y = 0;
	switch(buffer[1])
	{
	case 'L':												//���õ�
		for (i=2;buffer[i] != '+'; ++i)
		{
			temp1 += buffer[i];
		}
		++i; 
		for (y = i;buffer[y] != '#';y++)
		{
			temp2 += buffer[y];
		}
		if (LightCommand.AddLight(temp1,temp2) == TRUE)
		{
			temp = "S60";
			return temp;
		}
		else
		{
			temp = "S61";
			return temp;
		}
		break;
	case 'R':												//����·0x37
		for (i=2;buffer[i] != '+'; ++i)
		{
			temp1 += buffer[i];
		}
		++i; 
		for (y = i;buffer[y] != '#';y++)
		{
			temp2 += buffer[y];
		}
		if (RoadCommand.AddRoad(temp1,temp2) == TRUE)
		{
			temp = "S70";
			return temp;
		}
		else
		{
			temp = "S71";
			return temp;
		}
		break;
	case 'T':												//�����ն�
		for (i=2;buffer[i] != '+'; ++i)
		{
			temp1 += buffer[i];
		}
		++i; 
		for (y = i;buffer[y] != '#';y++)
		{
			temp2 += buffer[y];
		}
		if (TerminalCommand.AddTerminal(temp1,temp2) == TRUE)
		{
			temp = "S80";
			return temp;
		}
		else
		{
			temp = "S81";
			return temp;
		}
		break;
	case 'G':												//��������
		for (i=2;buffer[i] != '+'; ++i)
		{
			temp1 += buffer[i];
		}
		++i; 
		for (y = i;buffer[y] != '#';y++)
		{
			temp2 += buffer[y];
		}
		if (AreaCommand.AddArea(temp1,temp2) == TRUE)
		{
			temp = "S90";
			return temp;
		}
		else
		{
			temp = "S91";
			return temp;
		}
		break;
	default:
		temp = "";
		break;
	}
	return temp;
}
 /********************************************
�������ܣ���ȡ�Ƶ�״̬
 ********************************************/
CString  CClientServer::GetLedStatus(CHAR* buffer)
{
	CString temp, temp1;
	for (int i=1; i < 9; i++)
	{
		temp += buffer[i]; 
	}
//	temp1 = ServerDB.GetLightStatus(temp);
	return temp1;
}
 /********************************************
�������ܣ���ʼ����,״̬������ʼ��
 ********************************************/
BOOL CClientServer::Start(int nPort)
{
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
	TerminalCommand.m_cnn = m_conn;
	LightCommand.m_cnn = m_conn;
	RoadCommand.m_cnn = m_conn;
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
	for (UINT i = 2; i < 3; ++i)//m_nSvrThreadNum+2
	{
		m_hThreadList[i] = CreateThread(NULL,0,_WorkerThreadProc,this,0,NULL);
		if (m_hThreadList[i] == NULL)
		{
			return  FALSE;
		}
	}	
	return TRUE;
}

BOOL CClientServer::StopServer()
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
	if (this->DestroyQueue(QlistCtG) == TRUE)
	{
		return TRUE;
	}
}
 /********************************************
�������ܣ������߳�
 ********************************************/
DWORD WINAPI  CClientServer::_ListenThreadProc(LPVOID lpParam)
{
	CClientServer *ClientServer = (CClientServer *)lpParam;
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
 /********************************************
�������ܣ������߳�
 ********************************************/
DWORD WINAPI  CClientServer::_WorkerThreadProc(LPVOID lpParam)
{
	::CoInitialize(NULL);
	CClientServer  *pIOCPServer = (CClientServer*)lpParam;
	HANDLE hIOCP = pIOCPServer ->m_hCompletion;
	BOOL bSucess = false;
	DWORD dwIOSize;
	LPCIOCPBuffer lpPerIOData = new CIOCPBuffer;
	LPOVERLAPPED pOverLapped;
	LPCIOCPContext lpConnCtx;
	sockaddr_in addrAccept;
	int nResult;
	CString buffer1;
	char strTemp_send[BUFFER_SIZE];
	char strTemp_recv[BUFFER_SIZE];
	while (1)
	{
		if  (QlistGtC->front != QlistGtC->rear)
		{
			buffer1 = pIOCPServer->OutQueue(QlistGtC);
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
				MessageBox(NULL, "GPRS��������ʧ��", "��ʾ", MB_OK);
			}
			GetDlgItemText(H_ServerDlg,IDC_EDIT1,strTemp_send,BUFFER_SIZE);
			buffer1 += "\r\n";
			buffer1 += strTemp_send;
			SetDlgItemText(H_ServerDlg,IDC_EDIT1, buffer1);

		}		
		bSucess = GetQueuedCompletionStatus(hIOCP,
											&dwIOSize,
											(LPDWORD)&lpConnCtx,
											&pOverLapped,
											500);
		if ((!bSucess) &&(GetLastError() ==  WAIT_TIMEOUT) )
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
		if (!bSucess||(bSucess&&(dwIOSize==0)))
		{
			if ((lpConnCtx->sockAccept != INVALID_SOCKET)&&(lpConnCtx->LogIn == TRUE || lpConnCtx->LogIn == FALSE))
			{
				addrAccept = lpConnCtx->addrAccept;
				pIOCPServer->ConnListRemove(lpConnCtx);
				GetDlgItemText(H_ServerDlg,IDC_EDIT1,strTemp_send,BUFFER_SIZE);
				pIOCPServer->strSend.Format("�ͻ����˳� :%s\r\n",inet_ntoa(addrAccept.sin_addr));
				pIOCPServer->strSend+=(CString)strTemp_send;
				SetDlgItemText(H_ServerDlg,IDC_EDIT1,pIOCPServer->strSend);				
			}
			continue;
		}
		lpPerIOData = (LPCIOCPBuffer)(pOverLapped);
		/***************��¼�ж�****************/
		if (lpConnCtx->LogIn == FALSE)
		{
			if (lpPerIOData->oper == SVR_IO_READ)
			{
				if ((pIOCPServer->LogIn(lpConnCtx)) == TRUE)  //��¼�ɹ�
				{
					pIOCPServer->strRecv = "";
					pIOCPServer->strRecv = "I0";
					lpConnCtx->LogIn = TRUE;
					pIOCPServer->InitializeBuffer(lpPerIOData, SVR_IO_WRITE);
					lpPerIOData->wbuf.buf = (LPTSTR)(LPCTSTR)pIOCPServer->strRecv;
					lpPerIOData->wbuf.len=pIOCPServer->strRecv.GetLength();
					lpPerIOData->flags=0;
					nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
					if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
					{
						pIOCPServer->ConnListRemove(lpConnCtx);
						continue;
					}
					continue;
				}
				else 
				{
					pIOCPServer->strRecv = "";
					pIOCPServer->strRecv = "I1";
					pIOCPServer->InitializeBuffer(lpPerIOData, SVR_IO_WRITE);
					lpPerIOData->wbuf.buf = (LPTSTR)(LPCTSTR)pIOCPServer->strRecv;
					lpPerIOData->wbuf.len=pIOCPServer->strRecv.GetLength();
					lpPerIOData->flags=0;
					nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
					if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
					{
						pIOCPServer->ConnListRemove(lpConnCtx); 
						continue;
					}
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
			pIOCPServer->strRecv = "";
			pIOCPServer->strSend = "";
			pIOCPServer->strRecv.Format(("%s send message : "),inet_ntoa(lpConnCtx->addrAccept.sin_addr)) ;//= lpPerIOData->wbuf.buf inet_ntoa(lpConnCtx->addrAccept.sin_addr)
			pIOCPServer->strRecv += lpPerIOData->wbuf.buf;
		    pIOCPServer->strRecv += "\r\n";
			GetDlgItemText(H_ServerDlg,IDC_EDIT1,strTemp_recv,BUFFER_SIZE);	
			pIOCPServer->strRecv+=(CString)strTemp_recv;
			SetDlgItemText(H_ServerDlg,IDC_EDIT1,pIOCPServer->strRecv);

			pIOCPServer->strSend = pIOCPServer->DataCheck(lpPerIOData->wbuf.buf);
			pIOCPServer->InitializeBuffer(lpPerIOData, SVR_IO_WRITE);
			lpPerIOData->wbuf.buf = (LPTSTR)(LPCTSTR)pIOCPServer->strSend;
			lpPerIOData->wbuf.len=pIOCPServer->strSend.GetLength();
			lpPerIOData->flags=0;
			nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
			if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
			{
				pIOCPServer->ConnListRemove(lpConnCtx);
				continue;
			}
			/*pIOCPServer->strSend = 
			lpPerIOData->wbuf.buf = (LPTSTR)(LPCTSTR)pIOCPServer->strSend;
			lpPerIOData->wbuf.len=pIOCPServer->strSend.GetLength();
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
	CoUninitialize();
	return TRUE;
}
/********************************************************************
�����������߳�
���ܣ��ȴ�����ָֹͣ���ָ������������»ᱻ����
1.����StartServer����ʱ��ListenThreadδ����������
2.������ListenThread���й����г���
3.����StopServer����
********************************************************************/
DWORD WINAPI CClientServer::_AdminThread(LPVOID pParam)
{
	CClientServer  *pIOCPServer= (CClientServer *)pParam;
	//SetEvent(pIOCPServer->m_evtThreadLanched);	//֪ͨStratServer������Admin�߳�������,
	WaitForSingleObject(pIOCPServer->m_evtSvrToStop,INFINITE);	//����m_evtSvrToStop�¼����󱻴���
	if(pIOCPServer->m_sListen!=INVALID_SOCKET)
		closesocket(pIOCPServer->m_sListen);		//������ListenThread��accept���ó�������
	if(pIOCPServer->m_hCompletion)
	{
		//֪ͨService�߳̽���
		for(UINT i=0; i<pIOCPServer->m_nSvrThreadNum;i++)
			PostQueuedCompletionStatus(pIOCPServer->m_hCompletion,-1,0,NULL);
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

