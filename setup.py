#!/bin/env/python

from distutils.core import setup, Extension

setup(name='k-osek',
    version='0.2',
    description="'Konnex-Operationg System based on the OSEK/VDX-Standard'-Distribution",
    author='Christoph Schueler',
    author_email='cpu12.gems@googlemail.com',
    url='http://www.github.com/Christoph2/k-os',
#    py_modules=['OilParser','OilScanner'],
	packages=['kosek']
    # py_modules=['foo'],
#    ext_modules=[Extension('foo', ['foo.i','foo.c'])],
)
