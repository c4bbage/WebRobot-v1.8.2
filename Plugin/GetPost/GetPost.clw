; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=3
Class1=CGetPostApp
LastClass=CMainDlg
NewFileInclude2=#include "GetPost.h"
ResourceCount=5
NewFileInclude1=#include "stdafx.h"
Class2=CMainDlg
LastTemplate=CDialog
Resource1=IDR_MENU_HEADER
Class3=CHeaderDlg
Resource2=IDD_HEADER_DLG
Resource3=IDR_MENU_RETURN
Resource4=IDD_MAIN_DLG
Resource5=IDR_MENU_LOADSAVE

[CLS:CGetPostApp]
Type=0
HeaderFile=GetPost.h
ImplementationFile=GetPost.cpp
Filter=N
LastObject=CGetPostApp

[DLG:IDD_MAIN_DLG]
Type=1
Class=CMainDlg
ControlCount=16
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_URL,edit,1342242944
Control3=IDC_BUTTON_EXEC,button,1342242816
Control4=IDC_STATIC_HEADER,static,1342308608
Control5=IDC_COMBO_MOTHOD,combobox,1344339971
Control6=IDC_EDIT_HEADER,edit,1342247108
Control7=IDC_STATIC_POST,static,1342308352
Control8=IDC_EDIT_POST,edit,1342247108
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_RETURN,edit,1342247108
Control11=IDC_CHECK_UTF8TOANSI,button,1342242819
Control12=IDC_BUTTON_INSERT,button,1342242816
Control13=IDC_STATIC_LOAD_SAVE,static,1342309121
Control14=IDC_CHECK_MUTI_THREAD,button,1342242819
Control15=IDC_EDIT_THREAD,edit,1342242944
Control16=IDC_STATIC_THREAD,static,1342308352

[CLS:CMainDlg]
Type=0
HeaderFile=MainDlg.h
ImplementationFile=MainDlg.cpp
BaseClass=CDialog
Filter=W
VirtualFilter=dWC
LastObject=IDC_EDIT_THREAD

[DLG:IDD_HEADER_DLG]
Type=1
Class=CHeaderDlg
ControlCount=4
Control1=IDC_EDIT_HEADER,edit,1345388612
Control2=IDC_BUTTON_ADD,button,1342242816
Control3=IDC_BUTTON_CLEAR,button,1342242816
Control4=IDC_EDIT_FOCUS,edit,1342242944

[CLS:CHeaderDlg]
Type=0
HeaderFile=HeaderDlg.h
ImplementationFile=HeaderDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT_HEADER

[MNU:IDR_MENU_HEADER]
Type=1
Class=CMainDlg
Command1=ID_MENUITEM_COOKIE
Command2=ID_MENUITEM_REFERER
Command3=ID_MENUITEM_UA_BAIDU
Command4=ID_MENUITEM_UA_GOOGLE
Command5=ID_MENUITEM_UA_IE
Command6=ID_MENUITEM_UA_CHROME
Command7=ID_MENUITEM_UA_ANDROID
Command8=ID_MENUITEM_UA_IPHONE
Command9=ID_MENUITEM_FAKEIP
CommandCount=9

[MNU:IDR_MENU_RETURN]
Type=1
Class=CMainDlg
Command1=ID_MENUITEM_CLEAR
Command2=ID_MENUITEM_COPYALL
Command3=ID_MENUITEM_BROWSER
CommandCount=3

[MNU:IDR_MENU_LOADSAVE]
Type=1
Class=CMainDlg
Command1=ID_MENUITEM_SAVE
Command2=ID_MENUITEM_LOAD
CommandCount=2

