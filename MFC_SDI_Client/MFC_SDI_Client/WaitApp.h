#pragma once
#include "afxwin.h"
#include "MFC_SDI_Client.h"
#include "WaitDlg.h"
class CWaitApp :
	public CWinThread
{
	DECLARE_DYNCREATE(CWaitApp)
public:
	CWaitApp(void);
	~CWaitApp(void);
	CWaitDlg dlg;
	virtual int ExitInstance();
	virtual BOOL InitInstance();
protected:
	DECLARE_MESSAGE_MAP()
};
