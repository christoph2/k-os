#!/usr/bin/python
# -*- coding: utf-8 -*-

__version__ = '0.9.0'

__copyright__ = \
    """
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

  (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
                                       cpu12.gems@googlemail.com>

  All Rights Reserved

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

   s. FLOSS-EXCEPTION.txt
"""


class Token(object):

    def __init__(self, type_, attr = None, lineno = '???', filename = ''):
        self.type_ = type_
        self.attr = attr
        self.lineno = lineno
        self.filename = filename

    def __cmp__(self, o):
        return cmp(self.type_, o)

    def __getitem__(self, i):
        raise IndexError

    def __repr__(self):
        return "%s (type: '%s' attr: '%s' file: '%s' line: '%s')\n" % (
            self.__class__, self.type_, self.attr, self.filename, self.lineno
        )
