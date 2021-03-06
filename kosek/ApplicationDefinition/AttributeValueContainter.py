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
from collections import namedtuple
from kosek.ImplementationDefinition.AttributeDefinitions import ImplAttrType

logger = Logger()

TypeValue = namedtuple('Typevalue', 'value typeName')

class AttributeValueContainter(object):
    ID_VALUE        = 0
    BOOL_VALUE      = 1
    NUMBER_VALUE    = 2
    FLOAT_VALUE     = 3
    STRING_VALUE    = 4
    AUTO_VALUE      = 5

    def __init__(self, type_, **kwds):
        self._idValue = None
        self._booleanValue = None
        self._number = None
        self._stringValue = None
        self._values = []

        self.multiValues = []
        self.nestedValues = []

        self._type = type_
        for k, v in kwds.items():
            setattr(self, "_%s" % k, v)

    def __getattr__(self, attr):
        if attr == 'values':
            pass
        if self.values.hasAttribute(attr):
            return self.values.get(attr)
        else:
            raise AttributeError(attr)

    def _getValue(self):
        if self._type == AttributeValueContainter.AUTO_VALUE:
            return "AUTO"
        else:
            value = {
                AttributeValueContainter.ID_VALUE:      self._idValue,
                AttributeValueContainter.BOOL_VALUE:    self._booleanValue,
                AttributeValueContainter.NUMBER_VALUE:  self._number,
                AttributeValueContainter.FLOAT_VALUE:   self._number,
                AttributeValueContainter.STRING_VALUE:  self._stringValue,
            } [self._type]
            return value

    def _getType(self):
        return self._type

    def _getValues(self):
        return self._values

    typeCode = property(_getType)
    value = property(_getValue)
    values = property(_getValues)


def typeFromImplementationAttribute(type_):
    return {
      ImplAttrType.BOOLEAN: AttributeValueContainter.BOOL_VALUE,
      ImplAttrType.ENUM:    AttributeValueContainter.ID_VALUE,
      ImplAttrType.FLOAT:   AttributeValueContainter.FLOAT_VALUE,
      ImplAttrType.INT32:   AttributeValueContainter.NUMBER_VALUE,
      ImplAttrType.INT64:   AttributeValueContainter.NUMBER_VALUE,
      ImplAttrType.UINT32:  AttributeValueContainter.NUMBER_VALUE,
      ImplAttrType.UINT32:  AttributeValueContainter.NUMBER_VALUE,
      ImplAttrType.STRING:  AttributeValueContainter.STRING_VALUE
    } [type_]

