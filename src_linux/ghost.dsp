# Microsoft Developer Studio Project File - Name="ghost" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ghost - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ghost.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ghost.mak" CFG="ghost - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ghost - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ghost - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ghost - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /debug /machine:I386

!ENDIF 

# Begin Target

# Name "ghost - Win32 Release"
# Name "ghost - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\act_comm.c
# End Source File
# Begin Source File

SOURCE=.\act_enter.c
# End Source File
# Begin Source File

SOURCE=.\act_info.c
# End Source File
# Begin Source File

SOURCE=.\act_move.c
# End Source File
# Begin Source File

SOURCE=.\act_newc.c
# End Source File
# Begin Source File

SOURCE=.\act_obj.c
# End Source File
# Begin Source File

SOURCE=.\act_wiz.c
# End Source File
# Begin Source File

SOURCE=.\alias.c
# End Source File
# Begin Source File

SOURCE=.\ban.c
# End Source File
# Begin Source File

SOURCE=.\bank.c
# End Source File
# Begin Source File

SOURCE=.\bit.c
# End Source File
# Begin Source File

SOURCE=.\clan.c
# End Source File
# Begin Source File

SOURCE=.\comm.c
# End Source File
# Begin Source File

SOURCE=.\const.c
# End Source File
# Begin Source File

SOURCE=.\db.c
# End Source File
# Begin Source File

SOURCE=.\db2.c
# End Source File
# Begin Source File

SOURCE=.\effects.c
# End Source File
# Begin Source File

SOURCE=.\fight.c
# End Source File
# Begin Source File

SOURCE=.\flags.c
# End Source File
# Begin Source File

SOURCE=.\handler.c
# End Source File
# Begin Source File

SOURCE=.\healer.c
# End Source File
# Begin Source File

SOURCE=.\hedit.c
# End Source File
# Begin Source File

SOURCE=.\interp.c
# End Source File
# Begin Source File

SOURCE=.\lookup.c
# End Source File
# Begin Source File

SOURCE=.\magic.c
# End Source File
# Begin Source File

SOURCE=.\magic2.c
# End Source File
# Begin Source File

SOURCE=.\mem.c
# End Source File
# Begin Source File

SOURCE=.\mob_cmds.c
# End Source File
# Begin Source File

SOURCE=.\mob_prog.c
# End Source File
# Begin Source File

SOURCE=.\music.c
# End Source File
# Begin Source File

SOURCE=.\new_dbx.c
# End Source File
# Begin Source File

SOURCE=.\NewMagic.c
# End Source File
# Begin Source File

SOURCE=.\note.c
# End Source File
# Begin Source File

SOURCE=.\olc.c
# End Source File
# Begin Source File

SOURCE=.\olc_act.c
# End Source File
# Begin Source File

SOURCE=.\olc_mpcode.c
# End Source File
# Begin Source File

SOURCE=.\olc_save.c
# End Source File
# Begin Source File

SOURCE=.\recycle.c
# End Source File
# Begin Source File

SOURCE=.\remort.c
# End Source File
# Begin Source File

SOURCE=.\save.c
# End Source File
# Begin Source File

SOURCE=.\scan.c
# End Source File
# Begin Source File

SOURCE=.\skills.c
# End Source File
# Begin Source File

SOURCE=.\special.c
# End Source File
# Begin Source File

SOURCE=.\string.c
# End Source File
# Begin Source File

SOURCE=.\tables.c
# End Source File
# Begin Source File

SOURCE=.\taka.c
# End Source File
# Begin Source File

SOURCE=.\update.c
# End Source File
# Begin Source File

SOURCE=.\web.c
# End Source File
# Begin Source File

SOURCE=.\wedding.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\clan.h
# End Source File
# Begin Source File

SOURCE=.\db.h
# End Source File
# Begin Source File

SOURCE=.\interp.h
# End Source File
# Begin Source File

SOURCE=.\lookup.h
# End Source File
# Begin Source File

SOURCE=.\magic.h
# End Source File
# Begin Source File

SOURCE=.\merc.h
# End Source File
# Begin Source File

SOURCE=.\mob_cmds.h
# End Source File
# Begin Source File

SOURCE=.\music.h
# End Source File
# Begin Source File

SOURCE=.\olc.h
# End Source File
# Begin Source File

SOURCE=.\recycle.h
# End Source File
# Begin Source File

SOURCE=.\tables.h
# End Source File
# Begin Source File

SOURCE=.\taka.h
# End Source File
# Begin Source File

SOURCE=.\telnet.h
# End Source File
# Begin Source File

SOURCE=.\vnum.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
