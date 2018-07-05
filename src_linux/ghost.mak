# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=ghost - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to ghost - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ghost - Win32 Release" && "$(CFG)" != "ghost - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "ghost.mak" CFG="ghost - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ghost - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ghost - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "ghost - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ghost - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\ghost.exe"

CLEAN : 
	-@erase ".\Release\ghost.exe"
	-@erase ".\Release\act_newc.obj"
	-@erase ".\Release\olc_act.obj"
	-@erase ".\Release\string.obj"
	-@erase ".\Release\fight.obj"
	-@erase ".\Release\new_dbx.obj"
	-@erase ".\Release\ban.obj"
	-@erase ".\Release\olc.obj"
	-@erase ".\Release\comm.obj"
	-@erase ".\Release\update.obj"
	-@erase ".\Release\act_move.obj"
	-@erase ".\Release\act_wiz.obj"
	-@erase ".\Release\skills.obj"
	-@erase ".\Release\remort.obj"
	-@erase ".\Release\healer.obj"
	-@erase ".\Release\db.obj"
	-@erase ".\Release\save.obj"
	-@erase ".\Release\tables.obj"
	-@erase ".\Release\bank.obj"
	-@erase ".\Release\const.obj"
	-@erase ".\Release\effects.obj"
	-@erase ".\Release\act_enter.obj"
	-@erase ".\Release\db2.obj"
	-@erase ".\Release\interp.obj"
	-@erase ".\Release\note.obj"
	-@erase ".\Release\act_info.obj"
	-@erase ".\Release\special.obj"
	-@erase ".\Release\bit.obj"
	-@erase ".\Release\clan.obj"
	-@erase ".\Release\NewMagic.obj"
	-@erase ".\Release\music.obj"
	-@erase ".\Release\magic.obj"
	-@erase ".\Release\magic2.obj"
	-@erase ".\Release\handler.obj"
	-@erase ".\Release\olc_mpcode.obj"
	-@erase ".\Release\scan.obj"
	-@erase ".\Release\lookup.obj"
	-@erase ".\Release\act_obj.obj"
	-@erase ".\Release\mob_prog.obj"
	-@erase ".\Release\wedding.obj"
	-@erase ".\Release\mob_cmds.obj"
	-@erase ".\Release\flags.obj"
	-@erase ".\Release\hedit.obj"
	-@erase ".\Release\olc_save.obj"
	-@erase ".\Release\alias.obj"
	-@erase ".\Release\mem.obj"
	-@erase ".\Release\recycle.obj"
	-@erase ".\Release\act_comm.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ghost.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ghost.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib wsock32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/ghost.pdb" /machine:I386 /out:"$(OUTDIR)/ghost.exe" 
LINK32_OBJS= \
	"$(INTDIR)/act_newc.obj" \
	"$(INTDIR)/olc_act.obj" \
	"$(INTDIR)/string.obj" \
	"$(INTDIR)/fight.obj" \
	"$(INTDIR)/new_dbx.obj" \
	"$(INTDIR)/ban.obj" \
	"$(INTDIR)/olc.obj" \
	"$(INTDIR)/comm.obj" \
	"$(INTDIR)/update.obj" \
	"$(INTDIR)/act_move.obj" \
	"$(INTDIR)/act_wiz.obj" \
	"$(INTDIR)/skills.obj" \
	"$(INTDIR)/remort.obj" \
	"$(INTDIR)/healer.obj" \
	"$(INTDIR)/db.obj" \
	"$(INTDIR)/save.obj" \
	"$(INTDIR)/tables.obj" \
	"$(INTDIR)/bank.obj" \
	"$(INTDIR)/const.obj" \
	"$(INTDIR)/effects.obj" \
	"$(INTDIR)/act_enter.obj" \
	"$(INTDIR)/db2.obj" \
	"$(INTDIR)/interp.obj" \
	"$(INTDIR)/note.obj" \
	"$(INTDIR)/act_info.obj" \
	"$(INTDIR)/special.obj" \
	"$(INTDIR)/bit.obj" \
	"$(INTDIR)/clan.obj" \
	"$(INTDIR)/NewMagic.obj" \
	"$(INTDIR)/music.obj" \
	"$(INTDIR)/magic.obj" \
	"$(INTDIR)/magic2.obj" \
	"$(INTDIR)/handler.obj" \
	"$(INTDIR)/olc_mpcode.obj" \
	"$(INTDIR)/scan.obj" \
	"$(INTDIR)/lookup.obj" \
	"$(INTDIR)/act_obj.obj" \
	"$(INTDIR)/mob_prog.obj" \
	"$(INTDIR)/wedding.obj" \
	"$(INTDIR)/mob_cmds.obj" \
	"$(INTDIR)/flags.obj" \
	"$(INTDIR)/hedit.obj" \
	"$(INTDIR)/olc_save.obj" \
	"$(INTDIR)/alias.obj" \
	"$(INTDIR)/mem.obj" \
	"$(INTDIR)/recycle.obj" \
	"$(INTDIR)/act_comm.obj"

