/*
**  k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
**
**  (C) 2007-2013 by Christoph Schueler <github.com/Christoph2,
**                                       cpu12.gems@googlemail.com>
**
**  All Rights Reserved
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; version 2 of the License.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**  s. FLOSS-EXCEPTION.txt
**
*/

grammar osekoil;

///
/// Requires ANTLR >= 3.4!
///

options {
// debug = true;
   language = Python;
   output = AST;
   ASTLabelType = CommonTree;
   superClass = BaseParser;
}

tokens {
   FILE;
   OIL_VERSION;
   IMPLEMENTATION_DEFINITION;
   APPLICATION_DEFINITION;
   IMPL_REF_DEF;
   OBJECT_DEFINITION;
   IMPLEMENTATION_SPEC;
   ATTRIBUTE_NAME;
   REFERENCE_NAME;
   PARAMETER;
   ENUMERATION;
   ENUMERATOR_LIST;
   ENUMERATOR;
   BOOL_VALUES;
   AUTO_SPECIFIER;
   MULTIPLE_SPECIFIER;
   IMPLEMENTATION_DEF;
   IMPLEMENTATION_SPEC_LIST;
   IMPLEMENTATION_LIST;
   IMPL_PARAMETER_LIST;
   IMPL_ATTR_DEF;
   IMPL_DEF_LIST;
   BOOLEAN;
   NUMBER;
   NUMBER_RANGE;
   NUMBER_LIST;
   FLOAT_DEF;
   FLOAT_RANGE;
   DEFAULT_NUMBER;
   DEFAULT_FLOAT;
   DEFAULT_NAME;
   DEFAULT_STRING;
   DEFAULT_BOOL;
   DESCRIPTION;
   OBJECT;
   OBJECT_DEFINITION_LIST;
   OBJECT_REF_TYPE;
   OBJECT_NAME;
   PARAMETER_LIST;
   ATTRIBUTE_VALUE;
   ID_VALUE;
   BOOL_VALUE;
   NUMBER_VALUE;
   FLOAT_VALUE;
   STRING_VALUE;
   AUTO_VALUE;

    SHIT;
}


@parser::header {
   from kosek.BaseParser import BaseParser
   import kosek.ApplicationDefinition as ApplicationDefinition
   import kosek.ImplementationDefinition as ImplementationDefinition
   from kosek.ImplementationDefinition.Extras import Extras
   from kosek.ApplicationDefinition.Parameter import ParameterList
}

@parser::members {
   implDefinition = dict()
   standardResources = []
   internalResources = []
   linkedResources = []

}

@lexer::init {
   pass
}


file returns[value]
   : oilVersion implementationDefinition applicationDefinition
   {
   $value = ($implementationDefinition.value, $applicationDefinition.value)
   }
   -> ^(FILE oilVersion implementationDefinition applicationDefinition)
   ;


oilVersion
   : 'OIL_VERSION' ASSIGN STRING description? SEMI
   -> ^(OIL_VERSION STRING description?)
   ;

//
//
// Implementation Definition.
//
//
implementationDefinition returns[value]
   :  'IMPLEMENTATION' ID '{' implementationSpecList '}' description? SEMI
   {
   $value = ($implementationSpecList.value, $description.value)
   self.implDefinition = $implementationSpecList.value
   }
   -> ^(IMPLEMENTATION_DEFINITION implementationSpecList description?)
   ;


implementationSpecList returns[value]
@init {
    myList = []
}
@after {
    builder = ImplementationDefinition.ImplementationDefinitionBuilder(self, myList)
    $value = builder.postProcessing()
}
   :  specs += (implementationSpec { myList.append($implementationSpec.value) } ) *
   -> ^(IMPLEMENTATION_SPEC_LIST implementationSpec*)
   ;

implementationSpec returns[value]
scope {
    objectType
}
@init {
    $implementationSpec::objectType = []
}
   :  object { $implementationSpec::objectType = $object.text } '{' implementationList '}' description? SEMI
   {
$value = ImplementationDefinition.ImplementationSpec($implementationSpec::objectType, $implementationList.value, $description.value)
   }
      -> ^(IMPLEMENTATION_SPEC object implementationList description?)
   ;

