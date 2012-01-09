
// ServerDlg.h : ͷ�ļ�
//

#pragma once
#include "resource.h"		// ������ 

#include "GPRSServer.h"
#include "ClientServer.h"
#define WM_Command  WM_USER+10

// CServerDlg �Ի���
class CServerDlg : public CDialog
{
// ����
public:
	CServerDlg(CWnd* pParent = NULL);	// ��׼���캯��
// �Ի�������
	enum { IDD = IDD_SERVER_DIALOG };
	CClientServer   m_pClientServer;
	CGPRSServer    m_pGPRSServer;
	//CClientServer  m_pClientServer;


	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedEnd();
	afx_msg void OnBnClickedStop();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

extern HWND H_ServerDlg;