Utilitiesdir = $(datadir)/apps/Utilities
Librarydir = $(datadir)/genius/gel
Plugindescdir = $(datadir)/genius/plugins


#YACCFLAGS += -d -t
YFLAGS += -d -t
CFLAGS += @GNOME_SUPPORT@ -DLIBRARY_DIR=\""$(datadir)/genius/"\"

INCLUDES = \
	-DGNOMELOCALEDIR=\""$(datadir)/locale"\" \
	-DLOCALEDIR=\"$(datadir)/locale\" \
	-I$(includedir) \
	$(GMP_INCLUDEDIR) \
	$(GNOME_INCLUDEDIR)

bin_PROGRAMS = @GNOME_GENIUS@ genius

EXTRA_PROGRAMS = gnome-genius

gnome_genius_SOURCES = 	\
	calc.c		\
	calc.h		\
	eval.c		\
	eval.h		\
	gnome-genius.c	\
	util.c		\
	util.h		\
	dict.c		\
	dict.h		\
	funclib.c	\
	funclib.h	\
	mpwrap.c	\
	mpwrap.h	\
	matrix.c	\
	matrix.h	\
	matrixw.c	\
	matrixw.h	\
	matop.c		\
	matop.h		\
	compil.c	\
	compil.h	\
	structs.h	\
	lexer.l		\
	parseutil.h	\
	parseutil.c	\
	inter.c		\
	inter.h		\
	plugread.c	\
	plugread.h	\
	parse.y

gnome_genius_LDADD = \
	$(GNOME_LIBDIR) \
	$(GNOMEUI_LIBS) \
	@GMP_LIB@ \
	@LEXLIB@ \
	$(INTLLIBS) \
	@ZVT_LIBS@ \
	@READLINE_LIB@

genius_SOURCES = 	\
	calc.c		\
	calc.h		\
	eval.c		\
	eval.h		\
	genius.c	\
	util.c		\
	util.h		\
	dict.c		\
	dict.h		\
	funclib.c	\
	funclib.h	\
	mpwrap.c	\
	mpwrap.h	\
	matrix.c	\
	matrix.h	\
	matrixw.c	\
	matrixw.h	\
	matop.c		\
	matop.h		\
	compil.c	\
	compil.h	\
	structs.h	\
	parseutil.h	\
	parseutil.c	\
	inter.c		\
	inter.h		\
	lexer.l		\
	plugread_nongnome.c	\
	plugread.h	\
	parse.y

genius_LDADD = \
	-lm \
	$(GLIB_LIBS) \
	@GMP_LIB@ \
	@LEXLIB@ \
	$(INTLLIBS) \
	@READLINE_LIB@

BUILT_SOURCES = parse.h parse.c lexer.c

CLEANFILES += $(BUILT_SOURCES)

EXTRA_DIST = geniustest.pl geniustests.txt gnome-genius.desktop lib.gel test.plugin

Utilities_DATA = gnome-genius.desktop

Library_DATA = lib.gel lib.cgel

Plugindesc_DATA = test.plugin

MOSTLYCLEANFILES = lib.cgel

lib.cgel: lib.gel genius
	$(top_builddir)/src/genius --compile lib.gel > lib.cgel

parse.h: parse.y
	$(YACC) $(YFLAGS) $<
	test -f y.tab.c && mv -f y.tab.c parse.c
	test -f y.tab.h && mv -f y.tab.h parse.h
