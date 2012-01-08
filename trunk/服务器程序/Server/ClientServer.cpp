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
	//if(m_hListenThread != NULL)										//�رռ����߳�
	//::CloseHandle(m_hListenThread);
	delete m_conn;
}
/**************************************************
�������ܣ���̬������ʼ��
***************************************************/
int CClientServer::listen = 0;
/**************************************************
�������ܣ�������Ϣ�ж�
***************************************************/
 CString CClientServer::DataCheck(CHAR * buffer,int strlen)
 {
	CString temp;
	char * buffer_trans = (char *)malloc(strlen);
	ZeroMemory(buffer_trans,strlen);
	switch (buffer[0])
	{
	case 'B'://���ID
		if (AddID(buffer) != TRUE)
		{
			temp = "B1";
		}
		else temp = "";
		break;
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
		temp = "L0";
		break;
	case 'M':
		if (this->ChangePassword(buffer) == TRUE)
		{
			temp = "M0";
		}
		else temp = "M1";
		break;
	case 'N':  
		memcpy(buffer_trans,buffer,strlen);
		this->InQueue(QlistCtG,buffer_trans,strlen);			//��������
		break;
	case 'S':									//��������
		temp = this->FieldSet(buffer);
		break;
	default:
		temp ="";
		break;
	}
	free(buffer_trans);
	return temp;
 }
