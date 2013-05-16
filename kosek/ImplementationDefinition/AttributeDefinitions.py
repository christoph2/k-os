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
import sys

from kosek.Logger import Logger
from kosek.ImplementationDefinition.NestableDefinition import NestableDefinition
from kosek.ImplementationDefinition.Extras import Extras
from kosek.ImplementationDefinition.Enumeration import Enumeration, Enumerator

logger = Logger()

signedIntegerRange = lambda numberOfBits:   (-(2 ** (numberOfBits - 1)), (2 ** (numberOfBits - 1)) - 1)
unsignedIntegerRange = lambda numberOfBits: (2 ** numberOfBits) - 1

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


class AttributeDefinition(NestableDefinition):

    def __init__(self, attrName, autoSpec, mult, default, description, extra):
        self.attrName = attrName
        self.autoSpec = autoSpec
        self.mult = mult
        self.default = default
        self.description = description
        self._extra = extra
        self._dict = {}
        self._setupInstance()

        super(AttributeDefinition, self).__init__(attrName = attrName, autoSpec = autoSpec,
            mult = mult, default = default, desc = description, extra = extra
        )

    def _setupInstance(self):
        raise NotImplementedError()


class BooleanAttribute(AttributeDefinition):

    def _setupInstance(self):
        extra = self._extra
        if extra.type_ == Extras.BOOL_VALUES:
            if extra.boolValues:
                if extra.boolValues.trueParameterList:
                    self._dict['TRUE'] = extra.boolValues.trueParameterList.values.items()
                if extra.boolValues.falseParameterList:
                    self._dict['FALSE'] = extra.boolValues.falseParameterList.values.items()

            self.boolValues = extra.boolValues


class EnumAttribute(AttributeDefinition):

    def _setupInstance(self):
        if self.default:
            if self.default == u'AUTO':
                pass    # TODO: autosToBeResolved
            else:
                enum = [e.name for e in self.extra.enumeration.values()]
                if not self.default in enum:
                    logger.error("Default-Value '%s' for Attribute '%s' out of range."
                         % (self.default, self.attrName))
                else:
                    pass
        extra = self._extra
        self._enumeration = extra.enumeration
        #if extra.enumeration:
        #    self._dict.update(extra.enumeration.items())


def AttributeDefinitionFactory(dataType, attrName, autoSpec, mult, default, desc, extra):
    if dataType == ImplAttrType.BOOLEAN:
        return BooleanAttribute(attrName, autoSpec, mult, default, desc, extra)
    elif dataType == ImplAttrType.ENUM:
        return EnumAttribute(attrName, autoSpec, mult, default, desc, extra)


class ImplAttrDef(NestableDefinition):  # TODO: factory method!!!

    def __init__(self, dataType, attrName, autoSpec, mult, default, desc, extra):
        self.dataType = dataType
        self.attrName = attrName
        self.autoSpec = autoSpec
        self.mult = mult
        self.extra = extra
        self.default = default
        self._dict = {}

        if attrName == 'ORTI_DEBUG':
            pass

        if default:
            if dataType == ImplAttrType.ENUM:
                if default == u'AUTO':
                    pass    # TODO: autosToBeResolved
                else:
                    pong = default in extra.enumeration
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
                self._dict.update(extra.enumeration.items())
            #for enum in extra.enumeration.values():
            #    self._dict[enum.name] = enum
        elif extra.type_ == Extras.ATTRIBUTE_NAME:
            pass
        elif extra.type_ == Extras.BOOL_VALUES:
            ##extra.boolValues.trueParameterList.values._dict
            if extra.boolValues:
                if extra.boolValues.trueParameterList:
                    self._dict['TRUE'] = extra.boolValues.trueParameterList.values.items()
                if extra.boolValues.falseParameterList:
                    self._dict['FALSE'] = extra.boolValues.falseParameterList.values.items()

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
            numberFrom, numberTo = self.extra.numberFrom, self.extra.numberTo
            if numberFrom and numberTo:
                return "[%s .. %s]" % (numberFrom, numberTo)
        elif self.dataType == ImplAttrType.ENUM:
            enumeration = self.extra.enumeration
            if enumeration:
                return str([e.name for e in enumeration.values()])
        elif self.dataType == ImplAttrType.BOOLEAN:
            return ""

    def __getitem__(self, item):
        return self._dict[item]


    def __repr__(self):
        return "ATTR (%s - %s)" % (self.attrName, ImplAttrType.toString(self.dataType))


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


    ## @unittest.skipIf(sys.platform.startswith("win"), "Windows ???")
    ## @unittest.skipUnless(sys.platform.startswith("win"), "requires Windows")

def skipUnlessHasattr(obj, attr):
    """ This is an example for an user defined skip decorator.
    """
    if hasattr(obj, attr):
        return lambda func: func
    return unittest.skip("{0!r} doesn't have {1!r}".format(obj, attr))


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



class BaseAttributeDefinition(unittest.TestCase):
    def setUp(self):
        self.extras = Extras(Extras.BOOL_VALUES, {'boolValues': None})
        self.values = (ImplAttrType.BOOLEAN, 'STARTUPHOOK', False, False, None, '', self.extras )

    def tearDown(self):
        del self.extras
        del self.values


class TestAttributeDefinitiom(BaseAttributeDefinition):
    def testSetupInstanceMethodMustBeOverwritten(self):
        self.assertRaises(NotImplementedError, AttributeDefinition, None, None, None, None, None, None)


class TestAttributeDefinitionFactory(BaseAttributeDefinition):
    def testCreateBooleanAttributeDefinition(self):
        self.assertIsInstance(AttributeDefinitionFactory(*self.values), BooleanAttribute )

    def testCreateEnumAttributeDefinition(self):
        enumeration = Enumeration((Enumerator(u'ECC1', None, ''), Enumerator(u'(BCC1', None, ''),
            Enumerator(u'(BCC2', None, ''), Enumerator(u'(ECC2', None, ''))
        )
        extras = Extras(Extras.ENUMERATION, {'enumeration': enumeration})
        values = (ImplAttrType.ENUM, 'CC', False, False, None, '', extras )
        self.assertIsInstance(AttributeDefinitionFactory(*values), EnumAttribute )

        # TODO: enum Tests: default == AUTO, default != AUTO ( CALLSCHEDULE = UNKNOWN).

if __name__ == '__main__':
    unittest.main()

