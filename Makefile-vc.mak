###############################################################################
##  Makefile-vc
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

CPU=i386
APPVER=4.0
!include <win32.mak>

srcdir = .
bindir = .\bin

LGDIR = ..\lg
SCRLIBDIR = ..\ScriptLib
DH2DIR = ..\DH2
DH2LIB = $(DH2DIR)\dh2.lib

LUADIR = .\lua
LUAMOD = .\mod
LUAFLAGS = $(cflags) -W3 -wd4800 -TP -EHsc
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
	$(LUAMOD)\vec.c \
	$(LUAMOD)\final.c \
	$(LUAMOD)\llist.c \
	$(LUAMOD)\lext.c \
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
	$(bindir)\ctype.obj \
	$(bindir)\vec.obj \
	$(bindir)\final.obj \
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

DEFINES = -D_CRT_SECURE_NO_WARNINGS
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
LUADEBUG = -DLUA_USE_APICHECK -DLUA_DEBUG
!else
DEFINES = $(DEFINES) -DNDEBUG
CXXDEBUG = -MT -O2
LDDEBUG = -RELEASE
LGLIB = $(LGDIR)\lg.lib
SCR1LIB = $(SCRLIBDIR)\ScriptLib1.lib
SCR2LIB = $(SCRLIBDIR)\ScriptLib2.lib
SCR3LIB = $(SCRLIBDIR)\ScriptLib3.lib
!endif

LDFLAGS = -nologo
LIBDIRS =
LIBS = $(DH2LIB) $(LGLIB) uuid.lib $(baselibs)
INCLUDES = -I$(srcdir) -I$(LUAINC) -I$(LGDIR) -I$(SCRLIBDIR) -I$(DH2DIR)
CXXFLAGS = $(cflags) -nologo -W3 -wd4800 -wd4244 -TP -EHsc

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
RES_OBJS = $(bindir)\script.res
SHELL_OBJS = $(bindir)\LgShell.obj $(bindir)\shell.res

{$(srcdir)}.cpp{$(bindir)}.obj:
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME2) $(INCLUDES) -Fo$(bindir)\ -c $<

{$(LUADIR)}.c{$(bindir)}.obj:
	$(cc) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) $(DEFINES) -Fo$(bindir)\ -c $<

{$(LUAMOD)}.c{$(bindir)}.obj:
	$(cc) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) $(DEFINES) -Fo$(bindir)\ -c $<

{$(srcdir)}.rc{$(bindir)}.res:
	$(rc) $(RCFLAGS) -I. -fo$@ -r $<

.rc.res:
	$(rc) $(RCFLAGS) -I. -r $<

.cpp.obj:
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME2) $(INCLUDES) -c $<


all: $(bindir) lgs.osm lgscript.exe

clean:
	-del /y /q $(bindir)\*.*

$(bindir):
	mkdir $@

lgs.osm: $(LGS_OBJS) $(OSM_OBJS) $(RES_OBJS) $(LUAX_OBJ) $(LUA_OBJS) $(LUA_OBJ1)
	$(link) $(LDFLAGS) $(dlllflags) $(LDDEBUG) -base:0x11400000 $(LIBDIRS) -out:$@ \
	$(LGS_OBJS) $(OSM_OBJS) $(RES_OBJS) $(LUAX_OBJ) $(LUA_OBJS) $(LUA_OBJ1) \
	$(LIBS)

lgscript.exe: $(SHELL_OBJS) $(LUAX_OBJ) $(LUA_OBJS) $(LUA_OBJ2)
	$(link) $(LDFLAGS) $(exelflags) $(LDDEBUG) -out:$@ $(SHELL_OBJS) $(LUAX_OBJ) $(LUA_OBJS) $(LUA_OBJ2) $(baselibs)

$(bindir)\LgShell.obj: $(srcdir)\LgShell.cpp
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) $(DEFINES) -Fo$(bindir)\LgShell.obj -c $(srcdir)\LgShell.cpp

