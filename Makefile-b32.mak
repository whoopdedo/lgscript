###############################################################################
##  Makefile-b32
##
##  This file is part of LgScript
##  Copyright (C) 2011 Tom N Harris <telliamed@whoopdedo.org>
##
##  This program is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation; either version 2 of the License, or
##  (at your option) any later version.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with this program. If not, see <http://www.gnu.org/licenses/>.
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

srcdir = .
bindir = .\bin

LGDIR = ..\lg
SCRLIBDIR = ..\ScriptLib
DH2DIR = ..\DH2
DH2LIB = dh2.lib

LUADIR = .\lua
LUAMOD = .\mod
LUAFLAGS = -q -P -tWD -tWM
LUADEF = -DLUA_WIN -DLUA_ANSI -DLGSCRIPT
LUADEF2 = -DLUA_WIN -DLUA_ANSI -DLGSCRIPT -DLUA_NOTERM
LUADEBUG =
LUAINC = -I$(srcdir) -I$(LUADIR)
LUA_SRCS = $(LUADIR)\lapi.c $(LUADIR)\lapi.h \
	$(LUADIR)\lcode.c $(LUADIR)\lcode.h \
	$(LUADIR)\ldebug.c $(LUADIR)\ldebug.h \
	$(LUADIR)\ldo.c $(LUADIR)\ldo.h \
	$(LUADIR)\ldump.c \
	$(LUADIR)\lfunc.c $(LUADIR)\lfunc.h \
	$(LUADIR)\lgc.c $(LUADIR)\lgc.h \
	$(LUADIR)\llex.c $(LUADIR)\llex.h \
	$(LUADIR)\lmem.c $(LUADIR)\lmem.h \
	$(LUADIR)\lobject.c $(LUADIR)\lobject.h \
	$(LUADIR)\lopcodes.c $(LUADIR)\lopcodes.h \
	$(LUADIR)\lparser.c $(LUADIR)\lparser.h \
	$(LUADIR)\lstate.c $(LUADIR)\lstate.h \
	$(LUADIR)\lstring.c $(LUADIR)\lstring.h \
	$(LUADIR)\ltable.c $(LUADIR)\ltable.h \
	$(LUADIR)\ltm.c $(LUADIR)\ltm.h \
	$(LUADIR)\lundump.c $(LUADIR)\lundump.h \
	$(LUADIR)\lvm.c $(LUADIR)\lvm.h \
	$(LUADIR)\lzio.c $(LUADIR)\lzio.h \
	$(LUADIR)\lmathlib.c \
	$(LUADIR)\ltablib.c \
	$(LUADIR)\lstrlib.c \
	$(LUADIR)\loadlib.c \
	$(LUADIR)\lauxlib.c $(LUADIR)\lauxlib.h \
	$(LUADIR)\lbaselib.c \
	$(LUADIR)\ldblib.c \
	$(LUADIR)\liolib.c \
	$(LUADIR)\loslib.c \
	$(LUADIR)\llimits.h \
	$(LUADIR)\lualib.h \
	$(LUADIR)\luaconf.h \
	$(LUADIR)\lua.h \
	$(LUAMOD)\ctype.c $(LUAMOD)\ctype.h \
	$(LUAMOD)\progdir.c \
	$(LUAMOD)\vec.c \
	$(LUAMOD)\final.c \
	$(LUAMOD)\llist.c \
	$(LUAMOD)\lext.c \
	$(LUAMOD)\bit32.c \
	$(LUAMOD)\modlib.c \
	$(LUAMOD)\modlib.h
LUA_OBJS = $(bindir)\lapi.obj \
	$(bindir)\lcode.obj \
	$(bindir)\ldebug.obj \
	$(bindir)\ldo.obj \
	$(bindir)\ldump.obj \
	$(bindir)\lfunc.obj \
	$(bindir)\lgc.obj \
	$(bindir)\llex.obj \
	$(bindir)\lmem.obj \
	$(bindir)\lobject.obj \
	$(bindir)\lopcodes.obj \
	$(bindir)\lparser.obj \
	$(bindir)\lstate.obj \
	$(bindir)\lstring.obj \
	$(bindir)\ltable.obj \
	$(bindir)\ltm.obj \
	$(bindir)\lundump.obj \
	$(bindir)\lvm.obj \
	$(bindir)\lzio.obj \
	$(bindir)\lmathlib.obj \
	$(bindir)\ltablib.obj \
	$(bindir)\lstrlib.obj \
	$(bindir)\loadlib.obj \
	$(bindir)\progdir.obj \
	$(bindir)\ctype.obj \
	$(bindir)\vec.obj \
	$(bindir)\final.obj \
	$(bindir)\bit32.obj \
	$(bindir)\lext.obj
