dnl  
dnl  Copyright (C) 2005, 2006, 2007, 2008 Free Software Foundation, Inc.
dnl  
dnl  This program is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU General Public License as published by
dnl  the Free Software Foundation; either version 3 of the License, or
dnl  (at your option) any later version.
dnl  
dnl  This program is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl  GNU General Public License for more details.
dnl  You should have received a copy of the GNU General Public License
dnl  along with this program; if not, write to the Free Software
dnl  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
dnl  

dnl These aren't needed unless this is buult standalone. Currently Cygnal is
dnl built as part of Gnash, so this is included in the top level configure,.ac.
dnl

dnl AC_PREREQ(2.50)
dnl AC_INIT(gnash, trunk)
dnl AC_CONFIG_SRCDIR(libcore/gnash.h)
dnl AM_CONFIG_HEADER(gnashconfig.h)

dnl AC_CANONICAL_BUILD
dnl AC_CANONICAL_HOST

dnl dnl --------------------------------------------------------
dnl dnl Figure out development tool stuff
dnl dnl --------------------------------------------------------

dnl AC_PROG_CC
dnl AC_PROG_CXX
dnl AC_EXEEXT
dnl AC_PROG_INSTALL
dnl AM_COMPILER_LIB

AC_DEFUN([CYGNAL_PATHS],
[

dnl For Asynchronous I/O
dnl AC_CHECK_HEADERS(aio.h poll.h)

dnl Look for the various ways of blocking while waiting for I/O
AC_CHECK_FUNCS(pselect poll ppoll epoll)
 
])
