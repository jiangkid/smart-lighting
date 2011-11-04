
//
//���ݿ����ģ��
//

#include "DBConnection.h"
#pragma once

class CDataSet
{
public:
	CDataSet(void);
	~CDataSet(void);

	void Update();                                                                 //�����û��޸�
	void SetAsString(CString FieldName,CString Value);                                           //����FieldNameָ�����ֶε�ֵ
	void Close();                                                    //�ر����ݼ�
	virtual BOOL LoadData();                                 //�������ݼ�������
	CString GetAsString(CString FieldName);         //���FieldNameָ���ֶε�ֵ
	BOOL IsBOF();                                                 //��ǰ�α��Ƿ��ڵ�һ����¼֮ǰ           
	BOOL IsEOF();                                                 //��ǰ�α��Ƿ��ڴ������һ����¼֮��
	void MovePrevious();                                      //�Ƶ�ǰһ����¼
	void MovNext();                                              //�Ƶ���һ����¼
	void MoveLast();                                             //�Ƶ����һ����¼
	void MovFirst();                                              //�ƶ�����һ����¼

	CDBConnection *m_cnn;       //���ݿ����Ӷ���

protected:
	BOOL Open(CString SQL);    //�����ݼ�

private:
	BOOL m_Actived;      //�Ƿ��ڴ�״̬
	_RecordsetPtr m_pRs;
};