dnl -*-m4-*-
dnl Process this file with autoconf to produce a configure script.
AC_INIT(README.md, [0.2.5])
AC_CONFIG_SRCDIR([src/cppsim.hh])

configure_options="$@"
echo "libcppsim configure options: $configure_options"

AM_INIT_AUTOMAKE([foreign])
AM_CONFIG_HEADER(config.h)

dnl
dnl Options for ./configure
dnl
AC_ARG_ENABLE(debug,
[  --enable-debug          Disable all optimizations and turn on debugging],
CFLAGS="$CFLAGS -O0 -ggdb" CXXFLAGS="$CXXFLAGS -O0 -ggdb" LDFLAGS="$LDFLAGS -ggdb")

AC_ARG_ENABLE(handle-debug,
[  --enable-handle-debug   Debugs the handle class], 
CPPFLAGS="$CPPFLAGS -DHANDLE_DEBUG")

AC_ARG_ENABLE(memory-debug,
[  --enable-memory-debug   Enable memory debugging via the efence library],
LDFLAGS="$LDFLAGS -lefence")

AC_ARG_ENABLE(profile,
[  --enable-profile        Turn on profiling informations],
CFLAGS="$CFLAGS -pg" CXXFLAGS="$CXXFLAGS -pg" LDFLAGS="$LDFLAGS -pg")

AC_ARG_WITH(context,
[  --with-context=CTX      Set the context handling method (default: autodetect) 
                          mcsc      = makecontext/swapcontext
                          sjlj_copy = setjmp/longjmp with stack copying],
coroutine_method="$withval")


dnl
dnl Checks for programs.
dnl
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB

dnl Checks for libraries.
dnl Replace `main' with a function in -lcppsim:
dnl AC_CHECK_LIB(cppsim,main)

dnl
dnl Checks for header files.
dnl
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h assert.h)
AC_LANG_C

dnl
dnl Checks for typedefs, structures, and compiler characteristics.
dnl
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

dnl
dnl Check for context handling
dnl
if test "$coroutine_method" != mcsc -a \
        "$coroutine_method" != sjlj_copy ; then

        AC_CHECK_FUNCS(swapcontext setcontext makecontext)
        if test "$ac_cv_func_swapcontext" = yes && \
           test "$ac_cv_func_setcontext" = yes && \
           test "$ac_cv_func_makecontext" = yes ; then
                coroutine_method=mcsc
        else
                AC_CHECK_FUNCS(setjmp longjmp)
                if test "$ac_cv_func_setjmp" = yes && \
                   test "$ac_cv_func_longjmp" = yes ; then
                coroutine_method=sjlj_copy
                else
                AC_MSG_ERROR([Unknown context handling method])
                fi
        fi
fi
AC_MSG_CHECKING([which context handling method to use])
AC_MSG_RESULT([$coroutine_method])
AC_DEFINE_UNQUOTED(COROUTINE_METHOD,$coroutine_method,[Context handling method])

dnl
dnl C++ functions and libraries
dnl
AC_LANG_CPLUSPLUS
AC_CHECK_HEADERS(list string map vector algorithm,,[AC_MSG_ERROR(Required STL Headers not found)])
AC_CHECK_HEADERS(sstream strstream cstdio cstring)

AC_OUTPUT(test/Makefile src/Makefile doc/Makefile Makefile)
