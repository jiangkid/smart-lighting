// LightView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "LightView.h"


// CLightView

IMPLEMENT_DYNCREATE(CLightView, CView)

CLightView::CLightView()
{

}

CLightView::~CLightView()
{
}

BEGIN_MESSAGE_MAP(CLightView, CView)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(1, &CLightView::OnMyButton1)
	ON_BN_CLICKED(2, &CLightView::OnMyButton2)
	ON_BN_CLICKED(3, &CLightView::OnMyButton3)
END_MESSAGE_MAP()


// CLightView drawing

void CLightView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CLightView diagnostics

#ifdef _DEBUG
void CLightView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CLightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLightView message handlers

int CLightView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
// 	mybutton1.Create(_T("我的按钮1"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
// 		CRect(20,20,52,52), this, 1);
// 	//mybutton1.SubclassDlgItem(1,this);
// 	mybutton1.SetIcon(IDI_LIGHT_ON,IDI_LIGHT_OFF);
// 	mybutton1.DrawBorder(false);
// 	mybutton1.SetTooltipText(_T("主灯"));
// 	mybutton2.Create(_T("我的按钮2"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
// 		CRect(20+32,20,52+32,52), this, 2);
// 	mybutton2.SetIcon(IDI_LIGHT_OFF);
// 	mybutton2.DrawBorder(false);
// 	//mybutton2.SetFlat(false);
// 	mybutton2.SetTooltipText(_T("辅灯"));
// 	mybutton3.Create(_T("我的按钮3"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
// 		CRect(20,20+32,84,72),this,3);
// 	mybutton3.ShowWindow(SW_SHOW);
// 	mybutton3.SetTooltipText(_T("双灯"));
	

	return 0;
}

HBRUSH CLightView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	LOGBRUSH   logbrush;   
	COLORREF     bkcolor(RGB(210,230,190));   
	logbrush.lbHatch   =   0;   
	logbrush.lbStyle   =   BS_SOLID;   
	logbrush.lbColor   =bkcolor;   
	logbrush.lbColor   =bkcolor;   
	HBRUSH   m_hBrush=(HBRUSH)::CreateBrushIndirect(&logbrush);
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
void CLightView::OnMyButton1()
{
	//AfxMessageBox("zhudeng");
	if (p_MyBut[0]->m_LightInfo.GStatus)
	{
		unsigned char buffer1[30]={'N',0x2F,0x43,0x2F,0x01,
			'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
			0x0A,0xA1,0xB2,0x00,0x00,0x00,0x00,0x00,0xCC};
		send(theApp.m_ConnectSock,(char*)buffer1,30,0);
	}
	else
	{
		unsigned char buffer2[30]={'N',0x2F,0x43,0x2F,0x01,
		'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
		0x0A,0xA1,0xB1,0x00,0x00,0x00,0x00,0x00,0xCC};
		send(theApp.m_ConnectSock,(char*)buffer2,30,0);
	}

}
void CLightView::OnMyButton2()
{
	//AfxMessageBox("zhudeng");
	if (p_MyBut[1]->m_LightInfo.GStatus)
	{
		unsigned char buffer1[30]={'N',0x2F,0x43,0x2F,0x01,
			'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
			0x0A,0xA2,0xB2,0x00,0x00,0x00,0x00,0x00,0xCC};
		send(theApp.m_ConnectSock,(char*)buffer1,30,0);
	}
	else
	{
		unsigned char buffer2[30]={'N',0x2F,0x43,0x2F,0x01,
			'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
			0x0A,0xA2,0xB1,0x00,0x00,0x00,0x00,0x00,0xCC};
		send(theApp.m_ConnectSock,(char*)buffer2,30,0);
	}

}
void CLightView::OnMyButton3()
{
	//AfxMessageBox("zhudeng");
	if (p_MyBut[2]->m_LightInfo.GStatus)
	{
		unsigned char buffer1[30]={'N',0x2F,0x43,0x2F,0x01,
			'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
			0x0A,0xA3,0xB2,0x00,0x00,0x00,0x00,0x00,0xCC};
		send(theApp.m_ConnectSock,(char*)buffer1,30,0);
	}
	else
	{
		unsigned char buffer2[30]={'N',0x2F,0x43,0x2F,0x01,
			'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
			0x0A,0xA3,0xB1,0x00,0x00,0x00,0x00,0x00,0xCC};
		send(theApp.m_ConnectSock,(char*)buffer2,30,0);
	}

}

