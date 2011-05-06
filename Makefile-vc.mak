###############################################################################
##    Makefile-vc
##
##    This file is part of LgScript
##    Copyright (C) 2009 Tom N Harris <telliamed@whoopdedo.org>
##
##    Permission is hereby granted, free of charge, to any person obtaining
##    a copy of this software and associated documentation files (the
##    "Software"), to deal in the Software without restriction, including
##    without limitation the rights to use, copy, modify, merge, publish,
##    distribute, sublicense, and/or sell copies of the Software, and to
##    permit persons to whom the Software is furnished to do so.
##
##    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
##    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
##    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-
##    INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
##    BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
##    AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
##    IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
##    THE SOFTWARE.
##
###############################################################################

CPU=i386
APPVER=4.0
!include <win32.mak>

GAME = 2

srcdir = .

srcdir = .
bindir = .\commonobj
bin1dir = .\t1obj
bin2dir = .\t2obj
bin3dir = .\ss2obj
bindirectories = $(bindir) $(bin1dir) $(bin2dir) $(bin3dir)

LGDIR = ..\lg
SCRLIBDIR = ..\ScriptLib
DH2DIR = ..\DH2
DH2LIB = dh2.lib

DEFINES = -D_MT -DWINVER=0x0400 -D_WIN32_WINNT=0x0400 -DWIN32_LEAN_AND_MEAN
GAME1 = -D_DARKGAME=1
GAME2 = -D_DARKGAME=2
GAME3 = -D_DARKGAME=3

!ifdef DEBUG
DEFINES = $(DEFINES) -DDEBUG=1
CXXDEBUG = -MTd -Od
LDDEBUG = -DEBUG
LGLIB = $(LGDIR)\lg-d.lib
SCR1LIB = $(SCRLIBDIR)\ScriptLib1-d.lib
SCR2LIB = $(SCRLIBDIR)\ScriptLib2-d.lib
SCR3LIB = $(SCRLIBDIR)\ScriptLib3-d.lib
!else
DEFINES = $(DEFINES) -DNDEBUG
CXXDEBUG = -MT -O2 -G5
LDDEBUG = -RELEASE
LGLIB = $(LGDIR)\lg.lib
SCR1LIB = $(SCRLIBDIR)\ScriptLib1.lib
SCR2LIB = $(SCRLIBDIR)\ScriptLib2.lib
SCR3LIB = $(SCRLIBDIR)\ScriptLib3.lib
!endif

LDFLAGS = -nologo $(dlllflags) -base:0x10000000
LIBDIRS =
LIBS = $(LGLIB) uuid.lib $(baselibs)
INCLUDES = -I. -I$(srcdir) -I$(LGDIR) -I$(SCRLIBDIR) -I$(DH2DIR)
CXXFLAGS = $(cflags) -nologo -W3 -wd4800 -TP -EHsc

OSM_OBJS = $(bindir)\ScriptModule.obj $(bindir)\Script.obj
BASE_OBJS = $(bindir)\MsgHandlerArray.obj
BASE1_OBJS = $(bin1dir)\BaseTrap.obj $(bin1dir)\BaseScript.obj $(bin1dir)\CommonScripts.obj
BASE2_OBJS = $(bin2dir)\BaseTrap.obj $(bin2dir)\BaseScript.obj $(bin2dir)\CommonScripts.obj
BASE3_OBJS = $(bin3dir)\BaseTrap.obj $(bin3dir)\BaseScript.obj $(bin3dir)\CommonScripts.obj
SCR1_OBJS = $(bin1dir)\PublicScripts.obj $(bin1dir)\T1Scripts.obj
SCR2_OBJS = $(bin2dir)\PublicScripts.obj $(bin2dir)\T1Scripts.obj
SCR3_OBJS = $(bin3dir)\PublicScripts.obj $(bin3dir)\T1Scripts.obj
MISC1_OBJS = $(bin1dir)\ScriptDef.obj $(bin1dir)\utils.obj
MISC2_OBJS = $(bin2dir)\ScriptDef.obj $(bin2dir)\utils.obj
MISC3_OBJS = $(bin3dir)\ScriptDef.obj $(bin3dir)\utils.obj
RES1_OBJS = $(bin1dir)\script.res
RES2_OBJS = $(bin2dir)\script.res
RES3_OBJS = $(bin3dir)\script.res

