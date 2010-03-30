 # -*- coding: latin-1 -*-
 
__version__="0.9.0"

__copyright__="""
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 
  (C) 2007-2009 by Christoph Schueler <chris@konnex-tools.de>
  
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
"""

from spark import *
from Scanner import Scanner
from AST import AST
import GenORTI
import GenCfg


data=[]

Alarms={}
Appmodes={}
Com={}
Counters={}
Events={}
Idpus={}
Isrs={}
Messages={}
Nm={}
Os={}
Resources={}
Tasks={}

AppDefMap={"ALARM":Alarms,"APPMODE":Appmodes,"COM":Com,"COUNTER":Counters,
        "EVENT":Events,"IDPU":Idpus,"ISR":Isrs,"MESSAGE":Messages,
        "NM":Nm,"OS":Os,"RESOURCE":Resources,"TASK":Tasks}


UINT32_RANGE=('UINT32',0,(2**32)-1)
INT32_RANGE=('INT32',-(2**31),(2**31)-1)
UINT64_RANGE=('UINT64',0,(2**64)-1)
INT64_RANGE=('INT64',-(2**63),(2**63)-1)
FLOAT_RANGE=('FLOAT',-(1-(2**-24))*(2**127),(1-(2**-24))*(2**128))

NUMERIC_RANGES={'UINT32': UINT32_RANGE,'INT32': INT32_RANGE,'UINT64':
                UINT64_RANGE,'INT64': INT64_RANGE,'FLOAT': FLOAT_RANGE}

#
# Implementation-Definition
#

class ImplRefDef(object):
    def __init__(self,object_ref_type,name,multiple_specifier,description):
        self.object_ref_type=object_ref_type
        self.name=name
        self.multiple_specifier=multiple_specifier
        self.description=description

class ImplAttrDef(object):
    def __init__(self,name,attrType,auto_specifier,range,
                 multiple_specifier,default,description):
        self.name=name
        self.attrType=attrType
        self.auto_specifier=auto_specifier
        self.range=range
        self.multiple_specifier=multiple_specifier
        self.default=default        
        self.description=description

class BoolValues(object):
    def __init__(self,true_parameter_list,true_description,false_parameter_list,
                 false_description):
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
Idpu_ImplDef=ImplSpec('IDPU')
Isr_ImplDef=ImplSpec('ISR')
Message_ImplDef=ImplSpec('MESSAGE')
Nm_ImplDef=ImplSpec('NM')
Os_ImplDef=ImplSpec('OS')
Resource_ImplDef=ImplSpec('RESOURCE')
Task_ImplDef=ImplSpec('TASK')

ImplDefMap={"ALARM":Alarm_ImplDef,"APPMODE":Appmode_ImplDef,"COM":Com_ImplDef,
            "COUNTER":Counter_ImplDef,"EVENT":Event_ImplDef,"IDPU":Idpu_ImplDef,
            "ISR":Isr_ImplDef,"MESSAGE":Message_ImplDef,"NM":Nm_ImplDef,
            "OS":Os_ImplDef,"RESOURCE":Resource_ImplDef,"TASK":Task_ImplDef}

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
    ##
    ## returns None | Parameter-Description
    ##
    map=ImplDefMap[obj.type]
    if path==[]:
        return map.defs.get(name)
    for p in path:
        pass

def GetParameterPathToRootObject(obj,param):
    path=[]
    path.append(obj.name)
    parent=obj.parent
    while not isinstance(parent,ObjectDefinition):
        path.insert(0,parent.name)
        parent=parent.parent
    return (parent,path)

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
            print "%s-Value for attribute '%s' out of defined range, valid: %s." % \
                (impldef.attrType,attr.attribute_name,str(impldef.range))
            return False    
    typename,min,max=type_range
    if not (min <= value <= max):
        print "%s-Value for attribute '%s' out of datatype-range: valid: [%s .. %s]" % \
            (typename,attr.attribute_name,min,max)
        return False
    return True

