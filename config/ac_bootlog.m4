##*****************************************************************************
## $Id: ac_bootlog.m4,v 1.2 2005-06-14 23:01:49 achu Exp $
##*****************************************************************************

AC_DEFUN([AC_BOOTLOG],
[
  AC_MSG_CHECKING([for whether to build bootlog modules])
  AC_ARG_WITH([bootlog],
    AC_HELP_STRING([--with-bootlog], [Build bootlog modules]),
    [ case "$withval" in
        no)  ac_bootlog_test=no ;;
        yes) ac_bootlog_test=yes ;;
        *)   AC_MSG_ERROR([bad value "$withval" for --with-bootlog]) ;;
      esac
    ]
  )
  AC_MSG_RESULT([${ac_bootlog_test=yes}])
  
  # bootlog daemon can only be built as a dynamically loadable 
  # module
  if test "$ac_with_static_modules" != "yes" && 
     test "$ac_bootlog_test" = "yes"; then
     AC_CHECK_LIB([dl], [dlopen], [ac_bootlog_have_dl=yes], [])
     if test "$ac_bootlog_have_dl" = "yes"; then
        AC_CHECK_LIB([qsql], [qsql_init], [ac_bootlog_have_qsql=yes], [], [-ldl])
     fi
  fi

  if test "$ac_bootlog_have_qsql" = "yes"; then
     AC_DEFINE([WITH_BOOTLOG], [1], [Define if you want to build bootlog.])
     BOOTLOG_LIBS="-lqsql -ldl"
     MANPAGE_BOOTLOG=1
     ac_with_bootlog=yes
  else 
     MANPAGE_BOOTLOG=0
     ac_with_bootlog=no
  fi

  AC_SUBST(BOOTLOG_LIBS)
  AC_SUBST(MANPAGE_BOOTLOG)
])
