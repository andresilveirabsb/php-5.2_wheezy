dnl
dnl $Id: config.m4 239540 2007-07-11 23:20:37Z jani $
dnl

PHP_ARG_WITH(tux,,
[  --with-tux=MODULEDIR    Build PHP as a TUX module (Linux only)], no, no)

AC_MSG_CHECKING([for TUX])
if test "$PHP_TUX" != "no"; then
  INSTALL_IT="\$(INSTALL) -m 0755 $SAPI_SHARED $PHP_TUX/php5.tux.so"
  AC_CHECK_HEADERS(tuxmodule.h,[:],[AC_MSG_ERROR([Cannot find tuxmodule.h])])
  PHP_SELECT_SAPI(tux, shared, php_tux.c)
  AC_MSG_RESULT([$PHP_TUX])
else
  AC_MSG_RESULT(no)
fi
