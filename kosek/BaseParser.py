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


import antlr3
import kosek.osekoilLexer
from kosek.Logger import Logger


OBJECT_TYPES = ('ALARM', 'APPMODE', 'COM', 'COUNTER', 'EVENT', 'IPDU',
    'ISR', 'MESSAGE', 'NETWORKMESSAGE', 'NM', 'OS', 'RESOURCE', 'TASK',
)

class AttributeDictofDicts(object):
    def __init__(self, keys):
        self.dict_ = dict.fromkeys(keys)
        for key in self.dict_.iterkeys():
            self.dict_[key] = dict()

    def append(self, objectType, key, *values):
        if len(values) == 1:
            values = values[0]
        self.dict_[objectType][key] = values

    def contains(self, objectType, key):
        return key in self.dict_[objectType]

    def get(self, objectType, key):
        return self.dict_[objectType].get(key)

    def keys(self, objectType):
        return self.dict_[objectType].keys()

    def values(self, objectType):
        return self.dict_[objectType].values()

    def items(self, objectType):
        return self.dict_[objectType].items()


class BaseParser(antlr3.Parser):
    standardResources = []
    internalResources = []
    linkedResources = []
    internalMessages = []
    externalMessages = []

    def __init__(self, *args, **kwargs):
        super(BaseParser, self).__init__(*args, **kwargs)
        self.defaults = AttributeDictofDicts(OBJECT_TYPES)
        self.withAutos = AttributeDictofDicts(OBJECT_TYPES)
        self.objectDict = AttributeDictofDicts(OBJECT_TYPES)

    def appendDefault(self, objectType, attributeName, default):
        self.defaults.append(objectType, attributeName, default)

    def appendAutos(self, objectType, attributeName):
        self.withAutos.append(objectType, attributeName)

    def appendObject(self, objectType, name, object):
        self.objectDict.append(objectType, name, object)

    def getObject(self, objectType, object):
        return self.objectDict.get(objectType, object)



