 # -*- coding: latin-1 -*-
 
__version__="0.9.0"

__copyright__="""
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 
  (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de>
  
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

import sys


class OILError(object):
    def __init__(self,outFile=sys.stderr,verbose=False,silent=False):
        self.fatalErrorCounter=0
        self.errorCounter=0
        self.warningCounter=0
        self.informationCounter=0
        self.outFile=outFile
        self.verbose=verbose
        self.silent=silent

    def printMessage(self,_type,message,lineno=None,filename=None,code=None):
        if self.silent==False:
            if lineno and filename:
                self.outFile.write("%s[%s]:%s:%s - %s\n" % (_type,code,filename,lineno,message))
            elif filename:
                self.outFile.write("%s[%s]:%s - %s\n" % (_type,code,filename,message))
            else:
                self.outFile.write("%s[%s] - %s\n" % (_type,code,message))

    def fatalError(self,message,lineno=None,filename=None,code=''):
        self.printMessage("FATAL ERROR",message,lineno,filename,"F-"+code)
        self.fatalErrorCounter+=1
        sys.exit(1)

    def error(self,message,lineno=None,filename=None,code=''):
        self.printMessage("ERROR",message,lineno,filename,"E-"+code)
        self.errorCounter+=1

    def warning(self,message,lineno=None,filename=None,code=''):
        self.printMessage("WARNING",message,lineno,filename,"W-"+code)
        self.warningCounter+=1
        
    def information(self,message,lineno=None,filename=None,code=''):
        if self.verbose:
            self.printMessage("INFORMATION",message,lineno,filename,"I-"+code)
        self.informationCounter+=1
