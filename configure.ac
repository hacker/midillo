AC_INIT([midillo], [0.0], [midillo-bugs@klever.net])
AC_CONFIG_SRCDIR([include/midillo/SMF.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([dist-bzip2])

AC_PROG_INSTALL
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

AC_HEADER_STDC
AC_CHECK_HEADERS([sys/types.h sys/stat.h])
AC_CHECK_DECLS([environ],,,[
 #include <unistd.h>
])

AC_C_CONST

AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_PATH_PROG([XSLTPROC],[xsltproc],[true])

AC_WITH_PKGCONFIG

PKG_CHECK_MODULES([KONFORKA],[konforka],,[
 AC_MSG_ERROR([no konforka library found. get one from http://kin.klever.net/konforka/])
])

WANT_DOXYGEN="yes"
AC_ARG_ENABLE([doxygen],
 AC_HELP_STRING([--disable-doxygen],[do not generate documentation]),
 [
  test "${enableval}" = "no" && WANT_DOXYGEN="no"
 ]
)
if test "${WANT_DOXYGEN}" = "yes" ; then
 AC_WITH_DOXYGEN
 AC_WITH_DOT
else
 AM_CONDITIONAL([HAVE_DOXYGEN],[false])
 AM_CONDITIONAL([HAVE_DOT],[false])
fi

AC_CONFIG_FILES([
 Makefile
  midillo.pc
  Doxyfile
 include/Makefile
 lib/Makefile
 tools/Makefile
 man/Makefile
])
AC_OUTPUT