LUA_OBJ1 = \
	$(bindir)\lauxlib1.obj \
	$(bindir)\lbaselib1.obj \
	$(bindir)\ldblib1.obj \
	$(bindir)\modlib1.obj
LUA_OBJ2 = \
	$(bindir)\liolib.obj \
	$(bindir)\loslib.obj \
	$(bindir)\lauxlib2.obj \
	$(bindir)\lbaselib2.obj \
	$(bindir)\ldblib2.obj \
	$(bindir)\modlib2.obj
LUAX = $(srcdir)\luax.h $(srcdir)\luax.hpp
LUAX_SRCS = $(srcdir)\luax.cpp $(srcdir)\luax.h $(srcdir)\luax.hpp
LUAX_OBJ = $(bindir)\luax.obj
LUAX_OBJ2 = $(bindir)\luax2.obj

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
LUADEBUG = -DLUA_USE_APICHECK -DLUA_DEBUG
!else
DEFINES = $(DEFINES) -DNDEBUG
CXXDEBUG = -O2 -5 -OS -v-
LDDEBUG = -v-
LGLIB = lg.lib
SCR1LIB = ScriptLib1.lib
SCR2LIB = ScriptLib2.lib
SCR3LIB = ScriptLib3.lib
!endif

LDFLAGS = -q -aa -Tpd
LIBDIRS = -L.;$(LGDIR);$(SCRLIBDIR);$(DH2DIR);$(BCCROOT)\lib;$(BCCROOT)\lib\psdk
LIBS = $(DH2LIB) $(LGLIB) uuid.lib
INCLUDES = -I$(srcdir) -I$(LUAINC) -I$(LGDIR) -I$(SCRLIBDIR) -I$(DH2DIR)
CXXFLAGS = -q -P -tWD -tWM -w-par -w-inl
RCFLAGS = -r

BCC32DLL = c0d32.obj
BCC32EXE = c0x32.obj
BCC32RTLIB = cw32mt.lib
BCCJUNK = *.il* *.csm *.tds *.map

LGS_SRCS = \
	$(srcdir)\LgScript.cpp $(srcdir)\LgScript.h \
	$(srcdir)\LgInterpreter.cpp $(srcdir)\LgInterpreter.h \
	$(srcdir)\LgMessage.cpp $(srcdir)\LgMessage.h \
	$(srcdir)\LgMultiParm.cpp $(srcdir)\LgMultiParm.h \
	$(srcdir)\LgStructData.cpp $(srcdir)\LgStructData.h \
	$(srcdir)\LgLinkset.cpp $(srcdir)\LgLinkset.h \
	$(srcdir)\LgServices.cpp $(srcdir)\LgServices.h \
	$(srcdir)\LgServices1.cpp \
	$(srcdir)\LgServices2.cpp \
	$(srcdir)\LgServices3.cpp \
	$(srcdir)\strtocolor.cpp $(srcdir)\utils.h \
	$(srcdir)\version.rc
OSM_SRCS = $(srcdir)\ScriptModule.cpp $(srcdir)\ScriptModule.h \
	$(srcdir)\Script.cpp $(srcdir)\Script.h $(srcdir)\Dll.cpp \
	$(srcdir)\script.rc
SHELL_SRCS = $(srcdir)\LgShell.cpp \
	$(srcdir)\shell.rc
LGS_OBJS = \
	$(bindir)\LgScript.obj \
	$(bindir)\LgInterpreter.obj \
	$(bindir)\LgMessage.obj \
	$(bindir)\LgMultiParm.obj \
	$(bindir)\LgStructData.obj \
	$(bindir)\LgLinkset.obj \
	$(bindir)\LgServices.obj \
	$(bindir)\LgServices1.obj \
	$(bindir)\LgServices2.obj \
	$(bindir)\LgServices3.obj \
	$(bindir)\strtocolor.obj
