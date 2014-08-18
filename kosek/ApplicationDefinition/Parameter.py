#!/usr/bin/python
# -*- coding: utf-8 -*-

__version__ = '0.9.2rc-1'
__copyright__ = """
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
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

import itertools
import operator
import unittest

from kosek.Logger import Logger

logger = Logger()

class Parameter(object):
    def __init__(self, parameterName, parameterValue, description, objectName):
        if objectName == 'InitTask' and parameterName == 'RESOURCE':
            pass
        self._parameterName = parameterName
        self._parameterValue = parameterValue
        self._description = description
        self._objectName = objectName

    @property
    def parameterName(self):
        return self._parameterName

    @property
    def parameterValue(self):
        return self._parameterValue

    @property
    def description(self):
        return self._description

    @property
    def objectName(self):
        return self._objectName

    @property
    def value(self):
        return self._parameterValue.value


    @property
    def values(self):
        return self._parameterValue.values

    def __repr__(self):
        if self.description:
            return "PARAMETER(%s::%s = %s) : '%s'" % (self.objectName, self.parameterName, self.parameterValue, self.description)
        else:
            return "PARAMETER(%s::%s = %s)" % (self.objectName, self.parameterName, self.parameterValue)


class ParameterList(list):
    def hasAttribute(self, name):
        return any(p for p in self if p.parameterName == name)

    def hasEvents(self):
        return self.hasAttribute('EVENT')

    def hasResources(self):
        return self.hasAttribute('RESOURCE')

    def hasAutostarts(self):
        return self.hasAttribute('AUTOSTART')

    def hasAlarms(self):
        return self.hasAttribute('ALARM')

    def hasMessages(self):
        return self.hasAttribute('MESSAGE')

    def get(self, name):
        return [item for item in self if item.parameterName == name][0]

    def sort(self):
        sortedList = sorted(self, key = operator.attrgetter('parameterName'))
        return ParameterList(sortedList)

    def grouped(self):
        return [(key, list(group)) for key, group in itertools.groupby(self, key = operator.attrgetter('parameterName'))]

