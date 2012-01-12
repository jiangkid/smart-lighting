// GTRLView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "GTRLView.h"


// CGTRLView

IMPLEMENT_DYNCREATE(CGTRLView, CTreeView)

CGTRLView::CGTRLView()
: m_lightView(NULL)
{

}

CGTRLView::~CGTRLView()
{
}

BEGIN_MESSAGE_MAP(CGTRLView, CTreeView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CGTRLView::OnNMDblclk)
//	ON_NOTIFY_REFLECT(TVN_ITEMCHANGED, &CGTRLView::OnTvnItemChanged)
END_MESSAGE_MAP()


// CGTRLView diagnostics

#ifdef _DEBUG
void CGTRLView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CGTRLView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGTRLView message handlers

int CGTRLView::GetTNum(LPGInfo wGtInfo)
{
	int m_GTnum(0);
	for (int i=0;i<m_InitInfo.TNum;i++)
	{
		if (m_InitInfo.m_InitTInfo[i].GID==wGtInfo->GID)
		{
			m_GTnum++;
		}
	}
	return m_GTnum;
}

void CGTRLView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	list.Create (16,16,ILC_COLOR|ILC_MASK,3,3);//创建图标列表
	list.Add (AfxGetApp()->LoadIcon (IDI_CK));
	list.Add (AfxGetApp()->LoadIcon (IDI_BASE_INFO));
	list.Add (AfxGetApp()->LoadIcon (IDI_INPUT));
	this->GetTreeCtrl().SetImageList(&list,TVSIL_NORMAL);
	CTreeCtrl& CtlTree=(CTreeCtrl&)GetTreeCtrl ();
	CtlTree.SetBkColor (RGB(210,230,190));
	CtlTree.SetImageList (&list,TVSIL_NORMAL);
	int i=0;
	TV_INSERTSTRUCT tvRoot;//树根
	TV_INSERTSTRUCT tvSecond;//树枝
	TV_INSERTSTRUCT tvThree;//树叶
	TV_INSERTSTRUCT tvFour;
	tvRoot.hParent=NULL;
	tvRoot.item.pszText="路灯监控系统";
	tvRoot.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	tvRoot.item.iImage=i;
	tvRoot.item.iSelectedImage=i;
	HTREEITEM item_root=CtlTree.InsertItem (&tvRoot);
	for(i=0;i<m_InitInfo.GNum;i++)
	{	
		TreeInfo *GData = (TreeInfo*)malloc(sizeof(TreeInfo));
		ZeroMemory(GData,sizeof(TreeInfo));
		memcpy(GData->GID,m_InitInfo.m_InitGInfo[i].GID,2);
		CString str=_T("");
		str+=m_InitInfo.m_InitGInfo[i].GName;
		tvSecond.hParent=item_root;
		tvSecond.item.pszText=(LPTSTR)(LPCTSTR)str;
		tvSecond.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM;
		tvSecond.item.iImage=1;
		tvSecond.item.iSelectedImage=2;
		tvSecond.item.lParam=(LPARAM)GData;
		HTREEITEM item_second=CtlTree.InsertItem (&tvSecond);
//		CtlTree.SetItemData(item_second, (LPARAM)GData);
/*		int n=GetTNum(&m_InitInfo.m_InitGInfo[i]);*/
		int m(0);
		for (m=0;m<m_InitInfo.TNum;m++)
		{
			TreeInfo *TData = (TreeInfo*)malloc(sizeof(TreeInfo));
			ZeroMemory(TData,sizeof(TreeInfo));
			memcpy(TData->TID,m_InitInfo.m_InitTInfo[m].TID,4);
			if (m_InitInfo.m_InitTInfo[m].GID[0]==m_InitInfo.m_InitGInfo[i].GID[0]
			&&m_InitInfo.m_InitTInfo[m].GID[1]==m_InitInfo.m_InitGInfo[i].GID[1])
			{
				CString str1=_T("");
				str1+=m_InitInfo.m_InitTInfo[m].TName;
				tvThree.hParent=item_second;
				tvThree.item.pszText=(LPTSTR)(LPCTSTR)str1;
				tvThree.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM;
				tvThree.item.iImage=1;
				tvThree.item.iSelectedImage=2;
				tvThree.item.lParam=(LPARAM)TData;
				HTREEITEM item_third=CtlTree.InsertItem (&tvThree);
				int k(0);
				for (k=0;k<m_InitInfo.RNum;k++)
				{
					TreeInfo *RData = (TreeInfo*)malloc(sizeof(TreeInfo));
					ZeroMemory(RData,sizeof(TreeInfo));
					memcpy(RData->RID,m_InitInfo.m_InitRInfo[k].RID,6);
					if (m_InitInfo.m_InitRInfo[k].TID[0]==m_InitInfo.m_InitTInfo[m].TID[0]
					&&m_InitInfo.m_InitRInfo[k].TID[1]==m_InitInfo.m_InitTInfo[m].TID[1]
					&&m_InitInfo.m_InitRInfo[k].TID[2]==m_InitInfo.m_InitTInfo[m].TID[2]
					&&m_InitInfo.m_InitRInfo[k].TID[3]==m_InitInfo.m_InitTInfo[m].TID[3])
					{
						CString str2;
						str2+=m_InitInfo.m_InitRInfo[k].RName;
						tvFour.hParent=item_third;
						tvFour.item.pszText=(LPTSTR)(LPCTSTR)str2;
						tvFour.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM;
						tvFour.item.iImage=1;
						tvFour.item.iSelectedImage=2;
						tvFour.item.lParam=(LPARAM)RData;
						CtlTree.InsertItem (&tvFour);
					}
				}
			}
		}
	//设置列表视图的风格
	}
	CtlTree.Expand(item_root,TVE_EXPAND);
	DWORD dwStyle=GetWindowLong(CtlTree.m_hWnd ,GWL_STYLE);
	dwStyle|=TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT;
	::SetWindowLong (CtlTree.m_hWnd ,GWL_STYLE,dwStyle);
}

