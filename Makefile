###############################################################################
##    Makefile-gcc
##
##    This file is part of LgScript
##    Copyright (C) 2011 Tom N Harris <telliamed@whoopdedo.org>
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


.SUFFIXES:
.SUFFIXES: .o .cpp .rc
.PRECIOUS: %.o

srcdir = .
bindir = ./bin

LGDIR = ../lg
SCRLIBDIR = ../ScriptLib
DH2DIR = ../DH2
DH2LIB = -ldh2

LUADIR = ./lua
LUAMOD = ./mod
LUAFLAGS =  -W -Wall
LUADEF = -DLUA_WIN -DLUA_ANSI -DLGSCRIPT
LUADEF2 = -DLUA_WIN -DLUA_ANSI -DLGSCRIPT -DLUA_NOTERM
LUADEBUG =
LUAINC = -I$(srcdir) -I$(LUADIR)
LUA_SRCS = $(LUADIR)/lapi.c $(LUADIR)/lapi.h \
	$(LUADIR)/lcode.c $(LUADIR)/lcode.h \
	$(LUADIR)/ldebug.c $(LUADIR)/ldebug.h \
	$(LUADIR)/ldo.c $(LUADIR)/ldo.h \
	$(LUADIR)/ldump.c \
	$(LUADIR)/lfunc.c $(LUADIR)/lfunc.h \
	$(LUADIR)/lgc.c $(LUADIR)/lgc.h \
	$(LUADIR)/llex.c $(LUADIR)/llex.h \
	$(LUADIR)/lmem.c $(LUADIR)/lmem.h \
	$(LUADIR)/lobject.c $(LUADIR)/lobject.h \
	$(LUADIR)/lopcodes.c $(LUADIR)/lopcodes.h \
	$(LUADIR)/lparser.c $(LUADIR)/lparser.h \
	$(LUADIR)/lstate.c $(LUADIR)/lstate.h \
	$(LUADIR)/lstring.c $(LUADIR)/lstring.h \
	$(LUADIR)/ltable.c $(LUADIR)/ltable.h \
	$(LUADIR)/ltm.c $(LUADIR)/ltm.h \
	$(LUADIR)/lundump.c $(LUADIR)/lundump.h \
	$(LUADIR)/lvm.c $(LUADIR)/lvm.h \
	$(LUADIR)/lzio.c $(LUADIR)/lzio.h \
	$(LUADIR)/lmathlib.c \
	$(LUADIR)/ltablib.c \
	$(LUADIR)/lstrlib.c \
	$(LUADIR)/loadlib.c \
	$(LUADIR)/lauxlib.c $(LUADIR)/lauxlib.h \
	$(LUADIR)/lbaselib.c \
	$(LUADIR)/ldblib.c \
	$(LUADIR)/liolib.c \
	$(LUADIR)/loslib.c \
	$(LUADIR)/llimits.h \
	$(LUADIR)/lualib.h \
	$(LUADIR)/luaconf.h \
	$(LUADIR)/lua.h \
	$(LUAMOD)/vec.c \
	$(LUAMOD)/llist.c \
	$(LUAMOD)/lext.c \
	$(LUAMOD)/modlib.c \
	$(LUAMOD)/modlib.h
LUA_OBJS = $(bindir)/lapi.o \
	$(bindir)/lcode.o \
	$(bindir)/ldebug.o \
	$(bindir)/ldo.o \
	$(bindir)/ldump.o \
	$(bindir)/lfunc.o \
	$(bindir)/lgc.o \
	$(bindir)/llex.o \
	$(bindir)/lmem.o \
	$(bindir)/lobject.o \
	$(bindir)/lopcodes.o \
	$(bindir)/lparser.o \
	$(bindir)/lstate.o \
	$(bindir)/lstring.o \
	$(bindir)/ltable.o \
	$(bindir)/ltm.o \
	$(bindir)/lundump.o \
	$(bindir)/lvm.o \
	$(bindir)/lzio.o \
	$(bindir)/lmathlib.o \
	$(bindir)/ltablib.o \
	$(bindir)/lstrlib.o \
	$(bindir)/loadlib.o \
	$(bindir)/vec.o \
	$(bindir)/lext.o
