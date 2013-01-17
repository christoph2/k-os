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

class Extras(object):
    NUMBER_RANGE    = 0
    FLOAT_RANGE     = 1
    ENUMERATION     = 2
    ATTRIBUTE_NAME  = 3
    BOOL_VALUES     = 4
    DUMMY           = 5

    def __init__(self, type_, args = {}):
        self.type_ = type_
        for k,v in args.items():
            if k == 'range' and v == False:
                pass
            setattr(self, k, v)


class ImplementationSpec(object):
    def __init__(self, objectType, implementationList, desciption):
        self.objectType = objectType
        self.implementationList = implementationList
        self.desciption = desciption


class ImplAttrType(object):
    UINT32  = 0
    INT32   = 1
    UINT64  = 2
    INT64   = 3
    FLOAT   = 4
    ENUM    = 5
    STRING  = 6
    BOOLEAN = 7

    _types = {
        'UINT32'    : UINT32,
        'INT32'     : INT32,
        'UINT64'    : UINT64,
        'INT64'     : INT64,
        'FLOAT'     : FLOAT,
        'ENUM'      : ENUM,
        'STRING'    : STRING,
        'BOOLEAN'   : BOOLEAN
    }

    _types_rev = dict([(v,k) for k,v in _types.items()])
    _instance = None

    def __new__(cls):
        if not cls._instance:
            cls._instance = super(ImplAttrType, cls).__new__(cls)
        return cls._instance

    @classmethod
    def toString(cls, t):
        return cls._types_rev[t]

    @classmethod
    def fromString(cls, t):
        return cls._types[t]


class ImplParameterList(object):
    def __init__(self, implDefList):
        self.dict_ = dict()
        for attr in implDefList:
            self.dict_[attr.attrName] = attr


class NestableDefinition(object):
    def __init__(self, *args, **kws):
        pass


