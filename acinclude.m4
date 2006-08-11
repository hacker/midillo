dnl AC_WITH_PKGCONFIG([ACTION-IF-FOUND[,ACTION-IF-NOT-FOUND]])
dnl Outputs:
dnl AC_SUBST: PKGCONFIG_PKGCONFIG PKGCONFIG_DIR
dnl AM_CONDTIONAL: HAVE_PKGCONFIG
AC_DEFUN([AC_WITH_PKGCONFIG],[
 PKGCONFIG_PKGCONFIG=""
 PKGCONFIG_DIR=""
 HAVE_PKGCONFIG="no"
 EXPLICIT_PKGCONFIGDIR="no"
 test -z "${WANT_PKGCONFIG}" && WANT_PKGCONFIG=""
 AC_PATH_PROG([PKGCONFIG_PKGCONFIG],[pkg-config],[false])
 if test "${PKGCONFIG_PKGCONFIG}" != "false" ; then
  AC_ARG_WITH([pkgconfigdir],
   AC_HELP_STRING([--with-pkgconfigdir=dir],[Specify pkgconfig directory]),
   [
    if test "${withval}" = "no" ; then
     WANT_PKGCONFIG="no"
    else
     PKGCONFIG_DIR="${withval}"
     EXPLICIT_PKGCONFIGDIR="yes"
    fi
   ],[
    AC_MSG_CHECKING([for pkgconfig directory])
    PKGCONFIG_DIR="`${PKGCONFIG_PKGCONFIG} --debug 2>&1 | grep '^Scanning'| head -n 1 | cut -d\' -f2-|cut -d\' -f1`"
    AC_MSG_RESULT([${PKGCONFIG_DIR}])
   ]
  )
  if test -d "${PKGCONFIG_DIR}" ; then
   HAVE_PKGCONFIG=yes
   AC_SUBST([PKGCONFIG_PKGCONFIG])
   AC_SUBST([PKGCONFIG_DIR])
  else
   AC_MSG_NOTICE([unexistent pkgconfig directory: ${PKGCONFIG_DIR}])
   if test "${EXPLICIT_PKGCONFIGDIR}" = "yes" ; then
    HAVE_PKGCONFIG=yes
    AC_SUBST([PKGCONFIG_PKGCONFIG])
    AC_SUBST([PKGCONFIG_DIR])
   else
    ifelse([$2], , :, [$2])
   fi
  fi
 fi
 AM_CONDITIONAL([HAVE_PKGCONFIG],[test "${HAVE_PKGCONFIG}" = "yes"])
])

dnl AC_WITH_DOXYGEN([ACTION-IF-FOUND[,ACTION-IF-NOT-FOUND]])
dnl Outputs:
dnl AC_SUBST: DOXYGEN HAVE_DOXYGEN
dnl AM_CONDTIONAL: HAVE_DOXYGEN
AC_DEFUN([AC_WITH_DOXYGEN],[
 HAVE_DOXYGEN="no"
 AC_PATH_PROG([DOXYGEN],[doxygen],[false])
 if test "${DOXYGEN}" = "false" ; then
  ifelse([$2], , :, [$2])
 else
  HAVE_DOXYGEN="yes"
  AC_SUBST([DOXYGEN])
  $1
 fi
 AC_SUBST([HAVE_DOXYGEN])
 AM_CONDITIONAL([HAVE_DOXYGEN],[test "${HAVE_DOXYGEN}" = "yes"])
])

dnl AC_WITH_DOT([ACTION-IF-FOUND[,ACTION-IF-NOT-FOUND]])
dnl Outputs:
dnl AC_SUBST: DOT HAVE_DOT
dnl AM_CONDITIONAL: HAVE_DOT
AC_DEFUN([AC_WITH_DOT],[
 HAVE_DOT="no"
 AC_PATH_PROG([DOT],[dot],[false])
 if test "${DOT}" = "false" ; then
  ifelse([$2], , :, [$2])
 else
  HAVE_DOT="yes"
  AC_SUBST([DOT])
  $1
 fi
AC_SUBST([HAVE_DOT])
 AM_CONDITIONAL([HAVE_DOT],[test "${HAVE_DOT}" = "yes"])
])
