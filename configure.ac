dnl configure.ac - used to bootstrap the package with GNU Autotools
dnl
dnl Copyright (C) 2024  MahmoudESSE
dnl
dnl This program is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program.  If not, see <http://www.gnu.org/licenses/>.

dnl Prelude
AC_PREREQ([2.72])
AC_INIT([dr], [0.1.0], [mahmoudessehayli@gmail.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror 1.16])
AC_CONFIG_MACRO_DIR([m4])
AM_PROG_AR

dnl Checks for programs
LT_INIT
AM_GNU_GETTEXT_VERSION([0.22.4])
AM_GNU_GETTEXT([external])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_SED
AC_CHECK_PROGS([TAR], [tar], [:])
if test "$TAR" = :; then
   AC_MSG_ERROR([This package needs tar.])
fi

dnl Output files.
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
Makefile
lib/Makefile
src/Makefile
po/Makefile.in
])

AC_OUTPUT
