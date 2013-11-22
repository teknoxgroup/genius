Genius Calculator
=================

Genius calculator is a general purpose calculator and math tool with
many features.

For a manual read doc/manual.txt

****************************************************************************

Requirements:
	- lex (tested under flex)
	- yacc (tested under bison -y)
	- gmp (relatively new one required, tested with 4.1)
	  - mpfr (optional but highly recommended)
                 (part of gmp, but must be enabled)
	- glib 2.x
And for the GNOME frontend you need:
	- gtk+ 2.x (2.4 or greater is recommended, but any 2.x should work)
	- gnome libs
	- gtksourceview (optional but recommended)
All except gmp seem to be pretty much standard or Linux systems, and even
on most other platforms.

If you want to compile without the GNOME frontend, try the
  --disable-gnome
argument to the ./configure script.  You will miss out on the GUI stuff
(which includes the plotting) but you can use all the rest nicely.

It's under GPL so read COPYING

Note: the gtkextra/ directory which includes the plotting widgetry is
copyright: Adrian E. Feiguin <feiguin@ifir.edu.ar> and is under LGPL.  When
GtkExtra is actually released and stable and all that it will become a
requirement rather then being included like this.

George <jirka@5z.com>
