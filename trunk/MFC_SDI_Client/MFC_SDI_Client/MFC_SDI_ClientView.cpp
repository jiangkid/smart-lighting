
// MFC_SDI_ClientView.cpp : CMFC_SDI_ClientView ���ʵ��
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "MFC_SDI_ClientDoc.h"
#include "MFC_SDI_ClientView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_SDI_ClientView

IMPLEMENT_DYNCREATE(CMFC_SDI_ClientView, CView)

BEGIN_MESSAGE_MAP(CMFC_SDI_ClientView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()

// CMFC_SDI_ClientView ����/����

CMFC_SDI_ClientView::CMFC_SDI_ClientView()
{
	// TODO: �ڴ˴���ӹ������

}

CMFC_SDI_ClientView::~CMFC_SDI_ClientView()
{
}

BOOL CMFC_SDI_ClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFC_SDI_ClientView ����

void CMFC_SDI_ClientView::OnDraw(CDC* /*pDC*/)
{
	CMFC_SDI_ClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CMFC_SDI_ClientView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC_SDI_ClientView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMFC_SDI_ClientView ���

#ifdef _DEBUG
void CMFC_SDI_ClientView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_SDI_ClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_SDI_ClientDoc* CMFC_SDI_ClientView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_SDI_ClientDoc)));
	return (CMFC_SDI_ClientDoc*)m_pDocument;
}
#endif //_DEBUG