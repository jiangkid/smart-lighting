
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


#include <afxsock.h>            // MFC �׽�����չ
#include <afxcview.h>
#include "SkinPPLIB/SkinPPWTL.h"
#include <afxdisp.h>
#include <afxdhtml.h>


#define MAX_BUF_SIZE 4096			//���պͷ��ͻ����С
#define HEADLEN (sizeof(HDR))        //��ͷ����
#define WM_TRAYICON_MSG (WM_USER+100)
#define LENTH (sizeof(ZigbeeInfo))
#define RLENTH (sizeof(RoadListViewInfo))
#define GLENTH (sizeof(GPRSInfo))
#define TLENTH (sizeof(TerminalInfo))
#define WARNLENGTH sizeof(WarningInfo)
#define U8 unsigned char
#define ON  _T("��")
#define OFF _T("��")
#define UNUPDATA _T("δ����")
#define LOSTAV _T("Aȱ��")
#define LOSTBV _T("Bȱ��")
#define LOSTCV _T("Cȱ��")
#define HAV _T("A���ѹ��")
#define HBV _T("B���ѹ��")
#define HCV _T("C���ѹ��")
#define LAV _T("A���ѹ��")
#define LBV _T("B���ѹ��")
#define LCV _T("C���ѹ��")
#define MAINC _T("�������")
#define ASSISTC _T("�������")
#define ROADHC _T("·��������")
#define GPRSERROR _T("GPRS����Ͽ�")
#define DLONG sizeof(Decision)
#define MAPLENGTH sizeof(MAPInfo)
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


