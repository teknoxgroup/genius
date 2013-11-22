dnl
dnl GNOME_INIT_HOOK (script-if-gnome-enabled, failflag)
dnl
dnl if failflag is "fail" then GNOME_INIT_HOOK will abort if gnomeConf.sh
dnl is not found. 
dnl

AC_DEFUN([GNOME_INIT_HOOK],
[	
	AC_SUBST(GNOME_LIBS)
	AC_SUBST(GNOMEUI_LIBS)
	AC_SUBST(GTKXMHTML_LIBS)
	AC_SUBST(GNOME_LIBDIR)
	AC_SUBST(GNOME_INCLUDEDIR)

	AC_PATH_PROG(GNOME_CONFIG,gnome-config,no)
	if test "$GNOME_CONFIG" = "no"; then
	  no_gnome_config="yes"
	else
	  AC_MSG_CHECKING(if $GNOME_CONFIG works)
	  if $GNOME_CONFIG --libs-only-l gnome >/dev/null 2>&1; then
	    AC_MSG_RESULT(yes)
	    GNOME_LIBS="`$GNOME_CONFIG --libs-only-l gnome`"
	    GNOMEUI_LIBS="`$GNOME_CONFIG --libs-only-l gnomeui`"
	    GTKXMHTML_LIBS="`$GNOME_CONFIG --libs-only-l gtkxmhtml`"
	    GNOME_LIBDIR="`$GNOME_CONFIG --libs-only-L gnomeui`"
	    GNOME_INCLUDEDIR="`$GNOME_CONFIG --cflags gnomeui`"
            $1
	  else
	    AC_MSG_RESULT(no)
	    no_gnome_config="yes"
          fi
        fi

	if test x$exec_prefix = xNONE; then
	    if test x$prefix = xNONE; then
		gnome_prefix=$ac_default_prefix/lib
	    else
 		gnome_prefix=$prefix/lib
	    fi
	else
	    gnome_prefix=`eval echo \`echo $libdir\``
	fi
	
	AC_ARG_WITH(gnome-includes,
	[  --with-gnome-includes   Specify location of GNOME headers],[
	CFLAGS="$CFLAGS -I$withval"
	])
	
	AC_ARG_WITH(gnome-libs,
	[  --with-gnome-libs       Specify location of GNOME libs],[
	LDFLAGS="$LDFLAGS -L$withval"
	gnome_prefix=$withval
	])

	AC_ARG_WITH(gnome,
	[  --with-gnome            Specify prefix for GNOME files],[
	if test x$withval = xyes; then
	    dnl Note that an empty true branch is not valid sh syntax.
	    ifelse([$1], [], :, [$1])
        else
	    LDFLAGS="$LDFLAGS -L$withval/lib"
	    CFLAGS="$CFLAGS -I$withval/include"
	    gnome_prefix=$withval/lib
  	fi
	])

	if test "$no_gnome_config" = "yes"; then
          AC_MSG_CHECKING(for gnomeConf.sh file in $gnome_prefix)
	  if test -f $gnome_prefix/gnomeConf.sh; then
	    AC_MSG_RESULT(found)
	    echo "loading gnome configuration from $gnome_prefix/gnomeConf.sh"
	    . $gnome_prefix/gnomeConf.sh
	    $1
	  else
	    AC_MSG_RESULT(not found)
 	    if test x$2 = xfail; then
	      AC_MSG_ERROR(Could not find the gnomeConf.sh file that is generated by gnome-libs install)
 	    fi
	  fi
        fi
])

AC_DEFUN([GNOME_INIT],[
	GNOME_INIT_HOOK([],fail)
])