/********************************************
   �������ܣ��û���¼��֤
********************************************/
 BOOL CClientServer::LogIn(LPCIOCPContext  pContext)
 {
	CString  username,password;
	char IDentify;
	CString temp;
	char  *head = (char *)malloc(HeadLen);
	LPHDR pHead;
	memcpy(head,pContext->pPerIOData->data,HeadLen);
	pHead = (LPHDR)head;
	if (pHead->dataCheck != 'I')
	{
		return FALSE;
	}
	char *body = (char *)malloc(pHead->dataLen);
	memcpy(body,pContext->pPerIOData->data+HeadLen,pHead->dataLen);
	LPUSERINFO UserInfo;
	UserInfo = (LPUSERINFO)body;
	username = UserInfo->UserName;
	password = UserInfo->PassWord;
	IDentify = UserInfo->Idetify;
	if ((IDentify == '0') || (IDentify == '1'))
	{
		if (IDentify == '0')
		{
			temp = AdminRecordset.GetPswByAdminName(username);
			AdminRecordset.Close();
		}
		else
		{
			temp = UserRecordset.GetPswByUserName(username);
			UserRecordset.Close();
		}
	}
	else
	{
		return FALSE;
	}
	if (::strcmp(temp,password) != 0)
	{
		return FALSE;
	}
	  switch(IDentify)
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
	  free(head);
	  free(body);
	 return TRUE;
 }
 /**************************************************
 �������ܣ����ID
 ***************************************************/
 BOOL CClientServer::AddID(char *buffer)
 {
	CString  temp,PrevID;
	int i;
	for (i=2;buffer[i]!='#';++i)
	{
		temp += buffer[i];
	}
	switch (buffer[1])
	{
	case 'G':
		if (AreaCommand.AddAreaID(temp) != TRUE)
		{
			return FALSE;
		}
		break;
	case 'T':
		for (;i<7;++i)
		{
			PrevID += buffer[i];
		}
		if (TerminalCommand.AddTerminalID(PrevID,temp) != TRUE)
		{
			return FALSE;
		}
		break;
	case 'R':
		for (;i<9;++i)
		{
			PrevID += buffer[i];
		}
		if (RoadCommand.AddRoadID(PrevID,temp) != TRUE)
		{
			return FALSE;
		}
		break;
	case 'L':
		for (;i<11;++i)
		{
			PrevID += buffer[i];
		}
		if (LightCommand.AddLightID(PrevID,temp) != TRUE)
		{
			return FALSE;
		}
		break;
	}
	 return TRUE;
 }

 /**************************************************
 �������ܣ���ʼ��
 ***************************************************/
 BOOL CClientServer::InitializeClient(CHAR* buffer,CString &Area,CString &Termial,CString &Road,CString &Light)
 {
	int nResult;
	CString returnMenu;
	CString buffer1,User,AreaName,AreaID,TermialName,TermialID,RoadName,RoadID;
	Area = AreaRecordset.GetAllAreaAndCount();
	Termial = TerminalRecordset.GetAllTerminalsAndCount();
	Road = RoadRecordset.GetAllRoadsAndCount();
	Light = LightRecordset.GetAllLightsAndCount();
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
	CString temp,ID,CallName,UserName,sendMessage;
	char temp4;
	int i = 0;
	int y = 0;
	switch(buffer[1])
	{
	case GET_USERNAME:
		temp = UserRecordset.GetAllUserNameAndCount();
		sendMessage = 'S';
		temp4 = GET_USERNAME;
		sendMessage += temp4;
		if (temp != "")
		{
			sendMessage +='0';
			sendMessage += temp;
		}
		else sendMessage += '1';  
		break;
	case GET_GPRSID:
		temp = AreaRecordset.GetAllAreaIDAndCount();
		sendMessage = 'S';
		temp4 = GET_GPRSID;
		sendMessage += temp4;
		if (temp != "")
		{
			sendMessage +='0';
			sendMessage += temp;
		}
		else sendMessage += '1';  
		break;
	case GET_TERMINALID:
		for (i=4;buffer[i]!= '#'; ++i)
		{
			ID += buffer[i];
		}
		temp = AreaRecordset.GetTerminalIDsAndCountByAreaID(ID);
		sendMessage = 'S';
		temp4 = GET_TERMINALID;
		sendMessage += temp4;
		if (temp != "")
		{
			sendMessage +='0';
			sendMessage += temp;
		}
		else sendMessage += '1';  
		break;
	case GET_ROADID:
		for (i=4;buffer[i]!= '#'; ++i)
		{
			ID += buffer[i];
		}
		temp = TerminalRecordset.GetRoadIDsAndCountByTerminalID(ID);
		sendMessage = 'S';
		temp4 = GET_ROADID;
		sendMessage += temp4;
		if (temp != "")
		{
			sendMessage +='0';
			sendMessage += temp;
		}
		else sendMessage += '1';  
		break;
	case GET_LIGHTID:
		for (i=4;buffer[i]!= '#'; ++i)
		{
			ID += buffer[i];
		}
		temp = RoadRecordset.GetLightIDsAndCountByRoadID(ID);
		sendMessage = 'S';
		temp4 = GET_LIGHTID;
		sendMessage += temp4;
		if (temp != "")
		{
			sendMessage +='0';
			sendMessage += temp;
		}
		else sendMessage += '1';  
		break;
	case BIND_LIGHTID:												//���õ�
		for (i=2;buffer[i] != '+'; ++i)
		{
			ID += buffer[i];
		}
		++i; 
		for (y = i;buffer[y] != '#';y++)
		{
			CallName += buffer[y];
		}
		if (LightRecordset.SetLightName(ID,CallName) == TRUE)
		{
			sendMessage = "S60";
		}
		else
		{
			sendMessage = "S61";
		}
		break;
	case BIND_ROADID:												//����·0x37
		for (i=2;buffer[i] != '+'; ++i)
		{
			ID += buffer[i];
		}
		++i; 
		for (y = i;buffer[y] != '#';y++)
		{
			CallName += buffer[y];
		}
		if (RoadRecordset.SetRoadName(ID,CallName) == TRUE)
		{
			sendMessage = "S70";
		}
		else
		{
			sendMessage = "S71";
		}
		break;
	case BIND_TERMINALID:												//�����ն�
		for (i=2;buffer[i] != '+'; ++i)
		{
			ID += buffer[i];
		}
		++i; 
		for (y = i;buffer[y] != '#';y++)
		{
			CallName += buffer[y];
		}
		if (TerminalRecordset.SetTerminalName(ID,CallName) == TRUE)
		{
			sendMessage = "S80";
		}
		else
		{
			sendMessage = "S81";
		}
		break;
	case BIND_GPRSID:												//��������
		for (i=2;buffer[i] != '+'; ++i)
		{
			ID += buffer[i];
		}
		++i; 
		for (;buffer[i] != '+';i++)
		{
			CallName += buffer[i];
		}
		++i; 
		for (;buffer[i] != '#';i++)
		{
			UserName += buffer[i];
		}
		if (AreaRecordset.SetAreaNameAndIDUser(ID,CallName,UserName) == TRUE)
		{
			sendMessage = "S90";
		}
		else
		{
			sendMessage = "S91";
		}
		break;
	default:
		sendMessage = "";
		break;
	}
	return sendMessage;
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
		listen = 1;
	}
	return TRUE;
}
 /********************************************
�������ܣ������߳�
 ********************************************/
