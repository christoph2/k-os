# -*- coding: latin-1 -*-

__version__="0.9.0"

__copyright__="""
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 
   (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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

'''
try:
    import psyco
    psyco.full()
except ImportError:
    pass
'''

import string
import sys
import time
import inspect,types
from Cheetah.Template import Template
import ORTICfg

errObj=None
app=None


def simplifiedApplicationDefinition(appDefs):
    standardResources=[]
    internalResources=[]
    linkedResources=[]
    app=ApplicationDefinition(appDefs)
    for resource in app.resources:
        for key,value in resource.items():
            if key=='RESOURCEPROPERTY':
                if value.value=='STANDARD':
                    standardResources.append(resource)
                elif value.value=='INTERNAL':
                    internalResources.append(resource)
                elif value.value=='LINKED':
                    linkedResources.append(resource)
                else:
                    raise ValueError("Invalid Resourceproperty '%s'." % (value.value,))
    del app.resources
    setattr(app,'standardResources',standardResources)
    setattr(app,'internalResources',internalResources)
    setattr(app,'linkedResources',linkedResources)
    if len(app.linkedResources)>0:
        errObj.error("FIXME: Add support for linked resources.",filename="GenCfg.py")
    for num,task in enumerate(app.tasks):
        if task.has_key('EVENT') and len(task['EVENT']):
            task.taskType='BASIC'
        else:
            task.taskType='EXTENDED'
    return app


class ApplicationDefinition(object):
    """
    this class makes the OSEK-Application-Definition more accessible.
    """
    def __init__(self,appDefs):
        for name,appDef in appDefs.items():
            attr=name.lower()
            
            for a in appDef.values():
                for key,value in a.items():
                    self.setValues(value)
            if attr not in ('com','nm','os'):
                attr+='s'
                setattr(self,attr,[x for x in appDef.values()])
            elif len(appDef)>0:
                setattr(self,attr,appDef.values()[0])
                
    def setValues(self,obj):
        "Create shortcuts to values."
        if isinstance(obj,types.ListType):
            for o in obj:
                self.setValues(o)
        else:
            setattr(obj,'value',obj.attribute_value.value)


def writeTemplate(tmplFileName,outFileName,nameSpace):
    outFile=file(outFileName,'wt')
    tmpl=Template(file=tmplFileName, searchList=[nameSpace])
    print >> outFile, tmpl
    outFile.close()


def enumerateServices():
    res=[]
    for num,line in enumerate(ORTICfg.SERVICE_IDS):
        key,value=line
        str='            "%s" = %s' % (key,value)
        if num<len(ORTICfg.SERVICE_IDS)-1:
            str=str+","
        else:
            pass
        res.append(str)
    return '\n'.join(res)


def enumerateStatusCodes():
    res=[]
    for num,line in enumerate(ORTICfg.STATUS_TYPES):
        key,value=line
        str='            "%s" = %s' % (key,value)
        if num<len(ORTICfg.STATUS_TYPES)-1:
            str=str+","
        else:
            pass
        res.append(str)
    return '\n'.join(res)


def enumerateTasks():
    res=[]
    for num,task in enumerate(app.tasks,1):
        str='            "%s" = "&(OS_TCB[%u])"' % (task.name,num)
        if num<len(app.tasks):
            str=str+","
        else:
            pass
        res.append(str)
    return '\n'.join(res)


osVars={
    "lastError" : "OsLastError",
    "serviceID" : "Os_ServiceContext.id",
    "appMode" : "Os_AppMode",
    "isrID"     : "OsCurrentISRID",
    "runningTask" : "OsCurrentTID",
    "runningTaskPriority" : "OsCurrentTCB->CurrentPriority"
}

"""
    TRACE:
    =====
    - RUNNING[TASK|ID]ID
    - Event set, waiting for
    - Resource Lock
    - Alarm expired
"""

from collections import namedtuple

Stack=namedtuple("Stack","direction fillpattern")

def Generate(fname,AppDef,Info,errorObj):
    global errObj
    global app
    print
    print "Generating Configuration Files..."
    print

    errObj=errorObj
    app=simplifiedApplicationDefinition(AppDef)

    Info['stack']=Stack("DOWN",0x00)

    namespace={'app' : app, 'cfg' : ORTICfg, 'osVars' : osVars, 'info' : Info,
        'enumerateServices' : enumerateServices,
        'enumerateStatusCodes': enumerateStatusCodes,
        "enumerateTasks" : enumerateTasks,
        "sys" : sys, "time" : time
    }

    writeTemplate('hfile.tmpl','Os_Cfg.h',namespace)
    writeTemplate('cfile.tmpl','Os_Cfg.c',namespace)
    writeTemplate('ortifile.tmpl','App.ort',namespace)


def main():
    print enumerateServices()


if __name__=='__main__':
    main()
