
NAME = dwtct
LIBNAME = lib$(NAME)
SRCDIR = src
TESTDIR = test
CC = gcc -std=c99
CXX = g++ -std=c++0x
RM = rm -f
INSTALL = install
LN = ln -s -f

#objdump -p /usr/local/lib/libdwtct.so | grep SONAME
VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_PATCH = 0

ifeq ($(OS),Windows_NT)
	LIBTARGET = $(LIBNAME).dll
	CFLAGS += -DWIN32
	PREFIX = C:/dwtct
	LIBDIR = $(PREFIX)/dll
	INCDIR = $(PREFIX)/inc
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		LIBTARGET = $(LIBNAME).so
		CFLAGS += -DLINUX
		SONAME = -Wl,-soname,$(LIBTARGET).$(VERSION_MAJOR)
		PREFIX = /usr/local
		LIBDIR = $(PREFIX)/lib
		INCDIR = $(PREFIX)/include
	endif
endif

# default is release mode
debug ?= 0
ifeq ($(debug), 0)
	CFLAGS += -O2 -DNDEBUG
else
	CFLAGS += -Og -g
	LDFLAGS += -g
endif