OSM_OBJS = $(bindir)\ScriptModule.obj $(bindir)\Script.obj $(bindir)\Dll.obj
SCRIPT_RES = $(bindir)\script.res
SHELL_OBJS = $(bindir)\LgShell.obj
SHELL_RES = $(bindir)\shell.res

{$(srcdir)}.cpp{$(bindir)}.obj:
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME2) $(INCLUDES) -n$(bindir) -c $<

{$(LUADIR)}.c{$(bindir)}.obj:
	$(CC) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) $(DEFINES) -n$(bindir) -c $<

{$(LUAMOD)}.c{$(bindir)}.obj:
	$(CC) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) $(DEFINES) -n$(bindir) -c $<

{$(srcdir)}.rc{$(bindir)}.res:
	$(BRC32) -I.;$(BCCROOT)\Include $(DEFINES) -r $<

.rc.res:
	$(BRC32) -I.;$(BCCROOT)\Include $(DEFINES) -r $<

.cpp.obj:
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME2) $(INCLUDES) -c $<


all: $(bindir) lgs.osm lgscript.exe

clean:
	del /y /q $(bindir)\*.*

$(bindir):
	mkdir $@

lgs.osm: $(LGS_OBJS) $(OSM_OBJS) $(LUAX_OBJ) $(LUA_OBJS) $(LUA_OBJ1) $(SCRIPT_RES)
	$(ILINK32) $(LDFLAGS) -b:0x11400000 $(LIBDIRS) $(BCC32DLL) \
	$(LGS_OBJS) $(OSM_OBJS) $(LUAX_OBJ) $(LUA_OBJS) $(LUA_OBJ1), $@ ,, \
	import32.lib $(LIBS) $(BCC32RTLIB) , script.def , $(SCRIPT_RES)

lgscript.exe: $(SHELL_OBJS) $(LUAX_OBJ) $(LUA_OBJS) $(LUA_OBJ2) $(SHELL_RES)
	$(ILINK32) $(LDFLAGS) $(LIBDIRS) $(BCC32EXE) \
	$(SHELL_OBJS) $(LUAX_OBJ) $(LUA_OBJS) $(LUA_OBJ2), $@ ,, \
	import32.lib $(LIBS) $(BCC32RTLIB) ,, $(SHELL_RES)

$(bindir)\LgShell.obj: $(srcdir)\LgShell.cpp
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) $(DEFINES) -fo$(bindir)\LgShell.obj -c $(srcdir)\LgShell.cpp

$(bindir)\luax2.obj: $(srcdir)\luax.cpp $(srcdir)\luax.hpp $(srcdir)\luax.h
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) $(DEFINES) -fo$(bindir)\luax2.obj -c $(srcdir)\luax.cpp

$(bindir)\shell.res: $(srcdir)\shell.rc $(srcdir)\version.rc
	$(BRC32) -I.;$(BCCROOT)\Include $(DEFINES) -DBUILD=1000 -fo$@ -r $(srcdir)\shell.rc

$(bindir)\script.res: $(srcdir)\script.rc $(srcdir)\version.rc
	$(BRC32) -I.;$(BCCROOT)\Include $(DEFINES) -DBUILD=1000 -fo$@ -r $(srcdir)\script.rc

$(bindir)\Dll.obj: $(srcdir)\Dll.cpp
$(bindir)\Script.obj: $(srcdir)\Script.cpp $(srcdir)\Script.h
$(bindir)\ScriptModule.obj: $(srcdir)\ScriptModule.cpp $(srcdir)\ScriptModule.h $(srcdir)\LgInterpreter.h $(LUAX)
$(bindir)\LgInterpreter.obj: $(srcdir)\LgInterpreter.cpp $(srcdir)\LgInterpreter.h $(srcdir)\ScriptModule.h $(LUAX)
$(bindir)\LgScript.obj: $(srcdir)\LgScript.cpp $(srcdir)\LgScript.h $(srcdir)\LgInterpreter.h $(srcdir)\ScriptModule.h $(srcdir)\Script.h $(srcdir)\LgMessage.h $(srcdir)\LgMultiParm.h $(LUAX)
$(bindir)\LgMultiParm.obj: $(srcdir)\LgMultiParm.cpp $(srcdir)\LgMultiParm.h $(LUAMOD)\modlib.h $(LUAX)
$(bindir)\LgStructData.obj: $(srcdir)\LgStructData.cpp $(srcdir)\LgStructData.h $(LUAMOD)\modlib.h $(LUAX)
$(bindir)\LgMessage.obj: $(srcdir)\LgMessage.cpp $(srcdir)\LgMessage.h $(srcdir)\LgMultiParm.h $(LUAMOD)\modlib.h $(LUAX)
$(bindir)\LgLinkset.obj: $(srcdir)\LgLinkset.cpp $(srcdir)\LgLinkset.h $(srcdir)\LgStructData.h $(LUAX)
$(bindir)\LgServices.obj: $(srcdir)\LgServices.cpp $(srcdir)\LgServices.h $(srcdir)\LgMultiParm.h $(srcdir)\LgLinkset.h $(LUAMOD)\modlib.h $(LUAX)

