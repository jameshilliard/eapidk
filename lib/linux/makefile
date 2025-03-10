#<KHeader>
#+=========================================================================
#I  Project Name: EApiDK Embedded Application Development Kit
#+=========================================================================
#I  $HeadURL$
#+=========================================================================
#I   Copyright: Copyright (c) 2002-2009, Kontron Embedded Modules GmbH
#I      Author: John Kearney,                  John.Kearney@kontron.com
#I
#I     License: All rights reserved. This program and the accompanying 
#I              materials are licensed and made available under the 
#I              terms and conditions of the BSD License which 
#I              accompanies this distribution. The full text of the 
#I              license may be found at 
#I              http://opensource.org/licenses/bsd-license.php
#I              
#I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
#I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
#I              ANY KIND, EITHER EXPRESS OR IMPLIED.
#I
#I Description: Auto Created for makefile
#I
#+-------------------------------------------------------------------------
#I
#I  File Name            : makefile
#I  File Location        : lib\linux
#I  Last committed       : $Revision$
#I  Last changed by      : $Author$
#I  Last changed date    : $Date$
#I  ID                   : $Id$
#I
#+=========================================================================
#</KHeader>
ARCH = $(shell uname -s)
ifndef MACH
MACH = $(shell uname -m)
endif
ifeq ($(ARCH), HP-UX)
ARCH=HPUX
endif
# Note: for cross compilation
# This script does allow for CROSS Compilation
# to achieve this specify the following
# variables
# CFLAGS
# MACH
# INTERPRETER  (Not used for PE Format/Windows/Cygwin)
.SUFFIXES:
.SUFFIXES: .c .s .o .exe .H

# static library
AR = ar
RANLIB = ranlib
ARFLAGS = -ruv
InstallDir=/usr/lib

BinFolder=../../apps/bin/$(ARCH)/$(MACH)/rel
DBinFolder=../../apps/bin/$(ARCH)/$(MACH)/dbg
BuildDir=./build/$(MACH)/rel
DBuildDir=./build/$(MACH)/dbg
PrgFldr=$(join $(shell pwd)/, ..)
OalFldr=.
EAPI_INC=../../include/EApi.h
GET_VAR_FROM_CFILE=$(shell grep "$(2)" $(1) |sed -rn "s/^\#\s*define\s+$2\s+([0-9]+).*/\1/p")
EAPI_VERSION=$(call GET_VAR_FROM_CFILE,$(EAPI_INC),EAPI_VER)
EAPI_REVISION=$(call GET_VAR_FROM_CFILE,$(EAPI_INC),EAPI_REVISION)

LibName:=EApi
VPNPID:=PMG
LibFName:=$(LibName).$(EAPI_VERSION).$(EAPI_REVISION).o
SoName:=lib$(LibName).so.$(EAPI_VERSION)
SoFName:=lib$(LibName)$(VPNPID).so.$(EAPI_VERSION).$(EAPI_REVISION)
StaticLibFName:=$(LibName).a

#DBGOPT:=-gxcoff3
#DBGOPT:=-gxcoff+
#DBGOPT:=-gcoff3
#DBGOPT:=-gdwarf-2 -O0
#DBGOPT:=-g3
#DBGOPT:=-gstabs
#DBGOPT:=-gstabs+
DBGOPT:=-g
RELOPT:=-O3
INCLUDS=-I../../include/linux -I../ -I../include -I../../include
CFLAGS:=$(CFLAGS) -Wall -W -Werror -pedantic $(INCLUDS)
ifneq ($(OS), Windows_NT)
ifndef INTERPRETER
INTERPRETER:=$(shell readelf -l $(shell which ld)|grep "\[.*:.*\]"| sed -nr "s/\s*\[.*:\s+(.*)\]/\1/p")
endif
InterpFName:=interp.o Message.o
CFLAGS:=$(CFLAGS) -fvisibility=hidden -fPIC -Wl,-e,EApiMain -D EAPI_INTERPRETER=\"$(INTERPRETER)\"
else
InterpFName:=
endif

define FLD_BUILD_RULE
@mkdir -p $(@)
endef

define LIB_BUILD_RULE
$(LD) -o $(@) -r $^
endef

define STATIC_BUILD_RULE
$(AR) $(ARFLAGS) $(@) $^
$(RANLIB) $(@)
endef

define SHARED_BUILD_RULE
$(CC) $(CFLAGS) $(1) -v -shared -L. -o $(@) -Wl,-soname,$(SoName),-Map,$(basename $<).map  $^
objcopy --only-keep-debug  "$(@)" "$(@).debug" 
strip --strip-debug --strip-unneeded "$(@)"
objcopy --add-gnu-debuglink="$(@).debug" "$(@)"
endef

