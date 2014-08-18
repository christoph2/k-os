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

from collections import namedtuple
import os
import pkgutil
import stat
import sys
import time
from kosek.Logger import Logger
import kosek.ORTICfg as ORTICfg
from mako.template import Template
from mako.runtime import Context
from mako import exceptions
try:
    import cStringIO as StringIO
except ImportError:
    import StringIO


errObj = None
app = None
info = None


def formattedComment(comment):
    if not comment:
        return None
    result = []
    result.append("/*\n**")
    for line in comment.splitlines():
        result.append("**  %s" % line)
    result.append("**\n*/")
    return '\n'.join(result)


INTERNAL_MESSAGE_TYPES = ('SEND_STATIC_INTERNAL', 'SEND_ZERO_INTERNAL',
    'RECEIVE_ZERO_INTERNAL',
    'RECEIVE_UNQUEUED_INTERNAL', 'RECEIVE_QUEUED_INTERNAL'
)


def simplifiedApplicationDefinition(appDefs):
    standardResources = []
    internalResources = []
    linkedResources = []
    app = ApplicationDefinition(appDefs)

    setattr(app, 'standardResources', appDefs.standardResources)
    setattr(app, 'internalResources', appDefs.internalResources)
    setattr(app, 'linkedResources', appDefs.linkedResources)

    setattr(app, 'internalMessages', appDefs.internalMessages)
    setattr(app, 'externalMessages', appDefs.externalMessages)
    setattr(app, 'autostartedAlarms', appDefs.autostartedAlarms)
    setattr(app, 'autostartedTasks', appDefs.autostartedTasks)

    #if len(app.linkedResources) > 0x00:
    #    errObj.error('FIXME: Add support for linked resources.', filename='GenCfg.py')
    #
    #setattr(app, 'internalMessages',
    #    [msg for msg in app.messages if msg.MESSAGEPROPERTY.value in INTERNAL_MESSAGE_TYPES]
    #)
    #setattr(app, 'externalMessages',
    #    [msg for msg in app.messages if msg.MESSAGEPROPERTY.value not in INTERNAL_MESSAGE_TYPES]
    #)

    return app


class ApplicationDefinition(object):
    """
    This class makes the OSEK-Application-Definition more accessible.
    """
    def __init__(self, appDefs):
        for (name, appDef) in appDefs.definitionList.items():
            attr = name.lower()
            for a in appDef.values():
                for name, param in a.parameters.items():
                    if name in ('ORTI_DEBUG'):
                        pass
            if attr not in ('com', 'nm', 'os'):
                attr += 's'
                setattr(self, attr, [x for x in appDef.values()])
            elif len(appDef) > 0x00:
                setattr(self, attr, appDef.values()[0x00])

def HandleError(ctx, msg):
    #print [x for x in dir(ctx) if not x.startswith('__')]
    #print "KEYS: ", ctx.keys()
    #print "KWARGS: ", ctx.kwargs
    #print "ctx: '%s'." % str(ctx)
    print "msg: '%s'." % msg
    print
    return True

def writeTemplate(tmplFileName, outFileName, namespace = {}, encodeAsUTF8 = True, errorHandler = lambda ctx, msg: False):
    if os.access(outFileName, os.F_OK):
        os.chmod(outFileName, stat.S_IWRITE or stat.S_IREAD)
        os.unlink(outFileName)
    buf = StringIO.StringIO()
    ctx = Context(buf, **namespace)
    print tmplFileName.upper()
    try:
        tobj = Template(text = pkgutil.get_data('kosek', 'config/templates/%s' % tmplFileName),
                            output_encoding = 'utf-8', format_exceptions = False, error_handler = errorHandler
                        )
        tobj.render_context(ctx)
    except Exception as e:
        print str(e)
        print exceptions.text_error_template().render()
        raise
        #return None
    else:
        outFile = open(outFileName, mode='wt')
        outFile.write(buf.getvalue())


