// MFC_Client_Light.h : main header file for the MFC_CLIENT_LIGHT application
//

#if !defined(AFX_MFC_CLIENT_LIGHT_H__5A068AD3_EF89_41BD_8ECF_3BE27601AE8C__INCLUDED_)
#define AFX_MFC_CLIENT_LIGHT_H__5A068AD3_EF89_41BD_8ECF_3BE27601AE8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMFC_Client_LightApp:
// See MFC_Client_Light.cpp for the implementation of this class
//

class CMFC_Client_LightApp : public CWinApp
{
public:
	CMFC_Client_LightApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFC_Client_LightApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMFC_Client_LightApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_CLIENT_LIGHT_H__5A068AD3_EF89_41BD_8ECF_3BE27601AE8C__INCLUDED_)
