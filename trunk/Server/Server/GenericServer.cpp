#include "StdAfx.h"
#include "GenericServer.h"
#include "ServerDlg.h"

CGenericServer::CGenericServer(void)
{
	/*****�б�*****/
	m_pFreeBufferList = NULL;				//�������ڴ���������ڴ�ռ�
	m_pFreeContextList = NULL;	
	m_pConnectionList = NULL;
	m_ptrConnCtxHead =NULL;
	m_nFreeBufferCount = 0;
	m_nFreeContextCount = 0;
	m_nCurrentConnection = 0;
	
	/*****��ʼ���ٽ���*****/
	::InitializeCriticalSection(&m_FreeContextListLock);
	::InitializeCriticalSection(&m_ConnectionListLock);
	::InitializeCriticalSection(&TMSection);

	/*****�����¼�*****/
	m_nPort = 5005;
	m_hCompletion = NULL;			//��ɶ˿���Ϊ��
	m_sListen = INVALID_SOCKET;	 //�����׽�����Ϊ��Ч�׽���

	m_bShutDown = FALSE;			//��������ʼ״̬����
	m_bServerStarted = FALSE;

	// ��ʼ��WS2_32.dll
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	::WSAStartup(sockVersion, &wsaData);

	//��sysInfo.dwNumberOfProcessors*2��MAX_THREAD_NUM֮��Ľ�Сֵ����m_nSvcThreadNum;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	//m_nSvrThreadNum=sysInfo.dwNumberOfProcessors*2 < MAX_THREAD ? (sysInfo.dwNumberOfProcessors*2):MAX_THREAD;
	m_nSvrThreadNum=3;
	InitQueue();

}

/****************************��̬������ʼ��****************************/
LPQlist  CGenericServer::QlistCtG;
LPQlist  CGenericServer::QlistGtC;
BOOL CClientServer::AdminLogin = FALSE;

CGenericServer::~CGenericServer(void)
{
	if(m_sListen != INVALID_SOCKET)								 //�رռ����׽���
	::closesocket(m_sListen);		
	::DeleteCriticalSection(&m_FreeContextListLock);		//ɾ���ٽ���
	::DeleteCriticalSection(&m_ConnectionListLock);		
	::DeleteCriticalSection(&TMSection);
	WSACleanup();
}

/*****socket������󶨡���ɶ˿ڴ���*****/
BOOL CGenericServer::Initialize_Socket(int  nPort)
{
	m_nPort = nPort;
	/*****�׽��ִ������󶨵����ض˿ڣ��������ģʽ*****/
	m_sListen = socket(AF_INET,SOCK_STREAM,0);
	addrLocal.sin_family = AF_INET;
	addrLocal.sin_addr.S_un.S_addr = INADDR_ANY;
	addrLocal.sin_port=::htons(m_nPort);
	if(::bind(m_sListen,(sockaddr*)&addrLocal,sizeof(addrLocal)) == SOCKET_ERROR)
	{
		m_bServerStarted = FALSE;
		return FALSE;
	}
	::listen(m_sListen,300);
	m_hCompletion = CreateNewIoCompletionPort(0);
	if (m_hCompletion == NULL)
	{
		DWORD  CreIO_error = GetLastError();
		return  FALSE;
	}
	return TRUE;
}

void CGenericServer::InitializeBuffer(LPCIOCPBuffer pBuffer, IO_OPER  operation)
{
	ZeroMemory(pBuffer,sizeof(LPCIOCPBuffer));
	ZeroMemory(pBuffer->data, sizeof(pBuffer->data));
	pBuffer->OverLapped.hEvent=NULL;
	pBuffer->OverLapped.Internal=0;
	pBuffer->OverLapped.InternalHigh=0;
	pBuffer->OverLapped.Offset=0;
	pBuffer->OverLapped.OffsetHigh=0;
	pBuffer->wbuf.buf=(char *) &(pBuffer->data);
	pBuffer->wbuf.len=BUFFER_SIZE;
	pBuffer->oper=operation;
	pBuffer->flags=0;
	return ;
}
/***************************************************************
�������ܣ�����I/O��ɶ˿�
***************************************************************/
HANDLE CGenericServer::CreateNewIoCompletionPort(DWORD dwNumberOfConcurrentThreads)
{
	return (CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,dwNumberOfConcurrentThreads));
}

