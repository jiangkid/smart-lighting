
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "afxext.h"
#include "ClassView.h"
#include "FileView.h"
#include "MySplitter.h"
#include "TrueColorToolBar.h"
#include "UserCtrDlg.h"
#include "DecisionDlg.h"
#include "NewDayDecisionDlg.h"
#include "NewJDecisionDlg.h"
#include "NewSpcialDecisionDlg.h"
#include "NewWeekDecisionDlg.h"
#include "CVSetDlg.h"

#define MYWM_NOTIFYICON WM_USER+100
class CCoolBar;
class CMainFrame  : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CSize             NoTextButtonSize;
	CMFCToolBar       m_wndToolBar2;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
    CImageList        m_ImagneList;
   
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnUpdatePane(CCmdUI *pCmdUI);
 
 
	//afx_msg BOOL   OnToolTipText(UINT,   NMHDR*   pNMHDR,   LRESULT*   pResult);
 
	DECLARE_MESSAGE_MAP()

public:
	CMySplitter m_wndSplitter;
	//char nStatus[4];
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	bool m_bSplitted;
	
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	/*afx_msg void OnUpdateToolTexttips(CCmdUI* pCmdUI);*/
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	 
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
 
protected:

	 
	afx_msg void OnShowTool();
	afx_msg void OnHideTool();
	afx_msg void OnShowTree();
	afx_msg void OnHideTree();

public:
	afx_msg void OnCoommand();
	afx_msg void OnNewday();
	afx_msg void OnNewj();
	afx_msg void OnNewweek();
	afx_msg void OnNewspcial();
	afx_msg void OnUpdateModify(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUsercontrol(CCmdUI *pCmdUI);
	afx_msg void OnCvSet();
};


