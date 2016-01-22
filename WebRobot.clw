; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CBrowserDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WebRobot.h"

ClassCount=7
Class1=CWebRobotApp
Class2=CWebRobotDlg

ResourceCount=11
Resource2=IDD_PLUGIN_DLG
Resource1=IDR_MAINFRAME
Class3=CMainDlg
Resource3=IDD_WEBROBOT_DIALOG
Class4=CPluginDlg
Resource4=IDR_MENU_BROWSER_LIST
Resource5=IDR_TRAYICON
Resource6=IDD_BROWSER_SETTING_DLG
Class5=CBrowserDlg
Resource7=IDD_BROWSER_DLG
Class6=CBrowserSettingDlg
Resource8=IDR_MENU_PLUGIN
Resource9=IDD_SETTING_DLG
Resource10=IDD_MAIN_DLG
Class7=CSettingDlg
Resource11=IDR_MENU_BROWSER_SETTING

[CLS:CWebRobotApp]
Type=0
HeaderFile=WebRobot.h
ImplementationFile=WebRobot.cpp
Filter=N

[CLS:CWebRobotDlg]
Type=0
HeaderFile=WebRobotDlg.h
ImplementationFile=WebRobotDlg.cpp
Filter=D
LastObject=CWebRobotDlg
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_WEBROBOT_DIALOG]
Type=1
Class=CWebRobotDlg
ControlCount=0

[DLG:IDD_MAIN_DLG]
Type=1
Class=CMainDlg
ControlCount=4
Control1=IDC_TREE,SysTreeView32,1342248999
Control2=IDC_SPLITTER,static,1073741831
Control3=IDC_LIST,SysListView32,1342242829
Control4=IDC_STATIC_TEXT,static,1342308352

[CLS:CMainDlg]
Type=0
HeaderFile=MainDlg.h
ImplementationFile=MainDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_LIST

[DLG:IDD_PLUGIN_DLG]
Type=1
Class=CPluginDlg
ControlCount=0

[CLS:CPluginDlg]
Type=0
HeaderFile=PluginDlg.h
ImplementationFile=PluginDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CPluginDlg

[MNU:IDR_MENU_PLUGIN]
Type=1
Class=CMainDlg
Command1=ID_MENUITEM_REFRESH_PLUGIN_LIST
Command2=ID_MENUITEM_AUTOLOAD
Command3=ID_MENUITEM_OPEN_PLUGIN_DIR
CommandCount=3

[MNU:IDR_TRAYICON]
Type=1
Class=CWebRobotDlg
Command1=ID_MENUITEM_SHOW
Command2=ID_APP_SUSPEND
CommandCount=2

[DLG:IDD_BROWSER_DLG]
Type=1
Class=CBrowserDlg
ControlCount=11
Control1=IDC_EXPLORER,{8856F961-340A-11D0-A96B-00C04FD705A2},1342242816
Control2=IDC_LIST,SysListView32,1342242825
Control3=IDC_COMBO_URL,combobox,1344339970
Control4=IDC_BUTTON_GO,button,1342242816
Control5=IDC_BUTTON_BACKWARD,button,1342242816
Control6=IDC_BUTTON_FORWARD,button,1342242816
Control7=IDC_BUTTON_REFRESH,button,1342242816
Control8=IDC_CHECK_URLS,button,1342242819
Control9=IDC_SPLITTER2,static,1073741831
Control10=IDC_BUTTON_SETTING,button,1342242816
Control11=IDC_LIST_SEARCH_KEYWORD,SysListView32,1342242825

[CLS:CBrowserDlg]
Type=0
HeaderFile=BrowserDlg.h
ImplementationFile=BrowserDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EXPLORER

[DLG:IDD_BROWSER_SETTING_DLG]
Type=1
Class=CBrowserSettingDlg
ControlCount=7
Control1=IDC_CHECK_QMARK,button,1342242819
Control2=IDC_CHECK_BEEP,button,1342242819
Control3=IDC_CHECK_VSCROLLTOEND,button,1342242819
Control4=IDC_LIST_URL_FILTER,SysListView32,1342242829
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_GOOGLE_MIRROR,edit,1342242944
Control7=IDC_BUTTON_SAVE,button,1342242816

[CLS:CBrowserSettingDlg]
Type=0
HeaderFile=BrowserSettingDlg.h
ImplementationFile=BrowserSettingDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_LIST_URL_FILTER

[MNU:IDR_MENU_BROWSER_LIST]
Type=1
Class=CBrowserDlg
Command1=ID_MENUITEM_COPYURL
Command2=ID_MENUITEM_OPENURL
Command3=ID_MENUITEM_SENDTO_INJECTION
Command4=ID_MENUITEM_SAVEURLS
Command5=ID_MENUITEM_DELETEURL
Command6=ID_MENUITEM_CLEARLIST
CommandCount=6

[MNU:IDR_MENU_BROWSER_SETTING]
Type=1
Class=CBrowserSettingDlg
Command1=ID_MENUITEM_INSERT
Command2=ID_MENUITEM_DELETE
CommandCount=2

[DLG:IDD_SETTING_DLG]
Type=1
Class=CSettingDlg
ControlCount=3
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO_USER_AGENT,combobox,1344340034
Control3=IDC_BUTTON_SAVE,button,1342242816

[CLS:CSettingDlg]
Type=0
HeaderFile=SettingDlg.h
ImplementationFile=SettingDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BUTTON_SAVE

