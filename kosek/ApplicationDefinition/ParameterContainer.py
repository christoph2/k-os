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
from kosek.ApplicationDefinition.AttributeValueContainter import AttributeValueContainter

logger = Logger()

class ParameterContainer(object):
    def setAttribute(self, attr, value):
        try:
            mult = self.implDefinition[attr].mult
        except KeyError:
            pass
        if mult:
            if not hasattr(self, attr):
                setattr(self, attr, [])
            getattr(self, attr).append(value)
        else:
            setattr(self, attr, value)

    def _decorate(self, param):
        parameterValue = param.value.getValue()
        print "{%s} - '%s': %s[%s]" % (hex(id(self)), param.name, param.value.getTypeString(), parameterValue)
        if param.name in ('ACTION', 'AUTOSTART'):
            pass
        if param.value.type_ == AttributeValueContainter.ID_VALUE:
            if param.value.values:
                #print "\t*** ID: ", self.implDefinition
                try:
                    enum = [e for e in self.implDefinition[param.name].enumeration.values()]
                except KeyError:
                    enum = [e for e in self.parent.implDefinition[param.name].enumeration.values()]
                    # TODO: kann ein Parent-Looup wirklich unser Problem l�sen???
                implDef = [e for e in enum if e.name == param.value.idValue][0]
                ## TODO: Fehlerbehandlung

                setattr(self, param.name, NestedParameter(self, param.name,
                        parameterValue, param.value.values, implDef
                    )
                )
            else:
                self.setAttribute(param.name, parameterValue)
        elif param.value.type_ == AttributeValueContainter.BOOL_VALUE:
            if param.value.values:
                setattr(self, param.name, NestedParameter(self, param.name,
                        parameterValue, param.value.values,
                        self.implDefinition[param.name][str(parameterValue).upper()]
                    )
                )
            else:
                    self.setAttribute(param.name, param.value.booleanValue)
        elif param.value.type_ == AttributeValueContainter.NUMBER_VALUE:
            self.setAttribute(param.name, parameterValue)
        elif param.value.type_ == AttributeValueContainter.FLOAT_VALUE:
            self.setAttribute(param.name, parameterValue)
        elif param.value.type_ == AttributeValueContainter.STRING_VALUE:
            self.setAttribute(param.name, parameterValue)
        elif param.value.type_ == AttributeValueContainter.AUTO_VALUE:
            if not self.implDefinition[param.name].autoSpec:
                raise ValueError("AUTO not permitted for attribute '%s'." % param.name)
            else:
                pass


class NestedParameter(ParameterContainer):
    def __init__(self, parent, name, value, params, implDefinition):
        self.name = name
        self.value = value
        self.implDefinition = implDefinition
        self.params = params
        self.parent = parent
        for param in params:
            self._decorate(param)