/***************************************************************
�������ܣ����׽ӿ�����ɶ˿ڰ�
***************************************************************/
BOOL CGenericServer::AssociateWithIoCompletionPort(HANDLE hComPort,HANDLE hDevice,DWORD dwCompKey)
{
	return (CreateIoCompletionPort(hDevice,hComPort,dwCompKey,0)==hComPort);
}

/***************�����׽��������Ĳ�����***************/
LPCIOCPContext CGenericServer::AllocateContext(SOCKET s, SOCKADDR_IN  addraccept)
{
	CIOCPContext *pContext = new CIOCPContext;
	if (pContext == NULL)
	{
		return NULL;
	}
	::EnterCriticalSection(&m_FreeContextListLock);
	pContext->pNext=NULL;
	pContext->pPrec=NULL;
	pContext->sockAccept=s;
	pContext->LogIn = FALSE;
	pContext->ID = FALSE;
	ZeroMemory(pContext->Identify,sizeof(pContext->Identify));
	ZeroMemory(pContext->pPerIOData,sizeof(CIOCPBuffer));
	pContext->pPerIOData->OverLapped.hEvent=NULL;
	pContext->pPerIOData->OverLapped.Internal=0;
	pContext->pPerIOData->OverLapped.InternalHigh=0;
	pContext->pPerIOData->OverLapped.Offset=0;
	pContext->pPerIOData->OverLapped.OffsetHigh=0;
	pContext->pPerIOData->wbuf.buf=(char *)pContext->pPerIOData->data;
	pContext->pPerIOData->wbuf.len=BUFFER_SIZE;
	pContext->pPerIOData->oper=SVR_IO_READ;
	pContext->addrAccept = addraccept;
	pContext->pPerIOData->flags=0;

	//���׽ӿ�����ɶ˿ڰ�
	if(!AssociateWithIoCompletionPort(m_hCompletion,(HANDLE) s,(DWORD) pContext))
	{
		delete pContext;
		pContext =NULL;
		return NULL;
	}
	::LeaveCriticalSection(&m_FreeContextListLock);
	if(pContext!=NULL)
	{
		pContext->sockAccept = s;
	}
	return pContext;
}

/***************�������б������һ������***************/
void CGenericServer::ConnListAdd(LPCIOCPContext lpConnCtx)
{
	LPCIOCPContext  pTemp;
	EnterCriticalSection(&m_ConnectionListLock);
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
	LeaveCriticalSection(&m_ConnectionListLock);
}

//��������ϢlpConnCtx��CGenericServerģ��ά�ֵ�ȫ�ֱ�����ɾ��
//�����ӽ���ʱ����
//
void CGenericServer::ConnListRemove(LPCIOCPContext  lpConnCtx)
{
	LPCIOCPContext    pPrec;
	LPCIOCPContext    pNext;

	EnterCriticalSection(&m_ConnectionListLock);
	if (lpConnCtx->ID == TRUE)
	{
		AdminLogin = FALSE;
	}
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
			closesocket(lpConnCtx->sockAccept);
			lpConnCtx->sockAccept = INVALID_SOCKET;
		}
		delete lpConnCtx;
		lpConnCtx=NULL;
	}
	LeaveCriticalSection(&m_ConnectionListLock);
	return;
}

//��ɷ������˳�ʱ�ر����ӡ��ͷ���Դ�Ĺ�����
//��ȫ��������Ϣ�����е�ÿ���ڵ㣬�������ConnListRemove����

