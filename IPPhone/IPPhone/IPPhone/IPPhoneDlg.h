// IPPhoneDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "Communication.h"

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

private:
	CImageList    m_ImageList;
	
public:
	CButton m_BtnCallGroup;
	CButton m_BtnCallMany;
	CButton m_BtnRecentCall;
	CButton m_BtnContacts;
	CButton m_BtnCall;
	CButton m_BtnStopCall;
	CButton m_BtnRevoke;

	CCommunication  m_Communication;
	afx_msg void OnBnClickedButton1();
	CEdit m_Edit;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton0();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedBtnCall();
	afx_msg void OnBnClickedButRevoke();
	afx_msg void OnBnClickedButAnswer();
};
