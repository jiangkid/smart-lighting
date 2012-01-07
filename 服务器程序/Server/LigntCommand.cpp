#include "StdAfx.h"
#include "LightCommand.h"

CLightCommand::CLightCommand(void)
{
}

CLightCommand::~CLightCommand(void)
{
}

BOOL CLightCommand::AddLight(CString LightID,CString LightName/*,BOOL Status,CString GroupNum*/)
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;
	SQL.Format("Insert Into Lights([ID],[LightID],[LightName])Values(%d,\""+LightID+"\",\""+LightName+"\")",MaxID);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}
//向Light表中添加一个新的ID
BOOL CLightCommand::AddLightID(CString RoadID,CString LightID)
{
	CString LightSQL;
	CString RoadSQL;
	_RecordsetPtr pRoad;
	_variant_t vIDRoad;
	long idLight=0;
	int MaxID = GetMaxID();
	++MaxID;

	RoadSQL.Format("SELECT * FROM Roads WHERE RoadID='%s'",RoadID);
	try
	{
		pRoad.CreateInstance("ADODB.RecordSet");
		pRoad->Open((_variant_t)RoadSQL,_variant_t((IDispatch*)m_cnn->m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error&e)
	{
		AfxMessageBox(e.Description());
	}
	if(!pRoad->adoEOF)
	{
		vIDRoad=pRoad->GetCollect(_variant_t((long)0)); 
		if (vIDRoad.vt!=VT_NULL)
		{
			idLight=vIDRoad.lVal;        //与Terminals表建立一对多的关联字段IDArea的值
		}
	}
	LightSQL.Format("Insert Into Lights([ID],[LightID],[IDRoad])Values(%d,'%s',%ld)",MaxID,LightID,idLight);
	if (!ExcuteSQL(LightSQL))
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CLightCommand::AddLightID(CString LightID)
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;
	int IDRoad = 0;
	SQL.Format("Insert Into Lights([ID],[LightID],[IDRoad])Values(%d,\""+LightID+"\",%d)",MaxID,IDRoad);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;
}

int CLightCommand::GetMaxID()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM Lights";
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