void CGenericServer::ConnListClear()
{
	LPCIOCPContext pTemp1,pTemp2;

	EnterCriticalSection(&m_ConnectionListLock);
	pTemp1=m_ptrConnCtxHead;
	while(pTemp1)
	{
		pTemp2=pTemp1->pNext;
		ConnListRemove(pTemp1);
		pTemp1=pTemp2;
	}
	LeaveCriticalSection(&m_ConnectionListLock);
	return;
}

 void CGenericServer::ResetAll()
{
	m_ServerState=SERVER_STOP;
	m_sListen=INVALID_SOCKET;
	for(int i=0; i<3; i++)//for(int i=0; i<MAX_THREAD+2; i++)�˴������߳����ı�
	{
		if(m_hThreadList[i])
			CloseHandle(m_hThreadList[i]);
		m_hThreadList[i]=NULL;
	}
	if(m_hCompletion)
	{
		CloseHandle(m_hCompletion);
		m_hCompletion=NULL;
	}
	if(m_ptrConnCtxHead)
		ConnListClear();
	m_ptrConnCtxHead=NULL;
	return ;
}

 
 /******* ��ʼ������ ******/
 BOOL CGenericServer::InitQueue()
 {
    QlistCtG = new Qlist;
	QlistCtG->front = QlistCtG->rear = new ThreadMessage;
	QlistCtG->front->pnext = NULL;
	QlistGtC = new Qlist;
	QlistGtC->front = QlistGtC->rear = new ThreadMessage;
	QlistGtC->front->pnext = NULL;
	return TRUE;
 }

 /******* ���ٶ��� ����ͷ��β�����٣�******/
 BOOL CGenericServer::DestroyQueue(LPQlist Qlist)
 {
	while(Qlist->front)
	{
		Qlist->rear = Qlist->front->pnext;
		delete Qlist->front;
		Qlist->front = Qlist->rear;
	}
	return TRUE;
 }

 /******* ����Ϣ��ӵ�������******/
 BOOL CGenericServer::InQueue(LPQlist Qlist,CHAR * buffer,int lenth)
 {	
	EnterCriticalSection(&TMSection);
	LPThreadMessage Messagebuf;
	Messagebuf= new ThreadMessage;
	if (Messagebuf == NULL)
	{
	 return FALSE;
	}
	memset(Messagebuf,'\0',sizeof(ThreadMessage));
	//strcpy_s(Messagebuf->sData,buffer);
	memcpy(Messagebuf->sData,buffer,lenth);
	Messagebuf->strlen = lenth;
	Messagebuf->pnext = NULL;
	Qlist->rear->pnext = Messagebuf;
	Qlist->rear = Messagebuf; 
	LeaveCriticalSection(&TMSection);
	return TRUE;	
 }

  /******* ����Ϣ�Ӷ������Ƴ�******/
char* CGenericServer::OutQueue(LPQlist Qlist,int &length)
{
	char temp[BUFFER_SIZE];
	//char * temp = malloc(length);
	if ((Qlist->front == Qlist->rear) )
	{
		return "error";
	}
	EnterCriticalSection(&TMSection);
  	LPThreadMessage pItem;
	pItem = Qlist->front->pnext;
	ZeroMemory(temp,BUFFER_SIZE);
	length = pItem->strlen;
	memcpy(temp,pItem->sData,length);
	//temp = pItem->sData;
	
	Qlist->front->pnext = pItem->pnext;
	/*char recvt[BUFFER_SIZE];
	GetDlgItemText(H_ServerDlg,IDC_EDIT2,recvt,BUFFER_SIZE);		
	temp +="\r\n";
	temp+=recvt;
	SetDlgItemText(H_ServerDlg,IDC_EDIT2,temp);*/
	if (Qlist->rear == pItem)
	{
		Qlist->rear = Qlist->front;
	}
	delete pItem;
	pItem = NULL;
	LeaveCriticalSection(&TMSection);	
	return temp;
 }

CString CGenericServer::GetPswByAdminName(CString AdminName)
{
	_ConnectionPtr m_pConnection;
	_variant_t RecordsAffected;
	_RecordsetPtr m_pRecordset;
	//AfxEnableControlContainer();                                 
	CString SQL;
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");
		_bstr_t Connection = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=E:\\Server_last\\Server\\Demo.mdb";
		m_pConnection->Open(Connection,"","",adModeUnknown);
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.ErrorMessage());
		return NULL;
	}
	try
	{
		m_pRecordset.CreateInstance("ADODB.Recordset");
		SQL.Format("SELECT * From Admin Where Name = '%s'",AdminName);
		m_pRecordset = m_pConnection->Execute(_bstr_t (SQL),&RecordsAffected,adCmdText);
		//m_pRecordset = m_pConnection->Execute("SELECT * From Admin Where Name = 'martin'",&RecordsAffected,adCmdText);
	}
	catch(_com_error &e)
	{
		AfxMessageBox(e.ErrorMessage());
		for (long i =0;i <m_pConnection->Errors->Count; i++)
		{
			AfxMessageBox(m_pConnection->Errors->GetItem(_variant_t(i))->Description);
		}
	}
	_variant_t vPassword;
	CString strTemp;
	try
	{
		while (!m_pRecordset->adoEOF)
		{
			vPassword = m_pRecordset->GetCollect("Password");
			strTemp =(BSTR) vPassword.pbstrVal;
			m_pRecordset->MoveNext();
		}
	} 
	catch(_com_error &e)
	{
		AfxMessageBox(e.Description());
	}
	return strTemp;
	m_pRecordset->Close();
	m_pRecordset=NULL;
	m_pConnection->Close();
	m_pConnection = NULL;
}
 /********************************************
�������ܣ���16byte��char��IDת��Ϊ8byte��char��ID
 ********************************************/