def RangeCheck(attr,impldef):
    formal=impldef.attrType
    value=attr.attribute_value.value

    if formal in ['UINT32','INT32','UINT64','INT64','FLOAT']:
       if not NumericRangeCheck(attr,impldef,NUMERIC_RANGES[formal]):
           return False        
    elif formal=='ENUM':        
        if not impldef.range.Check(value):
            enum=impldef.range.GetRange()
            print "Undefined emumerator '%s' for attribute '%s', expected %s." % (value,attr.attribute_name,enum)
            return False
    return True

def TypeCompat(attr,impldef):
    TypeMap={'UINT32': 'number','INT32': 'number','UINT64': 'number',
             'INT64': 'number','FLOAT': 'float','ENUM': 'name',
             'STRING': 'string','BOOLEAN': 'boolean'}
    
    actual_type=attr.attribute_value.type
    if actual_type=='name' and attr.attribute_value.value=='AUTO':
        return True
    expected_type=TypeMap[impldef.attrType]
    if expected_type!=actual_type:
        print "<%s>-token for attribute '%s' expected." % (expected_type,attr.attribute_name)
        return False
    else:
        return True

def SemanticCheck(attr,impldef):
    if isinstance(attr,NestedParameter):
        return
    if attr.attribute_value.value=='AUTO':
        if impldef.auto_specifier==False:
            print "AUTO-Specifier for attribute '%s' not permitted." % attr.attribute_name
            return False
    if isinstance(impldef,ImplRefDef):
        if attr.attribute_value.value not in AppDefMap[impldef.name]:
            print "Reference to undefined %s '%s'" % (impldef.name,attr.attribute_value.value)
            return False
    else:
        if not TypeCompat(attr,impldef):
            return False
        if not RangeCheck(attr,impldef):
            return False        
    return True

#
# Parser-Objects.
#
class ObjectDefinition(dict):
    def __init__(self,name,type,description=None):
        self.name=name
        self.type=type
        self.description=description  
        self.path=[]

    def AddParameter(self,name,value):
        pd=GetParameterDefinition(self,name)
        
        if pd is None:
            print "unknown attribute '%s' for object '%s'" % (name,self.type)            
        else:
            if self.get(name) is not None:
                if pd.multiple_specifier==False:
                    print "Attribute '%s' already defined." % (name)
                    return            
            else:
                self[name]=[]

            if not SemanticCheck(value,pd):
                return
            self[name].append(value)


class NestedParameter(object):
    def __init__(self,name,parent):
        self.name=name
        self.parent=parent
        self.params=dict()
        self.root,self.path=GetParameterPathToRootObject(self,name)
        pd=GetParameterDefinition(self.root,name,self.path)

    def AddParameter(self,name,value):        

        if name in self.params:            
            self.params[name].append(value)
        else:
            self.params[name]=[]
            self.params[name].append(value)

class AttributeValue(object):
    def __init__(self,type,value,parameterised=False,parameter_list=None):
        self.type=type
        self.value=value
        self.parameterised=parameterised
        self.parameter_list=parameter_list

    def __repr__(self):
        return (self.value)

class Parameter(object):
    def __init__(self,attribute_name,attribute_value,description):
        self.attribute_name=attribute_name
        self.attribute_value=attribute_value
        self.description=description


