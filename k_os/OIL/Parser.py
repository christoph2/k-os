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

import types
from k_os.OIL.spark import GenericASTBuilder, GenericASTTraversal
from k_os.OIL.Scanner import Scanner
from k_os.OIL.Error import OILError
from k_os.OIL.AST import AST

from collections import namedtuple


Enumerator = namedtuple("Enumerator", "name impl_parameter_list description")
ImplRefDef = namedtuple("ImplRefDef", "object_ref_type, name, multiple_specifier, description")


EMPTY_APP_DEF = 'CPU cpu {};'

MAX_PRIORITIES = 16  # # todo: Cfg. !!!

data = []

Info = {}  # # unsorted Items.

errObj = None

References = {}


class ApplicationDefinition(object):
    _Alarms = {}
    _Appmodes = {}
    _Com = {}
    _Counters = {}
    _Events = {}
    _Ipdus = {}
    _Isrs = {}
    _Messages = {}
    _NetworkMessages = {}
    _Nm = {}
    _Os = {}
    _Resources = {}
    _Tasks = {}

    _Mapping = {
        'ALARM'         : _Alarms,
        'APPMODE'       : _Appmodes,
        'COM'           : _Com,
        'COUNTER'       : _Counters,
        'EVENT'         : _Events,
        'IPDU'          : _Ipdus,
        'ISR'           : _Isrs,
        'MESSAGE'       : _Messages,
        'NETWORKMESSAGE': _NetworkMessages,
        'NM'            : _Nm,
        'OS'            : _Os,
        'RESOURCE'      : _Resources,
        'TASK'          : _Tasks,
    }

    def __getitem__(self, item):
        return self._Mapping[item]

    def get(self, item):
        return self.__getitem__(item)

    def items(self):
        return self._Mapping.items()


    def __repr__(self):
        return """ALARMs: %s
APPMODEs: %s
COM: %s
COUNTERs: %s
EVENTs: %s
IPDUs: %s
ISRs: %s
MESSAGEs: %s
NETWORKMESSAGEs: %s
NM: %s
OS: %s
RESOURCEs: %s
TASKs: %s
        """ % (self._Alarms, self._Appmodes, self._Com, self._Counters,
               self._Events, self._Ipdus, self._Isrs, self._Messages,
               self._NetworkMessages, self._Nm, self._Os, self._Resources,
               self._Tasks)

applicationDefinition = ApplicationDefinition()


UINT32_RANGE = ('UINT32', 0, 2 ** 32 - 1)
INT32_RANGE = ('INT32', -2 ** 31, 2 ** 31 - 1)
UINT64_RANGE = ('UINT64', 0, 2 ** 64 - 1)
INT64_RANGE = ('INT64', -2 ** 63, 2 ** 63 - 1)
FLOAT_RANGE = ('FLOAT', -(1 - 2 ** -24) * 2 ** 127, (1 - 2 ** -24) * 2 ** 128)

NUMERIC_RANGES = {
    'UINT32': UINT32_RANGE,
    'INT32': INT32_RANGE,
    'UINT64': UINT64_RANGE,
    'INT64': INT64_RANGE,
    'FLOAT': FLOAT_RANGE,
    }

##
##  Helper-Functions.
##


def strToBool(value):
    '''Convert Python boolean to OIL format.'''
    if value.upper() == 'TRUE':
        return True
    elif value.upper() == 'FALSE':
        return False
    else:
        raise ValueError("Value must be either 'TRUE' or 'FALSE'")


##
##  Implementation-Definition.
##

class ImplAttrDef(object):
    def __init__(self, name, attrType, auto_specifier, valueRange, multiple_specifier, default, description):
        self.name = name
        self.attrType = attrType
        self.auto_specifier = auto_specifier  # todo: withAuto
        self.valueRange = valueRange
        self.multiple_specifier = multiple_specifier
        self.default = default
        if default is not None:
            if attrType == 'ENUM':
                values = valueRange.getValues() # attrType ,UINT32,UINT64,FLOAT
                if self.default == 'AUTO':
                    pass
                else:
                    if not valueRange.check(self.default):
                        errObj.error("Errornous Implementation Attribute '%s': DEFAULT-Value (%s) out of range."
                                 % (self.name, self.default))
            elif attrType in ('UINT32', 'UINT64', 'FLOAT'):
                if valueRange is not None:
                    if default < valueRange.lhs or default > valueRange.rhs:
                        errObj.error("Errornous Implementation Attribute '%s': DEFAULT-Value (%s) out of range."
                                 % (self.name, self.default))
            elif attrType == 'BOOLEAN':
                pass
            else:
                pass
        self.description = description

    #def getValue(self):
        #return self.attribute_value.value
     #   return self.value

    def __repr__(self):
        return "%s (name: '%s' type: '%s' array: '%s' default: '%s' range: '%s' description: '%s')\n" % \
            (self.__class__, self.name,self.attrType, self.multiple_specifier, self.default,
             self.valueRange, self.description)

#    value = property(getValue)


class BoolValues(object):
    def __init__(self, true_parameter_list, true_description, false_parameter_list, false_description):
        self._true_parameter_list = true_parameter_list
        self._true_description = true_description
        self._false_parameter_list = false_parameter_list
        self._false_description = false_description

    def get(self, value):
        if value == True:
            return self._true_parameter_list
        elif value == False:
            return self._false_parameter_list

    def getTrue(self):
        return self.get(True)

    def getFalse(self):
        return self.get(True)

    def getTrueDescription(self):
        return self._true_description

    def getFalseDescription(self):
        return self._false_description

    def __repr__(self):
        return "%s [True: %s (%s) False: %s (%s)]" % (self.__class__, self._true_parameter_list, self._true_description,
            self._false_parameter_list, self._false_description)


