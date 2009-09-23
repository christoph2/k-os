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

STD_ATTRS_OS="""
        TOTRACE ENUM "unsigned char" [
            "NoService"                     =   0,
            "ActivateTask"                  =   2,
            "TerminateTask"                 =   4,
            "ChainTask"                     =   6,
            "Schedule"                      =   8,
            "GetTaskID"                     =   10,
            "GetTaskState"                  =   12,
            "DisableAllInterrupts"          =   14,
            "EnableAllInterrupts"           =   16,
            "SuspendAllInterrupts"          =   18,
            "ResumeAllInterrupts"           =   20,
            "SuspendOSInterrupts"           =   22,
            "ResumeOSInterrupts"            =   24,
            "GetResource"                   =   26,
            "ReleaseResource"               =   28,
            "SetEvent"                      =   30,
            "ClearEvent"                    =   32,
            "GetEvent"                      =   34,
            "WaitEvent"                     =   36,
            "GetAlarmBase"                  =   38,
            "GetAlarm"                      =   40,
            "SetRelAlarm"                   =   42,
            "SetAbsAlarm"                   =   44,
            "CancelAlarm"                   =   46,
            "GetActiveApplicationMode"      =   48,
            "StartOS"                       =   50,
            "ShutdownOS"                    =   52,
            "GetApplicationID"              =   54,
            "GetISRID"                      =   56,
            "CallTrustedFunction"           =   58,
            "CheckISRMemoryAccess"          =   60,
            "CheckTaskMemoryAccess"         =   62,
            "CheckObjectAccess"             =   64,
            "CheckObjectOwnership"          =   66,
            "StartScheduleTableRel"         =   68,
            "StartScheduleTableAbs"         =   70,
            "StopScheduleTable"             =   72,
            "NextScheduleTable"             =   74,
            "StartScheduleTableSynchron"    =   76,
            "SyncScheduleTable"             =   78,
            "GetScheduleTableStatus"        =   80,
            "SetScheduleTableAsync"         =   82,
            "TerminateApplication"          =   84,
            "DisableInterruptSource"        =   86,
            "EnableInterruptSource"         =   88,    
            "InitCounter"                   =   90,
            "IncrementCounter"              =   92,
            "GetCounterInfo"                =   94,
            "GetCounterValue"               =   96,
            "GetElapsedCounterValue"        =   98,
            "StartCOM"                      =   100,
            "StopCOM"                       =   102,
            "InitMessage"                   =   104,
            "StartPeriodic"                 =   106,
            "StopPeriodic"                  =   108,
            "SendMessage"                   =   110,
            "ReceiveMessage"                =   112,
            "SendDynamicMessage"            =   114,
            "ReceiveDynamicMessage"         =   116,
            "SendZeroMessage"               =   118,
            "GetMessageStatus"              =   120
        ] SERVICETRACE, "OS Service Watch";

        ENUM [
            "E_OK"                      =   0,
            "E_OS_ACCESS"               =   1,
            "E_OS_CALLEVEL"             =   2,
            "E_OS_ID"                   =   3,
            "E_OS_LIMIT"                =   4,
            "E_OS_NOFUNC"               =   5,
            "E_OS_RESOURCE"             =   6,
            "E_OS_STATE"                =   7,
            "E_OS_VALUE"                =   8,    
            "E_OS_SERVICEID"            =   16,
            "E_OS_ILLEGAL_ADDRESS"      =   17,
            "E_OS_MISSINGEND"           =   18,
            "E_OS_DISABLEDINT"          =   19,
            "E_OS_STACKFAULT"           =   20,
            "E_OS_PROTECTION_MEMORY"    =   21,
            "E_OS_PROTECTION_TIME"      =   22,
            "E_OS_PROTECTION_ARRIVAL"   =   23,
            "E_OS_PROTECTION_LOCKED"    =   24,
            "E_OS_PROTECTION_EXCEPTION" =   25,
            "E_COM_ID"                  =   32,
            "E_COM_LENGTH"              =   33,
            "E_COM_LIMIT"               =   34,
            "E_COM_NOMSG"               =   35,
            "E_COM_SYS_UNINIT"          =   48
        ] LASTERROR;
"""

STD_ATTRS_TASK="""
        ENUM ]
            "SUSPENDED" =   0,
            "READY"     =   1,
            "RUNNING"   =   2,
            "WAITING"   =   3
        ] STATE;
"""

KOIL_VERSION="2.2"
KERNEL_VERSION="2.3"

VERSION_SECTION="""
VERSION
{
    KOIL = "%s";
    OSSEMANTICS = "ORTI", "%s";
};
""" % (KOIL_VERSION,KERNEL_VERSION)

def VersionSection(f):
    print >> f,VERSION_SECTION

def DeclarationSpec(f,AppDef):
    i=AppDef['TASK']
    j=i.items()
    
    for t in AppDef['TASK'].items():
        print >> f,t


"""
CurrentAppMode ==> OsAppMode
CurrentTask ==> OsCurrentTID
RunningPriority ==> __OS_TCBs[OsCurrentTID].CurrentPriority
"""

def GetObjects(AppDef,category,start=0):
    objs=AppDef[category].items()
    for num,item in enumerate(objs):
        name,obj=item
        yield (num+start,name,obj,)


def DeclarationSection(f,AppDef):
    Impl=AppDef["OS"]
    i=Impl.items()[0]
    
    ImplName="test" # todo: Manufacturer-String!!!
    print >> f,"IMPLEMENTATION %s" % (ImplName)
    print >> f,"{"

    print >> f,"    OS"
    print >> f,"    {"
    print >> f,STD_ATTRS_OS
    print >> f,"    };"

    for o in GetObjects(AppDef,"APPMODE",1):
         print o

    DeclarationSpec(f,AppDef)

    print >> f,"};"


def InformationSection(f):
    pass

def Generate(fname,AppDef):
#    print AppDef['TASK'].items()
    
    f=file(fname,"w")

    VersionSection(f)
    DeclarationSection(f,AppDef)
    InformationSection(f)
    f.close()


if __name__=='__main__':
    Generate("test.ort",AppDef)

