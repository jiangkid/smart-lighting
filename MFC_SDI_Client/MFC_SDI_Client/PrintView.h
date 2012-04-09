#pragma once
#include "PreviewPrint.h"
#define		WM_MAINFRAME_PRINT		WM_USER+1001	//打印消息

// CPrintView 视图

class CPrintView : public CScrollView
{
	DECLARE_DYNCREATE(CPrintView)

public:
	CPrintView();           // 动态创建所使用的受保护的构造函数
	virtual ~CPrintView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	afx_msg void	OnFilePrintPreview();
	afx_msg void    OnFilePrint();
	static BOOL CALLBACK	ProcClosePrintPreview(CFrameWnd* pFrameWnd);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	DECLARE_MESSAGE_MAP()
};