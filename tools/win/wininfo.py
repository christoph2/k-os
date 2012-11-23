#!/usr/bin/env python
# -*- coding: utf-8 -*-

from collections import namedtuple
import os
import sys

if sys.platform != 'win32':
    print "Sorry, you a not using Windows!"
    sys.exit(1)

import win32api

PROCESSOR_ARCHITECTURE_AMD64    = 9
# PROCESSOR_ARCHITECTURE_ARM      = Reserved
PROCESSOR_ARCHITECTURE_IA64     = 6
PROCESSOR_ARCHITECTURE_INTEL    = 0
PROCESSOR_ARCHITECTURE_UNKNOWN  = 0xffff

PROCESSOR_INTEL_386             = 386
PROCESSOR_INTEL_486             = 486
PROCESSOR_INTEL_PENTIUM         = 586
PROCESSOR_INTEL_IA64            = 2200
PROCESSOR_AMD_X8664             = 8664
#PROCESSOR_ARM (Reserved)

ARCH_MAP = {
    PROCESSOR_ARCHITECTURE_AMD64:   "AMD64",
    PROCESSOR_ARCHITECTURE_IA64:    "IA64",
    PROCESSOR_ARCHITECTURE_INTEL:   "INTEL",
    PROCESSOR_ARCHITECTURE_UNKNOWN: "UNKNOWN",
}

TYPE_MAP = {
    PROCESSOR_INTEL_386:            "i386",
    PROCESSOR_INTEL_486:            "i486",
    PROCESSOR_INTEL_PENTIUM:        "Pentium",
    PROCESSOR_INTEL_IA64:           "ia64",
    PROCESSOR_AMD_X8664:            "AMD X8664",
}

def getArchStr(arch):
    return ARCH_MAP.get(arch, "UNKNOWN")

def getTypeStr(type_):
    return TYPE_MAP.get(type_, "UNKNOWN")


ProcessorInfo = namedtuple("ProcessorInfo", "processorLevel processorRevision")

Info = namedtuple('Info', """
        processorArchitecture pageSize minimumApplicationAddress
        maximumApplicationAddress activeProcessorMask
        numberOfProcessors processorType allocationGranularity
        processorInfo""")


def getInfo():
    return Info(*win32api.GetSystemInfo())

def prettyPrint():
    info = getInfo()
    print
    print "Architecture:\t\t%s" % getArchStr(info.processorArchitecture)
    print "Processor-Type:\t\t%s" % getTypeStr(info.processorType)
    print "Processor-Level:\t%u" % info.processorInfo[0]
    print "Processor-Revision:\t%#X" % info.processorInfo[1]
    print "Number of Processors:\t%u" % info.numberOfProcessors
    print "Active Processor Mask:\t%#0X" % info.activeProcessorMask
    print "Pagesize:\t\t%#X" % info.pageSize
    print "Allocation-Granularity:\t%#X" % info.allocationGranularity
    print "Min. Load Address:\t%#X" % info.minimumApplicationAddress
    print "Max. Load Address:\t%#X" % info.maximumApplicationAddress

def main():
    prettyPrint()

if __name__ == '__main__':
    main()