LUA_OBJ1 = \
	$(bindir)/lauxlib1.o \
	$(bindir)/lbaselib1.o \
	$(bindir)/ldblib1.o \
	$(bindir)/modlib1.o
LUA_OBJ2 = \
	$(bindir)/liolib.o \
	$(bindir)/loslib.o \
	$(bindir)/lauxlib2.o \
	$(bindir)/lbaselib2.o \
	$(bindir)/ldblib2.o \
	$(bindir)/modlib2.o
LUAX = $(srcdir)/luax.h $(srcdir)/luax.hpp
LUAX_SRCS = $(srcdir)/luax.cpp $(srcdir)/luax.h $(srcdir)/luax.hpp
LUAX_OBJ = $(bindir)/luax.o
LUAX_OBJ2 = $(bindir)/luax2.o

CC = gcc
CXX = g++
AR = ar
LD = g++
DLLTOOL = dlltool
RC = windres

DEFINES = -DWINVER=0x0400 -D_WIN32_WINNT=0x0400 -DWIN32_LEAN_AND_MEAN
GAME1 = -D_DARKGAME=1
GAME2 = -D_DARKGAME=2
GAME3 = -D_DARKGAME=3

ifdef DEBUG
DEFINES := $(DEFINES) -DDEBUG
CXXDEBUG = -g -O0
LDDEBUG = -g
LGLIB = -llg-d
SCR1LIB = -lScript1-d
SCR2LIB = -lScript2-d
SCR3LIB = -lScript3-d
LUADEBUG = -DLUA_USE_APICHECK -DLUA_DEBUG
else
DEFINES := $(DEFINES) -DNDEBUG
CXXDEBUG = -O2
LDDEBUG =
LGLIB = -llg
SCR1LIB = -lScript1
SCR2LIB = -lScript2
SCR3LIB = -lScript3
endif

ARFLAGS = rc
LDFLAGS = -mwindows -mdll -Wl,--enable-auto-image-base
LIBDIRS = -L. -L$(LGDIR) -L$(SCRLIBDIR) -L$(DH2DIR)
LIBS = $(DH2LIB) $(LGLIB) -luuid
INCLUDES = -I$(srcdir) $(LUAINC) -I$(LGDIR) -I$(SCRLIBDIR) -I$(DH2DIR)
CXXFLAGS = -W -Wall -masm=intel -Wno-invalid-offsetof
DLLFLAGS = --add-underscore
DLLDEF = $(srcdir)/script.def

LGS_SRCS = \
	$(srcdir)/LgScript.cpp $(srcdir)/LgScript.h \
	$(srcdir)/LgInterpreter.cpp $(srcdir)/LgInterpreter.h \
	$(srcdir)/LgMessage.cpp $(srcdir)/LgMessage.h \
	$(srcdir)/LgMultiParm.cpp $(srcdir)/LgMultiParm.h \
	$(srcdir)/LgStructData.cpp $(srcdir)/LgStructData.h \
	$(srcdir)/LgLinkset.cpp $(srcdir)/LgLinkset.h \
	$(srcdir)/LgServices.cpp $(srcdir)/LgServices.h \
	$(srcdir)/LgServices1.cpp \
	$(srcdir)/LgServices2.cpp \
	$(srcdir)/LgServices3.cpp \
	$(srcdir)/strtocolor.cpp $(srcdir)/utils.h \
	$(srcdir)/version.rc
OSM_SRCS = $(srcdir)/ScriptModule.cpp $(srcdir)/ScriptModule.h \
	$(srcdir)/Script.cpp $(srcdir)/Script.h \
	$(srcdir)/script.rc
SHELL_SRCS = $(srcdir)/LgShell.cpp \
	$(srcdir)/shell.rc
LGS_OBJS = \
	$(bindir)/LgScript.o \
	$(bindir)/LgInterpreter.o \
	$(bindir)/LgMessage.o \
	$(bindir)/LgMultiParm.o \
	$(bindir)/LgStructData.o \
	$(bindir)/LgLinkset.o \
	$(bindir)/LgServices.o \
	$(bindir)/LgServices1.o \
	$(bindir)/LgServices2.o \
	$(bindir)/LgServices3.o \
	$(bindir)/strtocolor.o