void CGTRLView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	NM_TREEVIEW* pNMTreeView=(NM_TREEVIEW*)pNMHDR;
	CTreeCtrl& CtlTree=(CTreeCtrl&)GetTreeCtrl ();
	HTREEITEM select_item=CtlTree.GetSelectedItem();
	CString itemName = CtlTree.GetItemText(select_item);
	TreeInfo *itemData; 
	if (CtlTree.GetItemData(select_item))
	{
		itemData = (TreeInfo *)CtlTree.GetItemData(select_item);
	}
	else
		AfxMessageBox("data null");
	
	if (itemData->GID[0]!=0x00&&itemData->GID[1]!=0x00)
 	{
 		send(theApp.m_ConnectSock,itemData->GID,2,0);
 	}
 	if (itemData->TID[0]!=0x00&&itemData->TID[1]!=0x00&&
		itemData->TID[2]!=0x00&&itemData->TID[3]!=0x00)
 	{
 		send(theApp.m_ConnectSock,itemData->TID,4,0);
 	}
	if (itemData->RID[0]!=0x00&&itemData->RID[1]!=0x00&&
		itemData->RID[2]!=0x00&&itemData->RID[3]!=0x00&&
		itemData->RID[4]!=0x00&&itemData->RID[5]!=0x00)
 	{
		unsigned char buffer1[30]={'N',0x2F,0x43,0x2F,0x01,
			'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
			0x0A,0xA3,0xBD,0x00,0x00,0x00,0x00,0x00,0xCC};
		send(theApp.m_ConnectSock,(char*)buffer1,30,0);
 		//send(theApp.m_ConnectSock,itemData->RID,6,0);
		theApp.m_WaitDlg.DoModal();
		CSplitterWnd* m_wndSplitter=(CSplitterWnd*)GetParent();
		m_lightView=(CLightView*)m_wndSplitter->GetPane (0,1);
		m_lightView->OnLightShow();
 	}
	*pResult = 0;
}
