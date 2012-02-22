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
	CListCtrl& m_list = GetListCtrl();//�õ����õ�listctrl����
	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//��ȡ��ǰ���ڷ��
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //���ñ�����
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle); //���ô��ڷ��

	DWORD dwStyle = m_list.GetExtendedStyle();
	//ѡ��ĳ��ʹ���и�����ֻ�����ڱ������listctrl��

	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ�����뱨�����listctrl��
	m_list.SetExtendedStyle(dwStyle); //������չ���
	m_list.SetBkColor(RGB(200, 200, 200)); //���ñ�����ɫ
	m_list.SetTextBkColor(RGB(200, 200, 200)); //�����ı�������ɫ
	m_list.SetTextColor(RGB(10, 10, 80)); //�����ı���ɫ


	//�����еı��⣬Ϊ�˼��������ֻ��������
	m_list.InsertColumn( 0, "·��", LVCFMT_CENTER, 80 );
	m_list.InsertColumn( 1, "��", LVCFMT_CENTER, 110 );
	m_list.InsertColumn( 2, "��ID", LVCFMT_CENTER, 110 );
	m_list.InsertColumn( 3, "����״̬", LVCFMT_CENTER, 110 );
	m_list.InsertColumn( 4, "����״̬", LVCFMT_CENTER, 110 );
	m_list.InsertColumn( 5, "���±�־λ", LVCFMT_CENTER, 110 );
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
	CListCtrl &m_list = GetListCtrl(); //��ȡ��ǰ�б�ؼ���ָ��
	CMenu menu, *pSubMenu; //��������Ҫ�õ���cmenu����
	menu.LoadMenu(IDR_POPMENU); //װ���Զ�����Ҽ��˵�
	pSubMenu = menu.GetSubMenu(0); //��ȡ��һ�������˵� 
	CPoint oPoint; //����һ������ȷ�����λ�õ�λ��
	GetCursorPos(&oPoint); //��ȡ��ǰ����λ�� 
	//��ָ��λ����ʾ�����˵�
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this); 
	*pResult = 0;
}

void CLightListView::On32799()
{
	// TODO: Add your command handler code here
	CListCtrl &m_list = GetListCtrl(); //��ȡ��ǰ�б�ؼ���ָ��
	m_list.DeleteAllItems();
}

void CLightListView::LightToView(int nLCount)
{
	CListCtrl &m_list = GetListCtrl(); //��ȡ��ǰ�б�ؼ���ָ��
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
			m_list.SetItemText(nRow, 3, _T("��"));
		}
		else
			m_list.SetItemText(nRow, 3, _T("��"));
		if (theApp.m_ZigbeeInfo[i]->AssistStatus)
		{
			m_list.SetItemText(nRow, 4, _T("��"));
		}
		else
			m_list.SetItemText(nRow, 4, _T("��"));
		Sleep(50);
	}
}