object
   :  id = 'OS'
   |  id = 'TASK'
   |  id = 'COUNTER'
   |  id = 'ALARM'
   |  id = 'RESOURCE'
   |  id = 'EVENT'
   |  id = 'ISR'
   |  id = 'MESSAGE'
   |  id = 'COM'
   |  id = 'NM'
   |  id = 'APPMODE'
   |  id = 'IPDU'
   |  id = 'NETWORKMESSAGE'
   -> ^(OBJECT $id)
   ;

implementationList returns[value]
@init {
    myList = []
}
@after {
    $value = myList
}
   :  ids += (implementationDef { myList.append($implementationDef.values) } )*
   -> ^(IMPLEMENTATION_LIST implementationDef*)
   ;

implementationDef returns[values]
@after {
###
}
   :  id = implAttrDef
   {
   $values = ImplementationDefinition.AttributeDefinitionFactory($id.dataType, $id.attrName, $id.auto, $id.mult, $id.deflt, $id.desc, $id.extra)
   }
   -> ^(IMPLEMENTATION_DEF implAttrDef)
   | id2 = implRefDef
   {
   $values = ImplementationDefinition.ImplRefDef($id2.objectType, $id2.name, $id2.mult, $id2.desc)
   }
   -> ^(IMPLEMENTATION_DEF implRefDef)
   ;

implAttrDef returns [dataType, attrName, auto, mult, deflt, desc, extra]
scope {
    implAttrName
}