char* CGenericServer::Translation_ID(char* buffer, int Length)
{
	char c[8];
	ZeroMemory(c,8);
	int nCount = 0;
	CString temp;
	int i,j;
	for (i=0;i<Length/2;i++)
	{
		CString temp="";
		for (j=nCount*2;j<nCount*2+2;j++)
		{
			temp+=buffer[j+5];
		}
		sscanf(temp,"%2x",&c[nCount]);
		nCount++;
	}
	return c;
}

 /********************************************
�������ܣ���16byte��char��IDת��Ϊ8byte��char��ID
 ********************************************/
U8* CGenericServer::GPRSTranslationID(U8* buffer, int Length)
{
	U8 c[20];
	ZeroMemory(c,20);
	int nCount = 0;
	CString temp;
	int i,j;
	for (i=0;i<Length/2;i++)
	{
		CString temp="";
		for (j=nCount*2;j<nCount*2+2;j++)
		{
			temp+=buffer[j];
		}
		sscanf(temp,"%2x",&c[nCount]);
		nCount++;
	}
	return c;
}

 /******************************************************
�������ܣ���8byte��char��IDת��Ϊ16byte��char��ID
 ******************************************************/
char *CGenericServer::CharToCString(UCHAR* str, int nLength)
{
	//CString strShow=_T("");
	LPTSTR p;
	char szText[1024];
	ZeroMemory(szText, 1024);
	p = szText;
	for (int i = 0; i< nLength; i++)
	{
		p+= wsprintf(p, "%.2X", str[i]);  //�ⲿ��Ϊ�ؼ�����
	}
	//strShow.Format(_T("%s"), szText);
	return szText;
}
/************************************************************************/
/*����: ��˫�����в���Ԫ��Identify ,��ȡ����Ӧ��������(����Ա�ѵ�½����ȡ���Ĺ���Ա����Ȼȡ�����Ƕ�Ӧ����ͨ�û�)
����:Ԫ��Identify�������������ڵ�λ��*/
/************************************************************************/
LPCIOCPContext CGenericServer::LocateElement(char * identify)
{
	LPCIOCPContext  sock;
	if(m_ptrConnCtxHead == NULL)
	{
		return  sock = NULL;
	}
	CIOCPContext * p = m_ptrConnCtxHead;
	if (AdminLogin == TRUE) //�������Ա�Ѿ���¼������ID== true����
	{
		while (p->pNext!=NULL&&(p->ID != TRUE))
		{
			p = p->pNext;
		}
		if (p->ID == TRUE)
		{
			return  sock =  p;
		}
		else return sock = NULL;
	}
	else  //����Աû�е�½���������������
	{
		while (p->pNext!=NULL&&(strcmp(p->Identify,identify)!=0))
		{
			p = p->pNext;
		}
		if (strcmp(p->Identify,identify) == 0)
		{
			return  sock =  p;
		}
		else return sock =NULL;
	}		
}

/************************************************************************/
/*����: ��GPRS˫�����в���Ԫ��Identify ,��ȡ����Ӧ��������
����:Ԫ��Identify�������������ڵ�λ��*/
/************************************************************************/
LPCIOCPContext CGenericServer::LocateGPRSElement(char * identify,  int length)
{
	LPCIOCPContext  pGPRSContext;
	char buffer[2];
	//memcpy(buffer,identify,length);
	if(m_ptrConnCtxHead == NULL)
	{
		return  pGPRSContext = NULL;
	}
	CIOCPContext * p = m_ptrConnCtxHead;
	while (p->pNext!=NULL&&(strncmp(p->Identify,identify,length)!=0))
	{
		p = p->pNext;
	}
	if (strncmp(p->Identify,identify,length) == 0)
	{
		return  pGPRSContext =  p;
	}
	else return pGPRSContext =NULL;
}