$(bindir)\LgServices1.obj: $(srcdir)\LgServices1.cpp $(srcdir)\LgServices.h $(srcdir)\LgMultiParm.h $(LUAMOD)\modlib.h $(LUAX)
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME1) $(INCLUDES) -fo$(bindir)\LgServices1.obj -c $(srcdir)\LgServices1.cpp

$(bindir)\LgServices2.obj: $(srcdir)\LgServices2.cpp $(srcdir)\LgServices.h $(srcdir)\LgMultiParm.h $(LUAMOD)\modlib.h $(LUAX)
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME2) $(INCLUDES) -fo$(bindir)\LgServices2.obj -c $(srcdir)\LgServices2.cpp

$(bindir)\LgServices3.obj: $(srcdir)\LgServices3.cpp $(srcdir)\LgServices.h $(srcdir)\LgMultiParm.h $(LUAMOD)\modlib.h $(LUAX)
	$(CC) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME3) $(INCLUDES) -fo$(bindir)\LgServices3.obj -c $(srcdir)\LgServices3.cpp

$(bindir)\luax.obj: $(srcdir)\luax.cpp $(srcdir)\luax.hpp $(srcdir)\luax.h
$(bindir)\strtocolor.obj: $(srcdir)\strtocolor.cpp $(srcdir)\utils.h

$(bindir)\lapi.obj: $(LUADIR)\lapi.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lapi.h $(LUADIR)\lobject.h \
	$(LUADIR)\llimits.h $(LUADIR)\ldebug.h $(LUADIR)\lstate.h $(LUADIR)\ltm.h \
	$(LUADIR)\lzio.h $(LUADIR)\lmem.h $(LUADIR)\ldo.h $(LUADIR)\lfunc.h \
	$(LUADIR)\lgc.h $(LUADIR)\lstring.h $(LUADIR)\ltable.h \
	$(LUADIR)\lundump.h $(LUADIR)\lvm.h
$(bindir)\lcode.obj: $(LUADIR)\lcode.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lcode.h $(LUADIR)\llex.h \
	$(LUADIR)\lobject.h $(LUADIR)\llimits.h $(LUADIR)\lzio.h $(LUADIR)\lmem.h \
	$(LUADIR)\lopcodes.h $(LUADIR)\lparser.h $(LUADIR)\ldebug.h $(LUADIR)\lstate.h \
	$(LUADIR)\ltm.h $(LUADIR)\ldo.h $(LUADIR)\lgc.h $(LUADIR)\ltable.h
$(bindir)\ldebug.obj: $(LUADIR)\ldebug.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lapi.h $(LUADIR)\lobject.h \
	$(LUADIR)\llimits.h $(LUADIR)\lcode.h $(LUADIR)\llex.h $(LUADIR)\lzio.h \
	$(LUADIR)\lmem.h $(LUADIR)\lopcodes.h $(LUADIR)\lparser.h $(LUADIR)\ldebug.h \
	$(LUADIR)\lstate.h $(LUADIR)\ltm.h $(LUADIR)\ldo.h $(LUADIR)\lfunc.h \
	$(LUADIR)\lstring.h $(LUADIR)\lgc.h $(LUADIR)\ltable.h $(LUADIR)\lvm.h
