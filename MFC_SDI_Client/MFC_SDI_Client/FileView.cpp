
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView
CFileView::CFileView()
: m_szTreeName(_T(""))
, m_szTreeID(_T(""))
{
/*	ZeroMemory(itemData,sizeof(TreeInfo));*/
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_R_WORK, &CFileView::OnRWork)
	ON_COMMAND(ID_OPEN_R, &CFileView::OnOpenR)
	ON_COMMAND(ID_OPEN_G, &CFileView::OnOpenG)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ��Ϣ�������

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	// ������ͼ:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("δ�ܴ����ļ���ͼ\n");
		return -1;      // δ�ܴ���
	}
    theApp.m_pFileView=this;
	// ������ͼͼ��:
// 	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
// 	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
	m_FileViewImages.Create (16,16,ILC_COLOR|ILC_MASK,5,5);//����ͼ���б�
	m_FileViewImages.Add (AfxGetApp()->LoadIcon (IDI_ICON4));
	m_FileViewImages.Add (AfxGetApp()->LoadIcon (IDI_ICON5));
	m_FileViewImages.Add (AfxGetApp()->LoadIcon (IDI_ICON6));
	m_FileViewImages.Add (AfxGetApp()->LoadIcon (IDI_ICON7));
	m_FileViewImages.Add (AfxGetApp()->LoadIcon (IDI_ICON8));
	m_wndFileView.SetImageList(&m_FileViewImages,TVSIL_NORMAL);
	m_wndFileView.SetBkColor (RGB(210,230,190));
	m_wndFileView.SetImageList (&m_FileViewImages,TVSIL_NORMAL);

	//m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	//m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

	//OnChangeVisualStyle();

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	//m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	int i=0;
	TV_INSERTSTRUCT tvRoot;//����
	TV_INSERTSTRUCT tvSecond;//��֦
	TV_INSERTSTRUCT tvThree;//��Ҷ
	TV_INSERTSTRUCT tvFour;
	tvRoot.hParent=NULL;
	tvRoot.item.pszText="·�Ƽ��ϵͳ";
	tvRoot.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	tvRoot.item.iImage=0;
	tvRoot.item.iSelectedImage=1;
	HTREEITEM item_root=m_wndFileView.InsertItem (&tvRoot);
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
		HTREEITEM item_second=m_wndFileView.InsertItem (&tvSecond);
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
				tvThree.item.iImage=2;
				tvThree.item.iSelectedImage=3;
				tvThree.item.lParam=(LPARAM)TData;
				HTREEITEM item_third=m_wndFileView.InsertItem (&tvThree);
				int k(0);
				for (k=0;k<m_InitInfo.RNum;k++)
				{
					TreeInfo *RData = (TreeInfo*)malloc(sizeof(TreeInfo));
					ZeroMemory(RData,sizeof(TreeInfo));
					memcpy(RData->RID,m_InitInfo.m_InitRInfo[k].RID,6);
					if (  m_InitInfo.m_InitRInfo[k].TID[0]==m_InitInfo.m_InitTInfo[m].TID[0]
						&&m_InitInfo.m_InitRInfo[k].TID[1]==m_InitInfo.m_InitTInfo[m].TID[1]
						&&m_InitInfo.m_InitRInfo[k].TID[2]==m_InitInfo.m_InitTInfo[m].TID[2]
						&&m_InitInfo.m_InitRInfo[k].TID[3]==m_InitInfo.m_InitTInfo[m].TID[3])
					{
						CString str2;
						str2+=m_InitInfo.m_InitRInfo[k].RName;
						tvFour.hParent=item_third;
						tvFour.item.pszText=(LPTSTR)(LPCTSTR)str2;
						tvFour.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM;
						tvFour.item.iImage=3;
						tvFour.item.iSelectedImage=4;
						tvFour.item.lParam=(LPARAM)RData;
						m_wndFileView.InsertItem (&tvFour);
					}
					//free(RData);
				}
			}
			//free(TData);
		}
		//free(GData);
	}
	m_wndFileView.Expand(item_root,TVE_EXPAND);
	DWORD dwStyle=GetWindowLong(m_wndFileView.m_hWnd ,GWL_STYLE);
	dwStyle|=TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT;
	::SetWindowLong (m_wndFileView.m_hWnd ,GWL_STYLE,dwStyle);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);
	TreeInfo* itemData = new TreeInfo;
	ZeroMemory(itemData,sizeof(TreeInfo));
	int nDate=0;
	m_szTreeName=(_T(""));
	m_szTreeID=(_T(""));
	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// ѡ���ѵ�������:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);
		UINT flags = 0;
		hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
			//itemData = (TreeInfo *)pWndTree->GetItemData(hTreeItem);
			if (pWndTree->GetItemData(hTreeItem)!=NULL)
			{
				itemData = (TreeInfo *)pWndTree->GetItemData(hTreeItem);
			}
			else
			{
				AfxMessageBox("data null");
				return;
			}
			m_szTreeName = pWndTree->GetItemText(hTreeItem);
		}
		else
			return;
	}
	if (itemData->GID[0] == 0x00 && itemData->GID[1]==0x00 &&
		itemData->TID[0] == 0x00 && itemData->TID[1]==0x00 &&
		itemData->TID[2] == 0x00 && itemData->TID[3]==0x00 &&
		itemData->RID[0] == 0x00 && itemData->RID[1]==0x00 &&
		itemData->RID[2] == 0x00 && itemData->RID[3]==0x00 &&
		itemData->RID[4] == 0x00 && itemData->RID[5]==0x00)
		{return;}	
	else 
	{
		if (itemData->GID[0] != 0x00 && itemData->GID[1] != 0x00)
		{
			m_szTreeID+=itemData->GID[0];
			m_szTreeID+=itemData->GID[1];
			GID[0]=itemData->GID[0];
			GID[1]=itemData->GID[1];
			pWndTree->SetFocus();
			CMenu menu;   
			VERIFY(menu.LoadMenu(IDR_POPUP_GPRS));   
			CMenu* pPopup=menu.GetSubMenu(0);   
			ASSERT(pPopup!=NULL);   
			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,this); 
		}
		if (itemData->TID[0] != 0x00 && itemData->TID[1]!=0x00 &&
			itemData->TID[2] != 0x00 && itemData->TID[3]!=0x00)
		{
			for (int i(0);i<4;i++)
			{
				m_szTreeID+=itemData->TID[i];
				TID[i]=itemData->TID[i];
			}
			pWndTree->SetFocus();
			CMenu menu;   
			VERIFY(menu.LoadMenu(IDR_POPUP_T));   
			CMenu* pPopup=menu.GetSubMenu(0);   
			ASSERT(pPopup!=NULL);   
			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,this); 
		}
		if (itemData->RID[0] != 0x00 && itemData->RID[1]!=0x00 &&
			itemData->RID[2] != 0x00 && itemData->RID[3]!=0x00 &&
			itemData->RID[4] != 0x00 && itemData->RID[5]!=0x00)
		{
			for (int i(0);i<6;i++)
			{
				m_szTreeID+=itemData->RID[i];
				RID[i]=itemData->RID[i];
			}
			pWndTree->SetFocus();
			CMenu menu;   
			VERIFY(menu.LoadMenu(IDR_POPUP_R));   
			CMenu* pPopup=menu.GetSubMenu(0);   
			ASSERT(pPopup!=NULL);   
			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,this); 
		}
	}
