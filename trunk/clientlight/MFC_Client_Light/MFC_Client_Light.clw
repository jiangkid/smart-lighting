; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLightView
LastTemplate=CFormView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MFC_Client_Light.h"
LastPage=0

ClassCount=10
Class1=CMFC_Client_LightApp
Class2=CMFC_Client_LightDoc
Class3=CMFC_Client_LightView
Class4=CMainFrame

ResourceCount=6
Resource1=IDD_LOGIN_ADDMIN
Resource2=IDD_DIALOG2
Class5=CAboutDlg
Class6=CClientSocket
Class7=CUserLoginDlg
Resource3=IDD_ABOUTBOX
Class8=COnVIewStatusList
Resource4=IDR_MAINFRAME
Class9=CAddminLoginDlg
Resource5=IDD_LOGIN_USER
Class10=CLightView
Resource6=IDD_DIALOG3

[CLS:CMFC_Client_LightApp]
Type=0
HeaderFile=MFC_Client_Light.h
ImplementationFile=MFC_Client_Light.cpp
Filter=N
LastObject=CMFC_Client_LightApp

[CLS:CMFC_Client_LightDoc]
Type=0
HeaderFile=MFC_Client_LightDoc.h
ImplementationFile=MFC_Client_LightDoc.cpp
Filter=N
LastObject=CMFC_Client_LightDoc

[CLS:CMFC_Client_LightView]
Type=0
HeaderFile=MFC_Client_LightView.h
ImplementationFile=MFC_Client_LightView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CMFC_Client_LightView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_LOGIN_USER
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
Control2=IDC_STATIC,static,1342308481
Control3=IDC_STATIC,static,1342308353
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

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_BUTTON32772
Command2=ID_BUTTON32779
Command3=ID_CONNECT
Command4=ID_QUIT
Command5=ID_BTN_ADD_A
CommandCount=5

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_CONNECT
Command2=ID_QUIT
Command3=ID_LOGIN_ADDMIN
Command4=ID_LOGIN_USER
Command5=ID_ADDMIT
Command6=ID_NAME_A
Command7=ID_NAME_R
Command8=ID_NAME_L
Command9=ID_VIEW_TOOLBAR
Command10=ID_VIEW_STATUS_BAR
Command11=ID_APP_ABOUT
CommandCount=11

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
LastObject=COnVIewStatusList

[DLG:IDD_LOGIN_ADDMIN]
Type=1
Class=CAddminLoginDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308353
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_STATIC,static,1342308353
Control4=IDC_EDIT2,edit,1350631584
Control5=IDC_BTN_A_LOGIN,button,1342242816
Control6=IDC_BTN_A_Q,button,1342242816

[CLS:CAddminLoginDlg]
Type=0
HeaderFile=AddminLoginDlg.h
ImplementationFile=AddminLoginDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAddminLoginDlg

[DLG:IDD_LOGIN_USER]
Type=1
Class=CUserLoginDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT2,edit,1350631584
Control5=IDC_BTN_U_LOGIN,button,1342242816
Control6=IDC_BTN_U_Q,button,1342242816

[CLS:CUserLoginDlg]
Type=0
HeaderFile=UserLoginDlg.h
ImplementationFile=UserLoginDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CUserLoginDlg

[CLS:CLightView]
Type=0
HeaderFile=LightView.h
ImplementationFile=LightView.cpp
BaseClass=CFormView
Filter=D
LastObject=IDC_BUTTON3
VirtualFilter=VWC

[DLG:IDD_DIALOG3]
Type=1
Class=CLightView
ControlCount=9
Control1=IDC_BTN_LIGHT1,button,1342242816
Control2=IDC_BTN_LIGHT2,button,1342242816
Control3=IDC_BTN_LIGHT3,button,1342242816
Control4=IDC_STATIC,static,1342308353
Control5=IDC_STATIC,static,1342308353
Control6=IDC_STATIC,static,1342308353
Control7=IDC_BUTTON3,button,1342242816
Control8=IDC_BUTTON1,button,1342242816
Control9=IDC_BUTTON2,button,1342242816

