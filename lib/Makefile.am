# Makefile for the lib directory of dr
#
# Copyright (C) 2024  MahmoudESSE
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

AM_CFLAGS = -Wall -Werror -Wextra -std=gnu11
AM_CPPFLAGS = -I$(srcdir)/../src -I$(srcdir) -DLOCALEDIR=\"$(localedir)\"

lib_LTLIBRARIES = libstr.la libgettext.la libcli.la
libstr_la_SOURCES = str.h
libgettext_la_SOURCES = gettext.h
libcli_la_SOURCES = cli.h
LDADD = $(LIBINTL)

# CURRENT: the latest interface implemented
# REVISION: the implementation number of CURRENT
# AGE: the number of interfaces implemented, minus one.
#
# if the api is broken or we added new changes update
# the CURRENT and AGE. when we fix bugs we update
# the REVISION.
#
libstr_la_LDFLAGS = -version-info 0:0:0
libgettext_la_LDFLAGS = -version-info 0:0:0
libcli_la_LDFLAGS = -version-info 0:0:0
