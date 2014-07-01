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
from kosek.ImplementationDefinition.NestableDefinition import NestableDefinition
from kosek.ImplementationDefinition.AttributeDefinitions import AttributeDefinitionFactory, ImplAttrType
from kosek.ImplementationDefinition.Enumeration import Enumeration, Enumerator

logger = Logger()

class ImplementationSpec(object):
    def __init__(self, objectType, implementationList, desciption):
        self.objectType = objectType
        self.implementationList = implementationList
        self.desciption = desciption

class ImplParameterList(object):
    def __init__(self, implDefList):
        self._dict = dict([(attr.attrName, attr) for attr in implDefList])
        
    def values(self):
        return self._dict.values()
    
    def items(self):
        return self._dict


class ImplRefDef(NestableDefinition):
    def __init__(self, objectType, name, mult, desc):
        self.objectType = objectType
        self.attrName = name
        self.mult = mult
        self.desc = desc
        if mult == True:
            pass
        super(ImplRefDef, self).__init__(objectType = objectType, name = name,
            mult = mult, desc = desc
        )
        
    def validate(self, parameter, path):
        pass

    def __repr__(self):
        return "REF (%s - %s)" % (self.attrName, self.objectType)


class BoolValues(NestableDefinition):
    def __init__(self, trueParameterList, trueDescription, falseParameterList, falseDescription):
        self._dict = {}
        self.trueParameterList = trueParameterList
        if trueParameterList:
            self._dict['TRUE'] = dict()
            self._dict['TRUE'].update(trueParameterList.values.items())
        self.trueDescription = trueDescription
        self.falseParameterList = falseParameterList
        if falseParameterList:
            self._dict['FALSE'] = dict()
            self._dict['FALSE'].update(falseParameterList.values.items())
        self.falseDescription = falseDescription


class ImplementationDefinitionBuilder(object):
    def __init__(self, parser, definitionList):
        self.definitionList = definitionList
        self.parser = parser

    def postProcessing(self):
        result = {}
        for definition in self.definitionList:
            result[definition.objectType] = dict()
            if definition.objectType == 'COUNTER':
                pass
            for param in definition.implementationList:
                result[definition.objectType][param.attrName] = param #self.decorateParameter(param)
        return result

### ### ##  ###  #####  #######         ####### #######  #####  #######  #####
  #   #   #   #     #    #  #  #         #  #  #  #    # #     # #  #  # #     #
  #   #   ##  #     #       #               #     #      #          #    #
  #   #   ##  #     #       #               #     #  #   #          #    #
  #   #   # # #     #       #               #     ####    #####     #     #####
  #   #   #  ##     #       #    #######    #     #  #         #    #          #
  #   #   #  ##     #       #               #     #            #    #          #
  #   #   #   #     #       #               #     #    # #     #    #    #     #
   ###   ###  #   #####    ###             ###   #######  #####    ###    #####



if __name__ == '__main__':
    unittest.main()
