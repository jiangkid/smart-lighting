
// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "IOCPServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND show;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CServerDlg 对话框
CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CServerDlg::OnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CServerDlg::OnStop)

	ON_MESSAGE(WM_USER_CLIENT,&CServerDlg::OnClientMsg)
	ON_BN_CLICKED(IDC_BTN_SEND, &CServerDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	
	show = this->m_hWnd;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerDlg::OnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_pIOCPServer->SrartServer())
	{
		MessageBox(_T("启动服务器失败,请稍后再试!"));
		return;
	}

	UpdateData();
	//注册网络读取事件
//	if(SOCKET_ERROR == WSAAsyncSelect(g_pIOCPServer->m_sdListen,this->m_hWnd,WM_USER_CLIENT,FD_READ))
//	{
//		MessageBox(_T("注册网络读取事件失败!"));
//		return ;
//	}
	MessageBox(_T("服务器已启动!"));
}

void CServerDlg::OnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_pIOCPServer->StopServer())
	{
		MessageBox(_T("停止服务器失败,请稍后再试!"));
	}
	
	MessageBox(_T("服务器已停止!"));
	
}

LRESULT CServerDlg::OnClientMsg(WPARAM wParam,LPARAM lParam)
{
	IN_ADDR client;
	client.S_un.S_addr = wParam;
	char *ClientMsg[ ]={"请求连接","连接被接受","连接被接受","连接被拒绝","连接被拒绝","连接关闭","发生错误,连接结束"};
	CString info;
	info.Format(_T("From IP地址%s,%s",inet_ntoa(client),ClientMsg[lParam]));

	CString str = info;

	CString strTemp =_T("hello!");

//	str = g_pIOCPServer->strRecv;

	str +="\r\n";
	GetDlgItemText(IDC_EDIT_RECV,strTemp);

	str +=strTemp;
	SetDlgItemText(IDC_EDIT_RECV,str);
	return TRUE;
	/*
	CString str;
	CString strTemp =_T("hello!");

	str = g_pIOCPServer->strRecv;

	str +="\r\n";
	GetDlgItemText(IDC_EDIT_RECV,strTemp);

	str +=strTemp;
	SetDlgItemText(IDC_EDIT_RECV,str);
	return TRUE;
	*/
}


void CServerDlg::OnBnClickedBtnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//获得待发送的数据
//	CString strSend1;
//	GetDlgItemText(IDC_EDIT_SEND,strSend1);
	
//	g_pIOCPServer->strSend = strSend1.GetBuffer();
//	strSend1.ReleaseBuffer();
	//wcstombs(g_pIOCPServer->strSend,strSend1,BUFFER_SIZE);
//	g_pIOCPServer->str=(char *)strSend;
//	g_pIOCPServer->Send(strSend);

	DWORD dwIP;
	CString strSend;
	WSABUF wsabuf;
	DWORD dwSend;
	int len;
	SOCKADDR_IN addrTo;

	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->GetAddress(dwIP);

	addrTo.sin_addr.S_un.S_addr = htonl(dwIP);
	addrTo.sin_family = AF_INET;
	addrTo.sin_port = htons(6000);

	GetDlgItemText(IDC_EDIT_SEND,strSend);
	len = strSend.GetLength();
	wsabuf.buf = strSend.GetBuffer(len);
	wsabuf.len = len+1;

	if (SOCKET_ERROR==WSASendTo(g_pIOCPServer->m_sdListen,&wsabuf,1,&dwSend,0,(SOCKADDR*)&addrTo,sizeof(SOCKADDR),NULL,NULL))
	{
		MessageBox(_T("发送数据失败!"));
		SetDlgItemText(IDC_EDIT_SEND," ");
		return;
	}
	SetDlgItemText(IDC_EDIT_SEND," ");
}
