// LightClientDoc.cpp : implementation of the CLightClientDoc class
//

#include "stdafx.h"
#include "LightClient.h"

#include "LightClientDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightClientDoc

IMPLEMENT_DYNCREATE(CLightClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CLightClientDoc, CDocument)
	//{{AFX_MSG_MAP(CLightClientDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightClientDoc construction/destruction

CLightClientDoc::CLightClientDoc()
{
	// TODO: add one-time construction code here

}

CLightClientDoc::~CLightClientDoc()
{
}

BOOL CLightClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetTitle(_T("路灯监看系统客户端界面"));
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLightClientDoc serialization

void CLightClientDoc::Serialize(CArchive& ar)
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
// CLightClientDoc diagnostics

#ifdef _DEBUG
void CLightClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLightClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLightClientDoc commands