$(bindir)\luax2.obj: $(srcdir)\luax.cpp $(srcdir)\luax.hpp $(srcdir)\luax.h
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) $(DEFINES) -Fo$(bindir)\luax2.obj -c $(srcdir)\luax.cpp

$(bindir)\shell.res: $(srcdir)\shell.rc $(srcdir)\version.rc
	$(rc) $(RCFLAGS) -DBUILD=1000 -fo$@ -r $(srcdir)\shell.rc

$(bindir)\script.res: $(srcdir)\script.rc $(srcdir)\version.rc
	$(rc) $(RCFLAGS) -DBUILD=1000 -fo$@ -r $(srcdir)\script.rc

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
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME1) $(INCLUDES) -Fo$(bindir)\LgServices1.obj -c $(srcdir)\LgServices1.cpp

$(bindir)\LgServices2.obj: $(srcdir)\LgServices2.cpp $(srcdir)\LgServices.h $(srcdir)\LgMultiParm.h $(LUAMOD)\modlib.h $(LUAX)
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME2) $(INCLUDES) -Fo$(bindir)\LgServices2.obj -c $(srcdir)\LgServices2.cpp

$(bindir)\LgServices3.obj: $(srcdir)\LgServices3.cpp $(srcdir)\LgServices.h $(srcdir)\LgMultiParm.h $(LUAMOD)\modlib.h $(LUAX)
	$(cc) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME3) $(INCLUDES) -Fo$(bindir)\LgServices3.obj -c $(srcdir)\LgServices3.cpp

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

$(bindir)\ctype.obj: $(LUAMOD)\ctype.c $(LUAMOD)\ctype.h
$(bindir)\vec.obj: $(LUAMOD)\vec.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h
$(bindir)\final.obj: $(LUAMOD)\final.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h
$(bindir)\llist.obj: $(LUAMOD)\llist.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h
$(bindir)\lext.obj: $(LUAMOD)\lext.c \
	$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h

$(bindir)\lauxlib1.obj: $(LUADIR)\lauxlib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h
	$(cc) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) -Fo$(bindir)\lauxlib1.obj -c $(LUADIR)\lauxlib.c

$(bindir)\lbaselib1.obj: $(LUADIR)\lbaselib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
	$(cc) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) $(DEFINES) -Fo$(bindir)\lbaselib1.obj -c $(LUADIR)\lbaselib.c

$(bindir)\ldblib1.obj: $(LUADIR)\ldblib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
	$(cc) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) $(DEFINES) -Fo$(bindir)\ldblib1.obj -c $(LUADIR)\ldblib.c

$(bindir)\modlib1.obj: $(LUAMOD)\modlib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h
	$(cc) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) $(DEFINES) -Fo$(bindir)\modlib1.obj -c $(LUAMOD)\modlib.c

$(bindir)\lauxlib2.obj: $(LUADIR)\lauxlib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h
	$(cc) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) $(DEFINES) -Fo$(bindir)\lauxlib2.obj -c $(LUADIR)\lauxlib.c

$(bindir)\lbaselib2.obj: $(LUADIR)\lbaselib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
	$(cc) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) $(DEFINES) -Fo$(bindir)\lbaselib2.obj -c $(LUADIR)\lbaselib.c

$(bindir)\ldblib2.obj: $(LUADIR)\ldblib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h
	$(cc) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) $(DEFINES) -Fo$(bindir)\ldblib2.obj -c $(LUADIR)\ldblib.c

$(bindir)\modlib2.obj: $(LUAMOD)\modlib.c \
		$(LUADIR)\lua.h $(LUADIR)\luaconf.h $(LUADIR)\lauxlib.h $(LUADIR)\lualib.h $(LUAMOD)\modlib.h
	$(cc) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) $(DEFINES) -Fo$(bindir)\modlib2.obj -c $(LUAMOD)\modlib.c
