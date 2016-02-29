dnl $Id: config.m4 198520 2005-10-15 18:17:43Z derick $
dnl config.m4 for input timezonedbing extension

PHP_ARG_ENABLE(timezonedb, whether to enable timezonedb support,
[  --enable-timezonedb           Enable timezonedb support])

if test "$PHP_timezonedb" != "no"; then
  PHP_SUBST(TIMEZONEDB_SHARED_LIBADD)
  PHP_NEW_EXTENSION(timezonedb, timezonedb.c, $ext_shared)
  CPPFLAGS="$CPPFLAGS -Wall"
  INCLUDES="$INCLUDES -I$prefix/include/php/ext/date/lib"
fi