"$(OUTDIR)\ghost.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\ghost.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\ghost.exe"
	-@erase ".\Debug\act_comm.obj"
	-@erase ".\Debug\act_newc.obj"
	-@erase ".\Debug\magic2.obj"
	-@erase ".\Debug\effects.obj"
	-@erase ".\Debug\skills.obj"
	-@erase ".\Debug\remort.obj"
	-@erase ".\Debug\healer.obj"
	-@erase ".\Debug\special.obj"
	-@erase ".\Debug\act_move.obj"
	-@erase ".\Debug\note.obj"
	-@erase ".\Debug\tables.obj"
	-@erase ".\Debug\olc_mpcode.obj"
	-@erase ".\Debug\bit.obj"
	-@erase ".\Debug\flags.obj"
	-@erase ".\Debug\comm.obj"
	-@erase ".\Debug\handler.obj"
	-@erase ".\Debug\string.obj"
	-@erase ".\Debug\act_obj.obj"
	-@erase ".\Debug\olc_save.obj"
	-@erase ".\Debug\const.obj"
	-@erase ".\Debug\wedding.obj"
	-@erase ".\Debug\save.obj"
	-@erase ".\Debug\bank.obj"
	-@erase ".\Debug\update.obj"
	-@erase ".\Debug\act_info.obj"
	-@erase ".\Debug\recycle.obj"
	-@erase ".\Debug\mem.obj"
	-@erase ".\Debug\olc_act.obj"
	-@erase ".\Debug\fight.obj"
	-@erase ".\Debug\NewMagic.obj"
	-@erase ".\Debug\lookup.obj"
	-@erase ".\Debug\music.obj"
	-@erase ".\Debug\magic.obj"
	-@erase ".\Debug\clan.obj"
	-@erase ".\Debug\act_enter.obj"
	-@erase ".\Debug\db.obj"
	-@erase ".\Debug\db2.obj"
	-@erase ".\Debug\new_dbx.obj"
	-@erase ".\Debug\mob_prog.obj"
	-@erase ".\Debug\mob_cmds.obj"
	-@erase ".\Debug\act_wiz.obj"
	-@erase ".\Debug\ban.obj"
	-@erase ".\Debug\olc.obj"
	-@erase ".\Debug\scan.obj"
	-@erase ".\Debug\hedit.obj"
	-@erase ".\Debug\interp.obj"
	-@erase ".\Debug\alias.obj"
	-@erase ".\Debug\ghost.ilk"
	-@erase ".\Debug\ghost.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ghost.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ghost.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib wsock32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/ghost.pdb" /debug /machine:I386 /out:"$(OUTDIR)/ghost.exe" 
