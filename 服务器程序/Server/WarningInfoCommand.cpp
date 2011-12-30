#include "StdAfx.h"
#include "WarningInfoCommand.h"

CWarningInfoCommand::CWarningInfoCommand(void)
{
}

CWarningInfoCommand::~CWarningInfoCommand(void)
{
}

BOOL CWarningInfoCommand::AddWarningIfo(int TID,CString Time,CString ClassOfWarning,CString Info)
{	
	CString SQL;
	int MaxID = GetMaxIDOfWarningInfo();
	MaxID++;
// 	CString strTID;
// 	strTID.Format("%d",TID);
	// SQL.Format("Insert Into Users(ID,Name,Password,Area)Values(%d,\""+UserName+"\",\""+Psw+"\",\""+AreaName+"\")",MaxID);
	SQL.Format("Insert Into WarningInfo([ID],[TID],[Time],[ClassOfWarning],[Info])Values(%d,%d,\""+Time+"\",\""+ClassOfWarning+"\",\""+Info+"\")",MaxID,TID);
	if (ExcuteSQL(SQL))
	{
		return TRUE;
	}
	return FALSE;	
}

int CWarningInfoCommand::GetMaxIDOfWarningInfo()
{
	int nResult=0;
	_RecordsetPtr pRs;
	CString sql;
	sql="SELECT * FROM WarningInfo";
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
