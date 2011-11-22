; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMFC_Client_LightView
LastTemplate=CListCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MFC_Client_Light.h"
LastPage=0

ClassCount=8
Class1=CMFC_Client_LightApp
Class2=CMFC_Client_LightDoc
Class3=CMFC_Client_LightView
Class4=CMainFrame

ResourceCount=4
Resource1=IDR_MAINFRAME
Resource2=IDD_ABOUTBOX
Class5=CAboutDlg
Class6=CClientSocket
Class7=CMFC_Client_LightView1
Resource3=IDD_DIALOG1
Class8=COnVIewStatusList
Resource4=IDD_DIALOG2

[CLS:CMFC_Client_LightApp]
Type=0
HeaderFile=MFC_Client_Light.h
ImplementationFile=MFC_Client_Light.cpp
Filter=N

[CLS:CMFC_Client_LightDoc]
Type=0
HeaderFile=MFC_Client_LightDoc.h
ImplementationFile=MFC_Client_LightDoc.cpp
Filter=N

[CLS:CMFC_Client_LightView]
Type=0
HeaderFile=MFC_Client_LightView.h
ImplementationFile=MFC_Client_LightView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=IDC_EDIT1


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_QUIT
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=MFC_Client_Light.cpp
ImplementationFile=MFC_Client_Light.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

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

[CLS:CClientSocket]
Type=0
HeaderFile=ClientSocket.h
ImplementationFile=ClientSocket.cpp
BaseClass=CSocket
Filter=N
LastObject=CClientSocket

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=4
Control1=IDC_IPADDRESS1,SysIPAddress32,1342242816
Control2=IDC_STATIC,static,1342308353
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_BUTTON32772
Command2=ID_BUTTON32773
Command3=ID_BUTTON32779
Command4=ID_EDIT
Command5=ID_CONNECT
Command6=ID_BUTTON32775
Command7=ID_QUIT
CommandCount=7

[CLS:CMFC_Client_LightView1]
Type=0
HeaderFile=MFC_Client_LightView1.h
ImplementationFile=MFC_Client_LightView1.cpp
BaseClass=CListView
Filter=C

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_CONNECT
Command2=ID_QUIT
Command3=ID_VIEW_TOOLBAR
Command4=ID_VIEW_STATUS_BAR
Command5=ID_APP_ABOUT
CommandCount=5

[DLG:IDD_DIALOG2]
Type=1
Class=?
ControlCount=0

[CLS:COnVIewStatusList]
Type=0
HeaderFile=OnVIewStatusList.h
ImplementationFile=OnVIewStatusList.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC
LastObject=IDC_LIST2

