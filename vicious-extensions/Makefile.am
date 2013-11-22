## Process this file with automake to produce makefile.in
SUBDIRS=.

INCLUDES = \
 	-I.					\
	-I..					\
	-DGNOMELOCALEDIR=\""$(datadir)/locale"\" \
	$(GUI_CFLAGS)

# Use these only really for checking for deprecated use, not all the time
#	-DG_DISABLE_DEPRECATED			\
#	-DGDK_DISABLE_DEPRECATED		\
#	-DGDK_PIXBUF_DISABLE_DEPRECATED		\
#	-DGTK_DISABLE_DEPRECATED		\
#	-DGNOME_DISABLE_DEPRECATED		\
#

noinst_LIBRARIES = libvicious.a libvicious-gnome.a libviciousui.a libviciousui-gnome.a libviciousui-minimal.a

noinst_PROGRAMS = test-ve-config

libvicious_a_SOURCES =	\
	ve-misc.c	\
	ve-misc.h	\
	ve-config.c	\
	ve-config.h	\
	ve-signal.c	\
	ve-signal.h	\
	ve-nongnome.c	\
	vicious.h

libvicious_gnome_a_SOURCES = \
	ve-misc.c	\
	ve-misc.h	\
	ve-config.c	\
	ve-config.h	\
	ve-signal.c	\
	ve-signal.h	\
	ve-gnome.c	\
	vicious.h

libviciousui_a_SOURCES = \
	$(libvicious_a_SOURCES) \
	glade-helper.c	\
	glade-helper.h	\
	ve-miscui.c	\
	ve-miscui.h	\
	viciousui.h

libviciousui_gnome_a_SOURCES = \
	$(libvicious_gnome_a_SOURCES) \
	glade-helper.c	\
	glade-helper.h	\
	ve-miscui.c	\
	ve-miscui.h	\
	viciousui.h

libviciousui_minimal_a_SOURCES = \
	$(libvicious_a_SOURCES) \
	ve-miscui.c	\
	ve-miscui.h	\
	viciousui.h

test_ve_config_SOURCES = \
	test-ve-config.c

test_ve_config_LDADD = \
	$(GUI_LIBS)				\
	$(INTLLIBS)				\
	-L$(top_builddir)/vicious-extensions	\
	-lvicious
