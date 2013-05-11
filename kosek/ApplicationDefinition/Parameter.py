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

class Parameter(object):
    def __init__(self, parameterName, parameterValue, description, objectName):
        self._parameterName = parameterName
        self._parameterValue = parameterValue
        self._description = description
        self._objectName = objectName

    def getParameterName(self):
        return self._parameterName

    def getParameterValue(self):
        return self._parameterValue


    def getDescription(self):
        return self._description


    def getObjectName(self):
        return self._objectName


    def __repr__(self):
        if self.description:
            return "PARAMETER(%s::%s = %s) : '%s'" % (self.objectName, self.parameterName, self.parameterValue, self.description)
        else:
            return "PARAMETER(%s::%s = %s)" % (self.objectName, self.parameterName, self.parameterValue)

    parameterName = property(getParameterName, None, None, None)
    parameterValue = property(getParameterValue, None, None, None)
    description = property(getDescription, None, None, None)
    objectName = property(getObjectName, None, None, None)