ALL1_OBJS = $(SCR1_OBJS) $(BASE1_OBJS) $(BASE_OBJS) $(OSM_OBJS) $(MISC1_OBJS) $(RES1_OBJS)
ALL2_OBJS = $(SCR2_OBJS) $(BASE2_OBJS) $(BASE_OBJS) $(OSM_OBJS) $(MISC2_OBJS) $(RES2_OBJS)
ALL3_OBJS = $(SCR3_OBJS) $(BASE3_OBJS) $(BASE_OBJS) $(OSM_OBJS) $(MISC3_OBJS) $(RES3_OBJS)

{$(srcdir)}.cpp{$(bindir)}.obj:
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) $(GAME2) $(INCLUDES) -Fo$(bindir)\ -Fd$(bindir)\ -c $<

{$(srcdir)}.cpp{$(bin1dir)}.obj:
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) $(GAME1) $(INCLUDES) -Fo$(bin1dir)\ -Fd$(bin1dir)\ -c $<

{$(srcdir)}.cpp{$(bin2dir)}.obj:
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) $(GAME2) $(INCLUDES) -Fo$(bin2dir)\ -Fd$(bin2dir)\ -c $<

{$(srcdir)}.cpp{$(bin3dir)}.obj:
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) $(GAME3) $(INCLUDES) -Fo$(bin3dir)\ -Fd$(bin3dir)\ -c $<

{$(srcdir)}.rc{$(bindir)}.res:
	$(rc) $(RCFLAGS) -I. $(DEFINES) -r $<

{$(srcdir)}.rc{$(bin1dir)}.res:
	$(rc) $(RCFLAGS) -I. $(DEFINES) $(GAME1) -fo$@ -r $<

{$(srcdir)}.rc{$(bin2dir)}.res:
	$(rc) $(RCFLAGS) -I. $(DEFINES) $(GAME2) -fo$@ -r $<

{$(srcdir)}.rc{$(bin3dir)}.res:
	$(rc) $(RCFLAGS) -I. $(DEFINES) $(GAME3) -fo$@ -r $<

.rc.res:
	$(rc) $(RCFLAGS) -I. $(DEFINES) -r $<

.cpp.obj:
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) $(GAME2) $(INCLUDES) -c $<

.obj.osm:
	$(link) $(LDFLAGS) $(LIBDIRS) -out:$@ $(OSM_OBJS) $< $(SCR2LIB) $(DH2LIB) $(LIBS)

all: $(bindirectories) script-t1.osm script-t2.osm script-ss2.osm version.osm

clean:
	-del /y /q $(bindir)\*.*
	-del /y /q $(bin1dir)\*.*
	-del /y /q $(bin2dir)\*.*
	-del /y /q $(bin3dir)\*.*

$(bindir):
	mkdir $@
$(bin1dir):
	mkdir $@
$(bin2dir):
	mkdir $@
$(bin3dir):
	mkdir $@

$(bindir)\ScriptModule.obj: ScriptModule.cpp ScriptModule.h
$(bindir)\Script.obj: Script.cpp Script.h

$(bin1dir)\BaseScript.obj: BaseScript.cpp BaseScript.h Script.h ScriptModule.h MsgHandlerArray.h
$(bin2dir)\BaseScript.obj: BaseScript.cpp BaseScript.h Script.h ScriptModule.h MsgHandlerArray.h
$(bin3dir)\BaseScript.obj: BaseScript.cpp BaseScript.h Script.h ScriptModule.h MsgHandlerArray.h
$(bin1dir)\BaseTrap.obj: BaseTrap.cpp BaseTrap.h BaseScript.h Script.h
$(bin2dir)\BaseTrap.obj: BaseTrap.cpp BaseTrap.h BaseScript.h Script.h
$(bin3dir)\BaseTrap.obj: BaseTrap.cpp BaseTrap.h BaseScript.h Script.h

