#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([GLT], [1.4.0], [matthew.love@colorado.edu])
AC_CONFIG_AUX_DIR([./config])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_MACRO_DIR([m4])
AM_PROG_AR

AC_CHECK_FUNCS(dlopen)
if test $ac_cv_func_dlopen = yes
then DYNAMIC_AG=-export-dynamic
else DYNAMIC_AG=""
fi
AC_SUBST(DYNAMIC_AG)

AM_PROG_AR
AC_PROG_CC
AC_PROG_LIBTOOL

AC_CHECK_LIB([m], fabs, [], [
		  echo "Math library is required..."
		  exit -1])

AC_CHECK_HEADERS([stdlib.h string.h float.h])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
