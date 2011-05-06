###############################################################################
##    Makefile-b32
##
##    This file is part of LgScript
##    Copyright (C) 2009 Tom N Harris <telliamed@whoopdedo.org>
##
##    This program is free software; you can redistribute it and/or modify
##    it under the terms of the GNU General Public License as published by
##    the Free Software Foundation; either version 2 of the License, or
##    (at your option) any later version.
##
##    This program is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##    GNU General Public License for more details.
##
##    You should have received a copy of the GNU General Public License
##    along with this program; if not, write to the Free Software
##    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
##
###############################################################################

.autodepend
.cacheautodepend

.suffixes:
.suffixes: .o .cpp .rc .res

BCCROOT = D:\BCC

IMPLIB = $(BCCROOT)\bin\Implib
ILINK32 = $(BCCROOT)\bin\ILink32
TLIB = $(BCCROOT)\bin\TLib
BRC32 = $(BCCROOT)\bin\Brc32
TASM32 = $(BCCROOT)\bin\Tasm32
CC = $(BCCROOT)\bin\BCC32

GAME = 2

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

DEFINES = -DWINVER=0x0400 -D_WIN32_WINNT=0x0400 -DWIN32_LEAN_AND_MEAN
GAME1 = -D_DARKGAME=1
GAME2 = -D_DARKGAME=2
GAME3 = -D_DARKGAME=3

!ifdef DEBUG
DEFINES = $(DEFINES) -DDEBUG=1
CXXDEBUG = -v -y -O0
LDDEBUG = -v
LGLIB = lg-d.lib
SCR1LIB = ScriptLib1-d.lib
SCR2LIB = ScriptLib2-d.lib
SCR3LIB = ScriptLib3-d.lib
!else
DEFINES = $(DEFINES) -DNDEBUG
CXXDEBUG = -O2 -5 -OS -v-
LDDEBUG = -v-
LGLIB = lg.lib
SCR1LIB = ScriptLib1.lib
SCR2LIB = ScriptLib2.lib
SCR3LIB = ScriptLib3.lib
!endif

LDFLAGS = -q -aa -Tpd -c -b:0x10000000
LIBDIRS = -L.;$(LGDIR);$(SCRLIBDIR);$(DH2DIR);$(BCCROOT)\lib;$(BCCROOT)\lib\psdk
LIBS = $(DH2LIB) $(LGLIB) uuid.lib
INCLUDES = -I. -I$(srcdir) -I$(LGDIR) -I$(SCRLIBDIR) -I$(DH2DIR)
CXXFLAGS =  -q -P -tWD -tWM -w-par -w-inl
RCFLAGS = -r

BCC32STARTUP = c0d32.obj
BCC32RTLIB = cw32mt.lib
BCCJUNK = *.il* *.csm *.tds *.map

OSM_OBJS = $(bindir)\ScriptModule.obj $(bindir)\Script.obj
BASE_OBJS = $(bindir)\MsgHandlerArray.obj
BASE1_OBJS = $(bin1dir)\BaseTrap.obj $(bin1dir)\BaseScript.obj $(bin1dir)\CommonScripts.obj
BASE2_OBJS = $(bin2dir)\BaseTrap.obj $(bin2dir)\BaseScript.obj $(bin2dir)\CommonScripts.obj
BASE3_OBJS = $(bin3dir)\BaseTrap.obj $(bin3dir)\BaseScript.obj $(bin3dir)\CommonScripts.obj
SCR1_OBJS = $(bin1dir)\PublicScripts.obj $(bin1dir)\T1Scripts.obj
SCR2_OBJS = $(bin2dir)\PublicScripts.obj $(bin2dir)\T2Scripts.obj
SCR3_OBJS = $(bin3dir)\PublicScripts.obj $(bin3dir)\SS2Scripts.obj
MISC1_OBJS = $(bin1dir)\ScriptDef.obj $(bin1dir)\utils.obj
MISC2_OBJS = $(bin2dir)\ScriptDef.obj $(bin2dir)\utils.obj
MISC3_OBJS = $(bin3dir)\ScriptDef.obj $(bin3dir)\utils.obj
RES1_OBJS = $(bin1dir)\script.res
RES2_OBJS = $(bin2dir)\script.res
RES3_OBJS = $(bin3dir)\script.res

