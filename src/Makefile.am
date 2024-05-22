# Makefile for the src directory of dr
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

CC=gcc
AM_CFLAGS = -Wall -Werror -Wextra -std=gnu11
AM_CPPFLAGS = -I$(srcdir)/../lib -I$(srcdir) -DLOCALEDIR=\"$(localedir)\"

bin_PROGRAMS = dr
dr_SOURCES = main.c
dr_LDADD = ../lib/libstr.la
LDADD = $(LIBINTL)
