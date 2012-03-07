
// MFC_SDI_ClientView.h : CMFC_SDI_ClientView ��Ľӿ�
//


#pragma once
#include "afxext.h"

class CMFC_SDI_ClientDoc;
class CMFC_SDI_ClientView : public CView
{
protected: // �������л�����
	CMFC_SDI_ClientView();
	DECLARE_DYNCREATE(CMFC_SDI_ClientView)
// ����
public:
	CMFC_SDI_ClientDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CMFC_SDI_ClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
};

#ifndef _DEBUG  // MFC_SDI_ClientView.cpp �еĵ��԰汾
inline CMFC_SDI_ClientDoc* CMFC_SDI_ClientView::GetDocument() const
   { return reinterpret_cast<CMFC_SDI_ClientDoc*>(m_pDocument); }
#endif

