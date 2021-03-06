AC_INIT([vARCH], [0.2], [ben.vighy@gmail.com])

AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall dist-bzip2])

AC_PROG_CXX
AM_PROG_LEX
AC_PROG_YACC

# AM_CONFIG_HEADER([include/config.h])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	src/vasm/Makefile
])

CFLAGS="${CFLAGS=}"
CXXFLAGS="${CXXFLAGS=}"
AC_SUBST([CFLAGS],["${CFLAGS} -Wall"])
AC_SUBST([CXXFLAGS],["${CXXFLAGS} -Wall"])

AC_LANG_PUSH([C++])
###############################################################################
# Check for C++11 flag
my_save_cflags="$CXXFLAGS"
CXXFLAGS="-std=c++11"
AC_MSG_CHECKING([whether CC supports the flag -std=c++11])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
    [AC_MSG_RESULT([yes])]
    [CXX_11_FLAG=-std=c++11],
    [AC_MSG_RESULT([no])]
    [CXX_11_FLAG=-std=c++0x]
)
CXXFLAGS="$my_save_cflags"
AC_SUBST([CXXFLAGS],["${CXXFLAGS} ${CXX_11_FLAG}"])

###############################################################################
# Check if -stdlib=libc++ is needed
my_save_cflags="$CXXFLAGS"
CXXFLAGS="${CXX_11_FLAG} -stdlib=libc++"
AC_MSG_CHECKING([whether CC accepts the flag -stdlib=libc++])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
    [AC_MSG_RESULT([yes])]
    [LIBCXX_FLAG=-stdlib=libc++],
    [AC_MSG_RESULT([no])]
    [LIBCXX_FLAG=]
)
CXXFLAGS="$my_save_cflags"
AC_SUBST([CXXFLAGS],["${CXXFLAGS} ${LIBCXX_FLAG}"])


###############################################################################
# Check for ELFIO library
AC_CHECK_HEADER([elfio/elfio.hpp], [], [AC_MSG_ERROR([Please install ELFIO library])], [#include <string>])

###############################################################################
AC_LANG_POP([C++])

###############################################################################
# Debug builds
AC_ARG_WITH(messaging,
  AS_HELP_STRING([--with-messaging=level],
                 [enable various messaging levels, default: 1 (info)
                  Options: 3 (debug), 2 (warnings), 1 (info), 0 (no messages)]),
  [ case "${withval}" in
      3)     messaging=debug ;;
      2)     messaging=warn ;;
      1)     messaging=info ;;
      0)     messaging=nomessage ;;
      false) messaging=nomessage ;;
      no)    messaging=nomessage ;;
      *)   AC_MSG_ERROR([bad value ${withval} for --with-messaging]) ;;
    esac],
  [messaging=info])
AS_IF([test "x$messaging" == xdebug],
  [AC_DEFINE([DEBUG], [1],
        [Define if you want a debug build])
   ])
AS_IF([test "x$messaging" == xwarn],
  [AC_DEFINE([WARNING], [1],
        [Define if you want a build with warning information])
   ])
AS_IF([test "x$messaging" == xinfo],
  [AC_DEFINE([INFO], [1],
        [Define if you want a build with information])
   ])
#AS_IF([test "x$messaging" != xdebug],
#      [ AC_SUBST([CFLAGS],["${CFLAGS} -fomit-frame-pointer -fforce-addr"])
#        AC_SUBST([CXXFLAGS],["${CXXFLAGS} -fomit-frame-pointer -fforce-addr"])
#       ])
###############################################################################

AC_OUTPUT

dnl
dnl Output some configuration info for the user
dnl
echo ""
echo "  Configuration summary:"
echo ""
echo "        prefix:          $prefix"
echo "        exec_prefix:     $exec_prefix"
echo "        libdir:          $libdir"
echo "        includedir:      $includedir"

dnl
dnl Output the features included
dnl
echo ""
echo "        Compiler:        $CXX"
echo "        CXXFLAGS:        $CXXFLAGS"
echo ""
echo "        Verbosity level: $messaging"
echo ""
echo ""
