
// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "IOCPServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND show;
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
	ON_BN_CLICKED(IDC_BTN_START, &CServerDlg::OnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CServerDlg::OnStop)

	ON_MESSAGE(WM_USER_CLIENT,&CServerDlg::OnClientMsg)
	ON_BN_CLICKED(IDC_BTN_SEND, &CServerDlg::OnBnClickedBtnSend)
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

	
	show = this->m_hWnd;
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


void CServerDlg::OnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!g_pIOCPServer->SrartServer())
	{
		MessageBox(_T("����������ʧ��,���Ժ�����!"));
		return;
	}

	UpdateData();
	//ע�������ȡ�¼�
//	if(SOCKET_ERROR == WSAAsyncSelect(g_pIOCPServer->m_sdListen,this->m_hWnd,WM_USER_CLIENT,FD_READ))
//	{
//		MessageBox(_T("ע�������ȡ�¼�ʧ��!"));
//		return ;
//	}
	MessageBox(_T("������������!"));
}

void CServerDlg::OnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!g_pIOCPServer->StopServer())
	{
		MessageBox(_T("ֹͣ������ʧ��,���Ժ�����!"));
	}
	
	MessageBox(_T("��������ֹͣ!"));
	
}

LRESULT CServerDlg::OnClientMsg(WPARAM wParam,LPARAM lParam)
{
	IN_ADDR client;
	client.S_un.S_addr = wParam;
	char *ClientMsg[ ]={"��������","���ӱ�����","���ӱ�����","���ӱ��ܾ�","���ӱ��ܾ�","���ӹر�","��������,���ӽ���"};
	CString info;
	info.Format(_T("From IP��ַ%s,%s",inet_ntoa(client),ClientMsg[lParam]));

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//��ô����͵�����
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
		MessageBox(_T("��������ʧ��!"));
		SetDlgItemText(IDC_EDIT_SEND," ");
		return;
	}
	SetDlgItemText(IDC_EDIT_SEND," ");
}