void CLightView::OnLightShow()
{

	p_MyBut[0] = NewMyButton( 1, CRect(20,20,52,52),0);
	if (m_InitLInfo.LMainStatus[0]==0x00)
		p_MyBut[0]->SetIcon(IDI_LIGHT_OFF);
	else 
		p_MyBut[0]->SetIcon(IDI_LIGHT_ON);
	p_MyBut[0]->SetTooltipText(_T("主灯"));
	p_MyBut[0]->DrawBorder(FALSE);
	p_MyBut[0]->SetAlign(CButtonST::ST_ALIGN_VERT);
	p_MyBut[1] = NewMyButton( 2, CRect(20+32,20,52+32,52), 0 );
	if (m_InitLInfo.LSecondStatus[0]==0x00)
		p_MyBut[1]->SetIcon(IDI_LIGHT_OFF);
	else 
		p_MyBut[1]->SetIcon(IDI_LIGHT_ON);
	p_MyBut[1]->SetIcon(IDI_LIGHT_OFF);
	p_MyBut[1]->SetTooltipText(_T("辅灯"));
	p_MyBut[1]->DrawBorder(FALSE);
	p_MyBut[1]->SetAlign(CButtonST::ST_ALIGN_VERT);
	p_MyBut[2] = NewMyButton( 3, CRect(20,20+32,84,72), 0 );
	p_MyBut[2]->SetTooltipText(_T("双灯"));
	p_MyBut[2]->SetAlign(CButtonST::ST_ALIGN_VERT);

	p_MyBut[3] = NewMyButton( 4, CRect(20+100,20,52+100,52),0);
	p_MyBut[3]->SetIcon(IDI_LIGHT_OFF);
	p_MyBut[3]->SetTooltipText(_T("主灯"));
	p_MyBut[3]->DrawBorder(FALSE);
	p_MyBut[3]->SetAlign(CButtonST::ST_ALIGN_VERT);
	p_MyBut[4] = NewMyButton( 5, CRect(20+32+100,20,52+32+100,52), 0 );
	p_MyBut[4]->SetIcon(IDI_LIGHT_OFF);
	p_MyBut[4]->SetTooltipText(_T("辅灯"));
	p_MyBut[4]->DrawBorder(FALSE);
	p_MyBut[4]->SetAlign(CButtonST::ST_ALIGN_VERT);
	p_MyBut[5] = NewMyButton( 6, CRect(20+100,20+32,84+100,72), 0 );
	p_MyBut[5]->SetTooltipText(_T("双灯"));
	p_MyBut[5]->SetAlign(CButtonST::ST_ALIGN_VERT);

	p_MyBut[6] = NewMyButton( 7, CRect(20+100+100,20,52+100+100,52),0);
	p_MyBut[6]->SetIcon(IDI_LIGHT_OFF);
	p_MyBut[6]->SetTooltipText(_T("主灯"));
	p_MyBut[6]->DrawBorder(FALSE);
	p_MyBut[6]->SetAlign(CButtonST::ST_ALIGN_VERT);
	p_MyBut[7] = NewMyButton( 8, CRect(20+32+100+100,20,52+32+100+100,52), 0 );
	p_MyBut[7]->SetIcon(IDI_LIGHT_OFF);
	p_MyBut[7]->SetTooltipText(_T("辅灯"));
	p_MyBut[7]->DrawBorder(FALSE);
	p_MyBut[7]->SetAlign(CButtonST::ST_ALIGN_VERT);
	p_MyBut[8] = NewMyButton( 9, CRect(20+100+100,20+32,84+100+100,72), 0 );
	p_MyBut[8]->SetTooltipText(_T("双灯"));
	p_MyBut[8]->SetAlign(CButtonST::ST_ALIGN_VERT);
}
CButtonST* CLightView::NewMyButton(int nID, CRect rect , int nStyle)
{
	CString m_Caption;
	m_Caption.LoadString( nID ); //取按钮标题
	CButtonST *p_Button = new CButtonST();
	ASSERT_VALID(p_Button);
	p_Button->Create( m_Caption, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | nStyle, rect, this, nID );  //创建按钮
	return p_Button;
}

void CLightView::ChangeButton(void)
{
	if (m_InitLInfo.LMainStatus[0]==0x00)
	{
		m_InitLInfo.LMainStatus[0]=0x01;
		p_MyBut[0]->SetIcon(IDI_LIGHT_ON);
	}
	else
	{
		m_InitLInfo.LMainStatus[0]=0x00;
		p_MyBut[0]->SetIcon(IDI_LIGHT_OFF);
	}
	UpdateWindow();
}

void CLightView::ChangeSecondButton(void)
{
	if (m_InitLInfo.LSecondStatus[0]==0x00)
	{
		m_InitLInfo.LSecondStatus[0]=0x01;
		p_MyBut[1]->SetIcon(IDI_LIGHT_ON);
	}
	else
	{
		m_InitLInfo.LSecondStatus[0]=0x00;
		p_MyBut[1]->SetIcon(IDI_LIGHT_OFF);
	}
	UpdateWindow();
}
