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

import unittest

from kosek.Logger import Logger
from kosek.ApplicationDefinition.Parameter import Parameter
from kosek.ApplicationDefinition.AttributeValueContainter import AttributeValueContainter
from kosek.ApplicationDefinition.ParameterContainer import ParameterContainer, NestedParameter

logger = Logger()

class Info(object): pass


class ApplicationDefinitionBuilder(object):
    def __init__(self, parser, definitionList, description):
        self.definitionList = definitionList
        self.parser = parser
        self.description = description
        self.info = Info()

    def postProcessing(self):
        result = {}
        self.standardResources = self.parser.standardResources  # TODO: in der Grammatik handeln!!!
        self.internalResources = self.parser.internalResources
        self.linkedResources = self.parser.linkedResources
        self.internalMessages = self.parser.internalMessages
        self.externalMessages = self.parser.externalMessages

        self.autostartedTasks = self.getAutostartedObjects('TASK')
        self.autostartedAlarms = self.getAutostartedObjects('ALARM')

        self.setTaskFlags()

        self.calculatePriorities()
        self.calculateCeilingPriorities()

    def getObjects(self, objectType):
        return [v for k, v in self.definitionList.items() if k == objectType][0]

    def getAutostartedObjects(self, objectType):
        result = []
        # Filter nur vor�bergehend - AUTOSTART ist nicht optional!!!
        objs = [v for _, v in self.getObjects(objectType).items() if hasattr(v, 'AUTOSTART')]
        for obj in objs:
            if obj.AUTOSTART == True:
                result.append(obj)
            elif isinstance(obj.AUTOSTART, NestedParameter) and obj.AUTOSTART.value:
                result.append(obj)
        return result

    def setTaskFlags(self):
        for (_, task) in self.getObjects('TASK').items():
            flags = []
            if hasattr(task, 'EVENT'):
                flags.append('OS_TASK_ATTR_EXTENDED')
            if task.SCHEDULE == 'FULL':
                flags.append('OS_TASK_ATTR_FULLPREEMPT')
            if flags:
                task.flags = ' | '.join(flags)
            else:
                task.flags = '((uint8)0)'   # TODO: Sinnvolle Konstante!!!

    def calculateCeilingPriorities(self):
        usedResources = {}
        resources = dict(self.getObjects('RESOURCE').items())

        for name, task in self.getObjects('TASK').items():
            if hasattr(task, 'RESOURCE'):
                for resource in task.RESOURCE:
                    usedResources.setdefault(resource, []).append(task)
        for resourceName, tasks in usedResources.items():
            try:
                resources[resourceName].ceilingPriority = max([t.ABSOLUTE_PRIORITY for t in tasks])
            except KeyError:
                pass # TODO: Undefined resource!!!
        """
        for (key, values) in dict([(t[0], t[1].get('RESOURCE')) for t in applicationDefinition['TASK'].items()]).items():
            if values:
                for value in values:
                    usedResources.setdefault(value.value, []).append(key)
        for (key, value) in usedResources.items():
            # # Calculate Ceiling Priority.
            try:
                applicationDefinition['RESOURCE'][key].relativeCeilingPriority = \
                    max([applicationDefinition['TASK'][v]['PRIORITY'].value for v in value])
            except KeyError:
                pass  # Ignore non-existent Resources for now.
        """

    def calculatePriorities(self):
        #alm = tasks = self.getObjects('ALARM')
        #isr = tasks = self.getObjects('ISR')
        eCCx = False
        xCC2 = False
        self.info.priorities = dict()
        tasks = self.getObjects('TASK')
        self.info.numberOfPreemtiveTasks = 0
        self.info.numberOfNonPreemtiveTasks = 0
        for name, task in tasks.items():
            #priority = task.PRIORITY
            #task.RELATIVE_PRIORITY = priority
            if task.SCHEDULE == 'FULL':
                self.info.numberOfPreemtiveTasks += 1
            else:
                self.info.numberOfNonPreemtiveTasks += 1
            self.info.priorities.setdefault(task.PRIORITY, []).append(task)
        self.info.numberOfDistinctPriorities = len(self.info.priorities.keys())
        priorityMap = dict()
        for (idx, objects) in enumerate(self.info.priorities.values(), 1):
            if len(objects) > 1:
                xCC2 = True
            levelPriority = objects[0].PRIORITY
            activations = 0
            for object in objects:
                object.ABSOLUTE_PRIORITY = idx
                activations += object.ACTIVATION
            priorityMap[levelPriority] = (idx, activations)
        self.info.priorityMap = priorityMap