@init {

}
@after {

    if $deflt:
       self.appendDefault($implementationSpec::objectType, $attrName, $deflt)
    if $auto:
       self.appendAutos($implementationSpec::objectType, $attrName)
}
   :  'UINT32' autoSpecifier? numberRange? attributeName multipleSpecifier? defaultNumber? description? SEMI
      {
   $dataType = ImplementationDefinition.ImplAttrType.UINT32
   $attrName = $attributeName.text

   $implAttrDef::implAttrName = $attributeName.text


   $extra = Extras(Extras.NUMBER_RANGE, {'range' : $numberRange.range, 'numberFrom': $numberRange.nfrom,
       'numberTo': $numberRange.nto})
   if $autoSpecifier.text: $auto = True
   else: $auto = False
   if $multipleSpecifier.text: $mult = True
   else: $mult = False
   $deflt = $defaultNumber.value
   if $description.value: $desc = $description.value
   else: $desc = ""
      }
      -> ^(IMPL_ATTR_DEF 'UINT32' attributeName autoSpecifier? numberRange? multipleSpecifier? defaultNumber? description?)
   |  'INT32'     autoSpecifier? numberRange? attributeName multipleSpecifier? defaultNumber? description? SEMI
      {
   $dataType = ImplementationDefinition.ImplAttrType.INT32
   $attrName = $attributeName.text

   $implAttrDef::implAttrName = $attributeName.text

   $extra = Extras(Extras.NUMBER_RANGE, {'range' : $numberRange.range, 'numberFrom': $numberRange.nfrom,
       'numberTo': $numberRange.nto})
   if $autoSpecifier.text: $auto = True
   else: $auto = False
   if $multipleSpecifier.text: $mult = True
   else: $mult = False
   $deflt = $defaultNumber.value
   if $description.value: $desc = $description.value
   else: $desc = ""
      }
      -> ^(IMPL_ATTR_DEF 'INT32' attributeName autoSpecifier? numberRange? multipleSpecifier? defaultNumber? description?)
   |  'UINT64' autoSpecifier? numberRange? attributeName multipleSpecifier? defaultNumber? description? SEMI
      {
   $dataType = ImplementationDefinition.ImplAttrType.UINT64
   $attrName = $attributeName.text

   $implAttrDef::implAttrName = $attributeName.text

   $extra = Extras(Extras.NUMBER_RANGE, {'range' : $numberRange.range, 'numberFrom': $numberRange.nfrom,
       'numberTo': $numberRange.nto})
   if $autoSpecifier.text: $auto = True
   else: $auto = False
   if $multipleSpecifier.text: $mult = True
   else: $mult = False
   $deflt = $defaultNumber.value
   if $description.value: $desc = $description.value
   else: $desc = ""
      }
      -> ^(IMPL_ATTR_DEF 'UINT64' attributeName autoSpecifier? numberRange? multipleSpecifier? defaultNumber? description?)
   |  'INT64'     autoSpecifier? numberRange? attributeName multipleSpecifier? defaultNumber? description? SEMI
      {
   $dataType = ImplementationDefinition.ImplAttrType.INT64
   $attrName = $attributeName.text
   $extra = Extras(Extras.NUMBER_RANGE, {'range' : $numberRange.range, 'numberFrom': $numberRange.nfrom,
       'numberTo': $numberRange.nto})

   $implAttrDef::implAttrName = $attributeName.text

   if $autoSpecifier.text: $auto = True
   else: $auto = False
   if $multipleSpecifier.text: $mult = True
   else: $mult = False
   $deflt = $defaultNumber.value
   if $description.value: $desc = $description.value
   else: $desc = ""
      }
      -> ^(IMPL_ATTR_DEF 'INT64' attributeName autoSpecifier? numberRange? multipleSpecifier? defaultNumber? description?)
   |  'FLOAT'     autoSpecifier? floatRange? attributeName multipleSpecifier? defaultFloat? description? SEMI
      {
   $dataType = ImplementationDefinition.ImplAttrType.FLOAT
   $attrName = $attributeName.text
   $extra = Extras(Extras.FLOAT_RANGE, {'numberFrom': $floatRange.nfrom, 'numberTo': $floatRange.nto})

   $implAttrDef::implAttrName = $attributeName.text

   if $autoSpecifier.text: $auto = True
   else: $auto = False
   if $multipleSpecifier.text: $mult = True
   else: $mult = False
   $deflt = $defaultFloat.value
   if $description.value: $desc = $description.value
   else: $desc = ""
      }
      -> ^(IMPL_ATTR_DEF 'FLOAT' attributeName autoSpecifier? floatRange? multipleSpecifier? defaultFloat? description?)
   |  'ENUM'      autoSpecifier? enumeration attributeName multipleSpecifier? defaultName? description? SEMI
      {
   $dataType = ImplementationDefinition.ImplAttrType.ENUM
   $attrName = $attributeName.text

   $implAttrDef::implAttrName = $attributeName.text

   $extra = Extras(Extras.ENUMERATION, {'enumeration': $enumeration.values})
   if $autoSpecifier.text: $auto = True
   else: $auto = False
   if $multipleSpecifier.text: $mult = True
   else: $mult = False
   $deflt = $defaultName.value
   if $description.value: $desc = $description.value
   else: $desc = ""
      }
      -> ^(IMPL_ATTR_DEF 'ENUM' attributeName autoSpecifier? enumeration multipleSpecifier? defaultName? description?)
   |  'STRING' autoSpecifier? attributeName multipleSpecifier? defaultString? description? SEMI
      {
   $dataType = ImplementationDefinition.ImplAttrType.STRING
   $attrName = $attributeName.text

   $implAttrDef::implAttrName = $attributeName.text
   $extra = Extras(Extras.DUMMY)

   if $autoSpecifier.text: $auto = True
   else: $auto = False
   if $multipleSpecifier.text: $mult = True
   else: $mult = False
   $deflt = $defaultString.value
   if $description.value: $desc = $description.value
   else: $desc = ""
      }
      -> ^(IMPL_ATTR_DEF 'STRING' attributeName autoSpecifier? multipleSpecifier? defaultString? description?)
   |  'BOOLEAN'   autoSpecifier? boolValues? attributeName multipleSpecifier? defaultBool? description? SEMI
      {
   $dataType = ImplementationDefinition.ImplAttrType.BOOLEAN
   $attrName = $attributeName.text

   $implAttrDef::implAttrName = $attributeName.text

   $extra = Extras(Extras.BOOL_VALUES, {'boolValues': $boolValues.values})

   if $autoSpecifier.text: $auto = True
   else: $auto = False
   if $multipleSpecifier.text: $mult = True
   else: $mult = False
   $deflt = $defaultBool.value
   if $description.value: $desc = $description.value
   else: $desc = ""
      }
      -> ^(IMPL_ATTR_DEF 'BOOLEAN' attributeName autoSpecifier? boolValues? multipleSpecifier? defaultBool? description?)
   ;