class ImplSpec(object):
    def __init__(self, name):
        self.defs = dict()

    def __repr__(self):
        return repr(self.defs)


Alarm_ImplDef = ImplSpec('ALARM')
Appmode_ImplDef = ImplSpec('APPMODE')
Com_ImplDef = ImplSpec('COM')
Counter_ImplDef = ImplSpec('COUNTER')
Event_ImplDef = ImplSpec('EVENT')
Ipdu_ImplDef = ImplSpec('IPDU')
Isr_ImplDef = ImplSpec('ISR')
Message_ImplDef = ImplSpec('MESSAGE')
NetworkMessage_ImplDef = ImplSpec('NETWORKMESSAGE')
Nm_ImplDef = ImplSpec('NM')
Os_ImplDef = ImplSpec('OS')
Resource_ImplDef = ImplSpec('RESOURCE')
Task_ImplDef = ImplSpec('TASK')

ImplDefMap = {
    'ALARM': Alarm_ImplDef,
    'APPMODE': Appmode_ImplDef,
    'COM': Com_ImplDef,
    'COUNTER': Counter_ImplDef,
    'EVENT': Event_ImplDef,
    'IPDU': Ipdu_ImplDef,
    'ISR': Isr_ImplDef,
    'MESSAGE': Message_ImplDef,
    'NETWORKMESSAGE': NetworkMessage_ImplDef,
    'NM': Nm_ImplDef,
    'OS': Os_ImplDef,
    'RESOURCE': Resource_ImplDef,
    'TASK': Task_ImplDef,
    }


class NameMappedDict(dict):
    def __getattr__(self, attr):
        try:
            return self.__dict__[attr]
        except KeyError:
            return self.__getitem__(attr)

class Range(object):
    def __init__(self, lhs, rhs):
        self.lhs = lhs
        self.rhs = rhs

    def __repr__(self):
        return '[' + str(self.lhs) + ' .. ' + str(self.rhs) + ']'

    def Check(self, value):
        return self.lhs <= value <= self.rhs

    def GetRange(self):
        return (self.lhs, self.rhs)


class NumberRange(Range):
    def __init__(self, lhs, rhs):
        Range.__init__(self, lhs, rhs)


class NumberRangeRange(NumberRange):
    def __init__(self, lhs, rhs):
        NumberRange.__init__(self, lhs, rhs)


class NumberRangeList(NumberRange): # todo: eigenständig!!!
    def __init__(self, lhs):
        NumberRange.__init__(self, lhs, None)

    def __repr__(self):
        return str(self.lhs).replace('L', '')

    def Check(self, value):
        return value in self.lhs

    def GetRange(self):
        return self.lhs


class FloatRange(Range):
    def __init__(self, lhs, rhs):
        Range.__init__(self, lhs, rhs)


class EnumRange(object):  # Range
    def __init__(self, values):
        self._values = values
        self._dict = dict([(x[0], x[1]) for x in self._values])

    def getValues(self):
        return [v.name for v in self._values]

    def get(self, key):
        return self._dict.get(key, None)

    def items(self):
        return self._values

    def check(self, value):
        return value in (v.name for v in self._values)

    def __repr__(self):
        return "%s ['%s']\n" % (self.__class__, self._values)


def GetParameterDefinition(obj, name, path=[]):
    ##
    ## todo: 'Normale', 'Enum' und 'Boolean' Parameter unterscheiden!!!
    ##
    defMap = ImplDefMap.get(obj.type_)

    if path == []:
        return defMap.defs.get(name)
    if len(path) == 2:
        return defMap.defs.get(name)
    elif len(path) == 1:
        return defMap.defs.get(name)
    else:
        #for p in path:
        #    print p,
        pn = defMap.defs.get(path[1][1])
        try:
            for it in defMap.defs.items():
                pass #print it,
                ##for v in it.valueRange.lhs:
                ##    print v
        except:
            pass


def GetParameterPathToRootObject(obj, param):
    path = []
    path.append((obj.name, type(obj)))
    tobj = obj
    while tobj.parent != None:
        tobj = tobj.parent
        path.insert(0, (tobj.name, tobj.type_))
    return (tobj, path)


class ParameterContainer(NameMappedDict):
    def __init__(self, name):
        self.name = name

    def __getitem__(self, index):
        return self.get(index)


def NumericRangeCheck(attr, impldef, type_range):
    value = attr.value
    if value == 'AUTO':
        return True
    if impldef.valueRange is not None:
        if not impldef.valueRange.Check(value):
            valueRange = impldef.valueRange.GetRange()
            errObj.error("%s-Value for attribute '%s' out of defined range, valid: %s."
                          % (impldef.attrType, attr.name,
                         str(impldef.valueRange)))
            return False
    (typename, rangeMin, rangeMax) = type_range
    if not rangeMin <= value <= rangeMax:
        errObj.error("%s-Value for attribute '%s' out of datatype-range: valid: [%s .. %s]."
                      % (typename, attr.name, rangeMin, rangeMax))
        return False
    return True


