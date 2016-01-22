; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=3
Class1=CScanCSectionApp
LastClass=CMainDlg
NewFileInclude2=#include "ScanCSection.h"
ResourceCount=4
NewFileInclude1=#include "stdafx.h"
Class2=CMainDlg
LastTemplate=CDialog
Resource1=IDR_MENU
Resource2=IDR_MENU_KEY
Class3=CBingKeyDlg
Resource3=IDD_BINGKEY_DLG
Resource4=IDD_MAIN_DLG

[CLS:CScanCSectionApp]
Type=0
HeaderFile=ScanCSection.h
ImplementationFile=ScanCSection.cpp
Filter=N
LastObject=CScanCSectionApp

[DLG:IDD_MAIN_DLG]
Type=1
Class=CMainDlg
ControlCount=18
Control1=IDC_EDIT_URL,edit,1342242944
Control2=IDC_BUTTON_PARSE,button,1342242816
Control3=IDC_COMBO_IP,combobox,1344339970
Control4=IDC_EDIT_START_IP,edit,1342251136
Control5=IDC_EDIT_END_IP,edit,1342251136
Control6=IDC_COMBO_MODE,combobox,1344339971
Control7=IDC_COMBO_INTERFACE,combobox,1344339971
Control8=IDC_CHECK_GET_SERVER_TYPE,button,1342242819
Control9=IDC_CHECK_GET_DOMAIN_IP,button,1342242819
Control10=IDC_BUTTON_SCAN,button,1342242816
Control11=IDC_TREE,SysTreeView32,1342248999
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC_1,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC_INTERFACE,static,1342308608
Control16=IDC_STATIC_2,static,1342308352
Control17=IDC_STATIC_IP,static,1342308608
Control18=IDC_EDIT_SHOW,edit,1344346244

[CLS:CMainDlg]
Type=0
HeaderFile=MainDlg.h
ImplementationFile=MainDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_IP

[MNU:IDR_MENU]
Type=1
Class=CMainDlg
Command1=ID_MENUITEM_COPY_SELECTED
Command2=ID_MENUITEM_SAVE_HTML
Command3=ID_MENUITEM_SAVE_TXT
Command4=ID_MENUITEM_SAVE_URL
Command5=ID_MENUITEM_EXPAND
Command6=ID_MENUITEM_COLLAPSE
Command7=ID_MENUITEM_DELETE_ERROR_URL
Command8=ID_MENUITEM_DELETE_SELECTED
Command9=ID_MENUITEM_CLEAR
CommandCount=9

[DLG:IDD_BINGKEY_DLG]
Type=1
Class=CBingKeyDlg
ControlCount=1
Control1=IDC_LIST,SysListView32,1342242829

[CLS:CBingKeyDlg]
Type=0
HeaderFile=BingKeyDlg.h
ImplementationFile=BingKeyDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_STATIC_URL

[MNU:IDR_MENU_KEY]
Type=1
Class=CBingKeyDlg
Command1=ID_MENUITEM_USE_KEY
Command2=ID_MENUITEM_COPY_KEY
Command3=ID_MENUITEM_ADD_KEY
Command4=ID_MENUITEM_DELETE_KEY
CommandCount=4

