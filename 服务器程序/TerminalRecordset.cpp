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
BOOL CTerminalRecordset::SetTerminalName(int TID,CString TerminalName)
{
	CString SQL;
	SQL.Format("Select * From  Terminals Where [TID] = %d",TID);
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
CString CTerminalRecordset::GetTerminalName(int TID)
{
	CString SQL;
	CString TerminalName;
	SQL.Format("Select * From  Terminals Where [TID] = %d",TID);
	if (Open(SQL))
	{
		TerminalName=GetAsString("Name");
		return TerminalName;
	}
	return NULL;
	
}


