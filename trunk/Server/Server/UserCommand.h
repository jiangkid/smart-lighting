#pragma once
#include "dbcommand.h"

class CUserCommand :
	public CDBCommand
{
public:
	CUserCommand(void);
	~CUserCommand(void);

	//�½�һ���û�
	BOOL NewUser(CString UserName,CString Psw);

	//�����û�������
	BOOL ChangeUserName(CString UserName,CString NewName);

	//����ָ���û�������
	BOOL ChangeUserPsw(CString UserName,CString NewPsw);

	//����ָ���û�������
	BOOL ChangeUserArea(CString UserName,CString NewArea);

	//ɾ��ָ�����û�
	BOOL DeleteUser(CString UserName);

	//�ж��û��Ƿ��������ݿ��д���
	BOOL IsExistedUser(CString UserName);

private:
	//���ص�ǰUser����ID���ֵ
	int GetMaxUserID();
};
