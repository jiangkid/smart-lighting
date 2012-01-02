#include "StdAfx.h"
#include "RoadCommand.h"

CRoadCommand::CRoadCommand(void)
{
}

CRoadCommand::~CRoadCommand(void)
{
}

//
//向路的这张表中添加一个新的记录
//
BOOL CRoadCommand::AddRoad(CString RoadID, CString RoadName,CString Lights)
{
	CString SQL;
	int MaxID = GetMaxID();
	MaxID++;

	SQL.Format("Insert Into Roads([ID],[RoadID],[RoadName],[Lights])Values(%d,\""+RoadID+"\",\""+RoadName+"\",\""+Lights+"\")",MaxID);
	if (!ExcuteSQL(SQL))
	{
		return FALSE;
	}
	return TRUE;

}

int CRoadCommand::GetMaxID()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM Roads";
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



