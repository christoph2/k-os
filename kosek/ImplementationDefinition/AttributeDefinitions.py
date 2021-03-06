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
unsignedIntegerRange = lambda numberOfBits: (0, (2 ** numberOfBits) - 1)

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
        super(AttributeDefinition, self).__init__(attrName = attrName, autoSpec = autoSpec,
            mult = mult, default = default, desc = description, extra = extra
        )
        self.attrName = attrName
        self.autoSpec = autoSpec
        self.mult = mult
        self.default = default
        self.description = description
        self._extra = extra
        if mult == True:
            pass
        #self._dict = {}
        self._setupInstance()
        delattr(self, '_extra')

    def _setupInstance(self):
        raise NotImplementedError()

    def __contains__(self, value):
        raise NotImplementedError()

    def _validate(self, parameter, path):
        raise NotImplementedError()

    def _getDataType(self):
        return self._dataType

    dataType = property(_getDataType)


class BooleanAttribute(AttributeDefinition):

    def _setupInstance(self):
        self._dataType = ImplAttrType.BOOLEAN
        self.falseParameters = None
        self.trueParameters = None
        extra = self._extra
        if extra.type_ == Extras.BOOL_VALUES:
            if extra.boolValues:
                if extra.boolValues.trueParameterList:
                    self.trueParameters = extra.boolValues.trueParameterList.values.items()
                if extra.boolValues.falseParameterList:
                    self.falseParameters = extra.boolValues.falseParameterList.values.items()

            self.boolValues = extra.boolValues

    def __contains__(self, value):
        return True # Guaranteed by grammar.

    def getParameters(self, value):
        if value == True:
            return self.trueParameters
        elif value == False:
            return self.falseParameters
        else:
            raise ValueError(value)

    def _validate(self, parameter, path):
        parameterName = parameter.parameterName

        if parameterName in ('ORTI_DEBUG', 'TYPE'):
            pass
        value = parameter.parameterValue.value

        p2 = self.getParameters(value)

        for xy in parameter.parameterValue.values:
            if xy.parameterName == 'ACTION':
                pass
            try:
                implDef2 = self.getParameters(value)[xy.parameterName]
                implDef2.validate(xy, path)
            except TypeError as e:
                pass

        if not value in self:
            print "BOOLEAN paramter %s %s not in %s" (path, value, self, )


class EnumAttribute(AttributeDefinition):

    def _setupInstance(self):
        self._dataType = ImplAttrType.ENUM
        self._enumeration = self._extra.enumeration
        if self.default:
            if self.default == u'AUTO':
                pass    # TODO: autosToBeResolved
            else:
                #enum = [e.name for e in self.extra.enumeration.values()]
                if not self.default in self:    # ._enumeration:
                    logger.error("Default-Value '%s' for Attribute '%s' out of range." % (self.default, self.attrName))
                else:
                    pass

    def __contains__(self, value):
        # TODO: AUTO handling!!!
        res = value in self._enumeration
        return res

    def _getEnumeration (self):
        return self._enumeration

    def getParameters(self, value):
        enumerator = self._getEnumeration()[value]
        parameters = enumerator.parameters  # TODO: getParamaters!?
        return parameters

    def _validate(self, parameter, path):
        parameterName = parameter.parameterName
        value = parameter.parameterValue.value

        try:
            implDefinitions = self.getParameters(value).items() if self.getParameters(value) else {}
        except KeyError as e:
            pass    ## TODO: Errorhandling!!!

        for xy in parameter.parameterValue.values:
            implDefinition = implDefinitions.get(xy.parameterName)
            implDefinition.validate(xy, path)
        if not value in self:
            pass

    enumeration = property(_getEnumeration)