attributeName
   :  ID     -> ^(ATTRIBUTE_NAME ID)
   |  object -> ^(ATTRIBUTE_NAME object)
   ;

numberRange returns[range, nfrom, nto]
   :  ('[' (numberList
   {
   $nfrom = $numberList.values
   $range = False
   }
   -> ^(NUMBER_RANGE numberList)
   |  (numberFrom = number
   {
   $range = True
   $nfrom = long($numberFrom.text)
   } range numberTo = number
   { $nto = long($numberTo.text)
   }
   -> ^(NUMBER_RANGE $numberFrom $numberTo)
   )) ']')
   ;

numberList returns[values]
@init {
    values = []
}
   :  nums += number (',' nums += number)*
   {
   $values = [long(n.text) for n in $nums]
    }
   -> ^(NUMBER_LIST number+)
   ;

multipleSpecifier
   :  ('[' ']') -> ^(MULTIPLE_SPECIFIER)
   ;

floatRange returns[nfrom, nto]
   :  ('[' (numberFrom = FLOAT { $nfrom = float($numberFrom.text) } range numberTo = FLOAT { $nto = float($numberTo.text) }) ']')
   -> ^(FLOAT_RANGE $numberFrom $numberTo)
   ;

enumeration returns[values]
   :  '[' enumeratorList ']'
   {
    $values = ImplementationDefinition.Enumeration($enumeratorList.values)
   }
   -> ^(ENUMERATION enumeratorList)
   ;

enumeratorList returns[values]
@init {
myList = []
}
@after {
$values = myList
}
   :  enums += enumerator { myList.append(enums.value)} (',' enums += enumerator { myList.append(enums.value) })*
   -> ^(ENUMERATOR_LIST enumerator+)
   ;

enumerator returns[value]
@init {

}
@after {

}
   :  ID implParameterList? description?
   {
   $value = ImplementationDefinition.Enumerator($ID.text, $implParameterList.values, $description.value)

   }
   -> ^(ENUMERATOR ID implParameterList? description?)
   ;

boolValues returns[values]
@init {

}
@after {

}
   :  ('[' 'TRUE' tlist = implParameterList? tdesc = description? ',' 'FALSE' flist = implParameterList? fdesc = description? ']')
   {
    $values = ImplementationDefinition.BoolValues(tlist, tdesc, flist, fdesc)
   }
   -> ^(BOOL_VALUES ^('TRUE' $tlist? $tdesc?) ^('FALSE' $flist? $fdesc?))
   ;

autoSpecifier
   :  'WITH_AUTO' //-> ^(AUTO_SPECIFIER)
   ;

defaultNumber returns[value]
   :  (ASSIGN (
   num = number { $value = long($num.text) }  -> ^(DEFAULT_NUMBER $num)
   | id = 'NO_DEFAULT' { $value = $id.text }  -> ^(DEFAULT_NUMBER $id)
   | id = 'AUTO' { $value = $id.text }        -> ^(DEFAULT_NUMBER $id)
   ))
   ;

defaultFloat returns[value]
   :  (ASSIGN (
   num = float  { $value = float($num.text) } -> ^(DEFAULT_FLOAT $num)
   | id = 'NO_DEFAULT'  { $value = $id.text } -> ^(DEFAULT_FLOAT $id)
   | id = 'AUTO' { $value = $id.text }       -> ^(DEFAULT_FLOAT $id)
   ))
   ;

float
   :  FLOAT
   -> ^(FLOAT_DEF FLOAT)
   ;

defaultName returns[value]
   :  (ASSIGN (
   id = ID
   | id = 'NO_DEFAULT'
   | id = 'AUTO'
   )
   -> ^(DEFAULT_NAME $id)
   )
   { $value = $id.text }
   ;

defaultString returns[value]
   :  (ASSIGN (
   id = STRING
   | id = 'NO_DEFAULT'
   | id = 'AUTO'
   )
   -> ^(DEFAULT_STRING $id)
   )
   { $value = $id.text }
   ;