/*********************************************************************************
*�������ܣ���ʾ
*�������ݣ�
*��1�������������ͻ���GPRS����������Ϣ 
*��2���ͻ��˷���������������
*��3�����������͸�GPRS������
*��4��GPRS��������������Ϣ
**********************************************************************************/
void CGenericServer::ShowMessage(char *buffer, int length, CString name, int ID)
{
	U8   sendMessage[1024];
	U8 * strSendID;
	char Message[1024];
	char data[4096];
	char *temp, *head, *body;
	ConTrlInfo *m_ControlCode;
	LPHDR  pHead;
	CString  show;
	ZeroMemory(sendMessage, sizeof(sendMessage));
	ZeroMemory(Message, sizeof(Message));
	ZeroMemory(data, sizeof(data));

	switch(ID)
	{
	case 1:						 //�����ͻ���GPRS����������Ϣ
		show = "Server Send Message to Client ( ";
		show += name;
		show += " ) : ";
		memcpy(Message,buffer,3);
		show += Message;
		memcpy(sendMessage,buffer+3,4);
		temp = CharToCString(sendMessage,4);
		ZeroMemory(Message,sizeof(Message));
		memcpy(Message,temp,8);
		show += Message;
		ZeroMemory(Message,sizeof(Message));
		memcpy(Message,buffer+7,16);
		show += Message;
		ZeroMemory(sendMessage,sizeof(sendMessage));
		memcpy(sendMessage,buffer+23,9);
		temp = CharToCString(sendMessage,9);
		ZeroMemory(Message,sizeof(Message));
		memcpy(Message,temp,18);
		show += Message;
		break;

	case  2:				//�ͻ��˷����������
		if (buffer[0] == 'N')   //�ͻ���Ҫ���͵�GPRS������
		{
			show = name;
			head = (char *)malloc(HeadLen);
			memcpy(head,buffer,HeadLen);
			pHead = (LPHDR)head;
			body = (char*)malloc(pHead->dataLen);
			memcpy(body,buffer+HeadLen,pHead->dataLen);
			m_ControlCode = (ConTrlInfo *)body;
			strSendID = GPRSTranslationID(m_ControlCode->m_ID, 16);   //�Ƚ�IDת����ʮ�������ͣ���Ϊԭ��ID���ܴ���0x00,����ֱ�Ӹ�CString
			sendMessage[0] = 'N';
			memcpy(sendMessage+1, m_ControlCode->m_First, 4);
			memcpy(sendMessage+1+4,strSendID,8);
			memcpy(sendMessage+1+12,m_ControlCode->m_OrderType, 1);//�˴�m_OrderType��m_OrderObject�������ڷ���ʱ�ͻ���Ӧ���Ѿ�����
			memcpy(sendMessage+1+13,m_ControlCode->m_OrderObject, 1);
			memcpy(sendMessage+1+14,m_ControlCode->m_ActiveType, 1);
			memcpy(sendMessage+1+15,m_ControlCode->m_CheckData, 4);
			memcpy(sendMessage+1+19,m_ControlCode->m_EndBuffer, 2);
			temp = CharToCString(sendMessage,22);								//ȫ��ת��Ϊʮ�����ƺ���ת��ΪCString
			memcpy(Message,temp,44);
			show +=Message;
			free(head);
			free(body);
		}
		else		//�ͻ��˷��͵�����������д���ݿ������
		{
			ZeroMemory(Message,sizeof(Message));
			show = name;
			memcpy(sendMessage, buffer, length);
			temp = CharToCString(sendMessage, length);
			memcpy(Message, temp, length*2);
			show += Message;
		}
		break;

	case  3:   //���������͸�GPRS������
		ZeroMemory(Message,sizeof(Message));
		show = name;
		memcpy(sendMessage, buffer, length);
		temp = CharToCString(sendMessage, length);
		memcpy(Message, temp, length*2);
		show += Message;
		break;

	case 4:  //GPRS����������������
		ZeroMemory(Message,sizeof(Message));
		show = name;
		memcpy(sendMessage, buffer, length);
		temp = CharToCString(sendMessage, length);
		memcpy(Message, temp, length*2);
		show += Message;
		break;

	default:
		break;
	}
	GetDlgItemText(H_ServerDlg,IDC_EDIT1,data,BUFFER_SIZE);
	show += "\r\n";
	show += data;
	SetDlgItemText(H_ServerDlg,IDC_EDIT1, show);
}