##
##  Parser-Objects.
##
class ObjectDefinition(NameMappedDict):
    def __init__(self, name, type_, description=None):
        self.name = name
        self.type_ = type_
        self.description = description
        self.path = []
        self.parent = None
        self.implDef = ImplDefMap[self.type_]
        self.root = None
        #self.path = [(self.name, self.type_)]

    def AddParameter(self, name, value):
        pd = GetParameterDefinition(self, name, self.path)
        if pd is None:
            errObj.error("Unknown attribute '%s' for '%s'."  % (name, '::'.join([p[0] for p in value.path[:-1]])))
        else:
            multipleAttrs = pd.multiple_specifier
            if self.get(name) is not None:
                if multipleAttrs == False:
                    errObj.error("Only a single value allowed for attribute '%s'." % name)
                    return
            else:
                if multipleAttrs == True:
                    self[name] = []
            if not self.SemanticCheck(value, pd):
                return
            if multipleAttrs == True:
                self[name].append(value)
            else:
                self[name] = value

    def checkNestedParam(self, attr):
        if attr.implDef.attrType == 'BOOLEAN':
            if not isinstance(attr.value, types.BooleanType):
                errObj.error("Parameter '%s::%s' must be of type BOOLEAN."
                              % (self.name, '::'.join([p[0] for p in attr.path])))
                return False

            validParamValues = [v[1].name for v in attr.implDef.valueRange.get(attr.value)]
            for (paramType, paramList) in attr.items():
                implDef = attr[paramType]
                if isinstance(paramList, types.ListType):
                    for param in paramList:
                        if isinstance(param, NestedParameter):
                            return self.checkNestedParam(param)
                        else:
                            if param.name not in validParamValues:
                                errObj.error("Invalid Parameter '%s::%s::%s'."
                                         % (self.name, '::'.join([p[0] for p in attr.path]), param.name))
                                return False
                            else:
                                implDef = [v[1] for v in validParams if v[1].name == param.name][0]
                                return self.SemanticCheck(param, implDef)
                else:
                    param = paramList
                    if isinstance(param, NestedParameter):
                        return self.checkNestedParam(param)
                    else:
                        if param.name not in validParamValues:
                            errObj.error("Invalid Parameter '%s::%s::%s'."
                                     % (self.name, '::'.join([p[0] for p in attr.path]), param.name))
                            return False
                        else:
                            implDef = [v[1] for v in attr.implDef.valueRange.get(attr.value)
                                       if v[1].name == param.name][0]
                            return self.SemanticCheck(param, implDef)
        elif attr.implDef.attrType == 'ENUM':
            validParams = attr.implDef.valueRange.items() # attr.implDef.valueRange.lhs
            validParamValues = [v[0] for v in validParams]

            if attr.value not in validParamValues:
                errObj.error("Invalid Value '%s' for Parameter '%s::%s'."
                              % (attr.value, self.name, '::'.join([p[0] for p in attr.path])))
                return False
            else:
                #implDef = filter(lambda x: x.name == attr.value, validParams)

                #implDef = attr.implDef.valueRange.get(attr.value)[0][1] # todo: gehöhrt zu 'get' !?
                # implDef = attr.implDef.valueRange.get(attr.value)

                di = dict(map(lambda x: (x[1].name, x[1]), attr.implDef.valueRange.get(attr.value)))
                errorCount = 0
                for a in attr.items():
                    (paramType, param) = a
                    try:
                        implDef = di[paramType]
                        if paramType != implDef.name:
                            errObj.error("%s? Expected '%s::%s'."
                                 % (paramType, '::'.join([p[0] for p in attr.path]), implDef.name))
                            errorCount += 1
                        else:
                            if not self.SemanticCheck(param, implDef):
                                errorCount += 1
                    except:
                        errObj.error("Invalid Attribute '%s' for '%s'."  % (paramType, '::'.join([p[0] for p in attr.path])))
                        errorCount += 1
                if errorCount > 0:
                    return False
        return True

    def SemanticCheck(self, attr, impldef):
        if isinstance(attr, NestedParameter):
            return self.checkNestedParam(attr)
        if attr.attribute_value.value == 'AUTO':
            if impldef.auto_specifier == False:
                errObj.error("AUTO-Specifier for attribute '%s' not permitted." % attr.name)
                return False
        if isinstance(impldef, ImplRefDef):
            References.setdefault(self.name, []).append(attr)
        else:
            if not self.TypeCompat(attr, impldef):
                return False
            if not self.RangeCheck(attr, impldef):
                return False
        return True

    def RangeCheck(self, attr, impldef):
        formal = impldef.attrType
        value = attr.attribute_value.value

        if formal in ('UINT32', 'INT32', 'UINT64', 'INT64', 'FLOAT'):
            if not NumericRangeCheck(attr, impldef,
                    NUMERIC_RANGES[formal]):
                return False
        elif formal == 'ENUM':
            if not impldef.valueRange.check(value):
                if value == 'AUTO':
                    if impldef.auto_specifier == False:
                        errObj.error("AUTO-Specifier for attribute '%s' not permitted." % attr.name)
                        return False
                else:
                    enum = impldef.valueRange.getValues()
                    errObj.error("Undefined emumerator '%s' for attribute '%s', expected %s." % (value, attr.name, enum))
                    return False
        return True

    def TypeCompat(self, attr, impldef):
        TypeMap = {
            'UINT32': 'number',
            'INT32': 'number',
            'UINT64': 'number',
            'INT64': 'number',
            'FLOAT': 'float',
            'ENUM': 'name',
            'STRING': 'string',
            'BOOLEAN': 'boolean',
            }

        actual_type = attr.attribute_value.type_
        if actual_type == 'name' and attr.attribute_value.value == 'AUTO':
            return True
        expected_type = TypeMap[impldef.attrType]
        if actual_type == 'name' and expected_type == 'string':
            return True
        if expected_type != actual_type:
            errObj.error("<%s>-token for attribute '%s' expected." % (expected_type, attr.name))
            return False
        else:
            return True


