#!/usr/bin/python
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

import os
from optparse import OptionParser,OptionGroup
import OilParser
import OilPreproc 


def SetIncludePaths(paths):
    inc_paths=os.getenv('SYSGEN_INCLUDE')
    if inc_paths is not None:
        for p in inc_paths.split(';'):
            OilPreproc.AddToPathList(p)

    if paths is not None:
        for p in paths:
            OilPreproc.AddToPathList(p)            


def test():
    usage = "usage: %prog [options] oil_file"

    options=[]
    args=[]

    op=OptionParser(usage=usage,version="%prog " +__version__,description="Beschreibung des Programms.")
    op.add_option("-f","--command-file",type="string",metavar="FILE",
                  dest="command_file",help="read options from command-FILE")

    input_group=OptionGroup(op,"Input")
    input_group.add_option("-I","--include-path",dest="inc_path",action="append",metavar="dir",help="Add directory to the list of directories to be searched for include files.")
    op.add_option_group(input_group)

    group=OptionGroup(op,"Output")
    group.add_option("-o","--orti",help="generate orti-FILE",
                     dest="orti",action="store_true",default=False)
    group.add_option("-r","--resource-usage",help="generate resource statistics",
                     dest="res_usage",action="store_true",default=False
                     )
    group.add_option("-t","--test",help="verify only, don't generate anything",
                     dest="test",action="store_true",default=False)
    op.add_option_group(group)    

    (options, args) = op.parse_args()

    SetIncludePaths(options.inc_path)

    if len(args)==0:
        op.error("no input file")
    
    if len(args)!=1:
        op.error("incorrect number of arguments")

    print "\nStage I. Preprocessing..."
    OilPreproc.Parser(args[0])


if __name__=='__main__':
    test()

