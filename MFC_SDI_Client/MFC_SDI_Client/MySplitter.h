#pragma once
#include "afxext.h"

class CMySplitter :
	public CSplitterWnd
{
public:
	CMySplitter(void);
	~CMySplitter(void);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
