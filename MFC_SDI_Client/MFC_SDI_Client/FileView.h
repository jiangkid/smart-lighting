#pragma once
#include "ViewTree.h"
#include "DoRWorkDlg.h"
#include "MFC_SDI_Client.h"
#include "DoGWorkDlg.h"
#include "WaitForInfoDlg.h"


class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// 构造
public:
	CFileView();

	void AdjustLayout();
	void FillFileView();
	U8* GPRSTranslationID(U8* buffer, int Length);
// 属性
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;
	LPTreeInfo itemData;
	
protected:
	
	HTREEITEM hTreeItem;
// 实现
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//afx_msg void OnProperties();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
	CString m_szTreeName;
	CString m_szTreeID;
	CDoGWorkDlg* pdlg;
	CDoRWorkDlg* Rdlg;
	afx_msg void OnRWork();
	afx_msg void OnOpenR();
	U8	GID[2];
	U8  TID[4];
	U8  RID[6];
	U8  m_GetLightInfo[9]; 
	afx_msg void OnOpenG();
	CWaitForInfoDlg* dlg;
	afx_msg void OnR32848();
	afx_msg void OnR32849();
	afx_msg void OnR32850();
	afx_msg void OnR32851();
	afx_msg void OnR32852();
	afx_msg void OnR32853();
};