class IntegerAttribute(AttributeDefinition):

    def __init__(self, dataType, attrName, autoSpec, mult, default, description, extra):
        self._dataType = dataType
        self.signedIntegerRange = signedIntegerRange
        self.unsignedIntegerRange = unsignedIntegerRange
        super(IntegerAttribute, self).__init__(attrName = attrName, autoSpec = autoSpec,
            mult = mult, default = default, description = description, extra = extra
        )

    def _setupInstance(self):
        self._range = self._extra.range
        self._actualNumberFrom = self._extra.numberFrom
        self._actualNumberTo = self._extra.numberTo

        signed, bits = {
            ImplAttrType.UINT32:    (False, 32),
            ImplAttrType.UINT64:    (False, 64),
            ImplAttrType.INT32:     (True,  32),
            ImplAttrType.INT64:     (True,  64),
        }.get(self._dataType)

        if signed:
            result = self.signedIntegerRange(bits)
        else:
            result = self.unsignedIntegerRange(bits)
        self.naturalRangeFrom, self.naturalRangeTo = result

    def __contains__(self, value):
        result = False
        if isinstance(self._actualNumberFrom, list):
            result = value in self._actualNumberFrom    # Check against enumerated list.
        elif self._actualNumberFrom and self._actualNumberTo:
            result = self._actualNumberFrom <= value <= self._actualNumberTo
        else:
            result = self.naturalRangeFrom <= value <= self.naturalRangeTo
        return result

    def _validate(self, parameter, path):
        parameterName = parameter.parameterName
        value = parameter.parameterValue.value

        if not value in self:
            print "Invalid VALUE!!!"


class FloatAttribute(AttributeDefinition):

    def _setupInstance(self):
        #self._range = self._extra.range
        self._dataType = ImplAttrType.FLOAT
        self._actualNumberFrom = self._extra.numberFrom
        self._actualNumberTo = self._extra.numberTo

        if (self._extra.numberFrom and self._extra.numberTo) and ((self.default < self._extra.numberFrom) or (self.default > self._extra.numberTo)):
                    logger.error("Default-Value '%s' for Attribute '%s' out of range."
                             % (self.default, self.attrName))

    def __contains__(self,value):
        if self._actualNumberFrom and self._actualNumberTo:
            return self._actualNumberFrom <= value <= self._actualNumberTo
        else:
            return True

    def _validate(self, parameter, path):
        parameterName = parameter.parameterName
        value = parameter.parameterValue.value

        if not value in self:
            print "Invalid FLOAT VALUE!!!"


class StringAttribute(AttributeDefinition):
    def _setupInstance(self):
        self._dataType = ImplAttrType.STRING

    def __contains__(self, value):
        return True

    def _validate(self, parameter, path):
        pass


def AttributeDefinitionFactory(dataType, attrName, autoSpec, mult, default, desc, extra):
    if dataType == ImplAttrType.BOOLEAN:
        return BooleanAttribute(attrName, autoSpec, mult, default, desc, extra)
    elif dataType == ImplAttrType.ENUM:
        return EnumAttribute(attrName, autoSpec, mult, default, desc, extra)
    elif dataType in (ImplAttrType.UINT32, ImplAttrType.UINT64, ImplAttrType.INT32, ImplAttrType.INT64):
        return IntegerAttribute(dataType, attrName, autoSpec, mult, default, desc, extra)
    elif dataType == ImplAttrType.FLOAT:
        return FloatAttribute(attrName, autoSpec, mult, default, desc, extra)
    elif dataType == ImplAttrType.STRING:
        return StringAttribute(attrName, autoSpec, mult, default, desc, extra)


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

    def testCreateUint32AttributeDefinition(self):
        extra = Extras(Extras.NUMBER_RANGE, {'range': True, 'numberFrom': 32, 'numberTo': 128})
        values = (ImplAttrType.UINT32, 'IDLE_TASK_STACK_SIZE', False, False, 48, None, extra)
        self.assertIsInstance(AttributeDefinitionFactory(*values), IntegerAttribute )

    ## TODO:Other integer types!!!
    def testCreateFloatAttributeDefinition(self):
        extra = Extras(Extras.FLOAT_RANGE, {'range': True, 'numberFrom': 32, 'numberTo': 128})
        values = (ImplAttrType.FLOAT, 'IDLE_TASK_STACK_SIZE', False, False, 48.0, None, extra)
        self.assertIsInstance(AttributeDefinitionFactory(*values), FloatAttribute )

        # TODO: enum Tests: default == AUTO, default != AUTO ( CALLSCHEDULE = UNKNOWN).

    def testCreateStringAttributeDefinition(self):
        extra = Extras(Extras.DUMMY)
        values = (ImplAttrType.STRING, ' KOIL_VERSION', False, False, '2.2', None, extra)
        self.assertIsInstance(AttributeDefinitionFactory(*values), StringAttribute )

if __name__ == '__main__':
    unittest.main()

