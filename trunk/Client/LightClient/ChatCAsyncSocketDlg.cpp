// ChatCAsyncSocketDlg.cpp : implementation file
//

#include "stdafx.h"
#include "lightclient.h"
#include "ChatCAsyncSocketDlg.h"
#include <ASSERT.H>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatCAsyncSocketDlg dialog

#define BUFFER_SIZE 4096
CChatCAsyncSocketDlg::CChatCAsyncSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatCAsyncSocketDlg::IDD, pParent),m_ClientSocket(this),m_ListenSocket(this)
{
	//{{AFX_DATA_INIT(CChatCAsyncSocketDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChatCAsyncSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatCAsyncSocketDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChatCAsyncSocketDlg, CDialog)
	//{{AFX_MSG_MAP(CChatCAsyncSocketDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_EN_UPDATE(IDC_INPUTTEXT,SendMessageToPeer)
	ON_COMMAND(ID_Connect, OnConnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatCAsyncSocketDlg message handlers
BOOL CChatCAsyncSocketDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bActive=FALSE;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatCAsyncSocketDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

	CDialog::OnSysCommand(nID, lParam);

}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatCAsyncSocketDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatCAsyncSocketDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
void CChatCAsyncSocketDlg::OnConnect() 
{
	// TODO: Add your control notification handler code here
	sockaddr_in addr;
	CString buffer;
	UINT port;
	char serverIP[16];
	CWnd *subWnd;

	//get control input
	subWnd=GetDlgItem(IDC_LISTEN_PORT_C);
	subWnd->GetWindowText(buffer);
	port= UINT(atoi(LPCSTR(buffer)));
	subWnd=GetDlgItem(IDC_SERVERIP);
	subWnd->GetWindowText(buffer);
	strncpy(serverIP,LPCTSTR(buffer),16);

	//end last session
	ExitChat();

	m_bClient=TRUE;
	if(inet_addr(serverIP)==INADDR_NONE)
	{
		AfxMessageBox("Invalid IP Address!");
		return;
	}

	//create tcp/stream data socket
	m_ClientSocket.Create(0,SOCK_STREAM);

	//set the server address and port
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(serverIP);
	addr.sin_port = htons(port);   //short from host to network format
	
	//connect to server
	m_ClientSocket.Connect((sockaddr*)&addr, sizeof(addr));
	m_ChatWords+="Connecting...\r\n";
	RefreshScreen();
}

void CChatCAsyncSocketDlg::OnListen() 
{
	// TODO: Add your control notification handler code here
	CString buffer;
	CWnd *subWnd;
	UINT port;
	BOOL bRet;

	//get control input
//	subWnd=GetDlgItem(IDC_LISTEN_PORT_S);
//	subWnd->GetWindowText(buffer);
//	port= UINT(atoi(LPCSTR(buffer)));

	ExitChat();
	m_bClient=FALSE;
	
	//create a tcp/stream based socket
	bRet=m_ListenSocket.Create(port);
	if(bRet==FALSE)
	{
		m_ChatWords+="Server socket create error.\r\n";
		GetErrorReason(WSAGetLastError());
		RefreshScreen();
		m_ListenSocket.Close();
		return;
	}
		
	//listening
	bRet=m_ListenSocket.Listen(5);
	if(bRet == FALSE)
	{
		m_ChatWords+="Listen error.\r\n";
		RefreshScreen();
		AfxMessageBox("Listen Error.");
		return;
	}

	m_ChatWords+="Server is listening...\r\n";
	RefreshScreen();
	m_bActive=TRUE;
}

void CChatCAsyncSocketDlg::OnSocketReceive(CDataSocket *pSocket)
{
	if(m_bClient)
	{
		assert(pSocket==&m_ClientSocket);
		OnClientReceive();
	}
	else
		OnServerReceive(pSocket);
}

void CChatCAsyncSocketDlg::OnSocketClose(CDataSocket *pSocket)
{
	if(m_bClient)
	{
		assert(pSocket==&m_ClientSocket);
		OnClientClose();
	}
	else
		OnServerClose(pSocket);
}

void CChatCAsyncSocketDlg::OnClientConnect(int iResult)
{
	CString buffer;
	sockaddr_in name;
	int namelen;
	
	if(iResult!=0)
	{
		GetErrorReason(iResult);
		buffer.Format("Failed connected to server.\r\n");
		m_ChatWords+=buffer;
		RefreshScreen();
		m_ClientSocket.Close();
	}
	else
	{
		namelen=sizeof(sockaddr_in);
		m_ClientSocket.GetPeerName((sockaddr*)&name,&namelen);
		buffer.Format("Successfully connected to %s:%d.\r\n",inet_ntoa(name.sin_addr),ntohs(name.sin_port));
		m_ChatWords+=buffer;
		RefreshScreen();
		m_bActive=TRUE;
	}
}

void CChatCAsyncSocketDlg::OnClientReceive()
{
	char buffer[BUFFER_SIZE+1];
	int retCode;
	retCode=m_ClientSocket.Receive(buffer,BUFFER_SIZE,0);
	if(retCode!=SOCKET_ERROR)
	{
		buffer[retCode]=NULL;
		m_ChatWords+=buffer;
	}
	else
		GetErrorReason(WSAGetLastError());
	RefreshScreen();
}

void CChatCAsyncSocketDlg::OnClientClose()
{
	GetErrorReason(WSAGetLastError());
	m_ClientSocket.Close();
	m_ChatWords+="Server close session.Successfully log out.\r\n";
	RefreshScreen();
	m_bActive=FALSE;
}

void CChatCAsyncSocketDlg::OnServerAccept()
{
	CDataSocket *pSocket = new CDataSocket(this);
	int i,namelen;
	char buffer[BUFFER_SIZE+1];
	sockaddr_in name;
	namelen=sizeof(name);

	//accept the client request
	m_ListenSocket.Accept(*pSocket,NULL,NULL);
	//in fact we can get the peer when calling accept, here we use getpeername instead
	pSocket->GetPeerName((sockaddr*)&name,&namelen);
	//send a message of logining to other clients
	sprintf(buffer,"A guest joins us.(%s:%d)\r\n",inet_ntoa(name.sin_addr),ntohs(name.sin_port));
	m_ChatWords+=buffer;
	for(i=0;i<m_DataSockets.GetSize();i++)
		m_DataSockets[i]->SendData(buffer,strlen(buffer));
	//send a welcome message to current client
	sprintf(buffer, "Welcome !(You ID is: %s:%d.)\r\n",inet_ntoa(name.sin_addr),ntohs(name.sin_port));
	//send(socket,buffer,strlen(buffer),0);
	pSocket->SendData(buffer,strlen(buffer));
	//refresh srceen text
	RefreshScreen();
	//push the client socket down to the client sockets list
	m_DataSockets.Add(pSocket);
}

void CChatCAsyncSocketDlg::OnServerReceive(CDataSocket *pSocket)
{
	int i,retCode;
	char buffer[BUFFER_SIZE+1];
	//get the client message, the client socket ID is wParam
	retCode=pSocket->Receive(buffer,BUFFER_SIZE,0);
	buffer[retCode]=NULL;
	//send the received message to other clients
	for(i=0;i<m_DataSockets.GetSize();i++)
	{
		if(pSocket!=m_DataSockets[i])
			m_DataSockets[i]->SendData(buffer,strlen(buffer));
	}
	m_ChatWords+=buffer;
	//refresh screen
	RefreshScreen();
}

void CChatCAsyncSocketDlg::OnServerClose(CDataSocket *pSocket)
{
	int i,namelen;
	CString buffer;
	sockaddr_in name;
	namelen=sizeof(name);
	//close the client socket who has left, the client socket ID is wParam
	pSocket->GetPeerName((sockaddr*)&name,&namelen);
	pSocket->Close();
	//erase the client socket from client socks list
	for(i=0; i<m_DataSockets.GetSize(); i++)
	{
		if(m_DataSockets[i]==pSocket)
		{
			m_DataSockets.RemoveAt(i);
			break;
		}
	}
	//refresh screen
	buffer.Format("Client %s:%d left.\r\n", inet_ntoa(name.sin_addr),ntohs(name.sin_port));
	for(i=0; i<m_DataSockets.GetSize(); i++)
		m_DataSockets[i]->SendData(LPCSTR(buffer), buffer.GetLength());
	m_ChatWords+=buffer;
	RefreshScreen();
	delete pSocket;
}

//scroll the chat words
void CChatCAsyncSocketDlg::RefreshScreen()
{
	int n;
	GetDlgItem(IDC_SHOWTEXT)->SetWindowText(m_ChatWords);
	n=((CEdit *)(GetDlgItem(IDC_SHOWTEXT)))->GetLineCount();
	((CEdit *)(GetDlgItem(IDC_SHOWTEXT)))->LineScroll(n);
}

//send message to peer
void CChatCAsyncSocketDlg::SendMessageToPeer()
{
	CString buffer;
	int i;
	static int oldNumOfChars=0;
	CWnd *subwnd;

	subwnd=GetDlgItem(IDC_INPUTTEXT);
	subwnd->GetWindowText(buffer);
	if(oldNumOfChars!=buffer.GetLength())
	{
		oldNumOfChars=buffer.GetLength();
		return;
	}
	//empty content of input edit box
	subwnd->SetWindowText("");
	oldNumOfChars=0;
	if(!m_bActive)
	{
		m_ChatWords+=buffer;
		m_ChatWords+="(Hint: you are isolated now.)\r\n";
		RefreshScreen();
		return;
	}
	buffer+="\r\n";
	m_ChatWords+=buffer;
	RefreshScreen();
	if(m_bClient)
		m_ClientSocket.SendData(LPCSTR(buffer),buffer.GetLength());
	else
	{
		for(i=0;i<m_DataSockets.GetSize();i++)
			m_DataSockets[i]->SendData(LPCSTR(buffer),buffer.GetLength());
	}
}

void CChatCAsyncSocketDlg::ExitChat()
{
	int i;
	CString buffer;
	if(m_bActive)
	{
		if(m_bClient)
		{
			m_ClientSocket.Close();
			m_ChatWords+="Successfully log out.\r\n";
			RefreshScreen();
		}
		else
		{
			buffer="I will leave. Pls clients log out ASAP.\r\n";
			for(i=0; i<m_DataSockets.GetSize(); i++)
			{
				m_DataSockets[i]->SendData(LPCSTR(buffer),buffer.GetLength());
				m_DataSockets[i]->Close();
				delete m_DataSockets[i];
			}
			m_DataSockets.RemoveAll();
			
			m_ListenSocket.Close();
			m_ChatWords+="Successfully close server.\r\n";
			RefreshScreen();
		}
	}
	m_bActive=FALSE;
}

void CChatCAsyncSocketDlg::GetErrorReason(int nErrorCode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,nErrorCode,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,0,NULL);
	m_ChatWords+=(char *)lpMsgBuf;
	LocalFree( lpMsgBuf );
}

void CChatCAsyncSocketDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}
