# Microsoft Developer Studio Project File - Name="Spider" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Spider - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Spider.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Spider.mak" CFG="Spider - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Spider - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Spider - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Spider - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../bin/plugin/Spider.dll"

!ELSEIF  "$(CFG)" == "Spider - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../bin/plugin/Spider.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Spider - Win32 Release"
# Name "Spider - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Spider.cpp
# End Source File
# Begin Source File

SOURCE=.\Spider.def
# End Source File
# Begin Source File

SOURCE=.\Spider.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\common\AutoLock.h
# End Source File
# Begin Source File

SOURCE=.\MainDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Spider.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DirNotOpen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DirOpened.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HasSubLink.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Link.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MailURL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main.ico
# End Source File
# Begin Source File

SOURCE=.\res\RootSite.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ScanTopList.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Show_List.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Show_Tree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Spider.rc2
# End Source File
# Begin Source File

SOURCE=.\res\TESTURL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\URLLink.bmp
# End Source File
# Begin Source File

SOURCE=.\res\UrlLinkFocus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\what_file.bmp
# End Source File
# End Group
# Begin Group "ExClass"

# PROP Default_Filter ""
# Begin Group "SplitterControl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\SplitterControl\SplitterControl.cpp
# End Source File
# Begin Source File

SOURCE=..\common\SplitterControl\SplitterControl.h
# End Source File
# End Group
# Begin Group "IniFile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\IniFile\IniFile.cpp
# End Source File
# End Group
# Begin Group "WebHost"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\WebHost\GetWeb.cpp
# End Source File
# Begin Source File

SOURCE=..\common\WebHost\WebHost.cpp
# End Source File
# End Group
# Begin Group "InputDialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\InputDlg\InputDlg.cpp
# End Source File
# End Group
# Begin Group "Tools"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\Tools\ButtonXP.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Tools\ComboBoxXP.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Tools\ComboBoxXP.h
# End Source File
# Begin Source File

SOURCE=..\common\Tools\Draw.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Tools\Draw.h
# End Source File
# Begin Source File

SOURCE=..\common\Tools\MenuXP.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Tools\MenuXP.h
# End Source File
# Begin Source File

SOURCE=..\common\Tools\StatusBarXP.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Tools\StatusBarXP.h
# End Source File
# Begin Source File

SOURCE=..\common\Tools\ToolBarXP.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Tools\ToolbarXP.h
# End Source File
# Begin Source File

SOURCE=..\common\Tools\Tools.cpp
# End Source File
# End Group
# Begin Group "ButtonST"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\CButtonST\BtnST.cpp
# End Source File
# End Group
# Begin Group "ListCtrlEx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\ListCtrlEx\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=..\common\ListCtrlEx\ListCtrlEx.h
# End Source File
# End Group
# Begin Group "CoolMenu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\CoolMenu\NewMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\common\CoolMenu\NewToolBar.cpp
# End Source File
# Begin Source File

SOURCE=..\common\CoolMenu\NewToolBar.h
# End Source File
# End Group
# Begin Group "GridListCtrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\GridListCtrl\GridListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\common\GridListCtrl\GridListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\common\GridListCtrl\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\common\GridListCtrl\InPlaceEdit.h
# End Source File
# End Group
# Begin Group "StdioFileEx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\StdioFileEx\StdioFileEx.cpp
# End Source File
# Begin Source File

SOURCE=..\common\StdioFileEx\StdioFileEx.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