class NestedParameter(NameMappedDict):
    def __init__(self, name, value, parent):
        self.name = name
        self.value = value
        self.parent = parent
        self.type_ = name
        self.parameters = dict()

        keys = ImplDefMap.keys()

        if name == 'ACTION' and value == 'ACTIVATETASK' or name == 'DIRECTION':
            pass

        (self.root, self.path) = GetParameterPathToRootObject(self, name)

        if self.root.type_ not in keys:
            pass

        defMap = parent.implDef
        if isinstance(defMap, ImplSpec):
            try:
                self.implDef =  defMap.defs[name]
            except:
                self.implDef = None
        else:
            self.implDef = filter(lambda x: x[1].name == name , defMap.valueRange.get(parent.value))
            try:
                self.implDef = self.implDef[0][1]
            except:
                di = defMap.valueRange._dict.items()
                self.implDef = None

        if not self.implDef is None:
            if self.implDef.attrType == 'BOOLEAN':
                v = self.implDef.valueRange.get(value)
            elif self.implDef.attrType == 'ENUM':
                """
                for v in self.implDef.valueRange.getValues():
                    print v
                """
            else:
                pass    # todo: ErrorHandling!!!
        # self.implDef = GetParameterDefinition(self.root, name, self.path)

    def AddParameter(self, name, value):
        if self.implDef is None:
            return
        if self.implDef.multiple_specifier == True:
            self.setdefault(name, []).append(value)
        else:
            self[name] = value

    def __repr__(self):
        return "%s (type: '%s' name: '%s' value: '%s')\n" % (self.__class__, self.type_, self.name, self.value)

    def __setitem__(self, name, value):
        self.parameters[name] = value
        super(NestedParameter, self).__setitem__(name, value)
        #print self[name]


class AttributeValue(object):
    def __init__(self, type_, value, parameterised=False, parameter_list=None):
        self.type_ = type_
        self.value = value
        self.parameterised = parameterised
        self.parameter_list = parameter_list

    def __repr__(self):
        return self.value


class PropertyMixin(object):
    def getValue(self):
        return self.attribute_value.value

    def setValue(self, value):
        self.attribute_value.value = value

    def getName(self):
        return self.attribute_name

    def setName(self, name):
        self.attribute_name = name

    def getType(self):
        return self.attribute_value.type_

    def getParameterised(self):
        return self.attribute_value.parameterised

    def getParameterList(self):
        return self.attribute_value.parameter_list

    name = property(getName, setName)
    value = property(getValue, setValue)
    type = property(getType)
    parameterised = property(getParameterised)
    parameterList = property(getParameterList)


class Parameter(PropertyMixin, dict):
    def __init__(self, attribute_name, attribute_value, description=None):
        self.attribute_name = attribute_name
        self.attribute_value = attribute_value
        self.description = description

    def __repr__(self):
        return "%s (type: '%s' name: '%s' value: '%s' parameterised: '%s' parameter-list: '%s' description '%s')"  % (
            self.__class__, self.type, self.name, self.value, self.parameterised, self.parameterList, self.description)


class Parser(GenericASTBuilder):
    def __init__(self, AST, start):
        GenericASTBuilder.__init__(self, AST, start)

    def error(self, token):
        errObj.error("Syntax error at '%s'" % (token, ), lineno=token.lineno, filename=token.filename)
        raise SystemExit

    def p_expr(self, args):
        """
            file ::= OIL_version implementation_definition application_definition

            OIL_version ::= OIL_VERSION = version description ;

            version ::= string

            implementation_definition ::= IMPLEMENTATION name { implementation_spec_list } description ;

            implementation_spec_list ::= implementation_spec
            implementation_spec_list ::= implementation_spec_list implementation_spec

            implementation_spec ::= object { implementation_list } description ;

            implementation_list ::=
            implementation_list ::= implementation_def
            implementation_list ::= implementation_list implementation_def

            implementation_def ::= impl_attr_def
            implementation_def ::= impl_ref_def

            impl_attr_def ::= UINT32 auto_specifier number_range attribute_name multiple_specifier default_number description ;
            impl_attr_def ::= INT32 auto_specifier number_range attribute_name multiple_specifier default_number description ;
            impl_attr_def ::= UINT64 auto_specifier number_range attribute_name multiple_specifier default_number description ;
            impl_attr_def ::= INT64 auto_specifier number_range attribute_name multiple_specifier default_number description ;
            impl_attr_def ::= FLOAT auto_specifier float_range attribute_name multiple_specifier default_float description ;
            impl_attr_def ::= ENUM auto_specifier enumeration attribute_name multiple_specifier default_name description ;
            impl_attr_def ::= STRING auto_specifier attribute_name multiple_specifier default_string description ;
            impl_attr_def ::= BOOLEAN auto_specifier bool_values attribute_name multiple_specifier default_bool description ;


            impl_parameter_list ::=
            impl_parameter_list ::= { impl_def_list }

            impl_def_list ::=
            impl_def_list ::= implementation_def
            impl_def_list ::= implementation_def impl_def_list

            auto_specifier ::=
            auto_specifier ::= WITH_AUTO

            number_range ::=
            number_range ::= [ number range_op number ]
            number_range ::= [ number_list ]

            number_list ::= number
            number_list ::= number_list , number

            default_number ::=
            default_number ::= = number
            default_number ::= = NO_DEFAULT
            default_number ::= = AUTO

            float_range ::=
            float_range ::= [ float range_op float ]

            default_float ::=
            default_float ::= = float
            default_float ::= = NO_DEFAULT
            default_float ::= = AUTO

            enumeration ::= [ enumerator_list ]

            enumerator_list ::= enumerator
            enumerator_list ::= enumerator_list , enumerator

            enumerator ::= name description
            enumerator ::= name impl_parameter_list description

            bool_values ::=
            bool_values ::= [ TRUE impl_parameter_list description , FALSE impl_parameter_list description ]

            default_name ::=
            default_name ::= = name
            default_name ::= = NO_DEFAULT
            default_name ::= = AUTO

            default_string ::=
            default_string ::= = string
            default_string ::= = NO_DEFAULT
            default_string ::= = AUTO

            default_bool ::=
            default_bool ::= = boolean
            default_bool ::= = NO_DEFAULT
            default_bool ::= = AUTO

            impl_ref_def ::= object_ref_type reference_name multiple_specifier description ;

            object_ref_type ::= object_ref_lexeme

            reference_name ::= name
            reference_name ::= object

            multiple_specifier ::=
            multiple_specifier ::= [ ]

            application_definition ::= CPU name { object_definition_list } description ;

            object_definition_list ::=
            object_definition_list ::= object_definition
            object_definition_list ::= object_definition_list object_definition

            object_definition ::= object_name description ;
            object_definition ::= object_name { parameter_list } description ;

            object_name ::= object name

            parameter_list ::=
            parameter_list ::= parameter
            parameter_list ::= parameter_list parameter

            parameter ::= attribute_name = attribute_value description ;

            description ::=
            description ::= : string

            attribute_name ::= name
            attribute_name ::= object

            attribute_value ::= name
            attribute_value ::= name { parameter_list }
            attribute_value ::= boolean
            attribute_value ::= boolean { parameter_list }
            attribute_value ::= number
            attribute_value ::= float
            attribute_value ::= string
            attribute_value ::= AUTO

            object ::= object_lexeme

            boolean ::= TRUE
            boolean ::= FALSE
        """