class ObjectDefinitionList(object):
    def __init__(self, parser, definitionList):
        self.definitionList = definitionList
        self.parser = parser

    def run(self):
        implDefinition = self.parser.implDefinition
        result = {}
        autos = {}
        for definition in self.definitionList:
            implObjectDefinition = implDefinition[definition.objectType]
            if definition.objectType not in result:
                result[definition.objectType] = {}
            if definition.name not in result[definition.objectType]:
                result[definition.objectType][definition.name ] = definition
            #print "DEF: %s::%s" % (definition.objectType, definition.name)

            for parameter in definition.parameterList:
                if parameter.name == 'RESOURCE':
                    pass
                implParameterDefinition = implDefinition[definition.objectType][parameter.name ]
                #parameter.implDefinition = implParameterDefinition
                #print "DEF: %s::%s.%s" % (definition.objectType, definition.name, parameter.name)
                if parameter.value == 'AUTO':
                    autos[(definition.objectType, definition.name, parameter.name)] = (
                        parameter, implParameterDefinition
                    )
        return result

class ValueDescriptionPair(object):
    def __init__(self, value, description):
        self._value = value
        self._description = description

    def _getValue(self):
        return self._value

    def _getDescription(self):
        return self._description

    def __repr__(self):
        if self.description:
            return 'VALUE(%s : "%s")' % (self.value, self.description)
        else:
            return 'VALUE(%s)' % (self.value, )

    value = property(_getValue)
    description = property(_getDescription)


class ObjectDefinition(ParameterContainer):
    def __init__(self, parser, objectType, name, parameterList, description):
        self.parser = parser
        self.implDefinition = self.parser.implDefinition[objectType]
        self._setValues(objectType, name, parameterList, description)
        #self.description = []

    def _setValues(self, objectType, name, parameterList, description):
        if not hasattr(self, 'objectType'):
            self.objectType = objectType

        if not hasattr(self, 'implDefinition'):
            self.implDefinition = self.parser.implDefinition[objectType]

        if not hasattr(self, 'name'):
            self.name = name

        if not description:
            self.description = ''   # could be overritten!!!
        else:
            self.description = description

        if not parameterList:
            parameterList = []
        self.parameterList = parameterList

        for param in parameterList:
            self._decorate(param)

        if objectType == 'TASK':
            if [p for p in parameterList if p.name == 'EVENT']:
                self.taskType = 'EXTENDED'
            else:
                self.taskType = 'BASIC'

            if hasattr(self, 'RESOURCE'):
                hasResources = True
            else:
                hasResources = False

            if hasattr(self, 'Event'):
                hasEvents = True
            else:
                hasEvents = False
        elif objectType == 'RESOURCE':
            resourceProperty = parameterList[0].value.idValue
            if resourceProperty == 'STANDARD':
                self.parser.standardResources.append(self)
            elif resourceProperty == 'INTERNAL':
                self.parser.internalResources.append(self)
            elif resourceProperty == 'LINKED':
                self.parser.linkedResources.append(self)
        elif objectType == 'EVENT':
            pass

        for k, v in self.parser.defaults.items(objectType):
            if not hasattr(self, k):
                setattr(self, k, v)

    def appendValues(self, objectType, name, parameterList, description):
        ## TODO: Warning/Error wenn vorhandene Werte �berschrieben werden sollen!!!
        self._setValues(objectType, name, parameterList, description)
        return self