class ImplAttrDef(NestableDefinition):

    def __init__(self, dataType, attrName, autoSpec, mult, default, desc, extra):
        self.dataType = dataType
        self.attrName = attrName
        self.autoSpec = autoSpec
        self.mult = mult
        self.default = default
        self.dict_ = {}

        if attrName == 'ORTI_DEBUG':
            pass

        if default:
            if dataType == ImplAttrType.ENUM:
                if default == u'AUTO':
                    pass    # TODO: autosToBeResolved
                else:
                    enum = [e.name for e in extra.enumeration.values()]
                    if not default in enum:
                        logger.error("Default-Value '%s' for Attribute '%s' out of range."
                             % (default, attrName))
                    else:
                        pass
            elif dataType in (ImplAttrType.UINT32, ImplAttrType.UINT64,
              ImplAttrType.INT32, ImplAttrType.INT64):
                if extra.range == True:
                    if (default < extra.numberFrom) or (default > extra.numberTo):
                        logger.error("Default-Value '%s' for Attribute '%s' out of range."
                                 % (default, attrName))
                elif extra.range == False:
                    if not default in extra.extra.numberFrom:
                        if (default < extra.numberFrom) or (default > extra.numberTo):
                            logger.error("Default-Value '%s' for Attribute '%s' out of range."
                                     % (default, attrName))
            elif  dataType == ImplAttrType.FLOAT:
                if (extra.numberFrom and extra.numberTo) and ((default < extra.numberFrom) or (default > extra.numberTo)):
                    logger.error("Default-Value '%s' for Attribute '%s' out of range."
                             % (default, attrName))
            elif dataType == ImplAttrType.BOOLEAN:
                pass
        self.desc = desc
        if extra.type_ == Extras.NUMBER_RANGE:
            self.range = extra.range
            self.numberFrom = extra.numberFrom
            self.numberTo = extra.numberTo
        elif extra.type_ == Extras.FLOAT_RANGE:
            self.numberFrom = extra.numberFrom
            self.numberTo = extra.numberTo
        elif extra.type_ == Extras.ENUMERATION:
            self.enumeration = extra.enumeration
            if extra.enumeration:
                self.dict_.update(extra.enumeration.dict_)
            #for enum in extra.enumeration.values():
            #    self.dict_[enum.name] = enum
        elif extra.type_ == Extras.ATTRIBUTE_NAME:
            pass
        elif extra.type_ == Extras.BOOL_VALUES:
            ##extra.boolValues.trueParameterList.values.dict_
            if extra.boolValues:
                if extra.boolValues.trueParameterList:
                    self.dict_['TRUE'] = extra.boolValues.trueParameterList.values.dict_
                if extra.boolValues.falseParameterList:
                    self.dict_['FALSE'] = extra.boolValues.falseParameterList.values.dict_

            self.boolValues = extra.boolValues
        elif extra.type_ == Extras.DUMMY:
            pass
        #self.extra = extra
        if default and default not in self:
            print "Default value must be in %s" % self.defaultRangeRepr()

        super(ImplAttrDef, self).__init__(dataType = dataType, attrName = attrName,
            autoSpec = autoSpec, mult = mult, default = default,
            desc = desc, extra = extra
        )

    def __contains__(self, value):
        if value == 'AUTO':
            return True
        result = True
        if self.dataType in (ImplAttrType.UINT32, ImplAttrType.INT32,
                ImplAttrType.UINT64, ImplAttrType.INT64):
            numberFrom, numberTo = self.numberFrom, self.numberTo
            if numberFrom:
                if numberTo:
                    # Number range.
                    result = numberFrom <= value <= numberTo
                else:
                    # Number list.
                    result = value in numberFrom
        elif self.dataType == ImplAttrType.FLOAT:
            numberFrom, numberTo = self.numberFrom, self.numberTo
            if numberFrom and numberTo:
                result = numberFrom <= value <= numberTo
        elif self.dataType == ImplAttrType.ENUM:
            enumeration = self.enumeration
            if enumeration:
                result = value in [e.name for e in enumeration.values()]
        elif self.dataType == ImplAttrType.STRING:
            pass
        elif self.dataType == ImplAttrType.BOOLEAN:
            boolValues = self.boolValues
            if boolValues:
                pass
        return result

    def defaultRangeRepr(self):
        if self.dataType in (ImplAttrType.UINT32, ImplAttrType.INT32,
                ImplAttrType.UINT64, ImplAttrType.INT64):
            numberFrom, numberTo = self.numberFrom, self.numberTo
            if numberFrom:
                if numberTo:
                    # Number range.
                    return "[%s .. %s]" % (numberFrom, numberTo)
                else:
                    # Number list.
                    return str(numberFrom)
        elif self.dataType == ImplAttrType.FLOAT:
            numberFrom, numberTo = extra.numberFrom, extra.numberTo
            if numberFrom and numberTo:
                return "[%s .. %s]" % (numberFrom, numberTo)
        elif self.dataType == ImplAttrType.ENUM:
            enumeration = extra.enumeration
            if enumeration:
                return str([e.name for e in enumeration.values()])
        elif self.dataType == ImplAttrType.BOOLEAN:
            return ""

    def __getitem__(self, item):
        return self.dict_[item]


    def __repr__(self):
        return "ATTR (%s - %s)" % (self.attrName, ImplAttrType.toString(self.dataType))


class ImplRefDef(NestableDefinition):
    def __init__(self, objectType, name, mult, desc):
        self.objectType = objectType
        self.attrName = name
        self.mult = mult
        self.desc = desc
        super(ImplRefDef, self).__init__(objectType = objectType, name = name,
            mult = mult, desc = desc
        )

    def __repr__(self):
        return "REF (%s - %s)" % (self.attrName, self.objectType)