$(bin1dir)\CommonScripts.obj: CommonScripts.cpp CommonScripts.h
$(bin2dir)\CommonScripts.obj: CommonScripts.cpp CommonScripts.h
$(bin3dir)\CommonScripts.obj: CommonScripts.cpp CommonScripts.h

$(bin1dir)\PublicScripts.obj: PublicScripts.cpp PublicScripts.h CommonScripts.h BaseTrap.h BaseScript.h Script.h
$(bin2dir)\PublicScripts.obj: PublicScripts.cpp PublicScripts.h CommonScripts.h BaseTrap.h BaseScript.h Script.h
$(bin3dir)\PublicScripts.obj: PublicScripts.cpp PublicScripts.h CommonScripts.h BaseTrap.h BaseScript.h Script.h

$(bin1dir)\T1Scripts.obj: T1Scripts.cpp T1Scripts.h CommonScripts.h BaseTrap.h BaseScript.h Script.h
$(bin2dir)\T2Scripts.obj: T2Scripts.cpp T2Scripts.h CommonScripts.h BaseTrap.h BaseScript.h Script.h
$(bin3dir)\SS2Scripts.obj: SS2Scripts.cpp SS2Scripts.h CommonScripts.h BaseTrap.h BaseScript.h Script.h

$(bin1dir)\ScriptDef.obj: ScriptDef.cpp PublicScripts.h T1Scripts.h BaseTrap.h BaseScript.h ScriptModule.h genscripts.h
$(bin2dir)\ScriptDef.obj: ScriptDef.cpp PublicScripts.h T2Scripts.h BaseTrap.h BaseScript.h ScriptModule.h genscripts.h
$(bin3dir)\ScriptDef.obj: ScriptDef.cpp PublicScripts.h SS2Scripts.h BaseTrap.h BaseScript.h ScriptModule.h genscripts.h

$(bin1dir)\script.res: script.rc version.rc
$(bin2dir)\script.res: script.rc version.rc
$(bin3dir)\script.res: script.rc version.rc

script-t1.osm: $(ALL1_OBJS)
	$(link) $(LDFLAGS) -base:0x11200000 $(LIBDIRS) -out:$@ $(ALL1_OBJS) $(SCR1LIB) $(DH2LIB) $(LIBS)

script-t2.osm: $(ALL2_OBJS) $(RES2_OBJS)
	$(link) $(LDFLAGS) -base:0x11200000 $(LIBDIRS) -out:$@ $(ALL2_OBJS) $(SCR2LIB) $(DH2LIB) $(LIBS)

script-ss2.osm: $(ALL3_OBJS) $(RES3_OBJS)
	$(link) $(LDFLAGS) -base:0x11200000 $(LIBDIRS) -out:$@ $(ALL3_OBJS) $(SCR3LIB) $(DH2LIB) $(LIBS)

$(bindir)\scrversion.res: $(srcdir)\scrversion.rc
	$(rc) $(RCFLAGS) -fo$@ -r $(srcdir)\scrversion.rc

$(bindir)\scrversion.obj: $(srcdir)\scrversion.cpp $(srcdir)\scrversion.h
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) $(GAME3) $(INCLUDES) -Fo$(bindir)\ -Fd$(bindir)\ -c $(srcdir)\scrversion.cpp

version.osm: $(bindir)\scrversion.obj $(OSM_OBJS) $(bindir)\scrversion.res
	$(link) $(LDFLAGS) -base:0x12100000 $(LIBDIRS) -out:$@ $** $(LIBS) $(SCR2LIB) $(LIBS) version.lib

