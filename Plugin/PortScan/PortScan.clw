; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=3
Class1=CPortScanApp
LastClass=CMainDlg
NewFileInclude2=#include "PortScan.h"
ResourceCount=3
NewFileInclude1=#include "stdafx.h"
Class2=CMainDlg
LastTemplate=CDialog
Resource1=IDD_LIST_DLG
Class3=CListDlg
Resource2=IDD_MAIN_DLG
Resource3=IDR_MENU_TREE

[CLS:CPortScanApp]
Type=0
HeaderFile=PortScan.h
ImplementationFile=PortScan.cpp
Filter=N
LastObject=CPortScanApp

[DLG:IDD_MAIN_DLG]
Type=1
Class=CMainDlg
ControlCount=15
Control1=IDC_EDIT_URL,edit,1342242944
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC_IP,static,1342308352
Control4=IDC_BUTTON_RESOLVE,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_COMBO_IP_MODE,combobox,1344339971
Control7=IDC_COMBO_PORT,combobox,1344340034
Control8=IDC_COMBO_IP,combobox,1344339970
Control9=IDC_TREE,SysTreeView32,1342242855
Control10=IDC_CHECK_BANNER,button,1342242819
Control11=IDC_BUTTON_SCAN,button,1342242816
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT_MAX_THREAD,edit,1342251136
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT_TIMEOUT,edit,1342251136

[CLS:CMainDlg]
Type=0
HeaderFile=MainDlg.h
ImplementationFile=MainDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTON_SCAN

[DLG:IDD_LIST_DLG]
Type=1
Class=CListDlg
ControlCount=3
Control1=IDC_EDIT,edit,1344339972
Control2=IDC_BUTTON_OK,button,1342242816
Control3=IDC_STATIC,static,1342308352

[CLS:CListDlg]
Type=0
HeaderFile=ListDlg.h
ImplementationFile=ListDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTON_OK

[MNU:IDR_MENU_TREE]
Type=1
Class=CMainDlg
Command1=ID_MENUITEM_COPY
Command2=ID_MENUITEM_SAVE
Command3=ID_MENUITEM_EXPAND
Command4=ID_MENUITEM_COLLAPSE
Command5=ID_MENUITEM_EMPTY
CommandCount=5

