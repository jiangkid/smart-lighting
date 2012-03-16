
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "afxext.h"
#include "ClassView.h"
#include "FileView.h"
#include "MySplitter.h"
#include "TrueColorToolBar.h"
#include "UserCtrDlg.h"
#define MYWM_NOTIFYICON WM_USER+100
class CCoolBar;
class CMainFrame  : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar       m_wndMenuBar;
	
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

public:
	CMySplitter m_wndSplitter;
	//char nStatus[4];
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	bool m_bSplitted;
	CMFCToolBar       m_wndToolBar;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnClose();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnQuitexe();
	afx_msg void OnOpenexe();
	afx_msg void OnQuit();
	bool TrayMessage(DWORD dwMessage,UINT nIDI_ICON=0);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void StartTimer(UINT_PTR nIDEvent);
	afx_msg void OnUsercontrol();
	afx_msg void OnMainset();
	CMainSetDlg* Mdlg;
	CUserCtrDlg* Userdlg;
	 
 
};


