; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=2
Class1=CSubDomainApp
LastClass=CMainDlg
NewFileInclude2=#include "SubDomain.h"
ResourceCount=2
NewFileInclude1=#include "stdafx.h"
Class2=CMainDlg
LastTemplate=CDialog
Resource1=IDR_MENU_LIST
Resource2=IDD_MAIN_DLG

[CLS:CSubDomainApp]
Type=0
HeaderFile=SubDomain.h
ImplementationFile=SubDomain.cpp
Filter=N
LastObject=CSubDomainApp

[DLG:IDD_MAIN_DLG]
Type=1
Class=CMainDlg
ControlCount=7
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_DOMAIN,edit,1342242944
Control3=IDC_BUTTON_SCAN,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_THREAD,edit,1342242944
Control6=IDC_LIST,SysListView32,1342242825
Control7=IDC_CHECK_GET_INFO,button,1342242819

[CLS:CMainDlg]
Type=0
HeaderFile=MainDlg.h
ImplementationFile=MainDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CMainDlg

[MNU:IDR_MENU_LIST]
Type=1
Class=CMainDlg
Command1=ID_MENUITEM_OPEN_URL
Command2=ID_MENUITEM_COPY_DOMAIN
Command3=ID_MENUITEM_COPY_HTTP_DOMAIN
Command4=ID_MENUITEM_COPY_IP
Command5=ID_MENUITEM_SAVE_DOMAIN
Command6=ID_MENUITEM_SAVE_DOMAIN_IP
Command7=ID_MENUITEM_SAVE_HTTP_DOMAIN
Command8=ID_MENUITEM_SAVE_HTML
Command9=ID_MENUITEM_DELETE_SELETED
Command10=ID_MENUITEM_CLEAR_LIST
CommandCount=10