ALL1_OBJS = $(SCR1_OBJS) $(BASE1_OBJS) $(BASE_OBJS) $(OSM_OBJS) $(MISC1_OBJS)
ALL2_OBJS = $(SCR2_OBJS) $(BASE2_OBJS) $(BASE_OBJS) $(OSM_OBJS) $(MISC2_OBJS)
ALL3_OBJS = $(SCR3_OBJS) $(BASE3_OBJS) $(BASE_OBJS) $(OSM_OBJS) $(MISC3_OBJS)

{$(srcdir)}.cpp{$(bindir)}.obj:
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) $(GAME2) $(INCLUDES) -n$(bindir) -c $<

{$(srcdir)}.cpp{$(bin1dir)}.obj:
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) $(GAME1) $(INCLUDES) -n$(bin1dir) -c $<

{$(srcdir)}.cpp{$(bin2dir)}.obj:
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) $(GAME2) $(INCLUDES) -n$(bin2dir) -c $<

{$(srcdir)}.cpp{$(bin3dir)}.obj:
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) $(GAME3) $(INCLUDES) -n$(bin3dir) -c $<

{$(srcdir)}.rc{$(bindir)}.res:
	$(BRC32) -I.;$(BCCROOT)\Include $(DEFINES) -r $<

{$(srcdir)}.rc{$(bin1dir)}.res:
	$(BRC32) -I.;$(BCCROOT)\Include $(DEFINES) $(GAME1) -fo$@ -r $<

{$(srcdir)}.rc{$(bin2dir)}.res:
	$(BRC32) -I.;$(BCCROOT)\Include $(DEFINES) $(GAME2) -fo$@ -r $<

{$(srcdir)}.rc{$(bin3dir)}.res:
	$(BRC32) -I.;$(BCCROOT)\Include $(DEFINES) $(GAME3) -fo$@ -r $<

.rc.res:
	$(BRC32) -I.;$(BCCROOT)\Include $(DEFINES) -r $<

.cpp.obj:
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) $(GAME2) $(INCLUDES) -c $<

.obj.osm:
	$(ILINK32) $(LDFLAGS) $(LIBDIRS) $(BCC32STARTUP) $(OSM_OBJS) $<, $@ ,,import32.lib $(SCR2LIB) $(LIBS) $(BCC32RTLIB) , script-b32.def ,

all: $(bindirectories) script-t1.osm script-t2.osm script-ss2.osm version.osm

clean:
	del /y /q $(bindir)\*.*
	del /y /q $(bin1dir)\*.*
	del /y /q $(bin2dir)\*.*
	del /y /q $(bin3dir)\*.*

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

script-t1.osm: $(ALL1_OBJS) $(RES1_OBJS)
	$(ILINK32) $(LDFLAGS) -b:0x11200000 $(LIBDIRS) $(BCC32STARTUP) $(ALL1_OBJS), $@ ,,import32.lib $(SCR1LIB) $(LIBS) $(BCC32RTLIB) , script-b32.def , $(RES1_OBJS)

script-t2.osm: $(ALL2_OBJS) $(RES2_OBJS)
	$(ILINK32) $(LDFLAGS) -b:0x11200000 $(LIBDIRS) $(BCC32STARTUP) $(ALL2_OBJS), $@ ,,import32.lib $(SCR2LIB) $(LIBS) $(BCC32RTLIB) , script-b32.def , $(RES2_OBJS)

script-ss2.osm: $(ALL3_OBJS) $(RES3_OBJS)
	$(ILINK32) $(LDFLAGS) -b:0x11200000 $(LIBDIRS) $(BCC32STARTUP) $(ALL3_OBJS), $@ ,,import32.lib $(SCR3LIB) $(LIBS) $(BCC32RTLIB) , script-b32.def , $(RES3_OBJS)

$(bindir)\scrversion.res: $(srcdir)\scrversion.rc
	$(BRC32) -fo$@ -r $(srcdir)\scrversion.rc

$(bindir)\scrversion.obj: $(srcdir)\scrversion.cpp $(srcdir)\scrversion.h
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(DEFINES) $(GAME3) $(INCLUDES) -n$(bindir) -c $(srcdir)\scrversion.cpp

version.osm: $(bindir)\scrversion.obj $(OSM_OBJS) $(bindir)\scrversion.res
	$(ILINK32) $(LDFLAGS) -b:0x12100000 $(LIBDIRS) $(BCC32STARTUP) $(bindir)\scrversion.obj $(OSM_OBJS), $@ ,,import32.lib $(SCR2LIB) $(LGLIB) uuid.lib version.lib $(BCC32RTLIB) , script-b32.def , $(bindir)\scrversion.res
