; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=2
Class1=CScanInjectionApp
LastClass=CMainDlg
NewFileInclude2=#include "ScanInjection.h"
ResourceCount=3
NewFileInclude1=#include "stdafx.h"
Class2=CMainDlg
LastTemplate=CDialog
Resource1=IDD_MAIN_DLG
Resource2=IDR_MENU_TOP
Resource3=IDR_MENU_BOTTOM

[CLS:CScanInjectionApp]
Type=0
HeaderFile=ScanInjection.h
ImplementationFile=ScanInjection.cpp
Filter=N
LastObject=CScanInjectionApp

[DLG:IDD_MAIN_DLG]
Type=1
Class=CMainDlg
ControlCount=12
Control1=IDC_EDIT_TIMEOUT,edit,1342242944
Control2=IDC_EDIT_MAXTHREADNUM,edit,1342242944
Control3=IDC_EDIT_WEBPAGECHANGERATE,edit,1342242944
Control4=IDC_CHECK_NORMAL,button,1342242819
Control5=IDC_BUTTON_SCAN,button,1342242816
Control6=IDC_LIST_URL,SysListView32,1342242825
Control7=IDC_LIST_RESULT,SysListView32,1342242825
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_SPLITTER,static,1073741831

[CLS:CMainDlg]
Type=0
HeaderFile=MainDlg.h
ImplementationFile=MainDlg.cpp
BaseClass=CDialog
Filter=W
VirtualFilter=dWC
LastObject=ID_MENUITEM_IMPORT

[MNU:IDR_MENU_TOP]
Type=1
Class=CMainDlg
Command1=ID_MENUITEM_VISIT
Command2=ID_MENUITEM_COPY
Command3=ID_MENUITEM_SAVE
Command4=ID_MENUITEM_ADDURLTOLIST
Command5=ID_MENUITEM_IMPORT
Command6=ID_MENUITEM_DEL
Command7=ID_MENUITEM_CLEAR
CommandCount=7

[MNU:IDR_MENU_BOTTOM]
Type=1
Class=CMainDlg
Command1=ID_MENUITEM_VISIT_RESULT
Command2=ID_MENUITEM_COPY_RESULT
Command3=ID_MENUITEM_SAVE_RESULT
Command4=ID_MENUITEM_SCAN_TIMEOUT_URLS
Command5=ID_MENUITEM_DELETE_TIMEOUT_URLS
Command6=ID_MENUITEM_DEL_RESULT
Command7=ID_MENUITEM_CLEAR_RESULT
CommandCount=7

