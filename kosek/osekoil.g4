/*
**  k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
**
**  (C) 2007-2018 by Christoph Schueler <github.com/Christoph2,
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


oilfile: v = oilVersion i = implementationDefinition a = applicationDefinition
    ;


oilVersion: 'OIL_VERSION' '=' v = STRING d = description? ';'
    ;

//
//
// Implementation Definition.
//
//
implementationDefinition:  'IMPLEMENTATION' ID '{' i = implementationSpecList '}' d = description? ';'
    ;

implementationSpecList:  (items += implementationSpec) *
    ;

implementationSpec:  o = oil_object '{' i = implementationList '}' d = description? ';'
    ;

oil_object:
     oid = 'OS'
   | oid = 'TASK'
   | oid = 'COUNTER'
   | oid = 'ALARM'
   | oid = 'RESOURCE'
   | oid = 'EVENT'
   | oid = 'ISR'
   | oid = 'MESSAGE'
   | oid = 'COM'
   | oid = 'NM'
   | oid = 'APPMODE'
   | oid = 'IPDU'
   | oid = 'NETWORKMESSAGE'
   ;

implementationList:
   (items += implementationDef)*
   ;

implementationDef:
     a = implAttrDef
   | r = implRefDef
   ;

implAttrDef:
     t = 'UINT32'   a = autoSpecifier? numberRange?     n = attributeName m = multipleSpecifier? defaultNumber? d = description? ';'
   | t = 'INT32'    a = autoSpecifier? numberRange?     n = attributeName m = multipleSpecifier? defaultNumber? d = description? ';'
   | t = 'UINT64'   a = autoSpecifier? numberRange?     n = attributeName m = multipleSpecifier? defaultNumber? d = description? ';'
   | t = 'INT64'    a = autoSpecifier? numberRange?     n = attributeName m = multipleSpecifier? defaultNumber? d = description? ';'
   | t = 'FLOAT'    a = autoSpecifier? floatRange?      n = attributeName m = multipleSpecifier? defaultFloat?  d = description? ';'
   | t = 'ENUM'     a = autoSpecifier? enumeration      n = attributeName m = multipleSpecifier? defaultName?   d = description? ';'
   | t = 'STRING'   a = autoSpecifier?                  n = attributeName m = multipleSpecifier? defaultString? d = description? ';'
   | t = 'BOOLEAN'  a = autoSpecifier? boolValues?      n = attributeName m = multipleSpecifier? defaultBool?   d = description? ';'
   ;

attributeName:
     i = ID
   | o = oil_object
   ;

numberRange:
    '[' (numList = numberList | (numFrom = number rng numTo = number)) ']'
   ;

numberList:
    first = number (',' items += number)*
   ;

multipleSpecifier:
    '[' ']'
   ;

floatRange:
    ('[' (numberFrom = FLOAT rng numberTo = FLOAT) ']')
   ;

enumeration:
    '[' items = enumeratorList ']'
   ;

enumeratorList:
    first = enumerator (',' items += enumerator )*
   ;

enumerator:
    i = ID l = implParameterList? d = description?
   ;

boolValues:
    '[' 'TRUE' tl = implParameterList? td = description? ',' 'FALSE' fl = implParameterList? fd = description? ']'
   ;

autoSpecifier:  'WITH_AUTO'
   ;

defaultNumber:
    '=' (
         d = number
       | n = 'NO_DEFAULT'
       | a= 'AUTO'
   )
   ;

defaultFloat:
    '=' (
         f = FLOAT
       | n = 'NO_DEFAULT'
       | a = 'AUTO'
   )
   ;

defaultName:
    '=' (
         i = ID
       | n = 'NO_DEFAULT'
       | a = 'AUTO'
   )
   ;

defaultString:
    '=' (
         s = STRING
       | n = 'NO_DEFAULT'
       | a= 'AUTO'
   )
   ;

defaultBool:
    '=' (
         b = boolean
       | n = 'NO_DEFAULT'
       | a = 'AUTO'
   )
   ;

implParameterList:
    '{' items = implDefList '}'
   ;

implDefList:
    (items += implementationDef) *
   ;


implRefDef:
    o = objectRefType r = referenceName m = multipleSpecifier? d = description? ';'
   ;

objectRefType
   : oid = 'OS_TYPE'
   | oid = 'TASK_TYPE'
   | oid = 'COUNTER_TYPE'
   | oid = 'ALARM_TYPE'
   | oid = 'RESOURCE_TYPE'
   | oid = 'EVENT_TYPE'
   | oid = 'ISR_TYPE'
   | oid = 'MESSAGE_TYPE'
   | oid = 'COM_TYPE'
   | oid = 'NM_TYPE'
   | oid = 'APPMODE_TYPE'
   | oid = 'IPDU_TYPE'
   | oid = 'NETWORKMESSAGE_TYPE'
   ;

referenceName:
     i = ID
   | o = oil_object
   ;

//
//
// Application Definition.
//
//
applicationDefinition:
    'CPU' a = ID '{' o = objectDefinitionList '}' d = description? ';'
   ;

objectDefinitionList:
    (items += objectDefinition) *
   ;

objectDefinition:
    o = objectName ('{' p = parameterList '}')? d = description? ';'
   ;

objectName:
    oob = oil_object oid = ID
   ;

parameterList:
    (items += parameter)*
   ;

parameter:
    n = attributeName '=' v = attributeValue d = description? ';'
   ;

attributeValue:
     i = ID ('{' p0 = parameterList '}')?
   | b = boolean ('{' p1 = parameterList '}')?
   | n = number
   | f = FLOAT
   | s = STRING
   | a = 'AUTO'
   ;

description:
    ':' v = STRING
   ;

rng:
    '..'
    ;

number:
     INT
   | HEX
   ;

boolean:
     v = 'TRUE'
   | v = 'FALSE'
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

/*
LINE_NUMBER:
    '#line ' { print self.input.LT(1) } INT ' ' STRING { print $STRING.text }
    ;
*/

fragment
SIGN: () |  '+' | '-';

HEX:
    '0'('x' | 'X') HEX_DIGIT+
    ;

WS:
    (' ' | '\t' | '\r' | '\n') -> channel(HIDDEN)
    ;

STRING:
    '"' ( ESC_SEQ | ~('\\'|'"') )* '"'
    ;

fragment
EXPONENT : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;

fragment
HEX_DIGIT : ('0'..'9'|'a'..'f'|'A'..'F') ;

fragment
ESC_SEQ
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\u0022'|'\''|'\\')
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

