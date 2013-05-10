#!/usr/bin/python
# -*- coding: utf-8 -*-

__version__ = '0.9.2rc-1'
__copyright__ = """
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2013 by Christoph Schueler <github.com/Christoph2,
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

import unittest

from kosek.Logger import Logger

logger = Logger()

class AttributeValueContainter(object):
    ID_VALUE        = 0
    BOOL_VALUE      = 1
    NUMBER_VALUE    = 2
    FLOAT_VALUE     = 3
    STRING_VALUE    = 4
    AUTO_VALUE      = 5

    def __init__(self, type_, **kwds):
        self.type_ = type_
        for k,v in kwds.items():
            setattr(self, k, v)

    def getValue(self):
        if self.type_ == AttributeValueContainter.ID_VALUE:
            return self.idValue
        elif self.type_ == AttributeValueContainter.BOOL_VALUE:
            return self.booleanValue
        elif self.type_ == AttributeValueContainter.NUMBER_VALUE:
            return self.number
        elif self.type_ == AttributeValueContainter.FLOAT_VALUE:
            raise self.number
        elif self.type_ == AttributeValueContainter.STRING_VALUE:
            return self.stringValue
        elif self.type_ == AttributeValueContainter.AUTO_VALUE:
            return 'AUTO'

    def getTypeString(self):
        if self.type_ == AttributeValueContainter.ID_VALUE:
            return 'ID'
        elif self.type_ == AttributeValueContainter.BOOL_VALUE:
            return 'BOOL'
        elif self.type_ == AttributeValueContainter.NUMBER_VALUE:
            return 'NUMBER'
        elif self.type_ == AttributeValueContainter.FLOAT_VALUE:
            raise 'FLOAT'
        elif self.type_ == AttributeValueContainter.STRING_VALUE:
            return "STRING"
        elif self.type_ == AttributeValueContainter.AUTO_VALUE:
            return 'AUTO'


