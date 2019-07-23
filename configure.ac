#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([lasf], [1.4.2], [matthew.love@colorado.edu])
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

AC_PROG_CC
AC_PROG_LIBTOOL

AC_ARG_WITH([laszip_api],
 	AC_HELP_STRING([--with-laszip=path-to-laszip-dll], [Specify the location of laszip]), 
 	[ncloc=$withval
 	CFLAGS="$CFLAGS -I$ncloc/dll"
 	LDFLAGS="$LDFLAGS -L$ncloc/dll"])

AC_CHECK_LIB([m], fabs, [], [
		  echo "Math library is required..."
		  exit -1])

AC_CHECK_LIB([gdal], OGROpen, [], [
		  echo "GDAL library is required..."
		  exit -1])

dnl AC_CHECK_LIB([laszip_api], laszip_get_version, [], [
dnl 		  echo "laszip library is required..."
dnl  		  exit -1])

AC_CHECK_HEADERS([stdlib.h string.h math.h float.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile src/Makefile utils/Makefile])
AC_OUTPUT