$(bindir)\ldo.obj: $(LUADIR)\ldo.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\ldebug.h $(LUADIR)\lstate.h \
	$(LUADIR)\lobject.h $(LUADIR)\llimits.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h \
	$(LUADIR)\lmem.h $(LUADIR)\ldo.h $(LUADIR)\lfunc.h $(LUADIR)\lgc.h \
	$(LUADIR)\lopcodes.h $(LUADIR)\lparser.h $(LUADIR)\lstring.h \
	$(LUADIR)\ltable.h $(LUADIR)\lundump.h $(LUADIR)\lvm.h
$(bindir)\ldump.obj: $(LUADIR)\ldump.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lobject.h $(LUADIR)\llimits.h \
	$(LUADIR)\lstate.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h $(LUADIR)\lmem.h \
	$(LUADIR)\lundump.h
$(bindir)\lfunc.obj: $(LUADIR)\lfunc.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lfunc.h $(LUADIR)\lobject.h \
	$(LUADIR)\llimits.h $(LUADIR)\lgc.h $(LUADIR)\lmem.h $(LUADIR)\lstate.h \
	$(LUADIR)\ltm.h $(LUADIR)\lzio.h
$(bindir)\lgc.obj: $(LUADIR)\lgc.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\ldebug.h $(LUADIR)\lstate.h \
	$(LUADIR)\lobject.h $(LUADIR)\llimits.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h \
	$(LUADIR)\lmem.h $(LUADIR)\ldo.h $(LUADIR)\lfunc.h $(LUADIR)\lgc.h \
	$(LUADIR)\lstring.h $(LUADIR)\ltable.h
$(bindir)\llex.obj: $(LUADIR)\llex.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\ldo.h $(LUADIR)\lobject.h \
	$(LUADIR)\llimits.h $(LUADIR)\lstate.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h \
	$(LUADIR)\lmem.h $(LUADIR)\llex.h $(LUADIR)\lparser.h $(LUADIR)\lstring.h \
	$(LUADIR)\lgc.h $(LUADIR)\ltable.h
$(bindir)\lmem.obj: $(LUADIR)\lmem.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\ldebug.h $(LUADIR)\lstate.h \
	$(LUADIR)\lobject.h $(LUADIR)\llimits.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h \
	$(LUADIR)\lmem.h $(LUADIR)\ldo.h
$(bindir)\lobject.obj: $(LUADIR)\lobject.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\ldo.h $(LUADIR)\lobject.h \
	$(LUADIR)\llimits.h $(LUADIR)\lstate.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h \
	$(LUADIR)\lmem.h $(LUADIR)\lstring.h $(LUADIR)\lgc.h $(LUADIR)\lvm.h
$(bindir)\lopcodes.obj: $(LUADIR)\lopcodes.c \
	$(LUADIR)\lopcodes.h $(LUADIR)\llimits.h $(LUADIR)\lua.h $(LUADIR)\luaconf.h
$(bindir)\lparser.obj: $(LUADIR)\lparser.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lcode.h $(LUADIR)\llex.h \
	$(LUADIR)\lobject.h $(LUADIR)\llimits.h $(LUADIR)\lzio.h $(LUADIR)\lmem.h \
	$(LUADIR)\lopcodes.h $(LUADIR)\lparser.h $(LUADIR)\ldebug.h $(LUADIR)\lstate.h \
	$(LUADIR)\ltm.h $(LUADIR)\ldo.h $(LUADIR)\lfunc.h $(LUADIR)\lstring.h \
	$(LUADIR)\lgc.h $(LUADIR)\ltable.h
$(bindir)\lstate.obj: $(LUADIR)\lstate.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\ldebug.h $(LUADIR)\lstate.h \
	$(LUADIR)\lobject.h $(LUADIR)\llimits.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h \
	$(LUADIR)\lmem.h $(LUADIR)\ldo.h $(LUADIR)\lfunc.h $(LUADIR)\lgc.h \
	$(LUADIR)\llex.h $(LUADIR)\lstring.h $(LUADIR)\ltable.h
$(bindir)\lstring.obj: $(LUADIR)\lstring.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lmem.h $(LUADIR)\llimits.h \
	$(LUADIR)\lobject.h $(LUADIR)\lstate.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h \
	$(LUADIR)\lstring.h $(LUADIR)\lgc.h
$(bindir)\ltable.obj: $(LUADIR)\ltable.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\ldebug.h $(LUADIR)\lstate.h \
	$(LUADIR)\lobject.h $(LUADIR)\llimits.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h \
	$(LUADIR)\lmem.h $(LUADIR)\ldo.h $(LUADIR)\lgc.h $(LUADIR)\ltable.h
