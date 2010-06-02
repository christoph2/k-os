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

import inspect,types
from Cheetah.Template import Template


errObj=None


def simplifiedApplicationDefinition(appDefs):
    standardResources=[]
    internalResources=[]
    linkedResources=[]
    app=ApplicationDefinition(appDefs)
    for resource in app.resources:
        for key,value in resource.items():
            if key=='RESOURCEPROPERTY':
                pass
                if value.value=='STANDARD':
                    standardResources.append(value)
                elif value.value=='INTERNAL':
                    internalResources.append(value)
                elif value.value=='LINKED':
                    linkedResources.append(value)
                else:
                    raise ValueError("Invalid Resourceproperty '%s'." % (value.value,))
    del app.resources
    setattr(app,'standardResources',standardResources)
    setattr(app,'internalResources',internalResources)
    setattr(app,'linkedResources',linkedResources)
    if len(app.linkedResources)>0:
        errObj.error("FIXME: Add support for linked resources.",filename="GenCfg.py")
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


def Generate(fname,AppDef,errorObj):
    global errObj
    print
    print "Generating Configuration Files..."
    print

    errObj=errorObj
    app=simplifiedApplicationDefinition(AppDef)

    nameSpace={'app' : app}
    tmpl=Template(file='hfile.tmpl', searchList=[nameSpace])
    print tmpl
