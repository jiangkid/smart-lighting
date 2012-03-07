
// MFC_SDI_ClientView.h : CMFC_SDI_ClientView 类的接口
//


#pragma once
#include "afxext.h"

class CMFC_SDI_ClientDoc;
class CMFC_SDI_ClientView : public CView
{
protected: // 仅从序列化创建
	CMFC_SDI_ClientView();
	DECLARE_DYNCREATE(CMFC_SDI_ClientView)
// 属性
public:
	CMFC_SDI_ClientDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CMFC_SDI_ClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
};

#ifndef _DEBUG  // MFC_SDI_ClientView.cpp 中的调试版本
inline CMFC_SDI_ClientDoc* CMFC_SDI_ClientView::GetDocument() const
   { return reinterpret_cast<CMFC_SDI_ClientDoc*>(m_pDocument); }
#endif

