# Makefile -- build script for pick(1)
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Thu Apr  6 01:57:46 PM EEST 2023
# Copyright: (c) 2023 Luis Colorado.  All rights reserved.
# License: BSD

targets            = pick pick.1.gz
toclean            = $(targets)

prefix            ?= /usr/local
exec_prefix       ?= $(prefix)
bindir            ?= $(exec_prefix)/bin
datarootdir       ?= $(prefix)/share
mandir            ?= $(datarootdir)/man
man1dir           ?= $(mandir)/man1

XMOD              ?= -m 0755     # binary files
MMOD              ?= -m 0644     # manual page files
DMOD              ?= -m 0755     # directories modes

GROFF             ?= groff
RM	              ?= -rm -f
INSTALL           ?= install
IFLAGS            ?= -o root -g wheel
GROFF             ?= groff

pick_deps = 
pick_objs = pick.o
pick_ldlf =
pick_libs =
toclean += $(pick_objs)

all: $(targets)
clean:
	$(RM) $(toclean)
install: $(targets)
	$(INSTALL) $(IFLAGS) $(DMOD) -d $(bindir)
	$(INSTALL) $(IFLAGS) $(DMOD) -d $(man1dir)
	$(INSTALL) $(IFLAGS) $(XMOD) pick $(bindir)
	$(INSTALL) $(IFLAGS) $(XMOD) pick.1.gz $(man1dir)

pick: $(pick_deps) $(pick_objs)
	$(CC) -o $@ $(LDFLAGS) $($@_ldfl) $($@_objs) $($@_libs) $(LIBS)

pick.1.pdf: pick.1
	soelim $^ | $(GROFF) $(GROFFFLAGS) -Tpdf -mandoc >$@

pick.1.gz: pick.1
	gzip < $? > $@
