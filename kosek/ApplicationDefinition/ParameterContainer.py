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
#from kosek.ApplicationDefinition.AttributeValueContainter import AttributeValueContainter

logger = Logger()

class ParameterContainer(object):
    def setAttribute(self, attr, value):
        try:
            mult = self.implDefinition[attr].mult
        except KeyError:
            logger.exception(KeyError)
        except TypeError as e:
            pass
        if mult:
            if not hasattr(self, attr):
                setattr(self, attr, [])
            getattr(self, attr).append(value)
        else:
            setattr(self, attr, value)


class NestedParameter(ParameterContainer):
    def __init__(self, parent, name, value, params, implDefinition):
        self.name = name
        self.value = value
        self.implDefinition = implDefinition
        self.params = params
        self.parent = parent