OSM_OBJS = $(bindir)/ScriptModule.o $(bindir)/Script.o $(bindir)/exports.o
RES_OBJS = $(bindir)/script_res.o
SHELL_OBJS = $(bindir)/LgShell.o $(bindir)/shell_res.o

$(bindir)/%.o: $(srcdir)/%.cpp
	$(CXX) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME2) $(INCLUDES) -o $@ -c $<

$(bindir)/%_res.o: $(srcdir)/%.rc
	$(RC) $(DEFINES) -o $@ -i $<

$(bindir)/%.o: $(LUADIR)/%.c
	$(CXX) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) -o $@ -c $<

$(bindir)/%.o: $(LUAMOD)/%.c
	$(CXX) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) -o $@ -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME2) $(INCLUDES) -o $@ -c $<

%_res.o: %.rc
	$(RC) $(DEFINES) -o $@ -i $<

%.osm: %.o $(OSM_OBJS)
	$(LD) $(LDFLAGS) $(LDDEBUG) $(LIBDIRS) -o $@ $(DLL_DEF) $< $(OSM_OBJS) $(SCR2LIB) $(LIBS)

all: $(bindir) lgs.osm lgscript.exe

clean:
	$(RM) $(bindir)/*

$(bindir):
	mkdir -p $@

.INTERMEDIATE: $(bindir)/exports.o

buildno: $(LUA_SRCS) $(LGS_SRCS) $(OSM_SRCS) $(SHELL_SRCS)
	if test -f buildno; then \
		expr `cat buildno` + 1 >__buildno; \
		mv -f __buildno buildno; \
	else echo 1 >buildno; fi

$(bindir)/exports.o: $(bindir)/ScriptModule.o
	$(DLLTOOL) $(DLLFLAGS) --output-exp $@ $^

lgs.osm: $(LGS_OBJS) $(OSM_OBJS) $(RES_OBJS) $(LUAX_OBJ) $(LUA_OBJS) $(LUA_OBJ1)
	$(LD) $(LDFLAGS) -Wl,--image-base=0x11400000 $(LDDEBUG) $(LIBDIRS) -o $@ $(DLL_DEF) $^ $(LIBS)

lgscript.exe: $(SHELL_OBJS) $(LUAX_OBJ) $(LUA_OBJS) $(LUA_OBJ2)
	$(CXX) $(LDDEBUG) -o $@ $^

$(bindir)/LgShell.o: $(srcdir)/LgShell.cpp
	$(CXX) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) -o $@ -c $(srcdir)/LgShell.cpp

$(bindir)/luax2.o: $(srcdir)/luax.cpp $(srcdir)/luax.hpp $(srcdir)/luax.h
	$(CXX) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) -o $@ -c $(srcdir)/luax.cpp

$(bindir)/shell_res.o: $(srcdir)/shell.rc $(srcdir)/version.rc
	$(RC) $(DEFINES) -DBUILD=`cat buildno` -o $@ -i $<

$(bindir)/script_res.o: $(srcdir)/script.rc $(srcdir)/version.rc buildno
	$(RC) $(DEFINES) -DBUILD=`cat buildno` -o $@ -i $<

$(bindir)/Script.o: $(srcdir)/Script.cpp $(srcdir)/Script.h
$(bindir)/ScriptModule.o: $(srcdir)/ScriptModule.cpp $(srcdir)/ScriptModule.h $(srcdir)/LgInterpreter.h $(LUAX)
$(bindir)/LgInterpreter.o: $(srcdir)/LgInterpreter.cpp $(srcdir)/LgInterpreter.h $(srcdir)/ScriptModule.h $(LUAX)
$(bindir)/LgScript.o: $(srcdir)/LgScript.cpp $(srcdir)/LgScript.h $(srcdir)/LgInterpreter.h $(srcdir)/ScriptModule.h $(srcdir)/Script.h $(srcdir)/LgMessage.h $(srcdir)/LgMultiParm.h $(LUAX)
$(bindir)/LgMultiParm.o: $(srcdir)/LgMultiParm.cpp $(srcdir)/LgMultiParm.h $(LUAX)
$(bindir)/LgStructData.o: $(srcdir)/LgStructData.cpp $(srcdir)/LgStructData.h $(LUAX)
$(bindir)/LgMessage.o: $(srcdir)/LgMessage.cpp $(srcdir)/LgMessage.h $(srcdir)/LgMultiParm.h $(LUAX)
$(bindir)/LgLinkset.o: $(srcdir)/LgLinkset.cpp $(srcdir)/LgLinkset.h
$(bindir)/LgServices.o: $(srcdir)/LgServices.cpp $(srcdir)/LgServices.h $(srcdir)/LgMultiParm.h $(srcdir)/LgLinkset.h $(LUAX)

$(bindir)/LgServices1.o: $(srcdir)/LgServices1.cpp $(srcdir)/LgServices.h $(srcdir)/LgMultiParm.h $(LUAX)
	$(CXX) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME1) $(INCLUDES) -o $@ -c $(srcdir)/LgServices1.cpp

$(bindir)/LgServices2.o: $(srcdir)/LgServices2.cpp $(srcdir)/LgServices.h $(srcdir)/LgMultiParm.h $(LUAX)
	$(CXX) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME2) $(INCLUDES) -o $@ -c $(srcdir)/LgServices2.cpp

$(bindir)/LgServices3.o: $(srcdir)/LgServices3.cpp $(srcdir)/LgServices.h $(srcdir)/LgMultiParm.h $(LUAX)
	$(CXX) $(CXXFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(DEFINES) $(GAME3) $(INCLUDES) -o $@ -c $(srcdir)/LgServices3.cpp

$(bindir)/luax.o: $(srcdir)/luax.cpp $(srcdir)/luax.hpp $(srcdir)/luax.h

$(bindir)/lapi.o: $(LUADIR)/lapi.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lapi.h $(LUADIR)/lobject.h \
	$(LUADIR)/llimits.h $(LUADIR)/ldebug.h $(LUADIR)/lstate.h $(LUADIR)/ltm.h \
	$(LUADIR)/lzio.h $(LUADIR)/lmem.h $(LUADIR)/ldo.h $(LUADIR)/lfunc.h \
	$(LUADIR)/lgc.h $(LUADIR)/lstring.h $(LUADIR)/ltable.h \
	$(LUADIR)/lundump.h $(LUADIR)/lvm.h
$(bindir)/lcode.o: $(LUADIR)/lcode.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lcode.h $(LUADIR)/llex.h \
	$(LUADIR)/lobject.h $(LUADIR)/llimits.h $(LUADIR)/lzio.h $(LUADIR)/lmem.h \
	$(LUADIR)/lopcodes.h $(LUADIR)/lparser.h $(LUADIR)/ldebug.h $(LUADIR)/lstate.h \
	$(LUADIR)/ltm.h $(LUADIR)/ldo.h $(LUADIR)/lgc.h $(LUADIR)/ltable.h
$(bindir)/ldebug.o: $(LUADIR)/ldebug.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lapi.h $(LUADIR)/lobject.h \
	$(LUADIR)/llimits.h $(LUADIR)/lcode.h $(LUADIR)/llex.h $(LUADIR)/lzio.h \
	$(LUADIR)/lmem.h $(LUADIR)/lopcodes.h $(LUADIR)/lparser.h $(LUADIR)/ldebug.h \
	$(LUADIR)/lstate.h $(LUADIR)/ltm.h $(LUADIR)/ldo.h $(LUADIR)/lfunc.h \
	$(LUADIR)/lstring.h $(LUADIR)/lgc.h $(LUADIR)/ltable.h $(LUADIR)/lvm.h
$(bindir)/ldo.o: $(LUADIR)/ldo.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/ldebug.h $(LUADIR)/lstate.h \
	$(LUADIR)/lobject.h $(LUADIR)/llimits.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h \
	$(LUADIR)/lmem.h $(LUADIR)/ldo.h $(LUADIR)/lfunc.h $(LUADIR)/lgc.h \
	$(LUADIR)/lopcodes.h $(LUADIR)/lparser.h $(LUADIR)/lstring.h \
	$(LUADIR)/ltable.h $(LUADIR)/lundump.h $(LUADIR)/lvm.h
$(bindir)/ldump.o: $(LUADIR)/ldump.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lobject.h $(LUADIR)/llimits.h \
	$(LUADIR)/lstate.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h $(LUADIR)/lmem.h \
	$(LUADIR)/lundump.h
$(bindir)/lfunc.o: $(LUADIR)/lfunc.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lfunc.h $(LUADIR)/lobject.h \
	$(LUADIR)/llimits.h $(LUADIR)/lgc.h $(LUADIR)/lmem.h $(LUADIR)/lstate.h \
	$(LUADIR)/ltm.h $(LUADIR)/lzio.h
$(bindir)/lgc.o: $(LUADIR)/lgc.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/ldebug.h $(LUADIR)/lstate.h \
	$(LUADIR)/lobject.h $(LUADIR)/llimits.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h \
	$(LUADIR)/lmem.h $(LUADIR)/ldo.h $(LUADIR)/lfunc.h $(LUADIR)/lgc.h \
	$(LUADIR)/lstring.h $(LUADIR)/ltable.h
$(bindir)/llex.o: $(LUADIR)/llex.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/ldo.h $(LUADIR)/lobject.h \
	$(LUADIR)/llimits.h $(LUADIR)/lstate.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h \
	$(LUADIR)/lmem.h $(LUADIR)/llex.h $(LUADIR)/lparser.h $(LUADIR)/lstring.h \
	$(LUADIR)/lgc.h $(LUADIR)/ltable.h
$(bindir)/lmem.o: $(LUADIR)/lmem.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/ldebug.h $(LUADIR)/lstate.h \
	$(LUADIR)/lobject.h $(LUADIR)/llimits.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h \
	$(LUADIR)/lmem.h $(LUADIR)/ldo.h
$(bindir)/lobject.o: $(LUADIR)/lobject.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/ldo.h $(LUADIR)/lobject.h \
	$(LUADIR)/llimits.h $(LUADIR)/lstate.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h \
	$(LUADIR)/lmem.h $(LUADIR)/lstring.h $(LUADIR)/lgc.h $(LUADIR)/lvm.h
$(bindir)/lopcodes.o: $(LUADIR)/lopcodes.c \
	$(LUADIR)/lopcodes.h $(LUADIR)/llimits.h $(LUADIR)/lua.h $(LUADIR)/luaconf.h
$(bindir)/lparser.o: $(LUADIR)/lparser.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lcode.h $(LUADIR)/llex.h \
	$(LUADIR)/lobject.h $(LUADIR)/llimits.h $(LUADIR)/lzio.h $(LUADIR)/lmem.h \
	$(LUADIR)/lopcodes.h $(LUADIR)/lparser.h $(LUADIR)/ldebug.h $(LUADIR)/lstate.h \
	$(LUADIR)/ltm.h $(LUADIR)/ldo.h $(LUADIR)/lfunc.h $(LUADIR)/lstring.h \
	$(LUADIR)/lgc.h $(LUADIR)/ltable.h
$(bindir)/lstate.o: $(LUADIR)/lstate.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/ldebug.h $(LUADIR)/lstate.h \
	$(LUADIR)/lobject.h $(LUADIR)/llimits.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h \
	$(LUADIR)/lmem.h $(LUADIR)/ldo.h $(LUADIR)/lfunc.h $(LUADIR)/lgc.h \
	$(LUADIR)/llex.h $(LUADIR)/lstring.h $(LUADIR)/ltable.h
$(bindir)/lstring.o: $(LUADIR)/lstring.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lmem.h $(LUADIR)/llimits.h \
	$(LUADIR)/lobject.h $(LUADIR)/lstate.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h \
	$(LUADIR)/lstring.h $(LUADIR)/lgc.h
$(bindir)/ltable.o: $(LUADIR)/ltable.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/ldebug.h $(LUADIR)/lstate.h \
	$(LUADIR)/lobject.h $(LUADIR)/llimits.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h \
	$(LUADIR)/lmem.h $(LUADIR)/ldo.h $(LUADIR)/lgc.h $(LUADIR)/ltable.h
$(bindir)/ltm.o: $(LUADIR)/ltm.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lobject.h $(LUADIR)/llimits.h \
	$(LUADIR)/lstate.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h $(LUADIR)/lmem.h \
	$(LUADIR)/lstring.h $(LUADIR)/lgc.h $(LUADIR)/ltable.h
$(bindir)/lundump.o: $(LUADIR)/lundump.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/ldebug.h $(LUADIR)/lstate.h \
	$(LUADIR)/lobject.h $(LUADIR)/llimits.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h \
	$(LUADIR)/lmem.h $(LUADIR)/ldo.h $(LUADIR)/lfunc.h $(LUADIR)/lstring.h \
	$(LUADIR)/lgc.h $(LUADIR)/lundump.h
$(bindir)/lvm.o: $(LUADIR)/lvm.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/ldebug.h $(LUADIR)/lstate.h \
	$(LUADIR)/lobject.h $(LUADIR)/llimits.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h \
	$(LUADIR)/lmem.h $(LUADIR)/ldo.h $(LUADIR)/lfunc.h $(LUADIR)/lgc.h \
	$(LUADIR)/lopcodes.h $(LUADIR)/lstring.h $(LUADIR)/ltable.h $(LUADIR)/lvm.h
$(bindir)/lzio.o: $(LUADIR)/lzio.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/llimits.h $(LUADIR)/lmem.h \
	$(LUADIR)/lstate.h $(LUADIR)/lobject.h $(LUADIR)/ltm.h $(LUADIR)/lzio.h
$(bindir)/lctype.o: $(LUADIR)/lctype.c $(LUADIR)/lctype.h
$(bindir)/liolib.o: $(LUADIR)/liolib.c \
		$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h
$(bindir)/lmathlib.o: $(LUADIR)/lmathlib.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h
$(bindir)/loslib.o: $(LUADIR)/loslib.c \
		$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h
$(bindir)/lstrlib.o: $(LUADIR)/lstrlib.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h
$(bindir)/ltablib.o: $(LUADIR)/ltablib.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h
$(bindir)/loadlib.o: $(LUADIR)/loadlib.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h
$(bindir)/linit.o: $(LUADIR)/linit.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h

$(bindir)/vec.o: $(LUAMOD)/vec.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h
$(bindir)/llist.o: $(LUAMOD)/llist.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h
$(bindir)/lext.o: $(LUAMOD)/lext.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h

$(bindir)/lauxlib1.o: $(LUADIR)/lauxlib.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h
	$(CXX) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) -o $@ -c $(LUADIR)/lauxlib.c

$(bindir)/lbaselib1.o: $(LUADIR)/lbaselib.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h
	$(CXX) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) -o $@ -c $(LUADIR)/lbaselib.c

$(bindir)/ldblib1.o: $(LUADIR)/ldblib.c \
	$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h
	$(CXX) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) -o $@ -c $(LUADIR)/ldblib.c

$(bindir)/modlib1.o: $(LUAMOD)/modlib.c \
		$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h $(LUAMOD)/modlib.h
	$(CXX) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF2) $(LUAINC) -o $@ -c $(LUAMOD)/modlib.c

$(bindir)/lauxlib2.o: $(LUADIR)/lauxlib.c \
		$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h
	$(CXX) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) -o $@ -c $(LUADIR)/lauxlib.c

$(bindir)/lbaselib2.o: $(LUADIR)/lbaselib.c \
		$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h
	$(CXX) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) -o $@ -c $(LUADIR)/lbaselib.c

$(bindir)/ldblib2.o: $(LUADIR)/ldblib.c \
		$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h
	$(CXX) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) -o $@ -c $(LUADIR)/ldblib.c

$(bindir)/modlib2.o: $(LUAMOD)/modlib.c \
		$(LUADIR)/lua.h $(LUADIR)/luaconf.h $(LUADIR)/lauxlib.h $(LUADIR)/lualib.h $(LUAMOD)/modlib.h
	$(CXX) $(LUAFLAGS) $(CXXDEBUG) $(LUADEBUG) $(LUADEF) $(LUAINC) -o $@ -c $(LUAMOD)/modlib.c
