# Microsoft Developer Studio Project File - Name="WebRobot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WebRobot - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WebRobot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WebRobot.mak" CFG="WebRobot - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WebRobot - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WebRobot - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WebRobot - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"bin/WebRobot.exe"

!ELSEIF  "$(CFG)" == "WebRobot - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"bin/WebRobot.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "WebRobot - Win32 Release"
# Name "WebRobot - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BrowserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowserSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# Begin Source File

SOURCE=.\WebRobot.cpp
# End Source File
# Begin Source File

SOURCE=.\WebRobot.rc
# End Source File
# Begin Source File

SOURCE=.\WebRobotDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BrowserDlg.h
# End Source File
# Begin Source File

SOURCE=.\BrowserSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainDlg.h
# End Source File
# Begin Source File

SOURCE=.\PluginDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# Begin Source File

SOURCE=.\WebRobot.h
# End Source File
# Begin Source File

SOURCE=.\WebRobotDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\about.ico
# End Source File
# Begin Source File

SOURCE=.\res\back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boxMulti.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boxSingle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Browser.ico
# End Source File
# Begin Source File

SOURCE=.\res\check.bmp
# End Source File
# Begin Source File

SOURCE=.\res\forward.bmp
# End Source File
# Begin Source File

SOURCE=.\res\go.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ListMove.cur
# End Source File
# Begin Source File

SOURCE=.\res\main.ico
# End Source File
# Begin Source File

SOURCE=.\res\plugin.ico
# End Source File
# Begin Source File

SOURCE=.\res\PluginList.bmp
# End Source File
# Begin Source File

SOURCE=.\res\refresh.bmp
# End Source File
# Begin Source File

SOURCE=.\res\setting.bmp
# End Source File
# Begin Source File

SOURCE=.\res\setting.ico
# End Source File
# Begin Source File

SOURCE=.\res\SizeNS.cur
# End Source File
# Begin Source File

SOURCE=.\res\SizeWE.cur
# End Source File
# Begin Source File

SOURCE=.\res\uncheck.bmp
# End Source File
# Begin Source File

SOURCE=.\res\url.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WebRobot.ico
# End Source File
# Begin Source File

SOURCE=.\res\WebRobot.rc2
# End Source File
# End Group
# Begin Group "ExClass"

# PROP Default_Filter ""
# Begin Group "CoolTabCtrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExClass\CoolTabCtrl\CoolTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\CoolTabCtrl\CoolTabCtrl.h
# End Source File
# End Group
# Begin Group "SplitterControl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExClass\SplitterControl\SplitterControl.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\SplitterControl\SplitterControl.h
# End Source File
# End Group
# Begin Group "ListMoveItem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExClass\ListMoveItem\ListMoveItem.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\ListMoveItem\ListMoveItem.h
# End Source File
# End Group
# Begin Group "CRC32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExClass\CRC32\CRC32.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\CRC32\CRC32.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\CRC32\CRCFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\CRC32\CRCFile.h
# End Source File
# End Group
# Begin Group "XHTMLStatic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExClass\XHTMLStatic\XHTMLStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\XHTMLStatic\XHTMLStatic.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\XHTMLStatic\XNamedColors.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\XHTMLStatic\XNamedColors.h
# End Source File
# End Group
# Begin Group "CoolMenu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExClass\CoolMenu\NewMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\CoolMenu\NewMenu.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\CoolMenu\NewToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\CoolMenu\NewToolBar.h
# End Source File
# End Group
# Begin Group "IniFile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExClass\IniFile\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\IniFile\IniFile.h
# End Source File
# End Group
# Begin Group "TrayIcon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExClass\TrayIcon\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\TrayIcon\TrayIcon.h
# End Source File
# End Group
# Begin Group "WebHost"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExClass\WebHost\GetWeb.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\WebHost\GetWeb.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\WebHost\URLSturct_.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\WebHost\WebHost.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\WebHost\WebHost.h
# End Source File
# End Group
# Begin Group "CButtonST"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExClass\CButtonST\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\CButtonST\BtnST.h
# End Source File
# End Group
# Begin Group "Tools"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExClass\Tools\ButtonXP.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\Tools\ButtonXP.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\Tools\ComboBoxXP.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\Tools\Draw.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\Tools\Draw.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\Tools\MenuXP.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\Tools\MenuXP.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\Tools\StatusBarXP.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\Tools\StatusBarXP.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\Tools\ToolBarXP.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\Tools\ToolbarXP.h
# End Source File
# Begin Source File

SOURCE=.\ExClass\Tools\Tools.cpp
# End Source File
# End Group
# Begin Group "InputDlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExClass\InputDlg\InputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExClass\InputDlg\InputDlg.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\XPStyle.manifest
# End Source File
# End Target
# End Project
# Section WebRobot : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
# Section WebRobot : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
