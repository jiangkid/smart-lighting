#if !defined(AFX_LVERRORRECORD_H__05169265_BF9E_478E_801F_71B75E5E6FE6__INCLUDED_)
#define AFX_LVERRORRECORD_H__05169265_BF9E_478E_801F_71B75E5E6FE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LVErrorRecord.h : header file
//
#include "mscomm.h"
/////////////////////////////////////////////////////////////////////////////
// CLVErrorRecord view

class CLVErrorRecord : public CListView
{
protected:
	CLVErrorRecord();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLVErrorRecord)

// Attributes
public:

// Operations
public:
	void OnDelAlarmRecord();
	void OnPrintAlarmRecord();
	void OnRead();
	void OpenPrintCom();
	void PrintFunc(unsigned char ucFunc,unsigned char n);
	// 定义ADO连接、命令、记录集变量指针
	//_ConnectionPtr	m_pConnection;
	//_CommandPtr		m_pCommand;
	_RecordsetPtr	m_pRecordset;
	CListCtrl*      m_ErrorRecord;
	int             m_nRecordNum;
	unsigned int    m_nNum;
	CMSComm         m_PrintCom;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLVErrorRecord)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLVErrorRecord();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CLVErrorRecord)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LVERRORRECORD_H__05169265_BF9E_478E_801F_71B75E5E6FE6__INCLUDED_)
