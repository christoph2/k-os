# -*- coding: latin-1 -*-

__version__="0.9.0"

__copyright__="""
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 
   (C) 2007-2009 by Christoph Schueler <chris@konnex-tools.de,
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
"""

import os
import re
import sys

SCANNING=0
MULTI_LINE_COMMENT=1

FileList=[]
PathList=[]
LineNoMap={}

of=None

E_UNBALANCED_DELIMS=1

#
#os.pathsep
#

INCLUDE_DIRECTIVE=re.compile(r'[ \t]*?#[ \t]*?include[ \t]*?(?P<sdelim>[<"])(?P<incfile>.*?)(?P<edelim>[>"])(?P<rest>.*)')
STRING=re.compile(r'(?P<before>.*?)["](?P<string>.*?)["](?P<after>.*)')
START_COMMENT=re.compile(r'\/(?P<char>\/|\*)')
END_COMMENT=re.compile(r'\*\/')

def SevereError(fname,line_no,code,message):
    print "P%04u:%s:%04u: %s." %(code,os.path.basename(fname),line_no,message)
    sys.exit()

def StripStrings(line):
    match=STRING.match(line)
    while match is not None:        
        line=match.group('before')+match.group('after')
        match=STRING.match(line)
    return line        

def CreateOutFile(fname):
    dir,fn=os.path.split(fname)
    root,ext=os.path.splitext(fn)    
    return open(os.path.join(dir,root+".i"),"w")

def AddToPathList(path):
    if path not in PathList:
        print "Adding Include Path '%s'.\n" % (path)
        PathList.append(path)

def AddToFileList(fname):
    if fname not in FileList:
        FileList.append(fname)

def TryOpen(fname):
    if not os.path.exists(fname):
         return False
    else:
        return True

def Parse(fname):
    ##
    ##  todo: emit 'line' directives !!!
    ##      #line 106 "C:\\projekte\\csProjects\\common\\inc\\Std_Macros.h"
    ##
    try:    
        inf=open(fname)
    except IOError as e:
        print e
        sys.exit(2)
    print
    print '=' * 79
    print " SCANNING: '%s' ..." % (fname)
    print '=' * 79

    state=SCANNING    
    cmt_start_line=0
    inc_path=None
    FileFound=False    
    AddToFileList(fname)
    
    for line_no,line in enumerate(inf):        
        if state==SCANNING:
            tmp_line=StripStrings(line)
            start_comment_tmp=START_COMMENT.search(tmp_line)
            if start_comment_tmp is not None:
                start_comment=START_COMMENT.search(line)
                start=start_comment.start()
                if start_comment.group('char')=='/':                    
                    line=line[:start]+'\n'   ## Strip Single-Line-Comment.
                else:
                    end_comment=END_COMMENT.search(line)                    
                    if end_comment is None:                    
                        state=MULTI_LINE_COMMENT   ## Start Multi-Line-Comment.
                        cmt_start_line=line_no
                        line=line[:start]+'\n'                        
                        print "Multi-Line-Comment startet  @ line: ",cmt_start_line
                    else:
                        end=end_comment.end()
                        line=line[:start]+line[end:-1]+'\n'
            else:
                pass
            include_stmt=INCLUDE_DIRECTIVE.match(line)
            if include_stmt is not None:
                sdelim,incfile,edelim,_=include_stmt.groups(('sdelim','incfile','edelim','rest'))

                if (sdelim=='"' and edelim!='"') or (sdelim=='<' and edelim!='>'):
                    SevereError(fname,line_no,E_UNBALANCED_DELIMS,'Unbalanced deliminiters '+sdelim+','+edelim+' in #include-statement')
                else:
                    ## Warn, if no comment.                    
                    print >> of,'\n',
                    if sdelim=='"':
                        ## Try current directory first.
                        inc_path=os.path.join(os.path.curdir,incfile)
                        FileFound=TryOpen(inc_path)

                    if not FileFound:
                        for p in PathList:
                            inc_path=os.path.join(p,incfile)
                            FileFound=TryOpen(inc_path)
                            if FileFound:
                                break
                                print "FOUND: ",inc_path

                    if FileFound:
                        Parse(inc_path)
                        print "UND ZURUECK!!!",line_no  ## todo: Müsste die Zeilen-Nr. nicht erhöt sein???
                    else:
                        print "Could not open include file '%s'.\n" % (incfile)
                        sys.exit(1)
            else:
                print >> of,line,

        elif state==MULTI_LINE_COMMENT:
            end_comment=END_COMMENT.search(line)
            if end_comment is not None:
                end_comment_line=line_no
                state=SCANNING
                print "Multi-Line-Comment finished @ line: ",end_comment_line
                print >> of
            else:
                print >> of

    print "No. of Lines: ",line_no
    inf.close()

def Parser(fname):
    global of    
    of=CreateOutFile(fname)
    Parse(fname)
    of.close()    


if __name__=='__main__':
    Parser(r'test.oil')