LINK32_OBJS= \
	"$(INTDIR)/act_comm.obj" \
	"$(INTDIR)/act_newc.obj" \
	"$(INTDIR)/magic2.obj" \
	"$(INTDIR)/effects.obj" \
	"$(INTDIR)/skills.obj" \
	"$(INTDIR)/remort.obj" \
	"$(INTDIR)/healer.obj" \
	"$(INTDIR)/special.obj" \
	"$(INTDIR)/act_move.obj" \
	"$(INTDIR)/note.obj" \
	"$(INTDIR)/tables.obj" \
	"$(INTDIR)/olc_mpcode.obj" \
	"$(INTDIR)/bit.obj" \
	"$(INTDIR)/flags.obj" \
	"$(INTDIR)/comm.obj" \
	"$(INTDIR)/handler.obj" \
	"$(INTDIR)/string.obj" \
	"$(INTDIR)/act_obj.obj" \
	"$(INTDIR)/olc_save.obj" \
	"$(INTDIR)/const.obj" \
	"$(INTDIR)/wedding.obj" \
	"$(INTDIR)/save.obj" \
	"$(INTDIR)/bank.obj" \
	"$(INTDIR)/update.obj" \
	"$(INTDIR)/act_info.obj" \
	"$(INTDIR)/recycle.obj" \
	"$(INTDIR)/mem.obj" \
	"$(INTDIR)/olc_act.obj" \
	"$(INTDIR)/fight.obj" \
	"$(INTDIR)/NewMagic.obj" \
	"$(INTDIR)/lookup.obj" \
	"$(INTDIR)/music.obj" \
	"$(INTDIR)/magic.obj" \
	"$(INTDIR)/clan.obj" \
	"$(INTDIR)/act_enter.obj" \
	"$(INTDIR)/db.obj" \
	"$(INTDIR)/db2.obj" \
	"$(INTDIR)/new_dbx.obj" \
	"$(INTDIR)/mob_prog.obj" \
	"$(INTDIR)/mob_cmds.obj" \
	"$(INTDIR)/act_wiz.obj" \
	"$(INTDIR)/ban.obj" \
	"$(INTDIR)/olc.obj" \
	"$(INTDIR)/scan.obj" \
	"$(INTDIR)/hedit.obj" \
	"$(INTDIR)/interp.obj" \
	"$(INTDIR)/alias.obj"

"$(OUTDIR)\ghost.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "ghost - Win32 Release"
# Name "ghost - Win32 Debug"

!IF  "$(CFG)" == "ghost - Win32 Release"

!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\wedding.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_WEDDI=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	

