# Makefile -- build script for pick(1)
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Thu Apr  6 01:57:46 PM EEST 2023
# Copyright: (c) 2023 Luis Colorado.  All rights reserved.
# License: BSD

targets            = pick pick.1.gz
toclean            = $(targets)

OS                != uname -o

OWN-FreeBSD        = root
GRP-FreeBSD        = wheel

OWN-GNU/Linux      = bin
GRP-GNU/Linux      = bin

prefix            ?= /usr/local
exec_prefix       ?= $(prefix)
bindir            ?= $(exec_prefix)/bin
datarootdir       ?= $(prefix)/share
mandir            ?= $(datarootdir)/man
man1dir           ?= $(mandir)/man1

XMOD              ?= -m 0755
MMOD              ?= -m 0644
DMOD              ?= -m 0755
FMOD              ?= -m 0644

GROFF             ?= groff
RM	              ?= -rm -f
INSTALL           ?= install
IFLAGS            ?= -o $(OWN-$(OS)) -g $(GRP-$(OS))
GROFF             ?= groff

pick_deps = 
pick_objs = pick.o
pick_ldlf =
pick_libs =
toclean += $(pick_objs)

toinstall += $(bindir)/pick $(man1dir)/pick.1.gz

all: $(targets)
clean:
	$(RM) $(toclean)
install: $(toinstall)
deinstall uninstall:
	$(RM) $(toinstall)

$(bindir)/pick: pick $(bindir)
	$(INSTALL) $(IFLAGS) $(XMOD) pick $@

$(man1dir)/pick.1.gz: pick.1.gz $(man1dir)
	$(INSTALL) $(IFLAGS) $(FMOD) pick.1.gz $@

$(bindir) $(man1dir):
	$(INSTALL) $(IFLAGS) $(DMOD) -d $@

pick: $(pick_deps) $(pick_objs)
	$(CC) -o $@ $(LDFLAGS) $($@_ldfl) $($@_objs) $($@_libs) $(LIBS)

pick.1.pdf: pick.1
	soelim $^ | $(GROFF) $(GROFFFLAGS) -Tpdf -mandoc >$@

pick.1.gz: pick.1
	gzip < $? > $@
