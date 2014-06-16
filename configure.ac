#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([cook], [2014.06.16], [emmanuel.boudreault@gmail.com])
AC_CONFIG_SRCDIR([src])

# Checks for programs.
AC_PROG_CXX
AC_PROG_MAKE_SET

# Check for c++11
CXXFLAGS="$CXXFLAGS -std=c++11"

# CHECK for packages.
PKG_CHECK_MODULES([DEPS], [jansson])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([dup2])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
