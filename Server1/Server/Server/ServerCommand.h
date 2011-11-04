//
//对基本的数据命令模块进行继承和扩充
//

#pragma once
#include "dbcommand.h"

class CServerCommand :
	public CDBCommand
{
public:
	CServerCommand(void);
	~CServerCommand(void);
};
