
// MFC_SDI_ClientDoc.cpp : CMFC_SDI_ClientDoc 类的实现
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"

#include "MFC_SDI_ClientDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_SDI_ClientDoc

IMPLEMENT_DYNCREATE(CMFC_SDI_ClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFC_SDI_ClientDoc, CDocument)
END_MESSAGE_MAP()


// CMFC_SDI_ClientDoc 构造/析构

CMFC_SDI_ClientDoc::CMFC_SDI_ClientDoc()
{
	// TODO: 在此添加一次性构造代码

}

CMFC_SDI_ClientDoc::~CMFC_SDI_ClientDoc()
{
}

BOOL CMFC_SDI_ClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMFC_SDI_ClientDoc 序列化

void CMFC_SDI_ClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CMFC_SDI_ClientDoc 诊断

#ifdef _DEBUG
void CMFC_SDI_ClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFC_SDI_ClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFC_SDI_ClientDoc 命令
