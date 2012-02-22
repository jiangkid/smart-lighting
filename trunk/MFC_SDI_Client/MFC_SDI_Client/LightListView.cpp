// LightListView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "LightListView.h"


// CLightListView

IMPLEMENT_DYNCREATE(CLightListView, CListView)

CLightListView::CLightListView()
{

}

CLightListView::~CLightListView()
{
}

BEGIN_MESSAGE_MAP(CLightListView, CListView)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CLightListView::OnNMRClick)
	ON_COMMAND(ID_32799, &CLightListView::On32799)
END_MESSAGE_MAP()


// CLightListView diagnostics

#ifdef _DEBUG
void CLightListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CLightListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLightListView message handlers

BOOL CLightListView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	theApp.m_pLightListView=this;
	return CListView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CLightListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CListCtrl& m_list = GetListCtrl();//得到内置的listctrl引用
	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//获取当前窗口风格
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置报表风格
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle); //设置窗口风格

	DWORD dwStyle = m_list.GetExtendedStyle();
	//选中某行使整行高亮（只适用于报表风格的listctrl）

	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与报表风格的listctrl）
	m_list.SetExtendedStyle(dwStyle); //设置扩展风格
	m_list.SetBkColor(RGB(200, 200, 200)); //设置背景颜色
	m_list.SetTextBkColor(RGB(200, 200, 200)); //设置文本背景颜色
	m_list.SetTextColor(RGB(10, 10, 80)); //设置文本颜色


	//插入列的标题，为了简单起见，我只插入三列
	m_list.InsertColumn( 0, "路名", LVCFMT_CENTER, 80 );
	m_list.InsertColumn( 1, "灯", LVCFMT_CENTER, 110 );
	m_list.InsertColumn( 2, "灯ID", LVCFMT_CENTER, 110 );
	m_list.InsertColumn( 3, "主灯状态", LVCFMT_CENTER, 110 );
	m_list.InsertColumn( 4, "辅灯状态", LVCFMT_CENTER, 110 );
	m_list.InsertColumn( 5, "更新标志位", LVCFMT_CENTER, 110 );
	// TODO: Add your specialized code here and/or call the base class
}

void CLightListView::DeleteAll(void)
{
	CListCtrl &m_list = GetListCtrl(); 
	m_list.DeleteAllItems();
}

void CLightListView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CListCtrl &m_list = GetListCtrl(); //获取当前列表控件的指针
	CMenu menu, *pSubMenu; //定义下面要用到的cmenu对象
	menu.LoadMenu(IDR_POPMENU); //装载自定义的右键菜单
	pSubMenu = menu.GetSubMenu(0); //获取第一个弹出菜单 
	CPoint oPoint; //定义一个用于确定光标位置的位置
	GetCursorPos(&oPoint); //获取当前光标的位置 
	//在指定位置显示弹出菜单
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this); 
	*pResult = 0;
}

void CLightListView::On32799()
{
	// TODO: Add your command handler code here
	CListCtrl &m_list = GetListCtrl(); //获取当前列表控件的指针
	m_list.DeleteAllItems();
}

void CLightListView::LightToView(int nLCount)
{
	CListCtrl &m_list = GetListCtrl(); //获取当前列表控件的指针
	m_list.DeleteAllItems();
	for (int i=0;i<nLCount;i++)
	{
		CString strID=_T("");
		CString strName=_T("");
		for (int n=0;n<16;n++)
		{
			strID+=theApp.m_ZigbeeInfo[i]->LID[n];
		}
		for (int m=0;m<50;m++)
		{
			strName+=theApp.m_ZigbeeInfo[i]->LName[m];
		}
		int nRow = m_list.InsertItem(0, theApp.m_pFileView->m_szTreeName);
		m_list.SetItemText(nRow, 1, strName);
		m_list.SetItemText(nRow, 2, strID);
		if (theApp.m_ZigbeeInfo[i]->MainStatus)
		{
			m_list.SetItemText(nRow, 3, _T("开"));
		}
		else
			m_list.SetItemText(nRow, 3, _T("关"));
		if (theApp.m_ZigbeeInfo[i]->AssistStatus)
		{
			m_list.SetItemText(nRow, 4, _T("开"));
		}
		else
			m_list.SetItemText(nRow, 4, _T("关"));
		Sleep(50);
	}
}
