
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


#include <afxsock.h>            // MFC 套接字扩展
#include <afxcview.h>
#include "SkinPPLIB/SkinPPWTL.h"
#include <afxdisp.h>
#include <afxdhtml.h>


#define MAX_BUF_SIZE 4096			//接收和发送缓存大小
#define HEADLEN (sizeof(HDR))        //包头长度
#define WM_TRAYICON_MSG (WM_USER+100)
#define LENTH (sizeof(ZigbeeInfo))
#define RLENTH (sizeof(RoadListViewInfo))
#define GLENTH (sizeof(GPRSInfo))
#define TLENTH (sizeof(TerminalInfo))
#define WARNLENGTH sizeof(WarningInfo)
#define U8 unsigned char
#define ON  _T("开")
#define OFF _T("关")
#define UNUPDATA _T("未更新")
#define LOSTAV _T("A缺相")
#define LOSTBV _T("B缺相")
#define LOSTCV _T("C缺相")
#define HAV _T("A相电压高")
#define HBV _T("B相电压高")
#define HCV _T("C相电压高")
#define LAV _T("A相电压低")
#define LBV _T("B相电压低")
#define LCV _T("C相电压低")
#define MAINC _T("主电故障")
#define ASSISTC _T("备电故障")
#define ROADHC _T("路电流超了")
#define GPRSERROR _T("GPRS意外断开")
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


