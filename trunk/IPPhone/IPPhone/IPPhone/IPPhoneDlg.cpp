// IPPhoneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IPPhone.h"
#include "IPPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CIPPhoneDlg �Ի���




CIPPhoneDlg::CIPPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPPhoneDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIPPhoneDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	
	ON_COMMAND(5001, OnCall)
	ON_COMMAND(5002, OnRecentCall)
	ON_COMMAND(5003, OnLinkMan)
	ON_COMMAND(5004, OnBasicConfig)
END_MESSAGE_MAP()


// CIPPhoneDlg ��Ϣ�������

BOOL CIPPhoneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	UINT   array[4] = {5001, 5002, 5003, 5004};  
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ImageList.Create(32, 32, ILC_COLOR32 | ILC_MASK, 8, 0);//ILC_MASK��ͼƬ��͸����ʾ
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON5));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON6));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON7));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON8));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON9));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON10));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON11));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON12));

	//������
	BOOL  Ret = m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP|CBRS_GRIPPER |CBRS_TOOLTIPS| CBRS_FLYBY |  CBRS_SIZE_DYNAMIC , CRect(4,4,0,0)) ;
	BOOL  Ret1 = m_ToolBar.LoadToolBar(IDR_TOOLBAR1);
	if (!Ret  || !Ret1)
	{
		AfxMessageBox(_T("Load toolbar failed"));
		return FALSE;
	}
	m_ToolBar.GetToolBarCtrl().SetImageList(&m_ImageList);
	m_ToolBar.SetButtons(array, 4);
	m_ToolBar.SetButtonText(0, _T("������"));
	m_ToolBar.SetButtonText(1, _T("���ͨ��"));
	m_ToolBar.SetButtonText(2, _T("��ϵ��"));
	m_ToolBar.SetButtonText(3, _T("��������"));
	m_ToolBar.SetButtonInfo(0, array[0], TBBS_BUTTON, 3);
	m_ToolBar.SetButtonInfo(1, array[1], TBBS_BUTTON, 0);
	m_ToolBar.SetButtonInfo(2, array[2], TBBS_BUTTON, 7);
	m_ToolBar.SetButtonInfo(3, array[3], TBBS_BUTTON, 2);
	CSize  sizeButton, sizeImage;
	sizeImage.cx = 32;
	sizeImage.cy = 32;
	sizeButton.cx = 100;
	sizeButton.cy = 55;
	m_ToolBar.SetSizes(sizeButton, sizeImage);
	m_ToolBar.ShowWindow(SW_SHOW);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);  //���µ�����������С

	//������ʾ�ӽ���
	CRect rcDlg;
	GetDlgItem(IDC_PICTURE)->GetWindowRect(rcDlg);  //����ָ�����δ��ڵı߿��С���ӽ������ʾλ��
	ScreenToClient(rcDlg);//convert the coordinates���� of a RECT structure
	
	/*********�����̽���*********/
	m_Keypad = new CKeypad;
	m_Keypad->Create(IDD_DIALOG4);
	m_Keypad->m_BtnCall.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_Keypad->m_BtnStopCall.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_Keypad->MoveWindow(rcDlg);//Changes the position of the control.
	m_Keypad->m_BtnRevoke.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON4));
	m_Keypad->ShowWindow(SW_SHOW);

	/*********ͨ����¼����*********/
	m_RecentCall = new CRecentCall;
	m_RecentCall->Create(IDD_DIALOG1);
	m_RecentCall->m_List.InsertColumn(0, _T("ͨ������"));
	m_RecentCall->m_List.InsertColumn(1, _T("����绰IP"));
	m_RecentCall->m_List.InsertColumn(2, _T("ʱ��"));
	m_RecentCall->m_List.InsertColumn(3, _T("��ʽ"));
	m_RecentCall->m_List.SetColumnWidth(0, 80);
	m_RecentCall->m_List.SetColumnWidth(1, 115);
	m_RecentCall->m_List.SetColumnWidth(2, 120);
	m_RecentCall->m_List.SetColumnWidth(3, 90);
	m_RecentCall->m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//������չ���
	m_RecentCall->MoveWindow(rcDlg);
	m_RecentCall->ShowWindow(SW_HIDE);

	/*********��ϵ�˽���*********/
	m_LinkMan = new CLinkman;
	m_LinkMan->Create(IDD_DIALOG2);
	m_LinkMan->m_list.InsertColumn(0, _T("���"));
	m_LinkMan->m_list.InsertColumn(1, _T("�������"));
	m_LinkMan->m_list.InsertColumn(2, _T("����绰IP"));
	m_LinkMan->m_list.InsertColumn(3, _T("�������"));
	m_LinkMan->m_list.InsertColumn(4, _T("Ⱥ����"));
	m_LinkMan->m_list.SetColumnWidth(0, 60);
	m_LinkMan->m_list.SetColumnWidth(1, 80);
	m_LinkMan->m_list.SetColumnWidth(2, 120);
	m_LinkMan->m_list.SetColumnWidth(3, 70);
	m_LinkMan->m_list.SetColumnWidth(4, 70);
	m_LinkMan->m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//������չ���
	m_LinkMan->MoveWindow(rcDlg);
	m_LinkMan->ShowWindow(SW_HIDE);

	/*********���ý���*********/
	m_BasicConfig = new CBasicConfig;
	m_BasicConfig->Create(IDD_DIALOG3);
	m_BasicConfig->MoveWindow(rcDlg);
	m_BasicConfig->ShowWindow(SW_HIDE);
	//��ʼ��ͨ����Ϣ
	if (!m_Communication.InitCommunication(24))
	{
		AfxMessageBox(_T("��ʼ��ͨ����Ϣʧ�ܣ�"));
		return FALSE;
	}
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CIPPhoneDlg::OnPaint()
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
HCURSOR CIPPhoneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/***********************************************
�������ܣ���ʾ������
***********************************************/
void CIPPhoneDlg::OnCall()
{
	m_RecentCall->ShowWindow(SW_HIDE);
	m_LinkMan->ShowWindow(SW_HIDE);
	m_BasicConfig->ShowWindow(SW_HIDE);
	m_Keypad->GetFocus();
	m_Keypad->ShowWindow(SW_SHOW);
	
}
/***********************************************
�������ܣ�
***********************************************/
void CIPPhoneDlg::OnRecentCall()
{
	m_Keypad->ShowWindow(SW_HIDE);
	m_LinkMan->ShowWindow(SW_HIDE);
	m_BasicConfig->ShowWindow(SW_HIDE);
	m_RecentCall->GetFocus();
	
	m_RecentCall->ShowWindow(SW_SHOW);
}
/***********************************************
�������ܣ�
***********************************************/
void CIPPhoneDlg::OnLinkMan()
{
	m_Keypad->ShowWindow(SW_HIDE);
	m_RecentCall->ShowWindow(SW_HIDE);
	m_BasicConfig->ShowWindow(SW_HIDE);
	m_LinkMan->GetFocus();
	m_LinkMan->ShowWindow(SW_SHOW);
}
/***********************************************
�������ܣ�
***********************************************/
void CIPPhoneDlg::OnBasicConfig()
{
	CString  strIP, strPhone, strPort, strGroup;
	::GetPrivateProfileString(_T("BasicConfig"), _T("IP"), _T("error"), strIP.GetBuffer(20),  20, _T(".//config.ini"));
	strIP.ReleaseBuffer();
	::GetPrivateProfileString(_T("BasicConfig"), _T("PhoneNumber"), _T("error"), strPhone.GetBuffer(20),  20, _T(".//config.ini"));
	strPhone.ReleaseBuffer();
	::GetPrivateProfileString(_T("BasicConfig"), _T("Port"), _T("error"),  strPort.GetBuffer(20),  20, _T(".//config.ini"));
	strPort.ReleaseBuffer();
	::GetPrivateProfileString(_T("BasicConfig"), _T("GroupNumber"), _T("error"), strGroup.GetBuffer(20),  20, _T(".//config.ini"));
	strGroup.ReleaseBuffer();
	m_Keypad->ShowWindow(SW_HIDE);
	m_LinkMan->ShowWindow(SW_HIDE);
	m_RecentCall->ShowWindow(SW_HIDE);
	m_BasicConfig->GetFocus();
	m_BasicConfig->m_OldIP.SetWindowText(strIP);
	m_BasicConfig->m_OldPhone.SetWindowText(strPhone);
	m_BasicConfig->m_OldPort.SetWindowText(strPort);
	m_BasicConfig->m_OldGroup.SetWindowText(strGroup);
	m_BasicConfig->ShowWindow(SW_SHOW);
}