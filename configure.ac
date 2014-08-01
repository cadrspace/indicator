AC_INIT([indicator], [0.0.0], [poptsov.artyom@gmail.com])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([color-tests])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])], [AC_SUBST([AM_DEFAULT_VERBOSITY],1)])

AC_PROG_CC

AC_ARG_WITH([guilesitedir],
            [AS_HELP_STRING([--with-guilesitedir],
                            [use the specified installation path for Guile modules])],
            [case "x$withval" in
             xyes|xno) guilesitedir="";;
             *)        guilesitedir="$withval";;
             esac],
             [guilesitedir=""])

PKG_CHECK_MODULES([GUILE_2], [guile-2.0])

GUILE_PROGS
GUILE_FLAGS
GUILE_SITE_DIR

LT_INIT()

pkgdatadir="$datadir/$PACKAGE"
if test "x$guilesitedir" = "x"; then
   guilesitedir="$pkgdatadir"
fi
AC_SUBST([guilesitedir])

LIBGUILE_TERMIOS_INTERFACE="1:0:0"
AC_SUBST(LIBGUILE_TERMIOS_INTERFACE)

AC_CONFIG_FILES([Makefile am/Makefile termios/Makefile])

# Generate a Makefile, based on the results.
AC_OUTPUT

if test "$guilesitedir" != "$GUILE_SITE"; then
   # Guile has a different prefix than this module
   AC_MSG_WARN([]
   [The Guile modules will be installed in ${guilesitedir}.]
   [You should probably re-run `configure' with]
   [`--with-guilesitedir=$GUILE_SITE']
   [Otherwise, you will have to adjust the `GUILE_LOAD_PATH' environment]
   [variable.])
fi