def AddImplementationList(node, Accum):
    for k in node._kids:
        if k.type_ == 'implementation_list':
            AddImplementationList(k, Accum)
        else:
            Accum.append(k.exprValue)


def AddNumberList(nl, Accum):
    if len(nl._kids) == 1:
        Accum.append(nl._kids[0].exprValue)
    elif len(nl._kids) == 3:
        Accum.append(nl._kids[2].exprValue)
        AddNumberList(nl._kids[0], Accum)


def AddEnumeratorList(enum, Accum):
    if len(enum._kids) == 1:
        Accum.append(Enumerator(*enum._kids[0].exprValue))
    elif len(enum._kids) == 3:
        Accum.append(Enumerator(*enum._kids[2].exprValue))
        AddEnumeratorList(enum._kids[0], Accum)


def AddImplDefList(dl, Accum):
    for k in dl._kids:
        if k.type_ == 'impl_def_list':
            AddImplDefList(k, Accum)
        else:
            Accum.append(k.exprValue)


def AddParamter(obj, param):
    if param.exprValue.parameterised == True:
        n = AddNestedParameter(obj, param.exprValue.name, param.exprValue.value, param.exprValue.parameterList)
        obj.AddParameter(param.exprValue.name, n)
    else:
        obj.AddParameter(param.exprValue.name, param.exprValue)


def AddParameterList(obj, params):
    for p in params._kids:
        if p.type_ == 'parameter_list':
            AddParameterList(obj, p)
        else:
            AddParamter(obj, p)


def AddNestedParameter(parent, name, value, params):
    n = NestedParameter(name, value, parent)
    for p in params:
        if p.type_ == 'parameter_list':
            if name == 'FILTER':
                pass
            AddParameterList(n, p)
        else:
            AddParamter(n, p)
    return n


class TypeCheck(GenericASTTraversal):

    def __init__(self, ast):
        GenericASTTraversal.__init__(self, ast)
        self.postorder()

    def setDefaultValue(self, left, right):
        if hasattr(right, 'exprValue'):
            left.exprValue = right.exprValue
        else:
            left.exprValue = right.type_  # # AUTO

##
##  Implementation-Definition
##

    def n_OIL_version(self, node):
        pass

    def n_application_definition(self, node):
        name = node[1].exprValue
        descr = node[5].exprValue

    def n_implementation_definition(self, node):
        pass

    def n_implementation_spec(self, node):
        Accum = []
        AddImplementationList(node[2], Accum)
        node.exprValue = Accum
        obj = ImplDefMap[node[0].exprValue]
        for p in node.exprValue:
            obj.defs[p[1].name] = p[1]

    def n_implementation_def(self, node):
        node.exprValue = (node[0].type_, node[0].exprValue)

    def n_impl_attr_def(self, node):
        if node[0].type_ == 'STRING':
            attribute_name = node[2].exprValue
            valueRange = None
            multiple_specifier = node[3].exprValue
            default = node[4].exprValue
            description = node[5].exprValue
        else:
            attribute_name = node[3].exprValue
            valueRange = node[2].exprValue
            multiple_specifier = node[4].exprValue
            default = node[5].exprValue
            description = node[6].exprValue
        node.exprValue = ImplAttrDef(attribute_name, node._kids[0].type_, node._kids[1].exprValue,
            valueRange, multiple_specifier, default, description)

    def n_impl_ref_def(self, node):
        node.exprValue = ImplRefDef(node[0].exprValue, node[1].exprValue, node[2].exprValue, node[3].exprValue)

    def n_enumeration(self, node):
        Accum = []
        AddEnumeratorList(node[1], Accum)
        node.exprValue = EnumRange(Accum)

    def n_enumerator(self, node):
        if len(node) == 3:
            node.exprValue = (node[0].exprValue, node[1].exprValue, node[2].exprValue)
        else:
            node.exprValue = (node[0].exprValue, None, node[1].exprValue)

    def n_default_bool(self, node):
        if node._kids != []:
            node.exprValue = strToBool(node[1].exprValue)
        else:
            node.exprValue = None

    def n_default_name(self, node):
        if node._kids != []:
            self.setDefaultValue(node, node[1])
        else:
            node.exprValue = None

    def n_default_string(self, node):
        if node._kids != []:
            node.exprValue = node[1].exprValue
        else:
            node.exprValue = None

    def n_default_number(self, node):
        if node._kids != []:
            self.setDefaultValue(node, node[1])
        else:
            node.exprValue = None

    def n_default_float(self, node):
        if node._kids != []:
            self.setDefaultValue(node, node[1])
        else:
            node.exprValue = None

    def n_multiple_specifier(self, node):
        if node._kids != []:
            node.exprValue = True
        else:
            node.exprValue = False

    def n_auto_specifier(self, node):
        if node._kids != []:
            node.exprValue = True
        else:
            node.exprValue = False

    def n_reference_name(self, node):
        if node[0].type_ == 'object':
            node.exprValue = node[0][0].attr
        else:
            node.exprValue = node[0].attr

    def n_object(self, node):
        node.exprValue = node[0].attr

    def n_object_ref_type(self, node):
        node.exprValue = node[0].attr

    def n_number_range(self, node):
        if len(node._kids) == 5:
            (rangeMin, rangeMax) = (node[1].exprValue, node[3].exprValue)
            if rangeMin > rangeMax:
                errObj.error('Minimum of NUMBER-Range is greater than maximum.')
                return
            node.exprValue = NumberRangeRange(rangeMin, rangeMax)
        elif len(node._kids) == 3:
            Accum = []
            AddNumberList(node[1], Accum)
            Accum.sort()
            node.exprValue = NumberRangeList(Accum)
        else:
            node.exprValue = None

    def n_float_range(self, node):
        if len(node._kids) == 5:
            (rangeMin, rangeMax) = (node[1].exprValue, node[3].exprValue)
            if rangeMin > rangeMax:
                errObj.error('Minimum of FLOAT-Range is greater than maximum.')
                return
            node.exprValue = FloatRange(rangeMin, rangeMax)
        else:
            node.exprValue = None

    def n_bool_values(self, node):
        if node._kids != []:
            node.exprValue = BoolValues(node[2].exprValue, node[3].exprValue, node[6].exprValue, node[7].exprValue)
        else:
            node.exprValue = None

    def n_impl_parameter_list(self, node):
        if node._kids != []:
            Accum = []
            AddImplDefList(node[1], Accum)
            node.exprValue = Accum
        else:
            node.exprValue = None

    def n_description(self, node):
        if len(node._kids) == 2:
            node.exprValue = node[1].exprValue
        else:
            node.exprValue = None