$(bindir)\ltm.obj: $(LUADIR)\ltm.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lobject.h $(LUADIR)\llimits.h \
	$(LUADIR)\lstate.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h $(LUADIR)\lmem.h \
	$(LUADIR)\lstring.h $(LUADIR)\lgc.h $(LUADIR)\ltable.h
$(bindir)\lundump.obj: $(LUADIR)\lundump.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\ldebug.h $(LUADIR)\lstate.h \
	$(LUADIR)\lobject.h $(LUADIR)\llimits.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h \
	$(LUADIR)\lmem.h $(LUADIR)\ldo.h $(LUADIR)\lfunc.h $(LUADIR)\lstring.h \
	$(LUADIR)\lgc.h $(LUADIR)\lundump.h
$(bindir)\lvm.obj: $(LUADIR)\lvm.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\ldebug.h $(LUADIR)\lstate.h \
	$(LUADIR)\lobject.h $(LUADIR)\llimits.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h \
	$(LUADIR)\lmem.h $(LUADIR)\ldo.h $(LUADIR)\lfunc.h $(LUADIR)\lgc.h \
	$(LUADIR)\lopcodes.h $(LUADIR)\lstring.h $(LUADIR)\ltable.h $(LUADIR)\lvm.h
$(bindir)\lzio.obj: $(LUADIR)\lzio.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\llimits.h $(LUADIR)\lmem.h \
	$(LUADIR)\lstate.h $(LUADIR)\lobject.h $(LUADIR)\ltm.h $(LUADIR)\lzio.h
$(bindir)\liolib.obj: $(LUADIR)\liolib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
$(bindir)\lmathlib.obj: $(LUADIR)\lmathlib.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
$(bindir)\loslib.obj: $(LUADIR)\loslib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
$(bindir)\lstrlib.obj: $(LUADIR)\lstrlib.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
$(bindir)\ltablib.obj: $(LUADIR)\ltablib.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
$(bindir)\loadlib.obj: $(LUADIR)\loadlib.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
$(bindir)\linit.obj: $(LUADIR)\linit.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h

$(bindir)\ctype.obj: $(LUADIR)\ctype.c $(LUADIR)\ctype.h
$(bindir)\vec.obj: $(LUAMOD)\vec.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h
$(bindir)\final.obj: $(LUAMOD)\final.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h
$(bindir)\llist.obj: $(LUAMOD)\llist.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h
$(bindir)\bit32.obj: $(LUAMOD)\bit32.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h
$(bindir)\lext.obj: $(LUAMOD)\lext.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h

$(bindir)\lauxlib1.obj: $(LUADIR)\lauxlib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h
	$(CC) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) -fo$(bindir)\lauxlib1.obj -c $(LUADIR)\lauxlib.c

$(bindir)\lbaselib1.obj: $(LUADIR)\lbaselib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
	$(CC) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) $(DEFINES) -fo$(bindir)\lbaselib1.obj -c $(LUADIR)\lbaselib.c

$(bindir)\ldblib1.obj: $(LUADIR)\ldblib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
	$(CC) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) $(DEFINES) -fo$(bindir)\ldblib1.obj -c $(LUADIR)\ldblib.c

$(bindir)\modlib1.obj: $(LUAMOD)\modlib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h
	$(CC) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) $(DEFINES) -fo$(bindir)\modlib1.obj -c $(LUAMOD)\modlib.c

$(bindir)\lauxlib2.obj: $(LUADIR)\lauxlib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h
	$(CC) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) $(DEFINES) -fo$(bindir)\lauxlib2.obj -c $(LUADIR)\lauxlib.c

$(bindir)\lbaselib2.obj: $(LUADIR)\lbaselib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
	$(CC) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) $(DEFINES) -fo$(bindir)\lbaselib2.obj -c $(LUADIR)\lbaselib.c

$(bindir)\ldblib2.obj: $(LUADIR)\ldblib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
	$(CC) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) $(DEFINES) -fo$(bindir)\ldblib2.obj -c $(LUADIR)\ldblib.c

$(bindir)\modlib2.obj: $(LUAMOD)\modlib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h
	$(CC) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) $(DEFINES) -fo$(bindir)\modlib2.obj -c $(LUAMOD)\modlib.c
