// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D1CEE0C1_A396_4914_9BE1_788974F4BCFC__INCLUDED_)
#define AFX_MAINFRM_H__D1CEE0C1_A396_4914_9BE1_788974F4BCFC__INCLUDED_

#include "mscomm.h"
#include "excel.h"

#include <windows.h>
#include <dbt.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CChatCAsyncSocketDlg;
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ReportToExcel(bool bType);
	CSplitterWnd m_wndSplitter;
	virtual ~CMainFrame();
	_ConnectionPtr	m_pConnection;
	BOOL           m_bAllPart;      //全部或部分处理
	BOOL           m_bNumDate;      //按照序号或是日期处理
	LPTSTR         m_lpszNum1;      //序号1
	LPTSTR         m_lpszNum2;      //序号2
	LPTSTR         m_lpszDate1;		//日期1
	LPTSTR         m_lpszDate2;		//日期2
	BOOL  m_bChooseDB;
	Range         m_ExlRge; 
    _Worksheet    m_ExlSheet; 
    Sheets        m_ExlSheets; 
	_Application  m_ExlApp; 
	_Workbook     m_ExlBook; 
    Workbooks     m_ExlBooks; 
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CTabCtrlBar     m_wndOutput;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnConnect();
	afx_msg void OnTest();
	afx_msg void OnLogin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D1CEE0C1_A396_4914_9BE1_788974F4BCFC__INCLUDED_)