defaultBool returns[value]
   :  (ASSIGN (
   boolean         -> ^(DEFAULT_BOOL boolean)
   | id = 'NO_DEFAULT' -> ^(DEFAULT_BOOL $id)
   | id = 'AUTO'       -> ^(DEFAULT_BOOL $id)
   ))
   ;

implParameterList returns[values]
   :  ('{' implDefList
   {
    $values = ImplementationDefinition.ImplParameterList($implDefList.values)
   } '}')
   -> ^(IMPL_PARAMETER_LIST implDefList)
   ;

implDefList returns[values]
@init {
    myList = []
}
@after {
    $values = myList
}
   :  defs += (implementationDef { myList.append($implementationDef.values), }) *
   {

   }
   -> ^(IMPL_DEF_LIST implementationDef*)
   ;


implRefDef returns[objectType, name, mult, desc]
   :  objectRefType referenceName multipleSpecifier? description? SEMI
   {
   $objectType = $objectRefType.text
   $name = $referenceName.text
   if $multipleSpecifier.text: $mult = True
   else: $mult = False
   if $description.value: $desc = $description.value
   else: $desc = ""

   }
      -> ^(IMPL_REF_DEF objectRefType referenceName multipleSpecifier? description?)
   ;

objectRefType
   :  id = 'OS_TYPE'
   |  id = 'TASK_TYPE'
   |  id = 'COUNTER_TYPE'
   |  id = 'ALARM_TYPE'
   |  id = 'RESOURCE_TYPE'
   |  id = 'EVENT_TYPE'
   |  id = 'ISR_TYPE'
   |  id = 'MESSAGE_TYPE'
   |  id = 'COM_TYPE'
   |  id = 'NM_TYPE'
   |  id = 'APPMODE_TYPE'
   |  id = 'IPDU_TYPE'
   |  id = 'NETWORKMESSAGE_TYPE'
   -> ^(OBJECT_REF_TYPE $id)
   ;

referenceName
   :  ID -> ^(REFERENCE_NAME ID)
   |  object   -> ^(REFERENCE_NAME object)
   ;

//
//
// Application Definition.
//
//
applicationDefinition returns[value]
   :  'CPU' ID '{' objectDefinitionList '}' description? SEMI
   {
   $value = ApplicationDefinition.ApplicationDefinitionBuilder(self, $objectDefinitionList.value, $description.value)
   }
   ->  ^(APPLICATION_DEFINITION objectDefinitionList description?)
   ;


objectDefinitionList returns[value]
@init {
    myList = []
}
@after {
    odl = ApplicationDefinition.ObjectDefinitionList(self, myList)
    $value = odl.run()
}
   :  defs += (objectDefinition { myList.append($objectDefinition.value) } ) *
   -> ^(OBJECT_DEFINITION_LIST objectDefinition*)
   ;


objectDefinition returns[value]
scope {
    objectName
}
   :  objectName { $objectDefinition::objectName = $objectName.name }
      ('{' parameterList '}')? description? SEMI
   {
   obj = self.getObject($objectName.objectType, $objectName.name)
   if obj:
       $value = obj.appendValues($objectName.objectType, $objectName.name, $parameterList.value, $description.value)
   else:
      $value = ApplicationDefinition.ObjectDefinition(self, $objectName.objectType, $objectName.name, $parameterList.value, $description.value)
      self.appendObject($objectName.objectType, $objectName.name, $value)
   }
      -> ^(OBJECT_DEFINITION objectName parameterList? description?)
   ;


objectName returns[objectType, name]
   :  object ID {
    # print $object.text, $ID.text
    $objectType = $object.text
    $name = $ID.text
   }
   -> ^(OBJECT_NAME object ID)
   ;

parameterList returns[value]
@init {
    myList = ApplicationDefinition.ParameterList()
}
@after {
    myList = myList.sort()
    $value = myList
}
   :  (parameter { myList.append($parameter.value) } )*
   -> ^(PARAMETER_LIST parameter*)
   ;

parameter returns[value]
scope {
    attrName
}
   :  attributeName ASSIGN attributeValue description? SEMI
   {
    $parameter::attrName = $attributeName.text
    $value = ApplicationDefinition.Parameter($attributeName.text, $attributeValue.value, $description.value, $objectDefinition::objectName)
   }
   -> ^(PARAMETER attributeName attributeValue description?)
   ;

