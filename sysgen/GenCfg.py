
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

PROLOGUE="""/*
**
**		!!! AUTOMATICALLY GENERATED FILE - DO NOT EDIT !!!
**
*/
#if !defined(__OSCONFIG_H)
#define	__OSCONFIG_H

/**********************************************************************************************************
*                                          OS-Configuration Part
**********************************************************************************************************/
"""

EPILOGUE="""
#endif	/* __OSCONFIG_H */
"""

def itemgetter(*items):
    if len(items) == 1:
        item = items[0]
        def g(obj):
            return obj[item]
    else:
        def g(obj):
            return tuple(obj[item] for item in items)
    return g

"""


def Generate(fname,AppDef): 
    f=file(fname+".h","w")

    print >> f,PROLOGUE

    os=AppDef["OS"].items()[0][1]
        
    print >> f,"#define	OS_%s_STATUS" % (os["STATUS"][0].attribute_value)
    print >> f,"#define	OS_%s" % (os["CC"][0].attribute_value)

    """
    print filter(lambda attr: os[attr][0].attribute_value.value=='TRUE',["STARTUPHOOK",
        "ERRORHOOK","SHUTDOWNHOOK","PRETASKHOOK","POSTTASKHOOK","USEGETSERVICEID",
        "USEPARAMETERACCESS","USERESSCHEDULER"])
    """

##    '"IDLETIMEHOOK","O1SCHEDULER"'
    
    for attr in ["STARTUPHOOK","ERRORHOOK","SHUTDOWNHOOK","PRETASKHOOK",
              "POSTTASKHOOK","USEGETSERVICEID","USEPARAMETERACCESS","USERESSCHEDULER"]:
        if os[attr][0].attribute_value.value=='TRUE':
            print >> f,"#define OS_USE_%s" % (attr)

    print >> f,"""
/**********************************************************************************************************
*                                         APP-Configuration Part
**********************************************************************************************************/
         """

    print >> f,EPILOGUE

    f.close()
    
