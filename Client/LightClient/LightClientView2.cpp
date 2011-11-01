// LightClientView2.cpp : implementation file
//

#include "stdafx.h"
#include "lightclient.h"
#include "LightClientView2.h"
#include "LightClientDoc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightClientView2

IMPLEMENT_DYNCREATE(CLightClientView2, CTreeView)

CLightClientView2::CLightClientView2()
{
}

CLightClientView2::~CLightClientView2()
{
}


BEGIN_MESSAGE_MAP(CLightClientView2, CTreeView)
	//{{AFX_MSG_MAP(CLightClientView2)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightClientView2 drawing

void CLightClientView2::OnDraw(CDC* pDC)
{
	CDocument* pDoc = (CLightClientDoc*)CLightClientView2::GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CLightClientView2 diagnostics

#ifdef _DEBUG
void CLightClientView2::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLightClientView2::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLightClientView2 message handlers

void CLightClientView2::InitTree()
{
	//������ؼ�
	CTreeCtrl* pTree = &GetTreeCtrl();
	
	//Ϊ��������ߺͰ�ť
	pTree->ModifyStyle(0, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS);
	
	CString strText = _T("");
	
	//�����в�����
	HTREEITEM hRoot = pTree->InsertItem(_T("Root"));
	
	for (int i = 0; i < 4; i++)
	{
		strText.Format(_T("Item %d"), i);
		
		//�����в�����
		HTREEITEM hParent = pTree->InsertItem(strText, hRoot);
		
		for(int j = 0; j < 5; j++)
		{
			strText.Format(_T("SubItem %d %d"), i, j);
			
			//�����в�����
			pTree->InsertItem(strText, hParent);
		}
		
		//չ������
		pTree->Expand(hParent, TVE_EXPAND);	
	}
	
	//չ������
	pTree->Expand(hRoot, TVE_EXPAND);
	
	//ѡ������
	pTree->Select(hRoot, TVGN_CARET);
}

void CLightClientView2::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	InitTree();
	// TODO: Add your specialized code here and/or call the base class
	
}
CLightClientDoc* CLightClientView2::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLightClientDoc)));
	return (CLightClientDoc*)m_pDocument;
}