#!/usr/bin/python
# -*- coding: latin-1 -*-

"""
interface Node {
// NodeType
const unsigned short ELEMENT_NODE = 1;
const unsigned short ATTRIBUTE_NODE = 2;
const unsigned short TEXT_NODE = 3;
const unsigned short CDATA_SECTION_NODE = 4;
const unsigned short ENTITY_REFERENCE_NODE = 5;
const unsigned short ENTITY_NODE = 6;
const unsigned short PROCESSING_INSTRUCTION_NODE = 7;
const unsigned short COMMENT_NODE = 8;
const unsigned short DOCUMENT_NODE = 9;
const unsigned short DOCUMENT_TYPE_NODE = 10;
const unsigned short DOCUMENT_FRAGMENT_NODE = 11;
const unsigned short NOTATION_NODE = 12;
"""

import xml.dom.minidom as md


def TraverseNode(node):
    print "\tName: %s Value: %s" % (node.nodeName,node.nodeValue)

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
    