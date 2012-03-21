
// MFC_SDI_Client.cpp : ����Ӧ�ó��������Ϊ��
//


#include "stdafx.h"
#include "afxwinappex.h"
#include "MFC_SDI_Client.h"
#include "MainFrm.h"
#include "LoginDlg.h"
#include "MFC_SDI_ClientDoc.h"
#include "MFC_SDI_ClientView.h"
#include "SetG.h"
#include "Modify.h"
#include "TestDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CMFC_SDI_ClientApp
BEGIN_MESSAGE_MAP(CMFC_SDI_ClientApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMFC_SDI_ClientApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
//	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_SETUSER, &CMFC_SDI_ClientApp::OnSetuser)
	ON_COMMAND(ID_SetG, &CMFC_SDI_ClientApp::OnSetg)
	ON_COMMAND(ID_Modify, &CMFC_SDI_ClientApp::OnModify)
	ON_COMMAND(ID_CAgain, &CMFC_SDI_ClientApp::OnCagain)
	ON_COMMAND(ID_32783, &CMFC_SDI_ClientApp::On32783)
	ON_UPDATE_COMMAND_UI(ID_CAgain, &CMFC_SDI_ClientApp::OnUpdateCagain)
	ON_COMMAND(ID_RENEW, &CMFC_SDI_ClientApp::OnRenew)
END_MESSAGE_MAP()


// CMFC_SDI_ClientApp ����

CMFC_SDI_ClientApp::CMFC_SDI_ClientApp()
: m_return(false)
, m_InitTrue(false)
, m_connected(false)
, m_DlgMainONStatus(false)
, m_DlgMainOFFStatus(false)
, m_DlgAssistONStatus(false)
, m_DlgAssistOFFStatus(false)
, m_DlgDoubleONStatus(false)
, m_DlgDoubleOFFStatus(false)
{
	m_bHiColorIcons = TRUE;
	h1=NULL;
	ZeroMemory(nStatus,4);
	nRCount=0;
	//m_InitInfo=(IintInfo*)malloc(sizeof(IintInfo));
	//ZeroMemory(m_InitInfo,sizeof(IintInfo));
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CMFC_SDI_ClientApp ����

CMFC_SDI_ClientApp theApp;


// CMFC_SDI_ClientApp ��ʼ��

BOOL CMFC_SDI_ClientApp::InitInstance()
{
//  	CSplashWnd* pSplash = new CSplashWnd;
//  	pSplash->ShowWindow(m_nCmdShow);
//// 	Sleep(2500);
	//���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	//ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
//���Ǵ����� ���õ�
 	/* while(!m_InitTrue)
 	{
   		while (!m_return)
   			{
   				CLoginDlg dlg;
   				if (dlg.DoModal()==IDCANCEL)
   				{
   					return false;
   					break;
   				}
   				else
   				{
   					GetPrivateProfileString("Server","serverip",NULL,m_ip,20,"D:\\server.ini");
   					m_port=GetPrivateProfileInt("Server","serverport",0,"D:\\server.ini");
   					strNamePass.Format(_T("%s"),dlg.m_szNamePassword);
   					h1=::CreateThread(NULL, 0, ConnectThreadFunc, this, 0, NULL);
  					Sleep(500);
   				}
   			}
 			char a[3]={'L','G','#'};
 			send(m_ConnectSock,a,3,0);
 			if (m_WaitDlg.DoModal()==IDOK)
 			{
 					m_return =false;
 					closesocket(m_ConnectSock);
 					TerminateThread(h1,0);
 					AfxMessageBox(_T("��ʼ��ʧ�ܣ������´򿪣�"));
 			}
 			else 
 				m_InitTrue=true;
 		} */
/*���Դ���*/
     	/*GetPrivateProfileString("Server","serverip",NULL,m_ip,20,"D:\\server.ini");
     	m_port=GetPrivateProfileInt("Server","serverport",0,"D:\\server.ini");
     	h1=::CreateThread(NULL, 0, ConnectThreadFunc, this, 0, NULL);
  		if (m_WaitDlg.DoModal()==IDCANCEL)
  			m_return=FALSE;*/
	// ��׼��ʼ��
	 //���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMFC_SDI_ClientDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CMFC_SDI_ClientView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
	return FALSE;
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	//skinppLoadSkin(_T("vladstudio.ssk"));
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
}
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
public:
	/*afx_msg void OnUpdateCagain(CCmdUI *pCmdUI);*/
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	/*ON_UPDATE_COMMAND_UI(ID_CAgain, &CAboutDlg::OnUpdateCagain)*/
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CMFC_SDI_ClientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMFC_SDI_ClientApp �Զ������/���淽��

void CMFC_SDI_ClientApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMFC_SDI_ClientApp::LoadCustomState()
{
}

void CMFC_SDI_ClientApp::SaveCustomState()
{
}

// CMFC_SDI_ClientApp ��Ϣ�������

void CMFC_SDI_ClientApp::OnSetuser()
{

	CSetNewUser dlg;
	dlg.DoModal();
}

void CMFC_SDI_ClientApp::OnSetg()
{
	CSetG dlg;
	dlg.DoModal();
}

void CMFC_SDI_ClientApp::OnModify()
{
	// TODO: Add your command handler code here
	CModify dlg;
	dlg.DoModal();
}

void CMFC_SDI_ClientApp::OnCagain()
{
	// TODO: Add your command handler code here
	h1=::CreateThread(NULL, 0, ConnectThreadFunc, this, 0, NULL);
	Sleep(500);
	if (m_return)
	{
		AfxMessageBox(_T("�����ɹ���"));
		m_connected=false;
	}
	else
	{
		AfxMessageBox(_T("����ʧ�ܣ�"));
		m_connected=true;
	}
}

void CMFC_SDI_ClientApp::On32783()
{
	// TODO: Add your command handler code here
	CTestDlg dlg;
	dlg.DoModal();
}

int CMFC_SDI_ClientApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	//skinppExitSkin();
	//closesocket(m_ConnectSock);
	
	if (nRCount!=0)
	{
		for (int i(0);i<nRCount;i++)
		{
			free(m_RoadListInfo[i]);
		}
	}
	else
		return CWinAppEx::ExitInstance();

	
}

void CMFC_SDI_ClientApp::OnUpdateCagain(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_connected)
	{
		pCmdUI->Enable(TRUE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CMFC_SDI_ClientApp::OnRenew()
{
	// TODO: Add your command handler code here
	char a[3]={'L','G','#'};
	send(m_ConnectSock,a,3,0);
	m_WaitDlg.DoModal();
}

 
