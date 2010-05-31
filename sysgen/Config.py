#!/usr/bin/python
# -*- coding: latin-1 -*-

__version__="0.9.0"

__copyright__="""
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 
  (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de>
  
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

import xml.dom.minidom as md

def TraverseNode(node):
    print node.nodeName,node.nodeValue

    if node.nodeType==md.Node.ELEMENT_NODE:            
        attrs=node.attributes 
        if len(attrs)>0:
            print "Attribute: ",attrs.items(),len(attrs)
    if node.nodeType==md.Node.ATTRIBUTE_NODE:
        print "Attr-Node "+node.name
    elif node.nodeType==md.Node.TEXT_NODE:
        print node.data        
    for n in node.childNodes:
        TraverseNode(n)

def Parse(fname):
    dom=md.parse(fname)
    assert dom.documentElement.tagName=="kaossg"

    for node in dom.documentElement.childNodes:
        TraverseNode(node)     
    

if __name__=='__main__':
    Parse(r"kaossg.xml")
