// MFC_Client_LightDoc.h : interface of the CMFC_Client_LightDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFC_CLIENT_LIGHTDOC_H__E01D2077_312C_4F7E_B9E8_9ED70896D88F__INCLUDED_)
#define AFX_MFC_CLIENT_LIGHTDOC_H__E01D2077_312C_4F7E_B9E8_9ED70896D88F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMFC_Client_LightDoc : public CDocument
{
protected: // create from serialization only
	CMFC_Client_LightDoc();
	DECLARE_DYNCREATE(CMFC_Client_LightDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFC_Client_LightDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMFC_Client_LightDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMFC_Client_LightDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_CLIENT_LIGHTDOC_H__E01D2077_312C_4F7E_B9E8_9ED70896D88F__INCLUDED_)
