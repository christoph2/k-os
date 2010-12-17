# -*- coding: latin-1 -*-

__version__="0.9.0"

__copyright__="""
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2010 by Christoph Schueler <github.com/Christoph2,
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

'''
try:
    import psyco
    psyco.full()
except ImportError:
    pass
'''

from collections import namedtuple
import codecs
import os
import stat
import string
import sys
import time
import inspect,types
from Cheetah.Template import Template
import k_os.OIL.ORTICfg
from k_os.OIL.Parser import NestedParameter

errObj=None
app=None
info=None

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
        if 'EVENT' in task and len(task['EVENT']):
            task.taskType='BASIC'
        else:
            task.taskType='EXTENDED'
    return app


class ApplicationDefinition(object):
    """
    This class makes the OSEK-Application-Definition more accessible.
    """
    def __init__(self,appDefs):
        for name,appDef in appDefs.items():
            attr=name.lower()
            for a in appDef.values():
                for key,value in a.items():
                    if key=='ORTI_DEBUG':
                        pass
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
            if isinstance(obj,NestedParameter):
                for paramList in obj.items(): # _,paramList
                    if isinstance(paramList,types.ListType):
                        for param in paramList:
                            self.setValues(param)
                    else:
                        self.setValues(paramList[1])
            else:
                for key,value in obj.items():
                    self.setValues(value)
                setattr(obj,'value',obj.attribute_value.value)


def writeTemplate(tmplFileName,outFileName,nameSpace,encodeAsUTF8=True):
    if os.access(outFileName,os.F_OK):
        os.chmod(outFileName,stat.S_IWRITE or stat.S_IREAD)
##        os.unlink(outFileName)
    tmpl=Template(file=tmplFileName, searchList=[nameSpace])
    if encodeAsUTF8==True:
        outFile=codecs.open(outFileName,mode='wt',encoding='utf-8',errors='xmlcharrefreplace')
        outFile.write(unicode(codecs.BOM_UTF8,'utf-8'))
    else:
        outFile=open(outFileName,mode='wt')
    
    outFile.write(unicode(tmpl))
    outFile.close()
##    os.chmod(outFileName,os.O_RDONLY)


def enumerateServices():
    res=[]
    for num,line in enumerate(k_os.OIL.ORTICfg.SERVICE_IDS):
        key,value=line
        str='            "%s" = %s' % (key,value)
        if num<len(k_os.OIL.ORTICfg.SERVICE_IDS)-1:
            str=str+","
        res.append(str)
    return '\n'.join(res)


def enumerateStatusCodes():
    res=[]
    for num,line in enumerate(k_os.OIL.ORTICfg.STATUS_TYPES):
        key,value=line
        str='            "%s" = %s' % (key,value)
        if num<len(k_os.OIL.ORTICfg.STATUS_TYPES)-1:
            str=str+","
        res.append(str)
    return '\n'.join(res)


def enumeratePriorities():
    ## todo: PRIO_RES_SCHEDULER and IDLE !!!
    global info
    res=['            "NONE" = 0,']
    for num,(key,value) in enumerate(info['priorityMap'].items()):
        str='            "%s" = %s' % (key,value[0])
        if num<len(info['priorityMap'])-1:
            str=str+","
        res.append(str)
    return '\n'.join(res)


def enumerateTasks():
    res=['            "IdleTask" = "&(OS_TCB[0])",']
    for num,task in enumerate(app.tasks,1):
        str='            "%s" = "&(OS_TCB[%u])"' % (task.name,num)
        if num<len(app.tasks):
            str=str+","
        res.append(str)
    return '\n'.join(res)


def enumerateISR2s():
    res=['            "NONE" = 0,']
    for num,isr in enumerate(app.isrs,1):
        if isr['CATEGORY'].value==2:
            str='            "%s" = %u' % (isr.name,num)
            if num<len(app.isrs)-1:
                str=str+","
            res.append(str)
    return '\n'.join(res)

def getAlarmsForCounters():
    di=dict()
    for alarm in app.alarms:
        di.setdefault(alarm['COUNTER'].value,[]).append(alarm.name)
    return di


def getApplicationModes(obj):
    if isinstance(obj['AUTOSTART']['APPMODE'],types.ListType):
        return '|'.join(map(lambda x:x.value,obj['AUTOSTART']['APPMODE']))
    else:
        return obj['AUTOSTART']['APPMODE'].value


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

Register=namedtuple('Register','name type offset')
Stack=namedtuple("Stack","direction fillpattern")

def Generate(fname,AppDef,Info,errorObj):
    global errObj
    global app
    global info
    print
    print "Creating Configuration Files..."
    print

    errObj=errorObj
    app=simplifiedApplicationDefinition(AppDef)
    Info['stack']=Stack("DOWN",0x00)
    info=Info

    alarmsForCounters=getAlarmsForCounters()

    namespace={'app' : app, 'cfg' : k_os.OIL.ORTICfg, 'osVars' : osVars, 'info' : Info,
        'enumerateServices' : enumerateServices,
        'enumerateStatusCodes': enumerateStatusCodes,
        "enumerateTasks" : enumerateTasks, "enumeratePriorities": enumeratePriorities,
        "enumerateISR2s": enumerateISR2s,"alarmsForCounters": alarmsForCounters,
        "getApplicationModes" : getApplicationModes,
        "sys" : sys, "time" : time
    }

    writeTemplate(r'hfile.tmpl','Os_Cfg.h',namespace,encodeAsUTF8=False)
    writeTemplate('cfile.tmpl','Os_Cfg.c',namespace,encodeAsUTF8=False)
    writeTemplate('isrcfgfile.tmpl','ISR_Cfg.h',namespace,encodeAsUTF8=False)
    if app.os['ORTI_DEBUG'].value==True:
        info['vendor']='K_OS'

        info['koilVersion']=app.os['ORTI_DEBUG']['KOIL_VERSION'].value.strip('"')
        info['osekVersion']=app.os['ORTI_DEBUG']['OSEK_VERSION'].value.strip('"')

        info['context']=(
            Register('PC',  "uint16", 7),
            Register('Y',   "uint16", 5),
            Register('X',   "uint16", 3),
            Register('D',   "uint16", 1),
            Register('A',   "uint8",  2),
            Register('B',   "uint8",  1),
            Register('CCR', "uint8",  0),
        )
        writeTemplate('ortifile.tmpl','App.ort',namespace,encodeAsUTF8=False)


def main():
    print enumerateServices()


if __name__=='__main__':
    main()
