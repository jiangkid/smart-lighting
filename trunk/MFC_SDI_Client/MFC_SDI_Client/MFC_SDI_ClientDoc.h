
// MFC_SDI_ClientDoc.h : CMFC_SDI_ClientDoc ��Ľӿ�
//


#pragma once


class CMFC_SDI_ClientDoc : public CDocument
{
protected: // �������л�����
	CMFC_SDI_ClientDoc();
	DECLARE_DYNCREATE(CMFC_SDI_ClientDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CMFC_SDI_ClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


