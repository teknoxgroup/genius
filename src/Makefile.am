Utilitiesdir = $(datadir)/apps/Utilities
Librarydir = $(datadir)/genius


#YACCFLAGS += -d -t
YFLAGS += -d -t
CFLAGS += @READLINE_SUPPORT@ @GNOME_SUPPORT@ -DLIBRARY_DIR=\""$(datadir)/genius/"\"


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
	gnome-genius.h	\
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
	parse.h		\
	parse.y

gnome_genius_LDADD = \
	$(GNOME_LIBDIR) \
	$(GNOMEUI_LIBS) \
	@GMP_LIB@ \
	@LEXLIB@ \
	$(INTLLIBS) \
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
	lexer.l		\
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

EXTRA_DIST = geniustest.pl geniustests.txt gnome-genius.desktop lib.gel

Utilities_DATA = gnome-genius.desktop

Library_DATA = lib.gel lib.cgel

MOSTLYCLEANFILES = lib.cgel

lib.cgel: lib.gel genius
	$(top_builddir)/src/genius --compile lib.gel > lib.cgel

parse.h: parse.y
	$(YACC) $(YFLAGS) $<
	test -f y.tab.c && mv -f y.tab.c parse.c
	test -f y.tab.h && mv -f y.tab.h parse.h
