
// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "ServerDlg.h"
#include "Server.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND H_ServerDlg;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CServerDlg �Ի���




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
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_BN_CLICKED(IDC_START, &CServerDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_Clear, &CServerDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_END, &CServerDlg::OnBnClickedEnd)
END_MESSAGE_MAP()


// CServerDlg ��Ϣ�������

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	H_ServerDlg = this->m_hWnd;

	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID =  IDR_MAINFRAME;
	m_nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	m_nid.uCallbackMessage = WM_SHOWTASK;       //�Զ������Ϣ����
	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy(m_nid.szTip,"����������");
	Shell_NotifyIcon(NIM_ADD,&m_nid);   //�����������ͼ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		::MessageBox(NULL,"�����������ɹ���","��ʾ",MB_OK);
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
		Shell_NotifyIcon(NIM_DELETE, &m_nid);
		::MessageBox(NULL,"�������˳���","��ʾ",MB_OK);
		CDialog::OnOK();
	}	
}

LRESULT CServerDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME)
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP: // �Ҽ�����ʱ�����˵�
		{
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint); // �õ����λ��
			CMenu menu;
			menu.CreatePopupMenu(); // ����һ������ʽ�˵�
			menu.AppendMenu(MF_STRING, WM_DESTROY, "��ʼ");
			menu.AppendMenu(MF_STRING, WM_DESTROY, "�ر�");
			menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x ,lpoint->y, this);
			HMENU hmenu = menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
		}
		break;
	case WM_LBUTTONDBLCLK: // ˫������Ĵ���
		{
			this->ShowWindow(SW_SHOWNORMAL); // ��ʾ������
		}
		break;
	}
	return 0;
}

void CServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if(nType == SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE); // ����С���У�����������
	}
}

BOOL CServerDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	// ��������ɾ��ͼ��
	
	return CDialog::DestroyWindow();
}


