include $(top_srcdir)/am/snarf
include $(top_srcdir)/am/guilec

lib_LTLIBRARIES = libguile-termios.la

libguile_termios_la_SOURCES = 	\
	error.h			\
	error.c			\
	termios-const.c		\
	termios-const.h		\
	termios-func.c		\
	termios-func.h		\
	termios-type.c		\
	termios-type.h		\
	termios-main.c

BUILT_SOURCES = 		\
	termios-type.x		\
	termios-func.x

SCM_SOURCES =			\
	termios.scm

libguile_termios_la_CPPFLAGS = $(GUILE_CFLAGS)

libguile_termios_la_LDFLAGS = -module -no-undefined \
	-version-info $(LIBGUILE_TERMIOS_INTERFACE) $(GUILE_LDFLAGS)

pkgguilesitedir = $(guilesitedir)/indicator
nobase_dist_pkgguilesite_DATA = $(SCM_SOURCES)

AM_CPPFLAGS = -I$(top_srcdir)/termios -I$(top_builddir)/termios

snarfcppopts = $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(GUILE_CFLAGS) \
	$(AM_CPPFLAGS)

SUFFIXES = .x
.c.x:
	$(AM_V_SNARF)$(GUILE_SNARF) -o $@ $< $(snarfcppopts)

CLEANFILES = *.x

# GOBJECTS = $(SCM_SOURCES:%.scm=%.go)
# $(GOBJECTS): $(lib_LTLIBRARIES)	# Build the library first

# Handy way to remove the .go files without removing all the rest.
clean-go:
	-$(RM) -f $(GOBJECTS)
.PHONY: clean-go

## Makefile.am ends here
