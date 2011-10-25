; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "lightclient.h"
LastPage=0

ClassCount=9
Class1=CDataSocket
Class2=CLightClientApp
Class3=CAboutDlg
Class4=CLightClientDoc
Class5=CLightClientView
Class6=CListenSocket
Class7=CLoginDlg
Class8=CMainFrame

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDD_LOGIN
Resource3=IDD_CONNECT
Class9=CChatCAsyncSocketDlg
Resource4=IDR_MAINFRAME

[CLS:CDataSocket]
Type=0
BaseClass=CAsyncSocket
HeaderFile=DataSocket.h
ImplementationFile=DataSocket.cpp

[CLS:CLightClientApp]
Type=0
BaseClass=CWinApp
HeaderFile=LightClient.h
ImplementationFile=LightClient.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=LightClient.cpp
ImplementationFile=LightClient.cpp

[CLS:CLightClientDoc]
Type=0
BaseClass=CDocument
HeaderFile=LightClientDoc.h
ImplementationFile=LightClientDoc.cpp

[CLS:CLightClientView]
Type=0
BaseClass=CView
HeaderFile=LightClientView.h
ImplementationFile=LightClientView.cpp

[CLS:CListenSocket]
Type=0
BaseClass=CAsyncSocket
HeaderFile=ListenSocket.h
ImplementationFile=ListenSocket.cpp

[CLS:CLoginDlg]
Type=0
BaseClass=CDialog
HeaderFile=LoginDlg.h
ImplementationFile=LoginDlg.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

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
Class=?
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

[DLG:IDD_CONNECT]
Type=1
Class=CChatCAsyncSocketDlg
ControlCount=8
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,button,1342177287
Control3=IDC_LISTEN_PORT_C,edit,1350631552
Control4=IDC_CONNECT,button,1342242816
Control5=IDC_EXIT,button,1342242816
Control6=IDC_SERVERIP,edit,1350631552
Control7=IDC_SHOWTEXT,edit,1352730692
Control8=IDC_INPUTTEXT,edit,1350635652

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

[CLS:CChatCAsyncSocketDlg]
Type=0
HeaderFile=ChatCAsyncSocketDlg.h
ImplementationFile=ChatCAsyncSocketDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_Connect
VirtualFilter=dWC