"$(INTDIR)\wedding.obj" : $(SOURCE) $(DEP_CPP_WEDDI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_WEDDI=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\wedding.obj" : $(SOURCE) $(DEP_CPP_WEDDI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\act_enter.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_ACT_E=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	

"$(INTDIR)\act_enter.obj" : $(SOURCE) $(DEP_CPP_ACT_E) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_ACT_E=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\act_enter.obj" : $(SOURCE) $(DEP_CPP_ACT_E) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\act_info.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_ACT_I=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	".\recycle.h"\
	".\tables.h"\
	".\lookup.h"\
	

"$(INTDIR)\act_info.obj" : $(SOURCE) $(DEP_CPP_ACT_I) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_ACT_I=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	".\recycle.h"\
	".\tables.h"\
	".\lookup.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\act_info.obj" : $(SOURCE) $(DEP_CPP_ACT_I) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\act_move.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_ACT_M=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	

"$(INTDIR)\act_move.obj" : $(SOURCE) $(DEP_CPP_ACT_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_ACT_M=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\act_move.obj" : $(SOURCE) $(DEP_CPP_ACT_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\act_newc.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_ACT_N=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\recycle.h"\
	".\tables.h"\
	".\magic.h"\
	

"$(INTDIR)\act_newc.obj" : $(SOURCE) $(DEP_CPP_ACT_N) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_ACT_N=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\recycle.h"\
	".\tables.h"\
	".\magic.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\act_newc.obj" : $(SOURCE) $(DEP_CPP_ACT_N) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\act_obj.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_ACT_O=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	

"$(INTDIR)\act_obj.obj" : $(SOURCE) $(DEP_CPP_ACT_O) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_ACT_O=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\act_obj.obj" : $(SOURCE) $(DEP_CPP_ACT_O) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\act_wiz.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_ACT_W=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	".\recycle.h"\
	".\tables.h"\
	".\lookup.h"\
	".\magic.h"\
	

"$(INTDIR)\act_wiz.obj" : $(SOURCE) $(DEP_CPP_ACT_W) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_ACT_W=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	".\recycle.h"\
	".\tables.h"\
	".\lookup.h"\
	".\magic.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\act_wiz.obj" : $(SOURCE) $(DEP_CPP_ACT_W) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\alias.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_ALIAS=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	

"$(INTDIR)\alias.obj" : $(SOURCE) $(DEP_CPP_ALIAS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_ALIAS=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\alias.obj" : $(SOURCE) $(DEP_CPP_ALIAS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ban.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_BAN_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\recycle.h"\
	

"$(INTDIR)\ban.obj" : $(SOURCE) $(DEP_CPP_BAN_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_BAN_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\recycle.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\ban.obj" : $(SOURCE) $(DEP_CPP_BAN_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\bank.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_BANK_=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	

"$(INTDIR)\bank.obj" : $(SOURCE) $(DEP_CPP_BANK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_BANK_=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\bank.obj" : $(SOURCE) $(DEP_CPP_BANK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\bit.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_BIT_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\lookup.h"\
	

"$(INTDIR)\bit.obj" : $(SOURCE) $(DEP_CPP_BIT_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_BIT_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\lookup.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\bit.obj" : $(SOURCE) $(DEP_CPP_BIT_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\clan.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_CLAN_=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\recycle.h"\
	".\tables.h"\
	".\magic.h"\
	

"$(INTDIR)\clan.obj" : $(SOURCE) $(DEP_CPP_CLAN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_CLAN_=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\recycle.h"\
	".\tables.h"\
	".\magic.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\clan.obj" : $(SOURCE) $(DEP_CPP_CLAN_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\comm.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_COMM_=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Timeb.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\telnet.h"\
	".\merc.h"\
	".\interp.h"\
	".\recycle.h"\
	".\tables.h"\
	

"$(INTDIR)\comm.obj" : $(SOURCE) $(DEP_CPP_COMM_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_COMM_=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Timeb.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\telnet.h"\
	".\merc.h"\
	".\interp.h"\
	".\recycle.h"\
	".\tables.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\comm.obj" : $(SOURCE) $(DEP_CPP_COMM_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\const.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_CONST=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\magic.h"\
	".\interp.h"\
	

"$(INTDIR)\const.obj" : $(SOURCE) $(DEP_CPP_CONST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_CONST=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\magic.h"\
	".\interp.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\const.obj" : $(SOURCE) $(DEP_CPP_CONST) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\db.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_DB_C1c=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\db.h"\
	".\recycle.h"\
	".\music.h"\
	".\tables.h"\
	".\lookup.h"\
	".\olc.h"\
	

"$(INTDIR)\db.obj" : $(SOURCE) $(DEP_CPP_DB_C1c) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_DB_C1c=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\db.h"\
	".\recycle.h"\
	".\music.h"\
	".\tables.h"\
	".\lookup.h"\
	".\olc.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\db.obj" : $(SOURCE) $(DEP_CPP_DB_C1c) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\db2.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_DB2_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\db.h"\
	".\tables.h"\
	".\lookup.h"\
	

"$(INTDIR)\db2.obj" : $(SOURCE) $(DEP_CPP_DB2_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_DB2_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\db.h"\
	".\tables.h"\
	".\lookup.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\db2.obj" : $(SOURCE) $(DEP_CPP_DB2_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\effects.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_EFFEC=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\recycle.h"\
	

"$(INTDIR)\effects.obj" : $(SOURCE) $(DEP_CPP_EFFEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_EFFEC=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\recycle.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\effects.obj" : $(SOURCE) $(DEP_CPP_EFFEC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fight.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_FIGHT=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	

"$(INTDIR)\fight.obj" : $(SOURCE) $(DEP_CPP_FIGHT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_FIGHT=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\fight.obj" : $(SOURCE) $(DEP_CPP_FIGHT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\flags.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_FLAGS=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\tables.h"\
	

"$(INTDIR)\flags.obj" : $(SOURCE) $(DEP_CPP_FLAGS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_FLAGS=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\tables.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\flags.obj" : $(SOURCE) $(DEP_CPP_FLAGS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\handler.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_HANDL=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	".\recycle.h"\
	".\tables.h"\
	

"$(INTDIR)\handler.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_HANDL=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	".\recycle.h"\
	".\tables.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\handler.obj" : $(SOURCE) $(DEP_CPP_HANDL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\healer.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_HEALE=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\magic.h"\
	

"$(INTDIR)\healer.obj" : $(SOURCE) $(DEP_CPP_HEALE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_HEALE=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\magic.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\healer.obj" : $(SOURCE) $(DEP_CPP_HEALE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\hedit.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_HEDIT=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\tables.h"\
	".\olc.h"\
	".\lookup.h"\
	".\recycle.h"\
	

"$(INTDIR)\hedit.obj" : $(SOURCE) $(DEP_CPP_HEDIT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_HEDIT=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\tables.h"\
	".\olc.h"\
	".\lookup.h"\
	".\recycle.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\hedit.obj" : $(SOURCE) $(DEP_CPP_HEDIT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\interp.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_INTER=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	

"$(INTDIR)\interp.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_INTER=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\interp.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\lookup.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_LOOKU=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	

"$(INTDIR)\lookup.obj" : $(SOURCE) $(DEP_CPP_LOOKU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_LOOKU=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\lookup.obj" : $(SOURCE) $(DEP_CPP_LOOKU) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\magic.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_MAGIC=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	".\recycle.h"\
	

"$(INTDIR)\magic.obj" : $(SOURCE) $(DEP_CPP_MAGIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_MAGIC=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	".\recycle.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\magic.obj" : $(SOURCE) $(DEP_CPP_MAGIC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\magic2.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_MAGIC2=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	

"$(INTDIR)\magic2.obj" : $(SOURCE) $(DEP_CPP_MAGIC2) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_MAGIC2=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\magic2.obj" : $(SOURCE) $(DEP_CPP_MAGIC2) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mem.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_MEM_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	

"$(INTDIR)\mem.obj" : $(SOURCE) $(DEP_CPP_MEM_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_MEM_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\mem.obj" : $(SOURCE) $(DEP_CPP_MEM_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mob_cmds.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_MOB_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\mob_cmds.h"\
	

"$(INTDIR)\mob_cmds.obj" : $(SOURCE) $(DEP_CPP_MOB_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_MOB_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\mob_cmds.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\mob_cmds.obj" : $(SOURCE) $(DEP_CPP_MOB_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mob_prog.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_MOB_P=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\lookup.h"\
	

"$(INTDIR)\mob_prog.obj" : $(SOURCE) $(DEP_CPP_MOB_P) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_MOB_P=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\lookup.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\mob_prog.obj" : $(SOURCE) $(DEP_CPP_MOB_P) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\music.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_MUSIC=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\music.h"\
	".\recycle.h"\
	

"$(INTDIR)\music.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_MUSIC=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\music.h"\
	".\recycle.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\music.obj" : $(SOURCE) $(DEP_CPP_MUSIC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\new_dbx.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_NEW_D=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	

"$(INTDIR)\new_dbx.obj" : $(SOURCE) $(DEP_CPP_NEW_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_NEW_D=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\new_dbx.obj" : $(SOURCE) $(DEP_CPP_NEW_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NewMagic.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_NEWMA=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	".\recycle.h"\
	

"$(INTDIR)\NewMagic.obj" : $(SOURCE) $(DEP_CPP_NEWMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_NEWMA=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	".\recycle.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\NewMagic.obj" : $(SOURCE) $(DEP_CPP_NEWMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\note.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_NOTE_=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\recycle.h"\
	".\tables.h"\
	

"$(INTDIR)\note.obj" : $(SOURCE) $(DEP_CPP_NOTE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_NOTE_=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\recycle.h"\
	".\tables.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\note.obj" : $(SOURCE) $(DEP_CPP_NOTE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\olc.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_OLC_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\olc.h"\
	

"$(INTDIR)\olc.obj" : $(SOURCE) $(DEP_CPP_OLC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_OLC_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\olc.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\olc.obj" : $(SOURCE) $(DEP_CPP_OLC_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\olc_act.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_OLC_A=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\olc.h"\
	".\recycle.h"\
	".\lookup.h"\
	

"$(INTDIR)\olc_act.obj" : $(SOURCE) $(DEP_CPP_OLC_A) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_OLC_A=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\olc.h"\
	".\recycle.h"\
	".\lookup.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\olc_act.obj" : $(SOURCE) $(DEP_CPP_OLC_A) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\olc_mpcode.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_OLC_M=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\olc.h"\
	".\recycle.h"\
	

"$(INTDIR)\olc_mpcode.obj" : $(SOURCE) $(DEP_CPP_OLC_M) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_OLC_M=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\olc.h"\
	".\recycle.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\olc_mpcode.obj" : $(SOURCE) $(DEP_CPP_OLC_M) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\olc_save.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_OLC_S=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\olc.h"\
	

"$(INTDIR)\olc_save.obj" : $(SOURCE) $(DEP_CPP_OLC_S) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_OLC_S=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\olc.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\olc_save.obj" : $(SOURCE) $(DEP_CPP_OLC_S) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\recycle.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_RECYC=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\recycle.h"\
	

"$(INTDIR)\recycle.obj" : $(SOURCE) $(DEP_CPP_RECYC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_RECYC=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\recycle.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\recycle.obj" : $(SOURCE) $(DEP_CPP_RECYC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\remort.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_REMOR=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	

"$(INTDIR)\remort.obj" : $(SOURCE) $(DEP_CPP_REMOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_REMOR=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\remort.obj" : $(SOURCE) $(DEP_CPP_REMOR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\save.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_SAVE_=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\recycle.h"\
	".\tables.h"\
	".\lookup.h"\
	

"$(INTDIR)\save.obj" : $(SOURCE) $(DEP_CPP_SAVE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_SAVE_=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\recycle.h"\
	".\tables.h"\
	".\lookup.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\save.obj" : $(SOURCE) $(DEP_CPP_SAVE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\scan.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_SCAN_=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	

"$(INTDIR)\scan.obj" : $(SOURCE) $(DEP_CPP_SCAN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_SCAN_=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\scan.obj" : $(SOURCE) $(DEP_CPP_SCAN_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\skills.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_SKILL=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	".\recycle.h"\
	

"$(INTDIR)\skills.obj" : $(SOURCE) $(DEP_CPP_SKILL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_SKILL=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	".\recycle.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\skills.obj" : $(SOURCE) $(DEP_CPP_SKILL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\special.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_SPECI=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	

"$(INTDIR)\special.obj" : $(SOURCE) $(DEP_CPP_SPECI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_SPECI=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\magic.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\special.obj" : $(SOURCE) $(DEP_CPP_SPECI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\string.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_STRIN=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\olc.h"\
	

"$(INTDIR)\string.obj" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_STRIN=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\olc.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\string.obj" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tables.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_TABLE=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	

"$(INTDIR)\tables.obj" : $(SOURCE) $(DEP_CPP_TABLE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_TABLE=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\tables.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\tables.obj" : $(SOURCE) $(DEP_CPP_TABLE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\update.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_UPDAT=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\music.h"\
	

"$(INTDIR)\update.obj" : $(SOURCE) $(DEP_CPP_UPDAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_UPDAT=\
	{$(INCLUDE)}"\sys\Types.h"\
	".\merc.h"\
	".\interp.h"\
	".\music.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\update.obj" : $(SOURCE) $(DEP_CPP_UPDAT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\act_comm.c

!IF  "$(CFG)" == "ghost - Win32 Release"

DEP_CPP_ACT_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	".\recycle.h"\
	".\tables.h"\
	

"$(INTDIR)\act_comm.obj" : $(SOURCE) $(DEP_CPP_ACT_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

DEP_CPP_ACT_C=\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\sys\Time.h"\
	".\merc.h"\
	".\interp.h"\
	".\recycle.h"\
	".\tables.h"\
	".\vnum.h"\
	".\clan.h"\
	

"$(INTDIR)\act_comm.obj" : $(SOURCE) $(DEP_CPP_ACT_C) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
