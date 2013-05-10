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

import codecs
import logging
from optparse import OptionParser, OptionGroup
import os

import antlr3
from osekoilLexer import osekoilLexer
from osekoilParser import osekoilParser

from kosek.Logger import Logger

import kosek.GenCfg as GenCfg
import kosek.Preproc as Preproc


##
##  TODO: Package-Root: kosek
##


def SetIncludePaths(paths):
    inc_paths = os.getenv('KOS_INCLUDE')
    if inc_paths is not None:
        for p in inc_paths.split(';'):
            Preproc.AddToPathList(p)
    if paths is not None:
        for p in paths:
            Preproc.AddToPathList(p)


def Parse(fname):
    inputFile = antlr3.StringStream(codecs.open(fname, encoding = 'UTF-8').read())
    lexer = osekoilLexer(inputFile)

    tokens = antlr3.CommonTokenStream(lexer)
    parser = osekoilParser(tokens)

    file_return = parser.file()
    implementationDefinition, applicationDefinition = file_return.value
    applicationDefinition.postProcessing()

    return (implementationDefinition, applicationDefinition, applicationDefinition.info)


def main():
    print ("kosgen, Part of k_os (Konnex Operating-System based on the OSEK/VDX-Standard).")
    usage = 'usage: %prog [options] oil_file'

    options = []
    args = []

    op = OptionParser(usage = usage, version = '%prog ' + __version__,
        description = "Generate K-OS-Configuration from '.oil'-File."
    )
    op.add_option('-f', '--command-file', type = 'string', metavar = 'FILE', dest = 'command_file',
        help = 'read options from command-FILE'
    )

    input_group = OptionGroup(op, 'Input')
    input_group.add_option('-I', '--include-path', dest = 'inc_path', action = 'append',
        metavar = 'dir',
        help = """Add directory to the list of directories to be searched for include files.
        Environment-Variable 'KOS_INCLUDE' is also used to locate Include-Files."""

    )
    op.add_option_group(input_group)

    group = OptionGroup(op, 'Output')
    group.add_option('-o', '--orti', help = 'generate orti-FILE', dest = 'orti', action = 'store_true', default = False)
    group.add_option('-r', '--resource-usage', help = 'generate resource statistics', dest = 'res_usage',
            action = 'store_true', default = False)
    group.add_option('-t', '--test', help = "verify only, don't generate anything", dest = 'test',
        action = 'store_true', default = False)
    group.add_option('-V', '--verbose', help = 'print Information messages', dest = 'verbose',
        action = 'store_true', default = False)
    group.add_option('-S', '--silent', help = "don't print any messages.", dest = 'silent', action = 'store_true',
        default = False)

## keep immediate file.
    op.add_option_group(group)

    (options, args) = op.parse_args()

    SetIncludePaths(options.inc_path)

    if len(args) == 0:
        op.error('no input file')

    if len(args) != 1:
        op.error('incorrect number of arguments')

    error = Logger()
    logger = logging.getLogger('kos.oil.logger')
    if options.verbose:
        logger.setLevel(logging.DEBUG)
    elif options.silent:
        logger.setLevel(logging.CRITICAL)
    else:
        logger.setLevel(logging.WARNING)

    inFile = open(args[0])
    outFilename = os.path.splitext(os.path.abspath(args[0]))[0] + '.i'
    outFile = open(outFilename, 'w')
    Preproc.Preproc(inFile, outFile)
    outFile.close()

    try:
        inFile = open(outFilename)
    except IOError:
        error.fatalError("Could not open file '%s'.\n" % outFilename)
    else:
        (implDefMap, appDefMap, info) = Parse(outFilename)

        info.version = __version__

        if options.test == False:
            if error.errorCounter > 0:
                error.error('\n%u Error(s) occured during parsing, generating nothing.'
                              % error.errorCounter)
            else:
                GenCfg.Generate(os.path.splitext(args[0])[0], appDefMap, info)
# test.oil
# C:\projekte\csProjects\k-os\samples\first\first.oil


if __name__ == '__main__':
    main()
