// MFC_Client_LightDoc.cpp : implementation of the CMFC_Client_LightDoc class
//

#include "stdafx.h"
#include "MFC_Client_Light.h"

#include "MFC_Client_LightDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFC_Client_LightDoc

IMPLEMENT_DYNCREATE(CMFC_Client_LightDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFC_Client_LightDoc, CDocument)
	//{{AFX_MSG_MAP(CMFC_Client_LightDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFC_Client_LightDoc construction/destruction

CMFC_Client_LightDoc::CMFC_Client_LightDoc()
{
	// TODO: add one-time construction code here

}

CMFC_Client_LightDoc::~CMFC_Client_LightDoc()
{
}

BOOL CMFC_Client_LightDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMFC_Client_LightDoc serialization

void CMFC_Client_LightDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMFC_Client_LightDoc diagnostics

#ifdef _DEBUG
void CMFC_Client_LightDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFC_Client_LightDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFC_Client_LightDoc commands
