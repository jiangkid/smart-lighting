#include "StdAfx.h"
#include "AreaCommand.h"

CAreaCommand::CAreaCommand(void)
{
}

CAreaCommand::~CAreaCommand(void)
{
}

int CAreaCommand::GetMaxID()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM Areas";
	try
	{
		/*_RecordsetPtr pRs;*/
		pRs.CreateInstance("ADODB.RecordSet");
		pRs->Open((_variant_t)sql,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	while (!pRs->adoEOF)
	{
		pRs->MoveNext();
		nResult++;
	}
	pRs->Close();
	return nResult;
}

//
//��Areas���ű������һ���µļ�¼
//
BOOL CAreaCommand::AddArea(CString AreaID, CString AreaName)
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;
	//int IDUser = 0;
	SQL.Format("Insert Into Areas([ID],[AreaID],[AreaName])Values(%d,\""+AreaID+"\",\""+AreaName+"\")",MaxID);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

//
//��Areas���ű������һ���µ�ID
BOOL CAreaCommand::AddAreaID(CString AreaID)
{
	CString SQL;
	int MaxID = GetMaxID();
	//MaxID++;
	//int IDUser = 0;
	//SQL.Format("Insert Into Areas([ID],[AreaID],[IDUser])Values(%d,\""+AreaID+"\",%d)",MaxID,AreaID,IDUser);
	SQL.Format("Insert Into Areas([ID],[AreaID])Values(%d,\""+AreaID+"\")",MaxID);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

/************************************************************************/
/* ����:��Area�������һ���������Ϣ
����˵��:  areaID:����ID   areaName:��������   dutyArea:������
				telephone:�ֻ���     installTime:��װʱ��     installLocation:��װ�ص�*/
/************************************************************************/
BOOL CAreaCommand::AddAreaInfo(CString areaID,CString areaName,CString dutyArea,CString telephone,CString installTime,CString installLocation,CString userName)
{
	CString sql;
	int maxID = GetMaxID();
	sql.Format("Insert Into Areas([ID],[AreaID],[AreaName],[DutyArea],[Telephone],[InstallTime],[InstallLocation])Values(%d,\""+areaID+"\",\""+areaName+"\",\""+dutyArea+"\",\""+telephone+"\",\""+installTime+"\",\""+installLocation+"\")",maxID);
	if (ExcuteSQL(sql))
	{
		return TRUE;
	}
	return FALSE;
}