/*********************************************************************************
*�������ܣ����ͻ��˷��͵��������txt�ĵ���ÿ���½�һ���ĵ�constconst 
*�������ݣ��ͻ��˷��͵ĵ�����
**********************************************************************************/
bool CGenericServer::CommandRecord(char *Command,  char *name, int length)
{
		CTime time;
		CFile  pFile;									//�ļ�
		CFileException Error ;					//�����ַ
		CString  PathName;						//·��
		CString  Record;							//�����ļ�������
		CString strTime;
		char text[1024];							//д���ı��Ļ������������ȴ���text
		char *strRecord;
		int  number;
		int len;
		ULONGLONG dwLength ;			//�ı�����

		//���¶���ı��������ڽ�����ת����16���Ƶ���Ҫ
		U8   sendMessage[1024];
		U8 * strSendID;                                  //ָ��IDת����ʮ�������͵ķ���ֵ
		char Message[1024];
		char *temp, *head, *body;
		ConTrlInfo *m_ControlCode;
		LPHDR  pHead;
		CString  Command_16h;
	
		ZeroMemory(text, sizeof(text));
		ZeroMemory(Message, sizeof(Message));
		ZeroMemory(sendMessage, sizeof(sendMessage));
		time = CTime::GetCurrentTime(); 
		strTime = time.Format(".\\logo\\logo%Y-%m-%d.txt");				//ÿ�촴��һ��txt�ĵ�%Y-%m-%d-%H:%M:%S
		PathName = strTime;
		strTime.Empty();
		strTime = time.Format("%Y-%m-%d-%H:%M:%S : ");	
		//Ҫд���ļ�������
		Record.Empty();
		Record.Format(("%s Send Message at "), name);
		Record += strTime;
		if (Command[0] == 'N')													//�ͻ���Ҫ���͵�GPRS������
		{
			Command_16h.Empty();
			head = (char *)malloc(HeadLen);
			memcpy(head, Command, HeadLen);
			pHead = (LPHDR)head;
			body = (char*)malloc(pHead->dataLen);
			memcpy(body,Command+HeadLen,pHead->dataLen);
			m_ControlCode = (ConTrlInfo *)body;
			strSendID = GPRSTranslationID(m_ControlCode->m_ID, 16);   //�Ƚ�IDת����ʮ�������ͣ���Ϊԭ��ID���ܴ���0x00,����ֱ�Ӹ�CString
			sendMessage[0] = 'N';
			memcpy(sendMessage+1, m_ControlCode->m_First, 4);
			memcpy(sendMessage+1+4, strSendID,8);
			memcpy(sendMessage+1+12, m_ControlCode->m_OrderType, 1);//�˴�m_OrderType��m_OrderObject�������ڷ���ʱ�ͻ���Ӧ���Ѿ�����
			memcpy(sendMessage+1+13, m_ControlCode->m_OrderObject, 1);
			memcpy(sendMessage+1+14, m_ControlCode->m_ActiveType, 1);
			memcpy(sendMessage+1+15, m_ControlCode->m_CheckData, 4);
			memcpy(sendMessage+1+19, m_ControlCode->m_EndBuffer, 2);
			temp = CharToCString(sendMessage,22);								//ȫ��ת��Ϊʮ�����ƺ���ת��ΪCString
			memcpy(Message,temp,44);
			Command_16h = Message;
			free(head);
			free(body);
		}
		else		//�ͻ��˷��͵�����������д���ݿ������
		{
			Command_16h.Empty();
			ZeroMemory(Message,sizeof(Message));
			memcpy(sendMessage, Command, length);
			temp = CharToCString(sendMessage, length);
			memcpy(Message, temp, length*2);
			Command_16h = Message;
		}
		Record += Command_16h;
		Record += "\r\n";
		len = Record.GetLength();
		strRecord = Record.GetBuffer();
		Record.ReleaseBuffer();
		memcpy(text, strRecord, len);

		//�Ȳ鿴�ļ��Ƿ����
		if (!pFile.Open(PathName, CFile::modeReadWrite, &Error))  //�����ʧ�ܣ���ٶ�Դ�ļ������ڣ��½��ļ�
		{
			HANDLE  hFile = CreateFile(PathName,	GENERIC_WRITE, FILE_SHARE_READ,
				NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				return false;
			}
			CloseHandle(hFile);
			if (!pFile.Open(PathName, CFile::modeReadWrite, NULL))
			{
				return false;
			}
		}
		//��ȡ�ļ�����Դ���ݵĳ���
		dwLength = pFile.GetLength();
		//ʹָ��ָ��ԭ����������֮��
		pFile.Seek(dwLength, CFile::begin);
		pFile.Write(text, len);
		pFile.Flush();
		pFile.Close();
		return true;
}