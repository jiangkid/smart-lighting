#pragma once

#include <afxpriv.h>

// CPreviewPrint 视图

class CPreviewPrint : public CPreviewView
{
	DECLARE_DYNCREATE(CPreviewPrint)

	BOOL	m_bIsCloseFrame;

public:
	CPreviewPrint();           // 动态创建所使用的受保护的构造函数
	virtual ~CPreviewPrint();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	afx_msg void OnPreviewClose();
	afx_msg void OnPreviewPrint();

protected:
	DECLARE_MESSAGE_MAP()
};