class BoolValues(NestableDefinition):
    def __init__(self, trueParameterList, trueDescription, falseParameterList, falseDescription):
        self.dict_ = {}
        self.trueParameterList = trueParameterList
        if trueParameterList:
            self.dict_['TRUE'] = dict()
            self.dict_['TRUE'].update(trueParameterList.values.dict_)
        self.trueDescription = trueDescription
        self.falseParameterList = falseParameterList
        if falseParameterList:
            self.dict_['FALSE'] = dict()
            self.dict_['FALSE'].update(falseParameterList.values.dict_)
        self.falseDescription = falseDescription


class Enumeration(object):
    def __init__(self, enumerationList):
        self.dict_ = dict()
        self._enumerationList = enumerationList or []
        for enum in self._enumerationList:
            self.dict_[enum.name] = enum

    def values(self):
        return self.dict_.itervalues()

    def __getitem__(self, item):
        return self.dict_[item]


class Enumerator(NestableDefinition):
    def __init__(self, name, parameters, description):
        self.dict_ = dict()
        self.name = name
        self.parameters = parameters

        if self.parameters:
            self.dict_.update(parameters.dict_)

        self.description = description
        super(Enumerator, self).__init__(name = name,
            parameters = self.parameters, description = description
        )

    def __repr__(self):
        if self.parameters:
            if self.description:
                return "(%s %s) : '%s'" % (self.name, self.parameters, self.description)
            else:
                return "(%s %s)" % (self.name, self.parameters)
        else:
            if self.description:
                return "(%s) : '%s'" % (self.name, self.description)
            else:
                return "(%s)" % (self.name, )

    def __getitem__(self, item):
        return self.dict_[item]


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


class TestImplAttrType_toString(unittest.TestCase):

    def setUp(self):
        self.c = ImplAttrType()

    def test_toUI32(self):
        self.assertEquals(self.c.toString(ImplAttrType.UINT32), 'UINT32')

    def test_toI32(self):
        self.assertEquals(self.c.toString(ImplAttrType.INT32), 'INT32')

    def test_toUI64(self):
        self.assertEquals(self.c.toString(ImplAttrType.UINT64), 'UINT64')

    def test_toI64(self):
        self.assertEquals(self.c.toString(ImplAttrType.INT64), 'INT64')

    def test_toFloat(self):
        self.assertEquals(self.c.toString(ImplAttrType.FLOAT), 'FLOAT')

    def test_toEnum(self):
        self.assertEquals(self.c.toString(ImplAttrType.ENUM), 'ENUM')

    def test_toString(self):
        self.assertEquals(self.c.toString(ImplAttrType.STRING), 'STRING')

    def test_toBoolean(self):
        self.assertEquals(self.c.toString(ImplAttrType.BOOLEAN), 'BOOLEAN')

    def test_Exc(self):
        self.assertRaises(KeyError, self.c.toString,0x4223)


class TestImplAttrType_fromString(unittest.TestCase):
    def setUp(self):
        self.c = ImplAttrType()

    def test_fromUI32(self):
        self.assertEquals(self.c.fromString('UINT32'), ImplAttrType.UINT32)

    def test_fromI32(self):
        self.assertEquals(self.c.fromString('INT32'), ImplAttrType.INT32)

    def test_fromUI64(self):
        self.assertEquals(self.c.fromString('UINT64'), ImplAttrType.UINT64)

    def test_fromI64(self):
        self.assertEquals(self.c.fromString('INT64'), ImplAttrType.INT64)

    def test_fromFloat(self):
        self.assertEquals(self.c.fromString('FLOAT'), ImplAttrType.FLOAT)

    def test_fromEnum(self):
        self.assertEquals(self.c.fromString('ENUM'), ImplAttrType.ENUM)

    def test_fromString(self):
        self.assertEquals(self.c.fromString('STRING'), ImplAttrType.STRING)

    def test_fromBoolean(self):
        self.assertEquals(self.c.fromString('BOOLEAN'), ImplAttrType.BOOLEAN)

    def test_Exc(self):
        self.assertRaises(KeyError, self.c.fromString,"INVALID")

if __name__ == '__main__':
    unittest.main()
