animpp.1: main.c
	$(HELP2MAN) --output=$@ --no-info --name ' Displays a textual representation of an IFF/ANIM file on the standard output' --libtool ./animpp

AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = animpp
noinst_HEADERS = pp.h
man1_MANS = animpp.1

animpp_SOURCES = main.c pp.c
animpp_LDADD = ../libanim/libanim.la
animpp_CFLAGS = -I../libanim $(LIBIFF_CFLAGS)

EXTRA_DIST = animpp.1