define C_BUILD_RULE
$(CC) $(CFLAGS) $(1) -x c -c -Wa,-a,-ad -o  $(@) $< > $(basename $(@)).lst
$(CC) $(CFLAGS) $(1) -x c -S -o  $(basename $(@)).s $<
endef

define CLEAN_BUILD_FOLDER
@rm -f $(1)/*.log
@rm -f $(1)/*.o
@rm -f $(1)/*.a
@rm -f $(1)/*
@rm -f $(1)/*.lst
@rm -f $(1)/*.map
@rm -f $(1)/*.exeendef
endef

define REMOVE_BIN_FILES
@rm -f $(1)
@rm -f $(basename $(1)).map
endef

OBJECTS=EApiEmulGPIO.o \
      EApiEmulI2C.o \
      EApiInitLib.o \
      EApiEmulBacklight.o \
      EApiEmulBoardInfo.o \
      EApiEmulStorage.o \
      EApiEmulWDT.o \
      EApiOsNIX.o \
      EApiAPI.o DbgChk.o

DefaultTgt: buildall

buildall: clean quick

quick:$(BinFolder) \
      $(BuildDir) \
      $(DBinFolder) \
      $(DBuildDir) \
      $(DBinFolder)/$(StaticLibFName) \
      $(BinFolder)/$(StaticLibFName) \
      $(DBinFolder)/$(SoFName) \
      $(BinFolder)/$(SoFName)
	@echo SYSTEM ARCHITECTURE : $(ARCH)
	@echo CPU ARCHITECTURE .. : $(MACH)
	@echo Library Name ...... : $(LibName)
	@echo Shared Lib Name ... : $(SoName)
	@echo Static Lib Filename : $(StaticLibFName)
	@echo Shared Lib Filename : $(SoFName)
	@echo Interpreter ....... : $(INTERPRETER)

  

$(BinFolder):
	$(FLD_BUILD_RULE)

$(BuildDir):
	$(FLD_BUILD_RULE)

$(DBinFolder):
	$(FLD_BUILD_RULE)

$(DBuildDir):
	$(FLD_BUILD_RULE)

$(DBuildDir)/$(LibFName): $(addprefix $(DBuildDir)/,$(OBJECTS)) 
	$(call LIB_BUILD_RULE,$(DBGOPT))

$(BuildDir)/$(LibFName): $(addprefix $(BuildDir)/,$(OBJECTS))
	$(call LIB_BUILD_RULE,$(RELOPT))

$(DBinFolder)/$(StaticLibFName): $(addprefix $(DBuildDir)/,$(LibFName)) 
	$(call STATIC_BUILD_RULE,$(DBGOPT))

$(BinFolder)/$(StaticLibFName): $(addprefix $(BuildDir)/,$(LibFName))
	$(call STATIC_BUILD_RULE,$(RELOPT))

$(DBinFolder)/$(SoFName): $(addprefix $(DBuildDir)/,$(LibFName) $(InterpFName)) 
	$(call SHARED_BUILD_RULE,$(DBGOPT))

$(BinFolder)/$(SoFName): $(addprefix $(BuildDir)/,$(LibFName) $(InterpFName))
	$(call SHARED_BUILD_RULE,$(RELOPT))

$(DBuildDir)/%.o:$(OalFldr)/%.c
	$(call C_BUILD_RULE,$(DBGOPT))

$(BuildDir)/%.o:$(OalFldr)/%.c
	$(call C_BUILD_RULE,$(RELOPT))

$(DBuildDir)/%.o:$(PrgFldr)/%.c
	$(call C_BUILD_RULE,$(DBGOPT))

$(BuildDir)/%.o:$(PrgFldr)/%.c
	$(call C_BUILD_RULE,$(RELOPT))

$(DBuildDir)/%.o:$(PrgFldr)/%.cpp
	$(call C_BUILD_RULE,$(DBGOPT))

$(BuildDir)/%.o:$(PrgFldr)/%.cpp
	$(call C_BUILD_RULE,$(RELOPT))

$(InstallDir)/$(SoFName): $(DBinFolder)/$(SoFName)
	cp --force $< $(@)
	chmod 755 $(@)
	ln --force --symbolic $(@) $(dir $(@))$(SoName)
	ldconfig -n $(dir $(@))$(SoName)

install: $(InstallDir)/$(SoFName)

.PHONY: clean
.PHONY: help

clean::
	$(call REMOVE_BIN_FILES,$(DBuildDir)/$(SoFName))
	$(call REMOVE_BIN_FILES,$(BuildDir)/$(SoFName))
	$(call CLEAN_BUILD_FOLDER,$(DBuildDir))
	$(call CLEAN_BUILD_FOLDER,$(BuildDir))


help::
	@echo ###############################################################################
	@echo #        Build Mak Files
	@echo ###############################################################################
	@echo # help
	@echo # clean
	@echo # quick
	@echo # buildall
	@echo #

