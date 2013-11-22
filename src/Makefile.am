Utilitiesdir = $(datadir)/apps/Utilities

YACCFLAGS += -d -t
YFLAGS += -d -t
CFLAGS += @READLINE_SUPPORT@ @GNOME_SUPPORT@


INCLUDES =					\
	-DGNOMELOCALEDIR=\""$(datadir)/locale"\" 	\
	-DLOCALEDIR=\"$(datadir)/locale\"		\
	-I$(includedir)					\
	$(GMP_INCLUDEDIR)				\
	$(GNOME_INCLUDEDIR)				\
	$(GLIB_CFLAGS)

bin_PROGRAMS = @GNOME_GENIUS@ genius #gnome-genius-mdi

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
	mymath.c	\
	mymath.h	\
	mpwrap.c	\
	mpwrap.h	\
	matrix.c	\
	matrix.h	\
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
	mymath.c	\
	mymath.h	\
	mpwrap.c	\
	mpwrap.h	\
	matrix.c	\
	matrix.h	\
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

BUILT_SOURCES = parse.h

EXTRA_DIST = geniustest.pl geniustests.txt gnome-genius.desktop #gnome-genius-mdi.desktop

Utilities_DATA = gnome-genius.desktop #gnome-genius-mdi.desktop
