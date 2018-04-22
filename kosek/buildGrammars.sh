#!/bin/bash
java org.antlr.v4.Tool -Dlanguage=Python2 -long-messages osekoil.g4 -o ./py2/
java org.antlr.v4.Tool -Dlanguage=Python3 -long-messages osekoil.g4 -o ./py3/
