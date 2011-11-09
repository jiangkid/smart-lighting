//
//对数据库里数据进行访问和设置的封装
//

#pragma once
#include "dataset.h"

class CServerDataSet :
	public CDataSet
{
public:
	CServerDataSet(void);
	~CServerDataSet(void);

	//和user&password表的"user"列对应的方法
	CString GetUserName();
	void SetUserName(CString name);


};

extern CServerDataSet *g_pServerDataSet;
