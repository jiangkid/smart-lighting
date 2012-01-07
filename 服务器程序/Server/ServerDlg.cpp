
// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"

#include "ServerDlg.h"
#include "Server.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND H_ServerDlg;
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
	ON_BN_CLICKED(IDC_START, &CServerDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_Clear, &CServerDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_END, &CServerDlg::OnBnClickedEnd)
	ON_BN_CLICKED(IDC_STOP, &CServerDlg::OnBnClickedStop)
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
	H_ServerDlg = this->m_hWnd;
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


void CServerDlg::OnBnClickedStart()
{
 	int  nPort1,nPort2;
// 	GetDlgItem(IDC_CSPort)->GetWindowText(port1);
// 	GetDlgItem(IDC_GPRSPort)->GetWindowText(port2);
// 	int nPort1 = UINT(atoi(LPCSTR(port1)));
// 	int nPort2 = UINT(atoi(LPCSTR(port2)));
	
	nPort1=5002;
	nPort2=5003;
	//if ((m_pClientServer.Start(nPort1) != FALSE) && (m_pGPRSServer.StartServer(nPort2) != FALSE))
	if ((m_pClientServer.Start(nPort1)!=FALSE)&&(m_pGPRSServer.StartServer(nPort2)!=FALSE))
	{
		GetDlgItem(IDC_CSPort)->EnableWindow(FALSE);
		GetDlgItem(IDC_GPRSPort)->EnableWindow(FALSE);
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
		::MessageBox(NULL,"服务器开启成功！","提示",MB_OK);
	}	
}

void CServerDlg::OnBnClickedClear()
{
	GetDlgItem(IDC_EDIT1)->SetWindowText(NULL);
	GetDlgItem(IDC_EDIT2)->SetWindowText(NULL);
}

void CServerDlg::OnBnClickedEnd()
{
	if ((m_pClientServer.StopServer() != FALSE) && (m_pGPRSServer.StopServer() != FALSE))
	{
		::MessageBox(NULL,"服务器退出！","提示",MB_OK);
		CDialog::OnOK();
	}	
	//exit(0);
}

void CServerDlg::OnBnClickedStop()
{
	
}
