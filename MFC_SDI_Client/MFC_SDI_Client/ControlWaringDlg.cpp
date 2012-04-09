// ControlWaringDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "ControlWaringDlg.h"

// CControlWaringDlg dialog

IMPLEMENT_DYNAMIC(CControlWaringDlg, CDialog)

CControlWaringDlg::CControlWaringDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CControlWaringDlg::IDD, pParent)
{
}

CControlWaringDlg::~CControlWaringDlg()
{
}

void CControlWaringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CControlWaringDlg::GetWindowGraph()
{
	CRect	rectWnd;
	CBitmap	*pOldBitmap;
	CDC*	pDC			= GetDC();
	CDC*	pMemDC		= new CDC;

	//获取窗口位图
	GetWindowRect(rectWnd);
	m_bitmapPrint.DeleteObject();
	m_bitmapPrint.CreateCompatibleBitmap(pDC, rectWnd.Width(), rectWnd.Height());
	pMemDC->CreateCompatibleDC(pDC);
	pOldBitmap	= pMemDC->SelectObject(&m_bitmapPrint);
	PrintWindow(pMemDC, 0);

	pMemDC->SelectObject(pOldBitmap);

	//释放内存
	ReleaseDC(pMemDC);
	delete	pMemDC;
}

BEGIN_MESSAGE_MAP(CControlWaringDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_PRINTVIEW, &CControlWaringDlg::OnBnClickedBtnPrintview)
	ON_MESSAGE(WM_MAINFRAME_PRINT, OnPrintWindow)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CControlWaringDlg message handlers

void CControlWaringDlg::OnBnClickedBtnPrintview()
{
	// TODO: Add your control notification handler code here
	m_pPrintFrame	= new CPrintFrame(this);

	GetWindowGraph();

	m_pPrintFrame->DoPrintView();
}

LRESULT CControlWaringDlg::OnPrintWindow(WPARAM wParam, LPARAM lParam)
{
	CRect		rectWnd;
	BITMAP		bmGraphy;
	CBitmap		bitmapTemp, *pOldBmpGraphy;
	CDC*		pGraphyDC	= new CDC;
	CDC*		pDC			= GetDC();
	CDC*		pPrintDC	= (CDC*)wParam;
	CPrintInfo*	pInfo		= (CPrintInfo*)lParam;

	m_bitmapPrint.GetBitmap(&bmGraphy);
	pGraphyDC->CreateCompatibleDC(pDC);
	pOldBmpGraphy	= pGraphyDC->SelectObject(&m_bitmapPrint);

	pPrintDC->StretchBlt(pInfo->m_rectDraw.left, pInfo->m_rectDraw.top, pInfo->m_rectDraw.Width(), pInfo->m_rectDraw.Height(), pGraphyDC, 0, 0, bmGraphy.bmWidth, bmGraphy.bmHeight, SRCCOPY);

	pGraphyDC->SelectObject(pOldBmpGraphy);
	//释放内存
	ReleaseDC(pGraphyDC);
	delete	pGraphyDC;

	return 1;
}
void CControlWaringDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}
