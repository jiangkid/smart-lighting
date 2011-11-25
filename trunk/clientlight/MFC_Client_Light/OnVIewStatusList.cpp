// OnVIewStatusList.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_Client_Light.h"
#include "OnVIewStatusList.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COnVIewStatusList

COnVIewStatusList::COnVIewStatusList()
{
}

COnVIewStatusList::~COnVIewStatusList()
{
}


BEGIN_MESSAGE_MAP(COnVIewStatusList, CListCtrl)
	//{{AFX_MSG_MAP(COnVIewStatusList)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COnVIewStatusList message handlers
int COnVIewStatusList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void COnVIewStatusList::AddItem(roadinfo* recvinfo)
{

	LVITEM	lvi;
	
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;
	lvi.iItem = GetItemCount();
	//  lvi.iImage = i;
	lvi.iSubItem = 0;
	//	lvi.s=0;
	lvi.pszText = " ";
	lvi.cchTextMax = 64;
	InsertItem(&lvi);
	SetItemText(lvi.iItem,0,recvinfo->roadname);
	//CString aaa;
	//aaa.Format("%d k",recvinfo->raodvoltage/1000);
	//SetItemText(lvi.iItem,1,aaa.GetBuffer(0));
	//aaa.ReleaseBuffer();
		//SetItemText(lvi.iItem,2,recvinfo->roadname);
		//SetItemText(lvi.iItem,3,recvinfo->roadname);
		//SetItemText(lvi.iItem,4,recvinfo->roadname);
	SetHotItem(lvi.iItem);			//标注当前项目
	EnsureVisible(lvi.iItem,true);	//滚动影响的卷轴
	Update(lvi.iItem);
	//strcpy(name1[lvi.iItem],name2);
	//strcpy(name1[lvi.iItem+1],"none");
	//strcpy(nameph[lvi.iItem],namepath);
	//AfxMessageBox(name1[lvi.iItem]);
}


void COnVIewStatusList::Remove(char *roadname)
{
	CString str;
	for (int i=0;i<GetItemCount();i++)
	{
		str=GetItemText(i,0);
		if (strcmp(str,roadname)==0)
		{
			DeleteItem(i);
			break;
		}
	}

}



void COnVIewStatusList::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void COnVIewStatusList::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void COnVIewStatusList::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
