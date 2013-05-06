#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Os_Ctr as mod
import unittest

mod.OsCtr_InitCounters();

SYS_COUNTER = 0

class TestCounter(unittest.TestCase):

    def setUp(self):
         mod.InitCounter(SYS_COUNTER,0)

    def testIncrementCounterOK(self):
        res = mod.IncrementCounter(SYS_COUNTER)
        self.assertEquals(res, mod.E_OK)

    def testIncrementCounterNotOK(self):
        res = mod.IncrementCounter(99)
        self.assertEquals(res, mod.E_OS_ID)

    def testGetCounterInfoOK(self):
        info = mod.AlarmBaseType()
        res = mod.GetCounterInfo(SYS_COUNTER, info)
        self.assertEquals(res, mod.E_OK)

    def testGetCounterInfoNotOK(self):
        info = mod.AlarmBaseType()
        res = mod.GetCounterInfo(99, info)
        self.assertEquals(res, mod.E_OS_ID)

    def testGetCounterValueOK(self):
        res, value = mod.GetCounterValue(SYS_COUNTER)
        self.assertEquals(res, mod.E_OK)

    def testGetCounterValueNotOK(self):
        res, value = mod.GetCounterValue(99)
        self.assertEquals(res, mod.E_OS_ID)

    def testGetElapsedCounterValueOK(self):
        res, value, elapsedValue = mod.GetElapsedCounterValue(SYS_COUNTER)
        self.assertEquals(res, mod.E_OK)

    def testGetElapsedCounterValueNotOK(self):
        res, value, elapsedValue = mod.GetElapsedCounterValue(99)
        self.assertEquals(res, mod.E_OS_ID)

    def testGetCounterInfoReturnsExpectedValues(self):
        info = mod.AlarmBaseType()
        value = mod.GetCounterInfo(SYS_COUNTER, info)
        self.assertTrue((info.maxallowedvalue == 1000) and (info.ticksperbase == 1000) and (info.mincycle == 1))

    def testGetCounterInfoDontModifyInfoIfInvalidID(self):
        info = mod.AlarmBaseType()
        info.maxallowedvalue = 0xBEEF
        info.ticksperbase    = 0xDEAD
        info.mincycle        = 0xAFFE
        value = mod.GetCounterInfo(99, info)
        self.assertTrue((info.maxallowedvalue == 0xBEEF) and (info.ticksperbase == 0xDEAD) and (info.mincycle == 0xAFFE))

    def testGetCounterValueReturnsExpectedValues(self):
        mod.IncrementCounter(SYS_COUNTER)
        mod.IncrementCounter(SYS_COUNTER)
        mod.IncrementCounter(SYS_COUNTER)
        mod.IncrementCounter(SYS_COUNTER)

        res, value = mod.GetCounterValue(SYS_COUNTER)
        self.assertEquals(value, 4)




unittest.main()
