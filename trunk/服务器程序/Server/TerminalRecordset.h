#pragma once
#include "dbrecordset.h"

class CTerminalRecordset :
	public CDBRecordset
{
public:
	CTerminalRecordset(void);
	~CTerminalRecordset(void);

	//����Terminal���е���������
	BOOL LoadTerminalData();

	//�����ն˵�ID�������ն˵�����
	BOOL SetTerminalName(CString TerminalID,CString TerminalName);

	//����ID�鿴�ն˵�����
	CString GetTerminalName(CString TerminalID);

}; 