attributeValue returns[value]
@after {

}
   :  ID ('{' parameterList '}')?
   {
   $value = ApplicationDefinition.AttributeValueContainter(ApplicationDefinition.AttributeValueContainter.ID_VALUE,
       idValue = $ID.text, values = $parameterList.value or ParameterList()
   )
   } -> ^(ID_VALUE ID parameterList?)
   |  boolean ('{' parameterList '}')?
   {
   $value = ApplicationDefinition.AttributeValueContainter(ApplicationDefinition.AttributeValueContainter.BOOL_VALUE,
       booleanValue = $boolean.value, values = $parameterList.value or ParameterList()
   )
   } -> ^(BOOL_VALUE boolean parameterList?)
   |  number
   {
   $value = ApplicationDefinition.AttributeValueContainter(ApplicationDefinition.AttributeValueContainter.NUMBER_VALUE,
       number = $number.value
   )
   } -> ^(NUMBER_VALUE number)
   |  FLOAT
   {
   $value = ApplicationDefinition.AttributeValueContainter(ApplicationDefinition.AttributeValueContainter.FLOAT_VALUE,
       number = float($FLOAT.text)
   )

   } -> ^(FLOAT_VALUE FLOAT)
   |  STRING
   {
   $value = ApplicationDefinition.AttributeValueContainter(ApplicationDefinition.AttributeValueContainter.STRING_VALUE,
       stringValue = $STRING.text.strip('"')
   )
   } -> ^(STRING_VALUE STRING)
   |  'AUTO'
   {
   $value = ApplicationDefinition.AttributeValueContainter(ApplicationDefinition.AttributeValueContainter.AUTO_VALUE)
   } -> ^(AUTO_VALUE)
   ;


description returns[value]
   :  (':' STRING)
   { $value = $STRING.text.strip('"') }
   -> ^(DESCRIPTION STRING)
   ;

number returns[value]
   : id = INT { $value = long($INT.text) }
   | id = HEX { $value = long($HEX.text[2 : ], 16) }
   -> ^(NUMBER $id)
   ;

range
   :  ('..')=> '..'
   ;

boolean returns[value]
   : id = 'TRUE'  { $value = True  }
   | id = 'FALSE' { $value = False }
   //-> $id
   ;

//
// Lexemes
//
ID  : ('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
   ;

INT : '0'..'9'+
    ;

FLOAT
    :   ('0'..'9')+ '.' ('0'..'9')* EXPONENT?
    |   '.' ('0'..'9')+ EXPONENT?
    |   ('0'..'9')+ EXPONENT
    ;

LINE_NUMBER:
    '#line ' { print self.input.LT(1) } INT ' ' STRING { print $STRING.text }
    ;

//FLOAT: SIGN DECDIGITS+ '.' DECDIGITS* EXPONENT;

fragment
SIGN: () |  '+' | '-';

fragment
DECDIGITS
   :  ZERODIGIT POSDIGIT
   ;

SEMI
   :  ';';

ASSIGN
   :  '=';

LCURLY
   :  '{';

RCURLY
   :  '}';

LPAREN
   :  '(';

RRAREN
   :  ')';

LBRACK
   :  '[';


RBRACK
   :  ']';

fragment
ZERODIGIT
   : '0'
   ;

fragment
POSDIGIT
   :  '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
   ;


HEX :  '0'('x' | 'X') HEX_DIGIT+;


COMMENT
    :   '//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    |   '/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
    ;

WS  :
    ( ' '
    | '\t'
    | '\r'
    | '\n'
    )
    { $channel=HIDDEN; }
    ;

STRING
    :  '"' ( ESC_SEQ | ~('\\'|'"') )* '"'
    ;

fragment
EXPONENT : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;


fragment
HEX_DIGIT : ('0'..'9'|'a'..'f'|'A'..'F') ;


fragment
ESC_SEQ
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
    |   UNICODE_ESC
    |   OCTAL_ESC
    ;

fragment
OCTAL_ESC
    :   '\\' ('0'..'3') ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7')
    ;

fragment
UNICODE_ESC
    :   '\\' 'u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT
    ;

