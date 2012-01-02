#include "StdAfx.h"
#include "TerminalRecordset.h"

CTerminalRecordset::CTerminalRecordset(void)
{
}

CTerminalRecordset::~CTerminalRecordset(void)
{
}

//
//����Terminal�������е�����
//
BOOL CTerminalRecordset::LoadTerminalData()
{
	//ʹ��Open����������������
	return Open("Select * From Terminals");
}

//
//�����ն˵�ID�������ն˵�����,�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE
//
BOOL CTerminalRecordset::SetTerminalName(CString TerminalID,CString TerminalName)
{
	CString SQL;
	SQL.Format("Select * From  Terminals Where [TerminalID] = %s",TerminalID);
	if (Open(SQL))
	{
		SetAsString("Name",TerminalName);
		return TRUE;
	}
	return FALSE;
}

//
//����ID�鿴·������,����Ϊ·������
//
CString CTerminalRecordset::GetTerminalName(CString TerminalID)
{
	CString SQL;
	CString TerminalName;
	SQL.Format("Select * From  Terminals Where [TerminalID] = '%s'",TerminalID);
	if (Open(SQL))
	{
		TerminalName=GetAsString("Name");
		return TerminalName;
	}
	return NULL;
	
}