DWORD WINAPI  CClientServer::_WorkerThreadProc(LPVOID lpParam)
{
	while(!listen)
	{
		Sleep(100);
	}
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
	int length;
	CString buffer1;
	CString Area,Termial,Road,Light;
	char show[BUFFER_SIZE];
	char strTemp_send[3096];
	char strTemp_recv[3096];
	while (1)
	{
		if  (QlistGtC->front != QlistGtC->rear)
		{
			length = 0;
			buffer1 = pIOCPServer->OutQueue(QlistGtC, length);
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
		//ZeroMemory(&lpPerIOData,sizeof(CIOCPBuffer));
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
			pIOCPServer->strRecv.Format(("%s send message : "),inet_ntoa(lpConnCtx->addrAccept.sin_addr));
			/*GetDlgItemText(H_ServerDlg,IDC_EDIT1,strTemp_recv,BUFFER_SIZE);	
			memcpy(show,pIOCPServer->strRecv,sizeof(pIOCPServer->strRecv));
			memcpy(show+sizeof(pIOCPServer->strRecv),lpPerIOData->data,dwIOSize);
 			memcpy(show+sizeof(pIOCPServer->strRecv)+dwIOSize,"\r\n",2);
 			memcpy(show+sizeof(pIOCPServer->strRecv)+dwIOSize+2,strTemp_recv,sizeof(strTemp_recv));
			memcpy(show,lpPerIOData->data,dwIOSize);
			memcpy(show+dwIOSize,"\r\n",2);
			memcpy(show+dwIOSize+2,strTemp_recv,sizeof(strTemp_recv));
			SetDlgItemText(H_ServerDlg,IDC_EDIT1,show);		*/	
			pIOCPServer->strRecv += lpPerIOData->wbuf.buf;
 		    pIOCPServer->strRecv += "\r\n";
 			GetDlgItemText(H_ServerDlg,IDC_EDIT1,strTemp_recv,BUFFER_SIZE);	
			pIOCPServer->strRecv+=(CString)strTemp_recv;
 			SetDlgItemText(H_ServerDlg,IDC_EDIT1,pIOCPServer->strRecv);
			pIOCPServer->strSend = pIOCPServer->DataCheck(lpPerIOData->data,dwIOSize);
			if (pIOCPServer->strSend == "L0")
			{
				pIOCPServer->InitializeClient(lpPerIOData->wbuf.buf,Area,Termial,Road,Light);
				pIOCPServer->InitializeBuffer(lpPerIOData, SVR_IO_WRITE);
				if ((Area != "")&&(Termial != "")&&(Road != "")&&(Light != ""))
				{
					buffer1 = "L0G";
					buffer1 += Area;
					lpPerIOData->wbuf.buf = (LPTSTR)(LPCTSTR)buffer1;
					lpPerIOData->wbuf.len = buffer1.GetLength();
					lpPerIOData->flags = 0;
					nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
					if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
					{
						pIOCPServer->ConnListRemove(lpConnCtx);
						continue;
					}
					buffer1 = "L0T";
					buffer1 += Termial;
					lpPerIOData->wbuf.buf = (LPTSTR)(LPCTSTR)buffer1;
					lpPerIOData->wbuf.len = buffer1.GetLength();
					lpPerIOData->flags = 0;
					nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
					if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
					{
						pIOCPServer->ConnListRemove(lpConnCtx);
						continue;
					}
					buffer1 = "L0R";
					buffer1 += Road;
					lpPerIOData->wbuf.buf = (LPTSTR)(LPCTSTR)buffer1;
					lpPerIOData->wbuf.len = buffer1.GetLength();
					lpPerIOData->flags = 0;
					nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
					if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
					{
						pIOCPServer->ConnListRemove(lpConnCtx);
						continue;
					}
					buffer1 = "L0L";
					buffer1 += Light;
					lpPerIOData->wbuf.buf = (LPTSTR)(LPCTSTR)buffer1;
					lpPerIOData->wbuf.len = buffer1.GetLength();
					lpPerIOData->flags = 0;
					nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
					if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
					{
						pIOCPServer->ConnListRemove(lpConnCtx);
						continue;
					}
				}
				else
				{
					buffer1 = "L1G";
					lpPerIOData->wbuf.buf = (LPTSTR)(LPCTSTR)buffer1;
					lpPerIOData->wbuf.len = buffer1.GetLength();
					lpPerIOData->flags = 0;
					nResult=WSASend(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,lpPerIOData->flags,&(lpPerIOData->OverLapped),NULL);
					if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
					{
						pIOCPServer->ConnListRemove(lpConnCtx);
						continue;
					}
				}
			}
			else if(pIOCPServer->strSend == "")
			{
				pIOCPServer->InitializeBuffer(lpPerIOData, SVR_IO_READ);
				nResult=WSARecv(lpConnCtx->sockAccept,&(lpPerIOData->wbuf),1,NULL,&(lpPerIOData->flags),&(lpPerIOData->OverLapped),NULL);
				if (nResult == SOCKET_ERROR&&WSAGetLastError()!=ERROR_IO_PENDING)
				{
					pIOCPServer->ConnListRemove(lpConnCtx);
				}
				break;
			}
			else
			{
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
				ZeroMemory(lpConnCtx->pPerIOData->data,sizeof(lpConnCtx->pPerIOData->data));
			}
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

