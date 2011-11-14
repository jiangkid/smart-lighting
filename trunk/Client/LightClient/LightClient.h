// LightClient.h : main header file for the LIGHTCLIENT application
//

#if !defined(AFX_LIGHTCLIENT_H__7F1458AF_4995_4196_89F9_32E292B2940A__INCLUDED_)
#define AFX_LIGHTCLIENT_H__7F1458AF_4995_4196_89F9_32E292B2940A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "LVErrorRecord.h"
/////////////////////////////////////////////////////////////////////////////
// CLightClientApp:
// See LightClient.cpp for the implementation of this class
//

class CLightClientApp : public CWinApp
{
public:
	CLightClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightClientApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CLightClientApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
public:
	_ConnectionPtr	m_pConnection;
	_CommandPtr		m_pCommand;
	_RecordsetPtr	m_pRecordset;

	CLVErrorRecord	*pRecord;
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTCLIENT_H__7F1458AF_4995_4196_89F9_32E292B2940A__INCLUDED_)
