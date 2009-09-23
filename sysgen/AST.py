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

class AST(object):
    def __init__(self, type):
        self.type = type
        self._kids = []

    def __getitem__(self, i):
        return self._kids[i]

	def __len__(self):
		return len(self._kids)

    def __setslice__(self, low, high, seq):
        self._kids[low:high] = seq

	def __cmp__(self, o):
		return cmp(self.type, o)

    def __repr__(self):
        return str(self.type)
