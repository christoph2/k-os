#!/usr/bin/python
# -*- coding: utf-8 -*-

__version__ = '0.9.2rc-1'

__copyright__ = """
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2018 by Christoph Schueler <github.com/Christoph2,
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


from antlr4 import *

class osekoilListener(ParseTreeListener):

    def exitOilfile(self, ctx):
        ctx.value = dict(version = ctx.v.value, implementationDefinition = ctx.i.value, applicationDefinition = ctx.a.value)

    def exitOilVersion(self, ctx):
        d = ctx.d.value if ctx.d else None
        v = ctx.v.text
        ctx.value = dict(version = v, description = d)

    def exitImplementationDefinition(self, ctx):
        i = ctx.i.value
        d = ctx.d.value if ctx.d else None
        ctx.value = dict(items = i, description = d)

    def exitImplementationSpecList(self, ctx):
        ctx.value = [x.value for x in ctx.items]

    def exitImplementationSpec(self, ctx):
        ctx.value = dict(name = ctx.o.value, items = ctx.i.value, description = ctx.d.value if ctx.d else None)

    def exitOil_object(self, ctx):
        ctx.value = ctx.oid.text

    def exitImplementationList(self, ctx):
        ctx.value =[x.value for x in ctx.items]

    def exitImplementationDef(self, ctx):
        a = ctx.a
        r = ctx.r
        if a:
            tp = "ATTR"
            v = a.value
        else:
            tp = "REF"
            v = r.value
        ctx.value = dict(type = tp, value = v)

    def exitImplAttrDef(self, ctx):
        name = ctx.n.value
        tp = ctx.t.text
        desc = ctx.d.value if ctx.d else None
        mult = True if ctx.m else False
        auto  = True if ctx.a else False

        if tp in ('UINT32', 'INT32', 'UINT64', 'INT64'):
            d = ctx.defaultNumber()
            default = d.value if d else None
            rng = ctx.numberRange().value if ctx.numberRange() else None
        elif tp == 'FLOAT':
            d = ctx.defaultFloat()
            default = d.value if d else None
            rng = ctx.floatRange().value if ctx.floatRange() else None
        elif tp == 'ENUM':
            d = ctx.defaultName()
            default = d.value if d else None
            rng = ctx.enumeration().value if ctx.enumeration() else None
        elif tp == 'STRING':
          d = ctx.defaultString()
          default = d.value if d else None
          rng = None
        elif tp == 'BOOLEAN':
          d = ctx.defaultBool()
          default = d.value if d else None
          rng = ctx.boolValues().value if ctx.boolValues() else None
        ctx.value = dict(type = tp, name = name, auto = auto, mult = mult, default = default, range = rng, description = desc)

    def exitAttributeName(self, ctx):
        if ctx.i:
            v = ctx.i.text
            tp = "ID"
        elif ctx.o:
            v = ctx.o.value
            tp = "OBJECT"
        ctx.value = dict(type = tp, name = v)

    def exitNumberRange(self, ctx):
        if ctx.numList:
            v = ctx.numList.value
            tp = "LIST"
        else:
            v = (ctx.numFrom.value, ctx.numTo.value)
            tp = "RANGE"
        ctx.value = dict(type = tp, values = v)

    def exitNumberList(self, ctx):
        first = [ctx.first.value]
        items = [x.value for x in ctx.items]
        ctx.value = first + items

    def exitFloatRange(self, ctx):
        numFrom = float(ctx.numberFrom.text)
        numTo = float(ctx.numberTo.text)
        ctx.value = (numFrom, numTo, )

    def exitEnumeration(self, ctx):
        ctx.value = ctx.items.value

    def exitEnumeratorList(self, ctx):
        first = [ctx.first.value]
        items = [x.value for x in ctx.items]
        ctx.value = first + items

    def exitEnumerator(self, ctx):
        i = ctx.i.text
        l = ctx.l.value if ctx.l else None
        d = ctx.d.value if ctx.d else None
        ctx.value = dict(name = i, value = l, description = d)

    def exitBoolValues(self, ctx):
        tl = ctx.tl.value if ctx.tl else None
        fl = ctx.fl.value if ctx.fl else None
        td = ctx.td.value if ctx.td else None
        fd = ctx.fd.value if ctx.fd else None
        ctx.value = dict(trueValues = tl, trueDescription = td, falseValues = fl, falseDescription = fd)

    def exitDefaultNumber(self, ctx):
        if ctx.d:
            v = ctx.d.value
            s = False
        if ctx.n:
            v = ctx.n.text
            s = True
        if ctx.a:
            v = ctx.a.text
            s = True
        ctx.value = dict(value = v, special = s)

    def exitDefaultFloat(self, ctx):
        if ctx.f:
            v = float(ctx.f.text)
            s = False
        if ctx.n:
            v = ctx.n.text
            s = True
        if ctx.a:
            v = ctx.a.text
            s = True
        ctx.value = dict(value = v, special = s)

    def exitDefaultName(self, ctx):
        if ctx.i:
            v = ctx.i.text
            s = False
        if ctx.n:
            v = ctx.n.text
            s = True
        if ctx.a:
            v = ctx.a.text
            s = True
        ctx.value = dict(value = v, special = s)

    def exitDefaultString(self, ctx):
        if ctx.s:
            v = ctx.s.text
            s = False
        if ctx.n:
            v = ctx.n.text
            s = True
        if ctx.a:
            v = ctx.a.text
            s = True
        ctx.value = dict(value = v, special = s)

    def exitDefaultBool(self, ctx):
        if ctx.b:
            v = ctx.b.value
            s = False
        if ctx.n:
            v = ctx.n.text
            s = True
        if ctx.a:
            v = ctx.a.text
            s = True
        ctx.value = dict(value = v, special = s)

    def exitImplParameterList(self, ctx):
        ctx.value = ctx.items.value
        pass

    def exitImplDefList(self, ctx):
        ctx.value = [x.value for x in ctx.items]

    def exitImplRefDef(self, ctx):
        o = ctx.o.value
        r = ctx.r.value
        m = True if ctx.m else False
        d = ctx.d.value if ctx.d else None
        ctx.value = dict(type = o, name = r, multiple = m, description = d)

    def exitObjectRefType(self, ctx):
        ctx.value = ctx.oid.text

    def exitReferenceName(self, ctx):
        if ctx.i:
            ctx.value = dict(type = "ID", value = ctx.i.text)
        else:
            ctx.value = dict(type = "OBJECT", value = ctx.o.value)

    def exitApplicationDefinition(self, ctx):
        ctx.value = dict(name = ctx.a.text, definitions = ctx.o.value, description = ctx.d.value if ctx.d else None)

    def exitObjectDefinitionList(self, ctx):
        ctx.value = [x.value for x in ctx.items]

    def exitObjectDefinition(self, ctx):
        ctx.value = dict(name = ctx.o.value, parameters = ctx.p.value if ctx.p else None, description = ctx.d.value if ctx.d else None)

    def exitObjectName(self, ctx):
        ctx.value = dict(object = ctx.oob.value, name = ctx.oid.text)

    def exitParameterList(self, ctx):
        ctx.value = [x.value for x in ctx.items]

    def exitParameter(self, ctx):
        n = ctx.n.value if ctx.n else None
        v = ctx.v.value if ctx.v else None
        d = ctx.d.value if ctx.d else None
        ctx.value = dict(name = n, value = v, description = d)

    def exitAttributeValue(self, ctx):
        if ctx.i:
            v = ctx.i.text
            tp = "ID"
        elif ctx.b:
            v = ctx.b.value
            tp = "BOOL"
        elif ctx.n:
            v = ctx.n.value
            tp = "INT"
        elif ctx.f:
            v = float(ctx.f.text)
            tp = "FLOAT"
        elif ctx.s:
            v = ctx.s.text
            tp = "STRING"
        elif ctx.a:
            v = ctx.a.value
            tp = "AUTO"
        ctx.value = dict(type = tp, value = v)

    def exitDescription(self, ctx):
        ctx.value = ctx.v.text

    def exitNumber(self, ctx):
        i = ctx.INT()
        h = ctx.HEX()
        value = int(i.getText()) if i else int(h.getText(), 16)
        ctx.value = value

    def exitBoolean(self, ctx):
        ctx.value = ctx.v.text == 'TRUE'

