# The IHB tool is a utility to interface with Network of IHB modules
#
# Authors:
#	Copyright (C) 2020 Giuseppe Tipaldi <ing.giuseppe.tipaldi@gmail.com>

MAKEFLAGS += --no-print-directory

VERSION = 0.1.dev

DESTDIR ?=
PREFIX ?= /usr/local

CC ?= gcc

CFLAGS ?= -g -O2 -Wall -Wno-parentheses -Werror -Wvla

LIBS ?=

EXTRA_CFLAGS += \
		-Iinclude \
		-D IHB_VERSION="\"$(VERSION)\""

DEPS = ihb.h lib.h
OBJ = main.o ihblib.o lib.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(EXTRA_CFLAGS) $(LIBS)

ihbtool: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

all: ihbtool

.PHONY: clean

clean:
	rm -f $(OBJ) ihbtool

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f ihbtool $(DESTDIR)$(PREFIX)/bin
