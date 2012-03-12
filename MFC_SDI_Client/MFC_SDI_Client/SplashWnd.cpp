// SplashWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "SplashWnd.h"


// CSplashWnd

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

CSplashWnd::CSplashWnd()
{
	LPCTSTR lpszWndClass = AfxRegisterWndClass(NULL);
	CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,lpszWndClass,NULL,WS_POPUP
		,CW_USEDEFAULT,CW_USEDEFAULT,SPLASH_WIDTH,SPLASH_HEIGHT,NULL,NULL,NULL);
}

CSplashWnd::~CSplashWnd()
{
}


BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CSplashWnd message handlers
void CSplashWnd::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	AnimateWindow(800,AW_BLEND | AW_HIDE);
	CWnd::OnClose();
}

void CSplashWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent==1)
	{
		KillTimer(1);
		PostMessage(WM_CLOSE);
	}

	CWnd::OnTimer(nIDEvent);
}

BOOL CSplashWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	HINSTANCE hApp = ::GetModuleHandle(NULL);
	HBITMAP hbmp = ::LoadBitmap(hApp,MAKEINTRESOURCE(IDB_SPLASH));

	HDC hmdc = CreateCompatibleDC(pDC->GetSafeHdc());
	HBITMAP hbmpOld = (HBITMAP) :: SelectObject(hmdc,hbmp);

	::BitBlt(pDC->GetSafeHdc(),0,0,SPLASH_WIDTH,SPLASH_HEIGHT,hmdc,0,0,SRCCOPY);
	::SelectObject(hmdc,hbmpOld);
	::DeleteObject(hmdc);
	::DeleteDC(hmdc);
	return CWnd::OnEraseBkgnd(pDC);
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CenterWindow();
	SetTimer(1,3000,NULL);
	AnimateWindow(1200,AW_BLEND);
	return 0;
}
