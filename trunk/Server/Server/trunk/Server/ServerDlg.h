// ServerDlg.h : ͷ�ļ�
//
#include "resource.h"
#pragma once

#define  WM_DATASHOW  WM_USER+5
// CServerDlg �Ի���
class CServerDlg : public CDialog
{
// ����
public:
	CServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVER_DIALOG };
	//void Edit(LPCTSTR lParam);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedEnd();
	//afx_msg LRESULT OnDatashow(WPARAM wParam, LPARAM lParam);
};

extern CServerDlg   ServerDlg ;
extern HWND H_ServerDlg;