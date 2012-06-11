// IPPhoneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IPPhone.h"
#include "IPPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CIPPhoneDlg 对话框




CIPPhoneDlg::CIPPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPPhoneDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUT_CALLGROUP, m_BtnCallGroup);
	DDX_Control(pDX, IDC_BUT_CALLMANY, m_BtnCallMany);
	DDX_Control(pDX, IDC_BUT_RECENTCALL, m_BtnRecentCall);
	DDX_Control(pDX, IDC_BUT_CONTACTS, m_BtnContacts);
	DDX_Control(pDX, IDC_BTN_CALL, m_BtnCall);
	DDX_Control(pDX, IDC_BUT_ANSWER, m_BtnStopCall);
	DDX_Control(pDX, IDC_BUT_REVOKE, m_BtnRevoke);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
}

BEGIN_MESSAGE_MAP(CIPPhoneDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CIPPhoneDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CIPPhoneDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CIPPhoneDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CIPPhoneDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CIPPhoneDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CIPPhoneDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CIPPhoneDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CIPPhoneDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CIPPhoneDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CIPPhoneDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON0, &CIPPhoneDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON11, &CIPPhoneDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BTN_CALL, &CIPPhoneDlg::OnBnClickedBtnCall)
	ON_BN_CLICKED(IDC_BUT_REVOKE, &CIPPhoneDlg::OnBnClickedButRevoke)
	ON_BN_CLICKED(IDC_BUT_ANSWER, &CIPPhoneDlg::OnBnClickedButAnswer)
END_MESSAGE_MAP()


// CIPPhoneDlg 消息处理程序

BOOL CIPPhoneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	m_ImageList.Create(48, 48, ILC_COLOR24, 2, 0);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_BtnCall.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_BtnStopCall.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_BtnRevoke.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON4));
	//初始化通信信息
	if (!m_Communication.InitCommunication(24))
	{
		AfxMessageBox(_T("初始化通信信息失败！"));
		return FALSE;
	}
	
	


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIPPhoneDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIPPhoneDlg::OnPaint()
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
HCURSOR CIPPhoneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CIPPhoneDlg::OnBnClickedButton1()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo += '1';
	m_Edit.SetWindowText(ShowInfo);
}

void CIPPhoneDlg::OnBnClickedButton2()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo += '2';
	m_Edit.SetWindowText(ShowInfo);
}

void CIPPhoneDlg::OnBnClickedButton3()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo += '3';
	m_Edit.SetWindowText(ShowInfo);
}

void CIPPhoneDlg::OnBnClickedButton4()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo += '4';
	m_Edit.SetWindowText(ShowInfo);
}

void CIPPhoneDlg::OnBnClickedButton5()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo += '5';
	m_Edit.SetWindowText(ShowInfo);
}

void CIPPhoneDlg::OnBnClickedButton6()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo += '6';
	m_Edit.SetWindowText(ShowInfo);
}

void CIPPhoneDlg::OnBnClickedButton7()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo += '7';
	m_Edit.SetWindowText(ShowInfo);
}

void CIPPhoneDlg::OnBnClickedButton8()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo += '8';
	m_Edit.SetWindowText(ShowInfo);
}

void CIPPhoneDlg::OnBnClickedButton9()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo += '9';
	m_Edit.SetWindowText(ShowInfo);
}

void CIPPhoneDlg::OnBnClickedButton10()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo += '*';
	m_Edit.SetWindowText(ShowInfo);
}

void CIPPhoneDlg::OnBnClickedButton0()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo += '0';
	m_Edit.SetWindowText(ShowInfo);
}

void CIPPhoneDlg::OnBnClickedButton11()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo += '#';
	m_Edit.SetWindowText(ShowInfo);
}
/***********************************************
函数功能：接通电话或者连接
***********************************************/
void CIPPhoneDlg::OnBnClickedBtnCall()
{
	
}

void CIPPhoneDlg::OnBnClickedButRevoke()
{
	CString   ShowInfo;
	UpdateData(TRUE);
	m_Edit.GetWindowText(ShowInfo);
	ShowInfo.Empty();
	m_Edit.SetWindowText(ShowInfo);
}
/***********************************************
函数功能：接通电话或者连接
***********************************************/
void CIPPhoneDlg::OnBnClickedButAnswer()
{
	
}
