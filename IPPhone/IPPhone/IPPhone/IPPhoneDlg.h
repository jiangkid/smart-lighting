// IPPhoneDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "Communication.h"
#include "BasicConfig.h"
#include "Linkman.h"
#include "RecentCall.h"
#include "Keypad.h"
// CIPPhoneDlg 对话框
class CIPPhoneDlg : public CDialog
{
// 构造
public:
	CIPPhoneDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IPPHONE_DIALOG };

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
	CImageList		m_ImageList;
	CToolBar			m_ToolBar;
	CRecentCall		*m_RecentCall;
	CLinkman			*m_LinkMan;
	CBasicConfig	*m_BasicConfig;
	CKeypad			*m_Keypad;

	CButton	m_BtnCallGroup;
	CButton	m_BtnCallMany;
	CButton	m_BtnRecentCall;
	CButton	m_BtnContacts;


	CCommunication  m_Communication;

	afx_msg void OnCall();
	afx_msg void OnRecentCall();
	afx_msg void OnLinkMan();
	afx_msg void OnBasicConfig();
	afx_msg void OnClose();
};
