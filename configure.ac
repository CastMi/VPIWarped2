AC_INIT([VPIWarped2],[0.1])

AC_CONFIG_MACRO_DIR([m4])
AM_SILENT_RULES([yes])
# This is the version I have installed in my system but if you are able to
# execute with an older version (very likely) please update this numbers.
AM_INIT_AUTOMAKE([1.14 foreign subdir-objects])
# As above.
AC_PREREQ([2.69])

AC_LANG([C++])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_CXX_C_O

LT_INIT

# Check for baseline language coverage in the compiler for the C++11 standard.
dnl Strict conformance mode. Error if not supported
m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

AC_CHECK_HEADER([boost/logic/tribool.hpp],
                [],
                [AC_MSG_ERROR([Could not find tribool.hpp.])])
AC_CHECK_HEADER([boost/logic/tribool_io.hpp],
                [],
                [AC_MSG_ERROR([Could not find tribool_io.hpp.])])
AC_CHECK_HEADER([boost/test/unit_test.hpp],
                [],
                [AC_MSG_WARN([Could not find boost/test/unit_test.hpp. You will not be able to run unit tests.])])

# Check for libwarped
m4_include([m4/check_lib_warped.m4])
CHECK_LIB_WARPED

CPPFLAGS="$CPPFLAGS -Wall -Wextra -Wpedantic -Werror"

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([examples/Makefile])
AC_CONFIG_FILES([examples/circuit/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([tests/examples/Makefile])

AC_OUTPUT
