// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__E8DEFCC0_B42D_4A75_AF19_75F448A4CB92__INCLUDED_)
#define AFX_MAINFRM_H__E8DEFCC0_B42D_4A75_AF19_75F448A4CB92__INCLUDED_


#define WM_ADDLIST WM_USER+3
#define WM_DELETELIST WM_USER+4
#include "ClientSocket.h"
#include "OnViewStatus.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
struct addmitinfo
{
	char flag[2];//管理员标志位
	CString name;//100个字节
	CString password;//4个字节
};
struct userinfo
{
	char flag[2];//用户标志位
	CString name;//100个字节
	CString password;//4个字节
};
struct roadinfo
{
	char roadname[10];//文件信息
	float raodvoltage;//100个字节
	float roadcurrent;//4个字节
};
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	CClientSocket* m_chatsocket;
	bool chattrue;
	CSplitterWnd m_wndSplitter;
	COnViewStatus m_statuslist;
	CEdit m_port;
	CIPAddressCtrl m_ip1;
	virtual ~CMainFrame();
	CString m_csIP;
	CString m_Sport;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//void OnAddMember(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAddMember(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLoginAddmin();
	afx_msg void OnLoginUser();
	afx_msg void OnConnect();
	afx_msg void OnUpdateConnect(CCmdUI* pCmdUI);
	afx_msg void OnQuit();
	afx_msg void OnUpdateQuit(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__E8DEFCC0_B42D_4A75_AF19_75F448A4CB92__INCLUDED_)
