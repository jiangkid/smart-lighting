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

	//�����ն˵�ID����ȡ�ն�������·��ID
	CString GetRoadIDsByTerminalID(CString TerminalID);

	//�����ն˵�ID����ȡ�ն�������·������
	CString GetRoadNamesByTerminalID(CString TerminalID);

	//�����ն˵�ID����ȡ�ն�������·��ID����Ŀ
	CString GetRoadIDsAndCountByTerminalID(CString TerminalID);

}; 
