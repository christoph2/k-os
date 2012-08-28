#!/usr/bin/python
# -*- coding: utf-8 -*-

__version__ = '0.9.0'

__copyright__ = \
    """
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

from collections import namedtuple
import codecs
import os
import stat
import string
import sys
import time
import inspect
import types
from Cheetah.Template import Template
import k_os.OIL.ORTICfg
from k_os.OIL.Parser import NestedParameter
from k_os.OIL.Error import OILError

errObj = None
app = None
info = None


INTERNAL_MESSAGE_TYPES = ('SEND_STATIC_INTERNAL', 'SEND_ZERO_INTERNAL', 'RECEIVE_ZERO_INTERNAL',
                     'RECEIVE_UNQUEUED_INTERNAL', 'RECEIVE_QUEUED_INTERNAL')


def simplifiedApplicationDefinition(appDefs):
    standardResources = []
    internalResources = []
    linkedResources = []
    app = ApplicationDefinition(appDefs)
    for resource in app.resources:
        for (key, value) in resource.items():
            if key == 'RESOURCEPROPERTY':
                if value.value == 'STANDARD':
                    standardResources.append(resource)
                elif value.value == 'INTERNAL':
                    internalResources.append(resource)
                elif value.value == 'LINKED':
                    linkedResources.append(resource)
                else:
                    raise ValueError("Invalid Resourceproperty '%s'."
                            % (value.value, ))
    del app.resources
    setattr(app, 'standardResources', standardResources)
    setattr(app, 'internalResources', internalResources)
    setattr(app, 'linkedResources', linkedResources)
    if len(app.linkedResources) > 0x00:
        errObj.error('FIXME: Add support for linked resources.', filename='GenCfg.py')
    for (num, task) in enumerate(app.tasks):
        if 'EVENT' in task and len(task['EVENT']):
            task.taskType = 'BASIC'
        else:
            task.taskType = 'EXTENDED'

    setattr(app, 'internalMessages',
        [msg for msg in app.messages if msg.MESSAGEPROPERTY.value in INTERNAL_MESSAGE_TYPES]
    )
    setattr(app, 'externalMessages',
        [msg for msg in app.messages if msg.MESSAGEPROPERTY.value not in INTERNAL_MESSAGE_TYPES]
    )

    return app


class ApplicationDefinition(object):
    """
    This class makes the OSEK-Application-Definition more accessible.
    """
    def __init__(self, appDefs):
        for (name, appDef) in appDefs.items():
            attr = name.lower()
            for a in appDef.values():
                for (key, value) in a.items():
                    if key == 'ORTI_DEBUG':
                        pass
            if attr not in ('com', 'nm', 'os'):
                attr += 's'
                setattr(self, attr, [x for x in appDef.values()])
            elif len(appDef) > 0x00:
                setattr(self, attr, appDef.values()[0x00])


def writeTemplate(tmplFileName, outFileName, nameSpace, encodeAsUTF8 = True):
    if os.access(outFileName, os.F_OK):
        os.chmod(outFileName, stat.S_IWRITE or stat.S_IREAD)
##      os.unlink(outFileName)
    tmpl = Template(file=tmplFileName, searchList=[nameSpace])
    if encodeAsUTF8 == True:
        outFile = codecs.open(outFileName, mode='wt', encoding='utf-8', errors='xmlcharrefreplace')
        outFile.write(unicode(codecs.BOM_UTF8, 'utf-8'))
    else:
        outFile = open(outFileName, mode='wt')

    outFile.write(unicode(tmpl))
    outFile.close()
##  os.chmod(outFileName,os.O_RDONLY)


def enumerateServices():
    return ',\n'.join(map(lambda t: '            "%s" = %s' % (t[0], t[1]), k_os.OIL.ORTICfg.SERVICE_IDS))


def enumerateStatusCodes():
    return ',\n'.join(map(lambda t: '            "%s" = %u' % (t[0], t[1]), k_os.OIL.ORTICfg.STATUS_TYPES))


def enumeratePriorities():
    ## TODO: PRIO_RES_SCHEDULER and IDLE !!!

    return ',\n'.join(map(lambda n, t: '            "%s" = %u' % (t, (~n + 1) & 0x0f),
        range(len(info['priorityMap']) + 1), ['NONE'] + map(lambda x: x, info['priorityMap']))
    )


def enumerateTasks():
    return ',\n'.join(map(lambda n, t: '            "%s" = "%u"' % (t, n),
        range(len(app.tasks) + 1), ['IdleTask'] + map(lambda x: x.name, app.tasks))
    )


def enumerateISR2s():
    return ',\n'.join(map(lambda n, t: '            "%s" = %u' % (t, n),
        range(len(app.isrs) + 1), ['NONE'] + map(lambda x: x.name, app.isrs))
    )


def getAlarmsForCounters():
    di = dict()
    for alarm in app.alarms:
        di.setdefault(alarm['COUNTER'].value, []).append(alarm.name)
    return di


def getApplicationModes(obj):
    if isinstance(obj['AUTOSTART']['APPMODE'], types.ListType):
        return '|'.join(map(lambda x: x.value, obj['AUTOSTART']['APPMODE']))
    else:
        return obj['AUTOSTART']['APPMODE'].value


osVars = {
    'lastError': 'OsLastError',
    'serviceID': 'Os_ServiceContext.id',
    'appMode': 'Os_AppMode',
    'isrID': '(uint8)OsCurrentISRID',
    'runningTask': '(uint8)OsCurrentTID',
    'callevel': 'OsCallevel',
    'runningTaskPriority': 'OsCurrentTCB->CurrentPriority',
    }


Register = namedtuple('Register', 'name type offset')
Stack = namedtuple('Stack', 'direction fillpattern')

# all((isinstance(x,types.StringTypes) for x in l))


def Generate(fname, AppDef, Info):
    global errObj
    global app
    global info
    print
    print 'Creating Configuration Files...'
    print

    errObj = OILError()
    app = simplifiedApplicationDefinition(AppDef)
    Info['stack'] = Stack('DOWN', 0x00)
    info = Info

    alarmsForCounters = getAlarmsForCounters()

    namespace = {
        'app': app,
        'cfg': k_os.OIL.ORTICfg,
        'osVars': osVars,
        'info': Info,
        'enumerateServices': enumerateServices,
        'enumerateStatusCodes': enumerateStatusCodes,
        'enumerateTasks': enumerateTasks,
        'enumeratePriorities': enumeratePriorities,
        'enumerateISR2s': enumerateISR2s,
        'alarmsForCounters': alarmsForCounters,
        'getApplicationModes': getApplicationModes,
        'sys': sys,
        'time': time,
        }

    # app.tasks.sort(key=lambda x: x['RELATIVE_PRIORITY'].value)

    app.tasks.sort(key=lambda x: x['PRIORITY'].value)

    if info['useResources'] == False:
        osVars['runningTaskPriority'] = \
            '(OS_TaskConf[OsCurrentTID].Priority)'

#    from pkg_resources import Requirement, resource_filename
#    filename = resource_filename(Requirement.parse("MyProject"),"sample.conf")
    writeTemplate(r'hfile.tmpl', 'Os_Cfg.h', namespace, encodeAsUTF8 = False)
    writeTemplate('cfile.tmpl', 'Os_Cfg.c', namespace, encodeAsUTF8 = False)
    writeTemplate('isrcfgfile.tmpl', 'ISR_Cfg.h', namespace, encodeAsUTF8 = False)
    if app.os['ORTI_DEBUG'].value == True:
        info['vendor'] = 'K_OS'

        info['koilVersion'] = app.os['ORTI_DEBUG']['KOIL_VERSION'].value.strip('"')
        info['osekVersion'] = app.os['ORTI_DEBUG']['OSEK_VERSION'].value.strip('"')

        info['context'] = (
            Register('PC', 'uint16', 7),
            Register('Y', 'uint16', 5),
            Register('X', 'uint16', 3),
            Register('D', 'uint16', 1),
            Register('A', 'uint8', 2),
            Register('B', 'uint8', 1),
            Register('CCR', 'uint8', 0x00),
            )
        writeTemplate('ortifile.tmpl', 'App.ort', namespace, encodeAsUTF8 = False)
