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

from spark import GenericScanner
from  Token import Token

KW_LIST=("AUTO","BOOLEAN","CPU","ENUM","FALSE","FLOAT",
         "IMPLEMENTATION","INT32","INT64","NO_DEFAULT",
         "OIL_VERSION","STRING","TRUE","UINT32","UINT64",
         "WITH_AUTO")

OBJ_LIST=("ALARM","APPMODE","COM","COUNTER","EVENT","IPDU",
          "ISR","MESSAGE","NETWORKMESSAGE","NM","OS",
          "RESOURCE","TASK")

OBJ_TYPE_LIST=("ALARM_TYPE","APPMODE_TYPE","COM_TYPE",
               "COUNTER_TYPE","EVENT_TYPE","IPDU_TYPE",
               "ISR_TYPE","MESSAGE_TYPE","NETWORKMESSAGE_TYPE",
               "NM_TYPE","OS_TYPE","RESOURCE_TYPE","TASK_TYPE")


BOOL_VALUES=("TRUE","FALSE")


class OilScanner(GenericScanner):    
    def __init__(self, flags=0):
        GenericScanner.__init__(self,flags)
        self.lineno=1
        
    def tokenize(self, input):
        self.rv = []
        GenericScanner.tokenize(self, input)        
        return self.rv

    def t_whitespace(self, s):
        r" [ \t\r]+ "

    def t_str_const(self, s):
        r' ["].*["] '
        self.rv.append(Token('string', s))

    def t_comment(self,s):
        r" ((?:\/\*(?:[^*]|(?:\*+[^*\/]))*\*+\/)|(?:\/\/.*)) "

    def t_oneline_comment(self,s):
        r"//.*"
        
    def t_name(self, s):
        r" [a-zA-Z_][a-zA-Z0-9_]* "
        if s in KW_LIST:
            self.rv.append(Token(type=s,lineno=self.lineno))
        elif s in OBJ_LIST:
            self.rv.append(Token('object_lexeme',s))
        elif s in OBJ_TYPE_LIST:
            self.rv.append(Token('object_ref_lexeme',s))
        else:
            self.rv.append(Token('name', s))

    def t_hex_const(self,s):
        r" 0[xX][0-9a-fA-F]+ "
        self.rv.append(Token('number', long(s,16)))

    def t_dec_const(self,s):
        r" [+-]?[0-9]+ "
        self.rv.append(Token('number', long(s,10)))

    def t_float_const(self,s):
        r" [+-]?[0-9]+\.[0-9]+([eE][+-]?[0-9]+)? "
        self.rv.append(Token('float', float(s)))        

    def t_range(self,s):
        r' \.\. '
        self.rv.append(Token(type='range_op'))

    def t_ops(self,s):
        r" [(){}\[\]=;:,] "
        self.rv.append(Token(type=s))

    def t_newline(self,s):
        r" [\n] "
        self.lineno+=1
