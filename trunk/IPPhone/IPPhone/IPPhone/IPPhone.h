// IPPhone.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CIPPhoneApp:
// �йش����ʵ�֣������ IPPhone.cpp
//

class CIPPhoneApp : public CWinApp
{
public:
	CIPPhoneApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CIPPhoneApp theApp;