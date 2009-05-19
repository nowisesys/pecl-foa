dnl $Id$
dnl config.m4 for extension foa

PHP_ARG_WITH(libfoa, for libfoa support,
dnl Make sure that the comment is aligned:
[  --with-libfoa           Include libfoa support])

PHP_ARG_ENABLE(foa, whether to enable foa support,
dnl Make sure that the comment is aligned:
[  --enable-foa            Enable foa support])

if test "$PHP_FOA" != "no"; then
  dnl # --with-libfoa -> check with-path
  
  SEARCH_PATH="/usr/local /usr"          # you might want to change this
  SEARCH_FOR="/include/libfoa.h"         # you most likely want to change this
  if test -r $PHP_FOA/$SEARCH_FOR; then  # path given as parameter
    FOA_DIR=$PHP_FOA
  else # search default path list
    AC_MSG_CHECKING([for libfoa files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        FOA_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  if test -z "$FOA_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the foa distribution])
  fi

  dnl # --with-libfoa -> add include path
  PHP_ADD_INCLUDE($FOA_DIR/include)

  # --with-libfoa -> check for lib and symbol presence
  LIBNAME=foa # you may want to change this
  LIBSYMBOL=foa_init # you most likely want to change this 

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $FOA_DIR/lib, FOA_SHARED_LIBADD)
    AC_DEFINE(HAVE_FOALIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong foa lib version or lib not found])
  ],[
    -L$FOA_DIR/lib -lm -ldl
  ])
  PHP_SUBST(FOA_SHARED_LIBADD)

  PHP_NEW_EXTENSION(foa, src/foa.c, $ext_shared)
fi
