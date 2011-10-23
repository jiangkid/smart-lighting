; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CConnectDlg
LastTemplate=CAsyncSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "LightClient.h"
LastPage=0

ClassCount=8
Class1=CLightClientApp
Class2=CLightClientDoc
Class3=CLightClientView
Class4=CMainFrame

ResourceCount=4
Resource1=IDD_LOGIN
Resource2=IDD_CONNECT
Class5=CAboutDlg
Class6=CLoginDlg
Resource3=IDD_ABOUTBOX
Class7=CConnectDlg
Class8=CMysocket
Resource4=IDR_MAINFRAME

[CLS:CLightClientApp]
Type=0
HeaderFile=LightClient.h
ImplementationFile=LightClient.cpp
Filter=N
LastObject=ID_ID

[CLS:CLightClientDoc]
Type=0
HeaderFile=LightClientDoc.h
ImplementationFile=LightClientDoc.cpp
Filter=N
LastObject=CLightClientDoc

[CLS:CLightClientView]
Type=0
HeaderFile=LightClientView.h
ImplementationFile=LightClientView.cpp
Filter=C
LastObject=CLightClientView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_Connect
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=LightClient.cpp
ImplementationFile=LightClient.cpp
Filter=D
LastObject=ID_Login

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_Login
Command2=ID_Exit
Command3=ID_Connect
Command4=ID_ID
Command5=ID_DECISION
Command6=ID_APP_ABOUT
CommandCount=6

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_LOGIN]
Type=1
Class=CLoginDlg
ControlCount=7
Control1=IDC_LOGIN,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_EDIT2,edit,1350631552
Control6=IDC_EXIT,button,1342242816
Control7=IDC_CHECK1,button,1342242819

[CLS:CLoginDlg]
Type=0
HeaderFile=LoginDlg.h
ImplementationFile=LoginDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CLoginDlg
VirtualFilter=dWC

[DLG:IDD_CONNECT]
Type=1
Class=CConnectDlg
ControlCount=6
Control1=IDC_IPADDRESS1,SysIPAddress32,1342242816
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,button,1342177287
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_CONNECT,button,1342242816
Control6=IDC_Quit_Connect,button,1342242816

[CLS:CConnectDlg]
Type=0
HeaderFile=ConnectDlg.h
ImplementationFile=ConnectDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CConnectDlg
VirtualFilter=dWC

[CLS:CMysocket]
Type=0
HeaderFile=Mysocket.h
ImplementationFile=Mysocket.cpp
BaseClass=CAsyncSocket
Filter=N
VirtualFilter=q