class Parser(GenericASTBuilder):
    def __init__(self, AST, start):
        GenericASTBuilder.__init__(self, AST, start)

    def error(self, token):
        print token
        print "*** Syntax error at '%s' (line %u) ***" % (token, token.lineno)
        raise SystemExit

    def p_expr(self, args):
        """
            file ::= OIL_version implementation_definition application_definition

            OIL_version ::=	OIL_VERSION = version description ;            

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
            impl_parameter_list ::=	{ impl_def_list }

            impl_def_list ::=
            impl_def_list ::= implementation_def
            impl_def_list ::= implementation_def impl_def_list

            auto_specifier ::=
            auto_specifier ::= WITH_AUTO

            number_range ::=
            number_range ::= [ number range_op number ]
            number_range ::= [ number_list ]

            number_list ::=	number
            number_list ::= number_list , number

            default_number ::=	
            default_number ::= = number
            default_number ::= = NO_DEFAULT
            default_number ::= = AUTO

            float_range ::=
            float_range ::=	[ float range_op float ]

            default_float ::=
            default_float ::= = float
            default_float ::= = NO_DEFAULT
            default_float ::= = AUTO

            enumeration ::= [ enumerator_list ]

            enumerator_list ::= enumerator
            enumerator_list ::=	enumerator_list , enumerator

            enumerator ::= name description
            enumerator ::= name impl_parameter_list description

            bool_values ::=
            bool_values ::= [ TRUE impl_parameter_list description , FALSE impl_parameter_list description ]

            default_name ::=
            default_name ::= = name
            default_name ::= = NO_DEFAULT
            default_name ::= = AUTO

            default_string ::=
            default_string ::=	= string
            default_string ::=	= NO_DEFAULT
            default_string ::=	= AUTO

            default_bool ::=
            default_bool ::= = boolean
            default_bool ::= = NO_DEFAULT
            default_bool ::= = AUTO

            impl_ref_def ::= object_ref_type reference_name multiple_specifier description ;

            object_ref_type ::= object_ref_lexeme

            reference_name ::= name
            reference_name ::= object

            multiple_specifier ::=
            multiple_specifier ::=	[ ]

            application_definition ::=	CPU name { object_definition_list } description ;

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
            description ::=	: string

            attribute_name ::= name
            attribute_name ::= object
            
            attribute_value ::=	name
            attribute_value ::=	name { parameter_list }
            attribute_value ::=	boolean
            attribute_value ::=	boolean { parameter_list }
            attribute_value ::=	number
            attribute_value ::=	float
            attribute_value ::=	string
            attribute_value ::=	AUTO

            object ::= object_lexeme

            boolean ::= TRUE
            boolean ::= FALSE
                        
        """
##line_no
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

def AddNestedParameter(parent,name,params):
    n=NestedParameter(name,parent)
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

#
#   Implementation-Definition
#
    def n_OIL_version(self,node):
        print "OIL-Version: ",node[2][0].exprValue

    def n_application_definition(self,node):
        name=node[1].exprValue
        descr=node[5].exprValue    

    def n_implementation_definition(self,node):
        pass
        
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
            node.exprValue=node[1].exprValue
        else:
            node.exprValue=None

    def n_default_name(self,node):
        if node._kids!=[]:
            node.exprValue=node[1].exprValue
        else:
            node.exprValue=None

    def n_default_string(self,node):
        if node._kids!=[]:
            node.exprValue=node[1].exprValue
        else:
            node.exprValue=None               

    def n_default_number(self,node):
        if node._kids!=[]:
            node.exprValue=node[1].exprValue
        else:
            node.exprValue=None

    def n_default_float(self,node):
        if node._kids!=[]:
            node.exprValue=node[1].exprValue
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
                print "Minimum of NUMBER-Range is greater than maximum."
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
                print "Minimum of FLOAT-Range is greater than maximum."
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
            
#
#   Application-Definition
#
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
                ev=AttributeValue('boolean',node[0].exprValue,True,node[2])
            else:
                ev=AttributeValue('boolean',node[0].exprValue)
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
            obj_map[name]=ObjectDefinition(name,obj)
            o=obj_map[name]
        if len(node._kids)==6:
            AddParameterList(obj_map[name],node[2])

def scan(f):
    input=f.read()
    scanner=Scanner(re.LOCALE)
    return scanner.tokenize(input)

def parse(tokens,start):
    parser=Parser(AST,start)
    r=parser.parse(tokens)
    return r

def BuildAndCheck(ast):
    TypeCheck(ast)
    return ast

def ParseOil(fname):        
    global data

    try:
        f=open(fname)
    except IOError:
        print "could not open file '%s'.\n" % (fname)
        sys.exit(1)
    else:
        data=BuildAndCheck(parse(scan(f),'file'))
        f.close()
        return (ImplDefMap,AppDefMap)

def test():
    ImplDef,AppDef=ParseOil(r"test.oil")
    GenORTI.Generate("test.ort",AppDef)
    GenCfg.Generate("test",AppDef)
        
if __name__=="__main__":
    test()