// 	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);
	//int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	//m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	//m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

// void CFileView::OnProperties()
// {
// 	AfxMessageBox(_T("����...."));
// }

void CFileView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);
	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndFileView.SetFocus();
}
void CFileView::OnRWork()
{
	// TODO: Add your command handler code here
	CDoRWorkDlg dlg;
	dlg.DoModal();
}
void CFileView::OnOpenR()
{
	// TODO: Add your command handler code here
	ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
	ZeroMemory(m_GetLightInfo,8);
	pGetRInfo->m_First[0]=0x2F;
	pGetRInfo->m_First[1]=0x43;
	pGetRInfo->m_First[2]=0x2F;
	pGetRInfo->m_First[3]=0x7F;
	m_GetLightInfo[0]='Z';
	m_GetLightInfo[1]='0';
	for (int i=0;i<6;i++)
	{
		pGetRInfo->m_ID[i]=RID[i];
		m_GetLightInfo[i+2]=RID[i];
	}
	//pGetRInfo->m_OrderObject[];
	pGetRInfo->m_OrderType[0]=0x0A;
	pGetRInfo->m_OrderObject[0]=0xA3;
	pGetRInfo->m_ActiveType[0]=0xBD;
	pGetRInfo->m_EndBuffer[1]=0xCC;
	m_GetLightInfo[8]='#';
	SendContrlInfo(&hdr,pGetRInfo);
 	Sleep(5000);
	send(theApp.m_ConnectSock,(char*)m_GetLightInfo,9,0);
}

void CFileView::OnOpenG()
{
	// TODO: Add your command handler code here
	CDoGWorkDlg dlg;
	dlg.DoModal();
}