def alarmAction(alarm):
    if alarm.ACTION.value == 'ACTIVATETASK':
        action = 'ALM_ACTIVATETASK,\n       { (void*)GetTaskName(' + alarm.ACTION.TASK.value + ') }'
    elif alarm.ACTION.value == 'SETEVENT':
        action = 'ALM_SETEVENT,\n        { (void*)&alarmsEvents [' + str(alarm.pos) + '] }'
    elif alarm.ACTION.value == 'ALARMCALLBACK':
        action = 'ALM_CALLBACK,\n        { (void*)GetAlarmCallbackName(' + \
            alarm.ACTION.ALARMCALLBACKNAME.value + ') }'
    else:
        print "??? '%s'" % alarm
    return action

def mapNotification(n):
    di = {None : 'COM_NOTIFY_NONE', 'ACTIVATETASK' : 'COM_ACTIVATETASK', 'SETEVENT' : 'COM_SETEVENT',
    'COMCALLBACK' : 'COM_COMCALLBACK','FLAG' : 'COM_FLAG','INMCALLBACK' :'COM_COMINMCALLBACK'}
    if n in di:
        return di[n]
    else:
        print "Invalid Key: '%s'" % n
        return None


def getAlarmsForCounters():
    di = dict()
    for alarm in app.alarms:
        di.setdefault(alarm.parameters['COUNTER'][0].parameterValue.value, []).append(alarm.name)
    return di


def getApplicationModes(obj):
    return ''   ## TODO: Komplett überarbeiten!!!
    '''
    if isinstance(obj.AUTOSTART.APPMODE, types.ListType):
        return '|'.join(map(lambda x: x.value, obj.AUTOSTART.APPMODE))
    else:
        return obj.AUTOSTART.APPMODE
    '''


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

    errObj = Logger()
    app = simplifiedApplicationDefinition(AppDef)
    Info.stack = Stack('DOWN', 0x00)
    info = Info

    alarmsForCounters = getAlarmsForCounters()

    namespace = {
        'app':                  app,
        'osVars':               osVars,
        'info':                 Info,
        'ORTICfg':              ORTICfg,
        'alarmsForCounters':    alarmsForCounters,
        'getApplicationModes':  getApplicationModes,
        'mapNotification':      mapNotification,
        'alarmAction':          alarmAction,
        'time':                 time,
        }

    # app.tasks.sort(key=lambda x: x['RELATIVE_PRIORITY'].value)

    app.tasks.sort(key=lambda x: x.parameters['PRIORITY'][0].parameterValue.value)


#    if info['useResources'] == False:
#        osVars['runningTaskPriority'] = '(OS_TaskConf[OsCurrentTID].Priority)'

#    from pkg_resources import Requirement, resource_filename
#    filename = resource_filename(Requirement.parse("MyProject"),"sample.conf")

    writeTemplate('hfile.tmpl', 'Os_Cfg.h', namespace, encodeAsUTF8 = False, errorHandler = HandleError)
    writeTemplate('cfile.tmpl', 'Os_Cfg.c', namespace, encodeAsUTF8 = False, errorHandler = HandleError)
    writeTemplate('isrcfgfile.tmpl', 'ISR_Cfg.h', namespace, encodeAsUTF8 = False, errorHandler = HandleError)
    if 'ORTI_DEBUG' in app.os.parameters:
        info.vendor = 'K_OS'

        info.koilVersion = app.os.ORTI_DEBUG.KOIL_VERSION.value
        info.osekVersion = app.os.ORTI_DEBUG.OSEK_VERSION.value
        info.osekVersion = "2.3"
        info.koilVersion = "2.2"

        info.context = (
            Register('PC', 'uint16', 7),
            Register('Y', 'uint16', 5),
            Register('X', 'uint16', 3),
            Register('D', 'uint16', 1),
            Register('A', 'uint8', 2),
            Register('B', 'uint8', 1),
            Register('CCR', 'uint8', 0x00),
        )
        writeTemplate('ortifile.tmpl', 'App.ort', namespace, encodeAsUTF8 = False, errorHandler = HandleError)
