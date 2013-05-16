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
from kosek.ImplementationDefinition.NestableDefinition import NestableDefinition
#from kosek.ImplementationDefinition.AttributeDefinitions import ImplAttrDef, ImplAttrType

logger = Logger()

class Enumeration(object):
    def __init__(self, enumerationList):
        self._dict = dict([(e.name, e) for e in enumerationList])
        self._enumerationList = enumerationList

    def values(self):
        return self._dict.values()

    def items(self):
        return self._dict

    def __getitem__(self, item):
        return self._dict[item]
    
    def __contains__(self, item):
        return item in self._dict
    
    def __repr__(self):
        return "ENUMERATION(%s)" % str(self._enumerationList)


class Enumerator(NestableDefinition):
    def __init__(self, name, parameters, description):
        self._dict = dict()
        self.name = name
        self.parameters = parameters

        if self.parameters:
            self._dict.update(parameters.items())

        self.description = description
        super(Enumerator, self).__init__(name = name, parameters = self.parameters, description = description)

    def __repr__(self):
        if self.parameters:
            if self.description:
                return "(%s %s) : '%s'" % (self.name, self.parameters, self.description)
            else:
                return "(%s %s)" % (self.name, self.parameters)
        else:
            if self.description:
                return "(%s) : '%s'" % (self.name, self.description)
            else:
                return "(%s)" % (self.name, )

    def __getitem__(self, item):
        return self._dict[item]


### ### ##  ###  #####  #######         ####### #######  #####  #######  #####
  #   #   #   #     #    #  #  #         #  #  #  #    # #     # #  #  # #     #
  #   #   ##  #     #       #               #     #      #          #    #
  #   #   ##  #     #       #               #     #  #   #          #    #
  #   #   # # #     #       #               #     ####    #####     #     #####
  #   #   #  ##     #       #    #######    #     #  #         #    #          #
  #   #   #  ##     #       #               #     #            #    #          #
  #   #   #   #     #       #               #     #    # #     #    #    #     #
   ###   ###  #   #####    ###             ###   #######  #####    ###    #####


class TestEnumeration(unittest.TestCase):
    def testEnumeration(self):
        en = Enumeration((Enumerator(u'ECC1', None, ''), Enumerator(u'(BCC1', None, ''), 
            Enumerator(u'(BCC2', None, ''), Enumerator(u'(ECC2', None, '')))

        #self.assertEqual(True, False)

if __name__ == '__main__':
    unittest.main()