##
##  Application-Definition
##
    def n_name(self, node):
        node.exprValue = node.attr

    def n_boolean(self, node):
        node.exprValue = node[0].type_

    def n_number(self, node):
        node.exprValue = node.attr

    def n_float(self, node):
        node.exprValue = node.attr

    def n_string(self, node):
        node.exprValue = node.attr

    def n_attribute_name(self, node):
        node.exprValue = node[0].exprValue

    def n_attribute_value(self, node):
        if node[0].type_ == 'AUTO':
            ev = AttributeValue('name', 'AUTO')
        elif node[0].type_ == 'name':
            if len(node._kids) == 4:
                ev = AttributeValue('name', node[0].exprValue, True, node[2])
            else:
                ev = AttributeValue('name', node[0].exprValue)
        elif node[0].type_ == 'boolean':
            if len(node._kids) == 4:
                ev = AttributeValue('boolean',
                                    strToBool(node[0].exprValue), True, node[2])
            else:
                ev = AttributeValue('boolean',
                                    strToBool(node[0].exprValue))
        else:
            ev = AttributeValue(node[0].type_, node[0].exprValue)
        node.exprValue = ev

    def n_parameter(self, node):
        ntype = node[2][0].type_
        node.exprValue = Parameter(node[0].exprValue, node[2].exprValue, node[3].exprValue)

    def n_object_definition(self, node):
        obj = node[0][0].exprValue
        name = node[0][1].exprValue
        obj_map = applicationDefinition[obj]
        if name not in obj_map:
            descr = None
            if len(node) == 3 and node[1].exprValue:
                descr = node[1].exprValue.exprValue
            elif len(node) == 6 and node[4].exprValue:
                descr = node[4].exprValue.exprValue
            obj_map[name] = ObjectDefinition(name, obj, descr)
            o = obj_map[name]
        if len(node._kids) == 6:
            AddParameterList(obj_map[name], node[2])


def scan(data):
    import re
    scanner = Scanner(re.LOCALE)
    return scanner.tokenize(data)


def parse(tokens, start):
    parser = Parser(AST, start)
    r = parser.parse(tokens)
    return r


def BuildAndCheck(ast):
    TypeCheck(ast)
    return ast


def bitCount(mask):
    cnt = 0
    while mask != 0:
        mask >>= 1
        cnt = +1
    return cnt


def partitionByPredicate(list_, pred):
    not_ = lambda f: lambda x, f=f: not f(x)
    if not hasattr(list_, '__iter__'):
        raise TypeError("Parameter 'list_' is not a iteratable.")
    if not callable(pred):
        raise TypeError("Parameter 'pred' must be callable.")
    if list_ is None:
        return ([], [])
    part1 = filter(pred, list_)
    part2 = filter(not_(pred), list_)
    return (part1, part2)


def autoHandler(obj, attr, appDef, implDef, autoList):
    autoList.append((obj, attr, appDef, implDef))
    if obj == 'OS' and attr == 'CC':
        pass  # appDef[1][attr].attribute_value='AUTO'
    elif obj == 'TASK':
        if attr == 'TYPE':
            pass
    elif obj == 'EVENT':
        pass


def getAutoParameter(parameter, autoParameter):
    for param in parameter:
        if isinstance(param, types.ListType):
            getAutoParameter(param, autoParameter)
        else:
            if not isinstance(param, NestedParameter) and param.value == 'AUTO':
                autoParameter.append(param)


def mapTasksToEvents():
    mapping = dict()
    for (task, events) in [(t.name, t['EVENT']) for t in \
        (task for task in applicationDefinition['TASK'].values() if 'EVENT' in task)]:
        for event in events:
            mapping.setdefault(event.value, []).append(task)
    return mapping


