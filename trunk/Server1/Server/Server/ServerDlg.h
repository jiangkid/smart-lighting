
// ServerDlg.h : 头文件
//
#include "resource.h"		// 主符号
#include "DBConnection.h"
#pragma once


// CServerDlg 对话框
class CServerDlg : public CDialog
{
// 构造
public:
	
	CDBConnection *m_conn;
	CServerDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedEnd();
};

extern HWND H_ServerDlg;