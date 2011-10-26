// ServerDlg.h : 头文件
//
#include "resource.h"
#pragma once

#define  WM_DATASHOW  WM_USER+5
// CServerDlg 对话框
class CServerDlg : public CDialog
{
// 构造
public:
	CServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };
	//void Edit(LPCTSTR lParam);

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
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedEnd();
	//afx_msg LRESULT OnDatashow(WPARAM wParam, LPARAM lParam);
};

extern CServerDlg   ServerDlg ;
extern HWND H_ServerDlg;