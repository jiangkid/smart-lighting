#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CNewWeekDecisionDlg dialog

class CNewWeekDecisionDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewWeekDecisionDlg)

public:
	CNewWeekDecisionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewWeekDecisionDlg();

// Dialog Data
	enum { IDD = IDD_New_WeekD };
public:
	Decision* pGetInfo1;
	Decision* pGetInfo2;
	Decision* pGetInfo3;
	Decision* pGetInfo4;
	Decision* pGetInfo5;
	Decision* pGetInfo6;
	Decision* pGetInfo7;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Type;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnCancle();
	CDateTimeCtrl m_week1BA;
	CDateTimeCtrl m_week1EA;
// 	CDateTimeCtrl m_week1BH;
// 	CDateTimeCtrl m_week1EH;
	CDateTimeCtrl m_week2BA;
	CDateTimeCtrl m_week2EA;
// 	CDateTimeCtrl m_week2BH;
// 	CDateTimeCtrl m_week2EH;
	CDateTimeCtrl m_week3BA;
	CDateTimeCtrl m_week3EA;
// 	CDateTimeCtrl m_week3BH;
// 	CDateTimeCtrl m_week3EH;
	CDateTimeCtrl m_week4BA;
	CDateTimeCtrl m_week4EA;
// 	CDateTimeCtrl m_week4BH;
// 	CDateTimeCtrl m_week4EH;
	CDateTimeCtrl m_week5BA;
	CDateTimeCtrl m_week5EA;
// 	CDateTimeCtrl m_week5BH;
// 	CDateTimeCtrl m_week5EH;
	CDateTimeCtrl m_week6BA;
	CDateTimeCtrl m_week6EA;
// 	CDateTimeCtrl m_week6BH;
// 	CDateTimeCtrl m_week6EH;
	CDateTimeCtrl m_week7BA;
	CDateTimeCtrl m_week7EA;
// 	CDateTimeCtrl m_week7BH;
// 	CDateTimeCtrl m_week7EH;

	afx_msg void OnBnClickedBtnSure();
	CString m_weekName;
	void GetInfo1(void);
	void GetInfo2(void);
	void GetInfo3(void);
	void GetInfo4(void);
	void GetInfo5(void);
	void GetInfo6(void);
	void GetInfo7(void);
	//CEdit m_strName;
};
