 # -*- coding: utf-8 -*-

__version__="0.9.0"

__copyright__="""
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
from k_os.OIL.spark import *
from k_os.OIL.Scanner import Scanner
from k_os.OIL.Error import OILError
from k_os.OIL.AST import AST

EMPTY_APP_DEF="CPU cpu {};"

MAX_PRIORITIES=16   ## todo: Cfg. !!!

data=[]

Alarms={}
Appmodes={}
Com={}
Counters={}
Events={}
Ipdus={}
Isrs={}
Messages={}
NetworkMessages={}
Nm={}
Os={}
Resources={}
Tasks={}

Info={} ## unsorted Items.

errObj=None

References={}

AppDefMap={
    "ALARM" : Alarms, "APPMODE" : Appmodes, "COM" : Com, "COUNTER" : Counters, "EVENT":Events,
    "IPDU" : Ipdus, "ISR" : Isrs, "MESSAGE" : Messages, "NETWORKMESSAGE" : NetworkMessages,
    "NM" : Nm, "OS" : Os, "RESOURCE" : Resources, "TASK" : Tasks,
}

UINT32_RANGE=('UINT32',0,(2**32)-1)
INT32_RANGE=('INT32',-(2**31),(2**31)-1)
UINT64_RANGE=('UINT64',0,(2**64)-1)
INT64_RANGE=('INT64',-(2**63),(2**63)-1)
FLOAT_RANGE=('FLOAT',-(1-(2**-24))*(2**127),(1-(2**-24))*(2**128))


NUMERIC_RANGES={
    'UINT32' : UINT32_RANGE, 'INT32' : INT32_RANGE,
    'UINT64' : UINT64_RANGE, 'INT64' : INT64_RANGE,
    'FLOAT'  : FLOAT_RANGE
}


##
##  Helper-Functions.
##
def strToBool(str):
    if str.upper()=='TRUE':
        return True
    elif str.upper()=='FALSE':
        return False
    else:
        raise ValueError("Value must be either 'TRUE' or 'FALSE'")


##
##  Implementation-Definition.
##
class ImplRefDef(object):
    def __init__(self,object_ref_type,name,multiple_specifier,description):
        self.object_ref_type=object_ref_type
        self.name=name
        self.multiple_specifier=multiple_specifier
        self.description=description


class ImplAttrDef(object):
    def __init__(self,name,attrType,auto_specifier,range,multiple_specifier,default,description):
        self.name=name
        self.attrType=attrType
        self.auto_specifier=auto_specifier  # todo: withAuto
        self.range=range
        self.multiple_specifier=multiple_specifier
        self.default=default
        if default is not None:
            if attrType=='ENUM':
                values=[v[0] for v in range.lhs]    #attrType ,UINT32,UINT64,FLOAT
                if self.default=='AUTO':
                    pass
                else:
                    if self.default not in [v[0] for v in range.lhs]:
                        errObj.error("Errornous Implementation Attribute '%s': DEFAULT-Value (%s) out of range." % (self.name,self.default))
            elif attrType in ('UINT32','UINT64','FLOAT'):
                if range is not None:
                    if default<range.lhs or default>range.rhs:
                        errObj.error("Errornous Implementation Attribute '%s': DEFAULT-Value (%s) out of range." % (self.name,self.default))
            elif attrType=='BOOLEAN':
                pass
            else:
                pass
        self.description=description


class BoolValues(object):
    def __init__(self,true_parameter_list,true_description,false_parameter_list,false_description):
        self.true_parameter_list=true_parameter_list
        self.true_description=true_description
        self.false_parameter_list=false_parameter_list
        self.false_description=false_description


class ImplSpec(object):
    def __init__(self,name):
        self.defs=dict()


Alarm_ImplDef=ImplSpec('ALARM')
Appmode_ImplDef=ImplSpec('APPMODE')
Com_ImplDef=ImplSpec('COM')
Counter_ImplDef=ImplSpec('COUNTER')
Event_ImplDef=ImplSpec('EVENT')
Ipdu_ImplDef=ImplSpec('IPDU')
Isr_ImplDef=ImplSpec('ISR')
Message_ImplDef=ImplSpec('MESSAGE')
NetworkMessage_ImplDef=ImplSpec('NETWORKMESSAGE')
Nm_ImplDef=ImplSpec('NM')
Os_ImplDef=ImplSpec('OS')
Resource_ImplDef=ImplSpec('RESOURCE')
Task_ImplDef=ImplSpec('TASK')


ImplDefMap={"ALARM" : Alarm_ImplDef, "APPMODE" : Appmode_ImplDef,
            "COM" : Com_ImplDef, "COUNTER" : Counter_ImplDef,
            "EVENT" : Event_ImplDef, "IPDU" : Ipdu_ImplDef,
            "ISR" : Isr_ImplDef, "MESSAGE" : Message_ImplDef,
            "NETWORKMESSAGE" : NetworkMessage_ImplDef,
            "NM" : Nm_ImplDef, "OS" : Os_ImplDef,
            "RESOURCE" : Resource_ImplDef, "TASK" : Task_ImplDef
}


class Range(object):
    def __init__(self,lhs,rhs):
        self.lhs=lhs
        self.rhs=rhs

    def __repr__(self):
        return '['+str(self.lhs)+' .. '+str(self.rhs)+']'

    def Check(self,value):
        return self.lhs <= value <= self.rhs

    def GetRange(self):
        return (self.lhs,self.rhs)


class NumberRange(Range):
    def __init__(self,lhs,rhs):
        Range.__init__(self,lhs,rhs)


class NumberRangeRange(NumberRange):
    def __init__(self,lhs,rhs):
        NumberRange.__init__(self,lhs,rhs)


class NumberRangeList(NumberRange):
    def __init__(self,lhs):
        NumberRange.__init__(self,lhs,None)

    def __repr__(self):
        return str(self.lhs).replace('L','')

    def Check(self,value):
        return value in self.lhs

    def GetRange(self):
        return self.lhs


class FloatRange(Range):
    def __init__(self,lhs,rhs):
        Range.__init__(self,lhs,rhs)


class EnumRange(Range):
    def __init__(self,lhs):
        Range.__init__(self,lhs,None)

    def GetRange(self):
        return [x[0] for x in self.lhs]

    def Check(self,value):
        return value in self.GetRange()


def GetParameterDefinition(obj,name,path=[]):
    map=ImplDefMap.get(obj.type)
    return map.defs.get(name)


def GetParameterPathToRootObject(obj,param):
    path=[]
    path.append((obj.name,type(obj)))
    tobj=obj
    while tobj.parent:
        tobj=tobj.parent
        if not isinstance(tobj,ObjectDefinition):
            path.insert(0,(tobj.name,tobj.type))
    return (tobj,path)


class ParameterContainer(dict):
    def __init__(self,name):
        self.name=name

    def __getitem__(self,index):
        return self.get(index)


def NumericRangeCheck(attr,impldef,type_range):
    value=attr.attribute_value.value
    if value=='AUTO':
        return True
    if impldef.range is not None:
        if not impldef.range.Check(value):
            range=impldef.range.GetRange()
            errObj.error("%s-Value for attribute '%s' out of defined range, valid: %s." % \
                (impldef.attrType,attr.attribute_name,str(impldef.range)))
            return False
    typename,min,max=type_range
    if not (min <= value <= max):
        errObj.error("%s-Value for attribute '%s' out of datatype-range: valid: [%s .. %s]." % \
            (typename,attr.attribute_name,min,max))
        return False
    return True


##
##  Parser-Objects.
##
class ObjectDefinition(dict):
    def __init__(self,name,type,description=None):
        self.name=name
        self.type=type
        self.description=description
        self.path=[]

        self.parent=None

    def AddParameter(self,name,value):
        pd=GetParameterDefinition(self,name)
        if pd is None:
            errObj.error("Unknown attribute '%s' for object '%s'." % (name,self.type))
        else:
            multipleAttrs=pd.multiple_specifier
            if self.get(name) is not None:
                if multipleAttrs==False:
                    errObj.error("Only a single value allowed for attribute '%s'." % (name))
                    return
            else:
                if multipleAttrs==True:
                    self[name]=[]
            if not self.SemanticCheck(value,pd):
                return
            if multipleAttrs==True:
                self[name].append(value)
            else:
                self[name]=value

    def checkNestedParam(self,attr):
        if attr.implDef.attrType=='BOOLEAN':
            if not isinstance(attr.value,types.BooleanType):
                errObj.error("Parameter '%s::%s' must be of type BOOLEAN." % (
                    self.name,'::'.join([p[0] for p in attr.path]))
                )
                return False
            if attr.value==True:
                validParams=attr.implDef.range.true_parameter_list
            else:
                validParams=attr.implDef.range.false_parameter_list
            validParamValues=[v[1].name for v in validParams]
            for paramType,paramList in attr.items():
                implDef=attr[paramType]
                if isinstance(paramList,types.ListType):
                    for param in paramList:
                        if isinstance(param,NestedParameter):
                            return self.checkNestedParam(param)
                        else:
                            if param.attribute_name not in validParamValues:
                                errObj.error("Invalid Parameter '%s::%s::%s'." % (
                                    self.name,'::'.join([p[0] for p in attr.path]),param.attribute_name)
                                )
                                return False
                            else:
                                implDef=[v[1] for v in validParams if v[1].name==param.attribute_name][0]
                                return self.SemanticCheck(param,implDef)
                else:
                    param=paramList
                    if isinstance(param,NestedParameter):
                        return self.checkNestedParam(param)
                    else:
                        if param.attribute_name not in validParamValues:
                            errObj.error("Invalid Parameter '%s::%s::%s'." % (
                                self.name,'::'.join([p[0] for p in attr.path]),param.attribute_name)
                            )
                            return False
                        else:
                            implDef=[v[1] for v in validParams if v[1].name==param.attribute_name][0]
                            return self.SemanticCheck(param,implDef)

        elif attr.implDef.attrType=='ENUM':
            validParams=attr.implDef.range.lhs
            validParamValues=[v[0] for v in validParams]
            if attr.value not in validParamValues:
                errObj.error("Invalid Value '%s' for Parameter '%s::%s'." % (
                    attr.value,self.name,'::'.join([p[0] for p in attr.path]))
                )
                return False
            else:
                implDef=[x[1][0][1] for x in validParams if x[0]==attr.value]
                paramType,param=attr.items()[0]
                if paramType!=implDef[0].name:
                    errObj.error("%s? Expected '%s::%s::%s'." %
                        (paramType,self.name,
                         '::'.join([p[0] for p in attr.path]),implDef[0].name)
                    )
                    return False
                else:
                    return self.SemanticCheck(param,implDef[0])
        return True

    def SemanticCheck(self,attr,impldef):
        if isinstance(attr,NestedParameter):
            return self.checkNestedParam(attr)
        if attr.attribute_value.value=='AUTO':
            if impldef.auto_specifier==False:
                errObj.error("AUTO-Specifier for attribute '%s' not permitted." % attr.attribute_name)
                return False
        if isinstance(impldef,ImplRefDef):
            References.setdefault(self.name,[]).append(attr)
        else:
            if not self.TypeCompat(attr,impldef):
                return False
            if not self.RangeCheck(attr,impldef):
                return False
        return True

    def RangeCheck(self,attr,impldef):
        formal=impldef.attrType
        value=attr.attribute_value.value

        if formal in ('UINT32','INT32','UINT64','INT64','FLOAT'):
           if not NumericRangeCheck(attr,impldef,NUMERIC_RANGES[formal]):
               return False
        elif formal=='ENUM':
            if not impldef.range.Check(value):
                if value=='AUTO':
                    if impldef.auto_specifier==False:
                        errObj.error("AUTO-Specifier for attribute '%s' not permitted." % attr.attribute_name)
                        return False
                else:
                    enum=impldef.range.GetRange()
                    errObj.error("Undefined emumerator '%s' for attribute '%s', expected %s." %
                        (value,attr.attribute_name,enum)
                        )
                    return False
        return True

    def TypeCompat(self,attr,impldef):
        TypeMap={'UINT32': 'number','INT32': 'number','UINT64': 'number',
                 'INT64': 'number','FLOAT': 'float','ENUM': 'name',
                 'STRING': 'string','BOOLEAN': 'boolean'}

        actual_type=attr.attribute_value.type
        if actual_type=='name' and attr.attribute_value.value=='AUTO':
            return True
        expected_type=TypeMap[impldef.attrType]
        if actual_type=='name' and expected_type=='string':
            return True
        if expected_type!=actual_type:
            errObj.error("<%s>-token for attribute '%s' expected." % (expected_type,attr.attribute_name))
            return False
        else:
            return True


class NestedParameter(dict):
    def __init__(self,name,value,parent):
        self.name=name
        self.value=value
        self.parent=parent
        self.type=name
        self.root,self.path=GetParameterPathToRootObject(self,name)
        self.implDef=GetParameterDefinition(self.root,name,self.path)

    def AddParameter(self,name,value):
        if self.implDef.multiple_specifier==True:
            self.setdefault(name,[]).append(value)
        else:
            self[name]=value


class AttributeValue(object):
    def __init__(self,type,value,parameterised=False,parameter_list=None):
        self.type=type
        self.value=value
        self.parameterised=parameterised
        self.parameter_list=parameter_list

    def __repr__(self):
        return (self.value)


class Parameter(dict):    ## object
    def __init__(self,attribute_name,attribute_value,description=None):
        self.attribute_name=attribute_name
        self.attribute_value=attribute_value
        self.description=description


class Parser(GenericASTBuilder):
    def __init__(self, AST, start):
        GenericASTBuilder.__init__(self, AST, start)

    def error(self, token):
        errObj.error("Syntax error at '%s'" %
            (token,),lineno=token.lineno,filename=token.filename
        )
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


def AddImplementationList(node,Accum):
    for k in node._kids:
        if k.type=='implementation_list':
            AddImplementationList(k,Accum)
        else:
            Accum.append(k.exprValue)


def AddNumberList(nl,Accum):
    if len(nl._kids)==1:
        Accum.append(nl._kids[0].exprValue)
    elif len(nl._kids)==3:
        Accum.append(nl._kids[2].exprValue)
        AddNumberList(nl._kids[0],Accum)


def AddEnumeratorList(enum,Accum):
    if len(enum._kids)==1:
        Accum.append(enum._kids[0].exprValue)
    elif len(enum._kids)==3:
        Accum.append(enum._kids[2].exprValue)
        AddEnumeratorList(enum._kids[0],Accum)


def AddImplDefList(dl,Accum):
    for k in dl._kids:
        if k.type=='impl_def_list':
            AddImplDefList(k,Accum)
        else:
            Accum.append(k.exprValue)


def AddParamter(obj,param):
    if param.exprValue.attribute_value.parameterised==True:
        n=AddNestedParameter(obj,param.exprValue.attribute_name,
            param.exprValue.attribute_value.value,
            param.exprValue.attribute_value.parameter_list)
        obj.AddParameter(param.exprValue.attribute_name,n)
    else:
        obj.AddParameter(param.exprValue.attribute_name,param.exprValue)


def AddParameterList(obj,params):
    for p in params._kids:
        if (p.type=='parameter_list'):
            AddParameterList(obj,p)
        else:
            AddParamter(obj,p)


def AddNestedParameter(parent,name,value,params):
    n=NestedParameter(name,value,parent)
    for p in params:
        if p.type=='parameter_list':
            AddParameterList(n,p)
        else:
            AddParamter(n,p)
    return n


class TypeCheck(GenericASTTraversal):
    def __init__(self, ast):
        GenericASTTraversal.__init__(self, ast)
        self.postorder()

    def setDefaultValue(self,left,right):
        if hasattr(right,'exprValue'):
            left.exprValue=right.exprValue
        else:
            left.exprValue=right.type ## AUTO


##
##  Implementation-Definition
##
    def n_OIL_version(self,node): pass

    def n_application_definition(self,node):
        name=node[1].exprValue
        descr=node[5].exprValue

    def n_implementation_definition(self,node): pass

    def n_implementation_spec(self,node):
        Accum=[]
        AddImplementationList(node[2],Accum)
        node.exprValue=Accum
        obj=ImplDefMap[node[0].exprValue]
        for p in node.exprValue:
            obj.defs[p[1].name]=p[1]

    def n_implementation_def(self,node):
        node.exprValue=(node[0].type,node[0].exprValue)

    def n_impl_attr_def(self,node):
        if node[0].type=='STRING':
            attribute_name=node[2].exprValue
            range=None
            multiple_specifier=node[3].exprValue
            default=node[4].exprValue
            description=node[5].exprValue
        else:
            attribute_name=node[3].exprValue
            range=node[2].exprValue
            multiple_specifier=node[4].exprValue
            default=node[5].exprValue
            description=node[6].exprValue
        node.exprValue=ImplAttrDef(attribute_name,node._kids[0].type,
           node._kids[1].exprValue,range,multiple_specifier,default,description)

    def n_impl_ref_def(self,node):
        node.exprValue=ImplRefDef(node[0].exprValue,node[1].exprValue,
            node[2].exprValue,node[3].exprValue)

    def n_enumeration(self,node):
        Accum=[]
        AddEnumeratorList(node[1],Accum)
        node.exprValue=EnumRange(Accum)

    def n_enumerator(self,node):
        node.exprValue=(node[0].exprValue,node[1].exprValue,node[2].exprValue)

    def n_default_bool(self,node):
        if node._kids!=[]:
            node.exprValue=strToBool(node[1].exprValue)
        else:
            node.exprValue=None

    def n_default_name(self,node):
        if node._kids!=[]:
            self.setDefaultValue(node,node[1])
        else:
            node.exprValue=None

    def n_default_string(self,node):
        if node._kids!=[]:
            node.exprValue=node[1].exprValue
        else:
            node.exprValue=None

    def n_default_number(self,node):
        if node._kids!=[]:
            self.setDefaultValue(node,node[1])
        else:
            node.exprValue=None

    def n_default_float(self,node):
        if node._kids!=[]:
            self.setDefaultValue(node,node[1])
        else:
            node.exprValue=None

    def n_multiple_specifier(self,node):
        if node._kids!=[]:
            node.exprValue=True
        else:
            node.exprValue=False

    def n_auto_specifier(self,node):
        if node._kids!=[]:
            node.exprValue=True
        else:
            node.exprValue=False

    def n_reference_name(self,node):
        if node[0].type=='object':
            node.exprValue=node[0][0].attr
        else:
            node.exprValue=node[0].attr

    def n_object(self,node):
        node.exprValue=node[0].attr

    def n_object_ref_type(self,node):
        node.exprValue=node[0].attr

    def n_number_range(self,node):
        if len(node._kids)==5:
            min,max=node[1].exprValue,node[3].exprValue
            if (min>max):
                errObj.error("Minimum of NUMBER-Range is greater than maximum.")
                return
            node.exprValue=NumberRangeRange(min,max)
        elif len(node._kids)==3:
            Accum=[]
            AddNumberList(node[1],Accum)
            Accum.sort()
            node.exprValue=NumberRangeList(Accum)
        else:
            node.exprValue=None

    def n_float_range(self,node):
        if len(node._kids)==5:
            min,max=(node[1].exprValue,node[3].exprValue)
            if (min>max):
                errObj.error("Minimum of FLOAT-Range is greater than maximum.")
                return
            node.exprValue=FloatRange(min,max)
        else:
            node.exprValue=None

    def n_bool_values(self,node):
        if node._kids!=[]:
            node.exprValue=BoolValues(node[2].exprValue,node[3].exprValue,
                node[6].exprValue,node[7].exprValue)
        else:
            node.exprValue=None

    def n_impl_parameter_list(self,node):
        if node._kids!=[]:
            Accum=[]
            AddImplDefList(node[1],Accum)
            node.exprValue=Accum
        else:
            node.exprValue=None

    def n_description(self,node):
        if len(node._kids)==2:
            node.exprValue=node[1]
        else:
            node.exprValue=None

##
##  Application-Definition
##
    def n_name(self,node):
        node.exprValue=node.attr;

    def n_boolean(self,node):
        node.exprValue=node[0].type;

    def n_number(self, node):
        node.exprValue=node.attr;

    def n_float(self, node):
        node.exprValue=node.attr;

    def n_string(self,node):
        node.exprValue=node.attr;

    def n_attribute_name(self,node):
        node.exprValue=node[0].exprValue

    def n_attribute_value(self,node):
        if node[0].type=='AUTO':
            ev=AttributeValue('name','AUTO')
        elif node[0].type=='name':
            if len(node._kids)==4:
                ev=AttributeValue('name',node[0].exprValue,True,node[2])
            else:
                ev=AttributeValue('name',node[0].exprValue)
        elif node[0].type=='boolean':
            if len(node._kids)==4:
                ev=AttributeValue('boolean',strToBool(node[0].exprValue),True,node[2])
            else:
                ev=AttributeValue('boolean',strToBool(node[0].exprValue))
        else:
            ev=AttributeValue(node[0].type,node[0].exprValue)
        node.exprValue=ev

    def n_parameter(self,node):
        ntype=node[2][0].type;
        node.exprValue=Parameter(node[0].exprValue,node[2].exprValue,node[3].exprValue)

    def n_object_definition(self,node):
        obj=node[0][0].exprValue
        name=node[0][1].exprValue
        obj_map=AppDefMap[obj]
        if name not in obj_map:
            descr=None
            if len(node)==3 and node[1].exprValue:
                descr=node[1].exprValue.exprValue
            elif len(node)==6 and node[4].exprValue:
                descr=node[4].exprValue.exprValue
            obj_map[name]=ObjectDefinition(name,obj,descr)
            o=obj_map[name]
        if len(node._kids)==6:
            AddParameterList(obj_map[name],node[2])


def scan(input):
    scanner=Scanner(re.LOCALE)
    return scanner.tokenize(input)


def parse(tokens,start):
    parser=Parser(AST,start)
    r=parser.parse(tokens)
    return r


def BuildAndCheck(ast):
    TypeCheck(ast)
    return ast


def bitCount(mask):
    cnt=0
    while mask!=0:
        mask>>=1
        cnt=+1
    return cnt


def partitionByPredicate(list_,pred):
    not_ = lambda f: lambda x, f=f: not f(x)
    if not hasattr(list_,'__iter__'):
        raise TypeError("Parameter 'list_' is not a iteratable.")
    if not callable(pred):
        raise TypeError("Parameter 'pred' must be callable.")
    if list_ is None:
        return ([],[])
    part1=filter(pred,list_)
    part2=filter(not_(pred),list_)
    return (part1,part2)


def autoHandler(obj,attr,appDef,implDef,autoList):
    autoList.append((obj,attr,appDef,implDef))
    if obj=='OS' and attr=='CC':
        pass # appDef[1][attr].attribute_value='AUTO'
    elif obj=='TASK':
        if attr=='TYPE':
            pass
    elif obj=='EVENT':
        pass


def getAutoParameter(parameter,autoParameter):
    for param in parameter:
        if isinstance(param,types.ListType):
            getAutoParameter(param,autoParameter)
        else:
            if not isinstance(param,NestedParameter) and param.attribute_value.value=='AUTO':
                autoParameter.append(param)


def mapTasksToEvents():
    mapping=dict()
    for task,events in [
        (t.name,t['EVENT']) for t in (task for task in AppDefMap['TASK'].values() if 'EVENT' in task)
    ]:
        for event in events:
            mapping.setdefault(event.attribute_value.value,[]).append(task)
    return mapping


def checkBoolean(appDef,objName,attr,implDef,autoList):
    if attr=='ORTI_DEBUG':
        pass
    if isinstance(appDef[attr],NestedParameter):
        print "NESTED"
    else:
        paramList=None
        if implDef.range is not None:
            if appDef[attr].attribute_value.value==True:
                paramList=implDef.range.true_parameter_list
            else:
                paramList=implDef.range.false_parameter_list
        else:
            pass
        if paramList is not None:
            print "PARAM-LIST: '%s'." % paramList
            for param in paramList:
                checkAttr(appDef[attr],objName,param[1].name,param[1],autoList)
        else:
            pass


def checkEnum(appDef,objName,attr,implDef,autoList):
    if isinstance(appDef[attr],NestedParameter):
        print "NESTED"
    elif appDef[attr].attribute_value.parameterised:
        pass


def checkAttr(appDef,objName,attr,implDef,autoList):
    attrValue=None
    if attr in appDef:
        if isinstance(appDef[attr],NestedParameter):
            attrValue=appDef[attr].value
        else:
            if not isinstance(appDef[attr],types.ListType):
                if hasattr(appDef[attr],'attribute_value'):
                    attrValue=appDef[attr].attribute_value.value
                else:
                    pass
            else:
                pass
    else:
        attrValue=None
    if isinstance(implDef,ImplAttrDef):
        if implDef.default is None and implDef.multiple_specifier==False and attrValue is None:
            errObj.error("Missing required attribute '%s:%s'." % (objName,attr))
        elif implDef.auto_specifier==False and implDef.default=='AUTO':
            errObj.error(
                "'%s%s' - Errornous Implementation Definition: Default value 'AUTO' used without 'WITH_AUTO." %
                (objType,attr)
            )
        elif implDef.default is not None:
            if implDef.default=='AUTO':
                if appDef.type!='OS' and attr!='CC':
                    appDef[attr]=Parameter(attr,AttributeValue(implDef.attrType,None))
                objType=appDef.type
#                autoHandler(objType,attr,(appDef,),implDef,autoList)
            elif attrValue is None:
                errObj.information("Setting '%s:%s' to  default value '%s'." % (objName,attr,implDef.default))
                appDef[attr]=Parameter(attr,AttributeValue(implDef.attrType,implDef.default))
        elif implDef.multiple_specifier==True:
            pass
        if implDef.attrType=='BOOLEAN':
            checkBoolean(appDef,objName,attr,implDef,autoList)
        elif implDef.attrType=='ENUM':
            checkEnum(appDef,objName,attr,implDef,autoList)
    elif isinstance(implDef,ImplRefDef):
        if implDef.multiple_specifier==False and attrValue is None:
            errObj.error("Missing required attribute '%s:%s'." % (objName,attr))
    else:
        errObj.fatalError("Definition neither 'ImplAttrDef' nor 'ImplRefDef'.")


def setDefaults():
    Priorities={}
    ECCx=False
    osCC=None
    numNonTasks=0
    numPreTasks=0
    multipleActivations=False
    autoList=[]
    eventsToTasksMapping=dict()

    usedResources={}
    for key,values in dict([(t[0],t[1].get('RESOURCE')) for t in AppDefMap['TASK'].items()]).items():
        if values:
            for value in values:
                usedResources.setdefault(value.attribute_value.value,[]).append(key)
    for key,value in usedResources.items():
        ## Calculate Ceiling Priority.
        try:
            AppDefMap['RESOURCE'][key].relativeCeilingPriority=max(
                [AppDefMap['TASK'][v]['PRIORITY'].attribute_value.value for v in value]
            )
        except KeyError:
            pass    # Ignore non-existent Resources for now.

    for num,appmode in enumerate(AppDefMap['APPMODE'].values()):
        appmode.value=1<<num

    for objName,references in References.items():
        for reference in references:
            if reference.attribute_value.value not in AppDefMap[reference.attribute_name]:
                errObj.error("Undefined Reference '%s:%s' (%s)." %
                    (objName,reference.attribute_value.value,reference.attribute_name)
                )
    events=AppDefMap['EVENT']
    masks=frozenset([2**i for i in range(16)])
#    eventMasks=[e['MASK'].attribute_value.value for e in events]
#    m0=mapTasksToEvents()
    for eventName,taskNames in mapTasksToEvents().items():
        mask=1
        for taskName in taskNames:
#            eventsToTasksMapping.setdefault(eventName,[]).append(taskName)
#            t0=filter(lambda x: x,[t for t in AppDefMap['TASK'][taskName]['EVENT'] if t.attribute_value.value==eventName])
            eventsForTask=[e.attribute_value.value for e in AppDefMap['TASK'][taskName]['EVENT']]
            t0=filter(lambda x:x,[e for e in events.items() if e[0] in eventsForTask])
            autoEvents,nonAutoEvents=partitionByPredicate(t0,lambda x: x[1]['MASK'].attribute_value.value=='AUTO')
            if nonAutoEvents==[]:
                events[eventName]['MASK'].attribute_value.value=min(masks)
            else:
                pass

    autostartedTasks=filter(lambda x: x['AUTOSTART'].value==True,
        (filter(lambda x: isinstance(x['AUTOSTART'],NestedParameter),AppDefMap['TASK'].values()))
    )
    Info['autostartedTasks']=autostartedTasks
    autostartedAlarms=filter(lambda x: x['AUTOSTART'].value==True,
        (filter(lambda x: isinstance(x['AUTOSTART'],NestedParameter),AppDefMap['ALARM'].values()))
    )
    Info['autostartedAlarms']=autostartedAlarms

    if len(autostartedAlarms)>0:
        for counter,alarm in map(lambda x:(AppDefMap['COUNTER'][x['COUNTER'].attribute_value.value],x),
        autostartedAlarms):
            alarmCycleTime=alarm['AUTOSTART']['CYCLETIME'].attribute_value.value
            alarmAlarmTime=alarm['AUTOSTART']['ALARMTIME'].attribute_value.value
            counterMinCycle=counter['MINCYCLE'].attribute_value.value
            counterMaxAllowedValue=counter['MAXALLOWEDVALUE'].attribute_value.value
            if alarmCycleTime<counterMinCycle:
                errObj.error("CYCLETIME (%u) of ALARM '%s' smaller then MINCYCLE (%u) of associated COUNTER '%s'" %
                    (alarmCycleTime,alarm.name,counterMinCycle,counter.name)
                )

    if len(autostartedTasks)+len(autostartedAlarms)==0:
        errObj.warning("Neither TASKs nor ALARMs are AUTOSTARTed.")

    if not AppDefMap.get("OS"):
        errObj.error("Missing required Object 'OS'.")
    elif len(AppDefMap.get("OS"))>1:
        errObj.error("There must be exactly one 'OS'-Object.")
    if AppDefMap.get("COM") and len(AppDefMap.get("COM"))>1:
        errObj.error("There can be at most one 'COM'-Object.")
    if AppDefMap.get("NM") and len(AppDefMap.get("NM"))>1:
        errObj.error("There can be at most one 'NM'-Object.")
    for objType,appDef1 in AppDefMap.items():
        implDefs=ImplDefMap[objType].defs
        implAttrs=implDefs.keys()
        for objName,appDef2 in appDef1.items():
            if objType=='EVENT':
                pass
            appAttrs=appDef2.keys()
            autoParameter=[]
            getAutoParameter(appDef2.values(),autoParameter)
            for p in autoParameter:
                attr=p.attribute_name
                implDef=implDefs[attr]
                autoHandler(objType,attr,(appDef1,appDef2),implDef,autoList)
#            for attr in filter(lambda x: x not in appAttrs,implAttrs):
            for attr in implAttrs:
                implDef=implDefs[attr]
                checkAttr(appDef2,objName,attr,implDef,autoList)
            if objType=='TASK':
                priority=appDef2['PRIORITY'].attribute_value.value
                Priorities.setdefault(priority,[]).append(appDef2)
                appDef2['RELATIVE_PRIORITY']=appDef2['PRIORITY']    ## rename.
                appDef2['RELATIVE_PRIORITY'].attribute_name='RELATIVE_PRIORITY'
                del appDef2['PRIORITY']
                if appDef2['SCHEDULE'].attribute_value.value=='FULL':
                    numPreTasks+=1
                else:
                    numNonTasks+=1
                if appDef2['ACTIVATION'].attribute_value.value>1:
                    multipleActivations=True
                    ECCx=True   ## todo: check NoOfEvent!!!
            elif objType=='OS':
                osCC=appDef2['CC']
    numberOfDistinctPriorities=len(Priorities)
    if numberOfDistinctPriorities>MAX_PRIORITIES:
        errObj.error(
            "This OSEK-OS-Implementation supports at most %u Priority-Levels (Application uses %u)" %
            (MAX_PRIORITIES,numberOfDistinctPriorities)
        )
    Info['numberOfDistinctPriorities']=numberOfDistinctPriorities
    xCC2=False

    for key,value in [v for v in AppDefMap['RESOURCE'].items() if not hasattr(v,'relativeCeilingPriority')]:
        errObj.warning("Unreferenced Resource '%s'." % value.name)
        AppDefMap['RESOURCE'].pop(key)

    if AppDefMap['RESOURCE']=={}:
        Info['useResources']=False
    else:
        Info['useResources']=True

    priorityMap=dict()
    for num,levelObjs in enumerate(Priorities.values(),1):
        if len(levelObjs)>1:
            xCC2=True
        levelPriority=levelObjs[0]['RELATIVE_PRIORITY'].attribute_value.value
        hasResources=False
        for g in filter(lambda r: r.relativeCeilingPriority==levelPriority,
            [v for v in AppDefMap['RESOURCE'].values()]):
            g.ceilingPriority=num
            hasResources=True   # todo: xCC2 = True
        activations=0
        for obj in levelObjs:
            obj['PRIORITY']=Parameter('PRIORITY',AttributeValue('number',num))
            activations+=obj['ACTIVATION'].attribute_value.value
        activations+=hasResources and 1 or 0
        priorityMap[levelPriority]=(num,activations)
    Info['priorityMap']=priorityMap
    if osCC.attribute_value.value=='AUTO':
        xCC2=xCC2 or multipleActivations
        if ECCx==False:
            if xCC2==False: cc='BCC1'
            else:           cc='BCC2'
        else:
            if xCC2==False: cc='ECC1'
            else:           cc='ECC2'
        osCC.attribute_value.value=cc
    else:
        pass
        "WARNING: Calculated CC, requested CC"
        "ERROR: Incompatible CC, required: %s"


def ParseOil(input,implDefOnly=False):
    global data
    global errObj

    errObj=OILError()

    data=BuildAndCheck(parse(scan(input),'file'))
    if not implDefOnly:
        setDefaults()
    return (ImplDefMap,AppDefMap,Info)
