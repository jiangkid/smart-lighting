// LightClientDoc.h : interface of the CLightClientDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTCLIENTDOC_H__527D61AB_D1EA_41D0_BF3A_7961D2F70860__INCLUDED_)
#define AFX_LIGHTCLIENTDOC_H__527D61AB_D1EA_41D0_BF3A_7961D2F70860__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLightClientDoc : public CDocument
{
protected: // create from serialization only
	CLightClientDoc();
	DECLARE_DYNCREATE(CLightClientDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightClientDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLightClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLightClientDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTCLIENTDOC_H__527D61AB_D1EA_41D0_BF3A_7961D2F70860__INCLUDED_)