def checkBoolean(appDef, objName, attr, implDef, autoList):
    if attr == 'ORTI_DEBUG':
        pass
    if isinstance(appDef[attr], NestedParameter):
        #print 'NESTED: %s' % appDef[attr]
        paramList = None
        if implDef.valueRange is not None:
            paramList = implDef.valueRange.get(appDef[attr].value)
    else:
        paramList = None
        if implDef.valueRange is not None:
            paramList = implDef.valueRange.get(appDef[attr].value)
            '''
            if appDef[attr].value == True:
                paramList = implDef.valueRange.true_parameter_list
            else:
                paramList = implDef.valueRange.false_parameter_list
            '''
        else:
            pass
    if paramList is not None:
        #print "PARAM-LIST: '%s'." % paramList
        for param in paramList:
            checkAttr(appDef[attr], objName, param[1].name, param[1], autoList)
        else:
            pass


def checkEnum(appDef, objName, attr, implDef, autoList):
    if isinstance(appDef[attr], NestedParameter):
        pass
        # print "NESTED: '%s'" % (appDef[attr].value in implDef.valueRange.getValues())
    elif appDef[attr].parameterised:
        print "parameterised:"
        pass


def checkAttr(appDef, objName, attr, implDef, autoList):
    attrValue = None
    if attr in appDef:
        if isinstance(appDef[attr], NestedParameter):
            attrValue = appDef[attr].value
        else:
            if not isinstance(appDef[attr], types.ListType):
                if hasattr(appDef[attr], 'attribute_value'):
                    attrValue = appDef[attr].value
                else:
                    pass
            else:
                pass
    else:
        attrValue = None
    if isinstance(implDef, ImplAttrDef):
        if implDef.default is None and implDef.multiple_specifier  == False and attrValue is None:
            errObj.error("Missing required attribute '%s:%s'."
                         % (objName, attr))
        elif implDef.auto_specifier == False and implDef.default == 'AUTO':
            errObj.error("'%s%s' - Errornous Implementation Definition: Default value 'AUTO' used without 'WITH_AUTO."
                          % (objType, attr))
        elif implDef.default is not None:
            if implDef.default == 'AUTO':
                if appDef.type_ != 'OS' and attr != 'CC':
                    appDef[attr] = Parameter(attr, AttributeValue(implDef.attrType, None))
                objType = appDef.type_
            elif attrValue is None:

#                autoHandler(objType,attr,(appDef,),implDef,autoList)

                if appDef.path != []:
                    errObj.information("Setting '%s::%s' to default value '%s'." % (
                        '::'.join([p[0] for p in appDef.path]), attr, implDef.default)
                    )
                else:
                    errObj.information("Setting '%s::%s' to default value '%s'." % (
                        objName, attr, implDef.default)
                    )

                appDef[attr] = Parameter(attr, AttributeValue(implDef.attrType, implDef.default))
        elif implDef.multiple_specifier == True:
            pass
        if implDef.attrType == 'BOOLEAN' and attrValue is not None:
            checkBoolean(appDef, objName, attr, implDef, autoList)
        elif implDef.attrType == 'ENUM' and attrValue is not None:
            checkEnum(appDef, objName, attr, implDef, autoList)
    elif isinstance(implDef, ImplRefDef):
        if implDef.multiple_specifier == False and attrValue is None:
            errObj.error("Missing required attribute '%s:%s'." % (objName, attr))
    else:
        errObj.fatalError("Definition neither 'ImplAttrDef' nor 'ImplRefDef'.")


def setDefaults():
    Priorities = {}
    ECCx = False
    osCC = None
    numNonTasks = 0
    numPreTasks = 0
    multipleActivations = False
    autoList = []
    eventsToTasksMapping = dict()

    usedResources = {}
    for (key, values) in dict([(t[0], t[1].get('RESOURCE')) for t in applicationDefinition['TASK'].items()]).items():
        if values:
            for value in values:
                usedResources.setdefault(value.value, []).append(key)
    for (key, value) in usedResources.items():

        # # Calculate Ceiling Priority.

        try:
            applicationDefinition['RESOURCE'][key].relativeCeilingPriority = \
                max([applicationDefinition['TASK'][v]['PRIORITY'].value for v in value])
        except KeyError:
            pass  # Ignore non-existent Resources for now.

    for (num, appmode) in enumerate(applicationDefinition['APPMODE'].values()):
        appmode.value = 1 << num

    for (objName, references) in References.items():
        for reference in references:
            if reference.attribute_name == 'SENDINGMESSAGE':
                reference.attribute_name = 'MESSAGE'
            if reference.value not in applicationDefinition[reference.name]:
                errObj.error("Undefined Reference '%s:%s' (%s)." % (objName, reference.value, reference.name))
    events = applicationDefinition['EVENT']
    masks = frozenset([2 ** i for i in range(16)])

#    eventMasks=[e['MASK'].value for e in events]
#    m0=mapTasksToEvents()

    for (eventName, taskNames) in mapTasksToEvents().items():
        mask = 1
        for taskName in taskNames:

#            eventsToTasksMapping.setdefault(eventName,[]).append(taskName)
#            t0=filter(lambda x: x,[t for t in applicationDefinition['TASK'][taskName]['EVENT'] if t.value==eventName])

            eventsForTask = [e.value for e in applicationDefinition['TASK'][taskName]['EVENT']]
            t0 = filter(lambda x: x, [e for e in events.items() if e[0] in eventsForTask])
            (autoEvents, nonAutoEvents) = partitionByPredicate(t0, lambda x: x[1]['MASK'].value == 'AUTO')
            if nonAutoEvents == []:
                events[eventName]['MASK'].value = min(masks)
            else:
                pass
    if not applicationDefinition.get('OS'):
        errObj.error("Missing required Object 'OS'.")
    elif len(applicationDefinition.get('OS')) > 1:
        errObj.error("There must be exactly one 'OS'-Object.")
    if applicationDefinition.get('COM') and len(applicationDefinition.get('COM')) > 1:
        errObj.error("There can be at most one 'COM'-Object.")
    if applicationDefinition.get('NM') and len(applicationDefinition.get('NM')) > 1:
        errObj.error("There can be at most one 'NM'-Object.")
    for (objType, appDef1) in applicationDefinition.items():
        implDefs = ImplDefMap[objType].defs
        implAttrs = implDefs.keys()
        for (objName, appDef2) in appDef1.items():
            if objType == 'EVENT':
                pass
            appAttrs = appDef2.keys()
            autoParameter = []

            if appDef2.has_key('ORTI_DEBUG'):
                    pass

            getAutoParameter(appDef2.values(), autoParameter)
            for p in autoParameter:
                attr = p.name
                implDef = implDefs[attr]

                autoHandler(objType, attr, (appDef1, appDef2), implDef, autoList)
            for attr in implAttrs:
                implDef = implDefs[attr]
                checkAttr(appDef2, objName, attr, implDef, autoList)
            if objType == 'TASK':
                priority = appDef2['PRIORITY'].value
                Priorities.setdefault(priority, []).append(appDef2)
                appDef2['RELATIVE_PRIORITY'] = appDef2['PRIORITY']  # # rename.
                appDef2['RELATIVE_PRIORITY'].name = 'RELATIVE_PRIORITY'
                del appDef2['PRIORITY']
                if appDef2.has_key('SCHEDULE') and appDef2['SCHEDULE'].value == 'FULL':
                    numPreTasks += 1
                else:
                    numNonTasks += 1
                if appDef2['ACTIVATION'].value > 1:
                    multipleActivations = True
                    ECCx = True  # # todo: check NoOfEvent!!!
            elif objType == 'OS':
                osCC = appDef2['CC']
    numberOfDistinctPriorities = len(Priorities)
    if numberOfDistinctPriorities > MAX_PRIORITIES:
        errObj.error('This OSEK-OS-Implementation supports at most %u Priority-Levels (Application uses %u)'
                      % (MAX_PRIORITIES, numberOfDistinctPriorities))
    Info['numberOfDistinctPriorities'] = numberOfDistinctPriorities
    xCC2 = False

    autostartedTasks = filter(lambda x: x['AUTOSTART'].value == True,
        filter(lambda x: isinstance(x['AUTOSTART'], NestedParameter), applicationDefinition['TASK'].values()))
    Info['autostartedTasks'] = autostartedTasks
    autostartedAlarms = filter(lambda x: x['AUTOSTART'].value == True,
        filter(lambda x: isinstance(x['AUTOSTART'], NestedParameter), applicationDefinition['ALARM'].values()))
    Info['autostartedAlarms'] = autostartedAlarms

    if len(autostartedAlarms) > 0:
        for (counter, alarm) in map(lambda x: (applicationDefinition['COUNTER' ][x['COUNTER'].value], x), autostartedAlarms):
            alarmCycleTime = alarm['AUTOSTART']['CYCLETIME'].value
            alarmAlarmTime = alarm['AUTOSTART']['ALARMTIME'].value
            counterMinCycle = counter['MINCYCLE'].value
            counterMaxAllowedValue = counter['MAXALLOWEDVALUE'].value
            if alarmCycleTime < counterMinCycle:
                errObj.error("CYCLETIME (%u) of ALARM '%s' smaller then MINCYCLE (%u) of associated COUNTER '%s'"
                              % (alarmCycleTime, alarm.name, counterMinCycle, counter.name))

    if len(autostartedTasks) + len(autostartedAlarms) == 0:
        errObj.warning('Neither TASKs nor ALARMs are AUTOSTARTed.')


    for (key, value) in [v for v in applicationDefinition['RESOURCE'].items()
                         if not hasattr(v, 'relativeCeilingPriority')]:
        errObj.warning("Unreferenced Resource '%s'." % value.name)
        applicationDefinition['RESOURCE'].pop(key)

    if applicationDefinition['RESOURCE'] == {}:
        Info['useResources'] = False
    else:
        Info['useResources'] = True

    priorityMap = dict()
    for (num, levelObjs) in enumerate(Priorities.values(), 1):
        if len(levelObjs) > 1:
            xCC2 = True
        levelPriority = levelObjs[0]['RELATIVE_PRIORITY'].value
        hasResources = False
        for g in filter(lambda r: r.relativeCeilingPriority  == \
                        levelPriority, [v for v in applicationDefinition['RESOURCE'].values()]):
            g.ceilingPriority = num
            hasResources = True  # todo: xCC2 = True
        activations = 0
        for obj in levelObjs:
            obj['PRIORITY'] = Parameter('PRIORITY',AttributeValue('number', num))
            activations += obj['ACTIVATION'].value
        activations += hasResources and 1 or 0
        priorityMap[levelPriority] = (num, activations)
    Info['priorityMap'] = priorityMap
    if osCC.value == 'AUTO':
        xCC2 = xCC2 or multipleActivations
        if ECCx == False:
            if xCC2 == False:
                cc = 'BCC1'
            else:
                cc = 'BCC2'
        else:
            if xCC2 == False:
                cc = 'ECC1'
            else:
                cc = 'ECC2'
        osCC.value = cc
    else:
        pass


def ParseOil(inputData, implDefOnly=False):
    global data
    global errObj

    errObj = OILError()

    data = BuildAndCheck(parse(scan(inputData), 'file'))
    if not implDefOnly:
        setDefaults()
    return (ImplDefMap, applicationDefinition, Info)
