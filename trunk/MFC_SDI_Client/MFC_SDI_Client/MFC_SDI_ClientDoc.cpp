
// MFC_SDI_ClientDoc.cpp : CMFC_SDI_ClientDoc ���ʵ��
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


// CMFC_SDI_ClientDoc ����/����

CMFC_SDI_ClientDoc::CMFC_SDI_ClientDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CMFC_SDI_ClientDoc::~CMFC_SDI_ClientDoc()
{
}

BOOL CMFC_SDI_ClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CMFC_SDI_ClientDoc ���л�

void CMFC_SDI_ClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CMFC_SDI_ClientDoc ���

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


// CMFC_SDI_ClientDoc ����
