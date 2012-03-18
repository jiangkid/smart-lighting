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
//向Areas这张表中添加一条新的记录
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
//向Areas这张表中添加一个新的ID
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
/* 功能:向Area表中添加一条区域的信息
参数说明:  areaID:区域ID   areaName:区域名称   dutyArea:责任区
				telephone:手机号     installTime:安装时间     installLocation:安装地点*/
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
