#!/usr/bin/python
# -*- coding: utf-8 -*-

__version__="0.9.0"

__copyright__="""
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

  (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
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

import logging
import os
from optparse import OptionParser,OptionGroup
import sys
import k_os.OIL.Parser
import k_os.OIL.Preproc
import k_os.OIL.GenCfg
from  k_os.OIL.Error import OILError

from collections import namedtuple

def SetIncludePaths(paths):
    inc_paths=os.getenv('KOS_INCLUDE')
    if inc_paths is not None:
        for p in inc_paths.split(';'):
            k_os.OIL.Preproc.AddToPathList(p)
    if paths is not None:
        for p in paths:
            k_os.OIL.Preproc.AddToPathList(p)


def main():
    usage = "usage: %prog [options] oil_file"

    options=[]
    args=[]

    op=OptionParser(usage=usage,version="%prog " +__version__,description="Generate K-OS-Configuration from '.oil'-File.")
    op.add_option("-f","--command-file",type="string",metavar="FILE",
                  dest="command_file",help="read options from command-FILE")

    input_group=OptionGroup(op,"Input")
    input_group.add_option("-I","--include-path",dest="inc_path",action="append",metavar="dir",
        help="Add directory to the list of directories to be searched for include files. "
        "Environment-Variable 'KOS_INCLUDE' is also used to locate Include-Files.")

    op.add_option_group(input_group)

    group=OptionGroup(op,"Output")
    group.add_option("-o","--orti",help="generate orti-FILE",
                     dest="orti",action="store_true",default=False)
    group.add_option("-r","--resource-usage",help="generate resource statistics",
                     dest="res_usage",action="store_true",default=False
                     )
    group.add_option("-t","--test",help="verify only, don't generate anything",
                     dest="test",action="store_true",default=False)

    group.add_option("-V","--verbose",help="print Information messages",
                     dest="verbose",action="store_true",default=False)

    group.add_option("-S","--silent",help="don't print any messages.",
                     dest="silent",action="store_true",default=False)

## keep immediate file.
    op.add_option_group(group)

    (options, args)=op.parse_args()

    SetIncludePaths(options.inc_path)

    if len(args)==0:
        op.error("no input file")

    if len(args)!=1:
        op.error("incorrect number of arguments")

    error=OILError()
    logger=logging.getLogger("kos.oil.logger")
    if options.verbose:
        logger.setLevel(logging.DEBUG)
    elif options.silent:
        logger.setLevel(logging.CRITICAL)
    else:
        logger.setLevel(logging.WARNING)

    inFile=open(args[0])
    outFilename=os.path.splitext(os.path.abspath(args[0]))[0]+'.i'
    outFile=open(outFilename,"w")
    k_os.OIL.Preproc.Preproc(inFile,outFile)
    outFile.close()

    try:
        inFile=open(outFilename)
    except IOError:
        errObj.fatalError("Could not open file '%s'.\n" % (fname))
    else:
        input=inFile.read()
        implDefMap,appDefMap,info=k_os.OIL.Parser.ParseOil(input)
        info['version']=__version__.replace('.','_')
        if options.test==False:
            if error.errorCounter>0:
                print >> sys.stderr, "\n%u Error(s) occured during parsing, generating nothing." % error.errorCounter
            else:
                k_os.OIL.GenCfg.Generate(os.path.splitext(args[0])[0],appDefMap,info)

if __name__=='__main__':
    main()
