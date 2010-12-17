#!/bin/env/python

from distutils.core import setup,Extension
from setuptools import find_packages

setup(
    name='k_osek',
    version='0.2',
    description="'Konnex-Operationg System based on the OSEK/VDX-Standard'-Distribution",
    author='Christoph Schueler',
    author_email='cpu12.gems@googlemail.com',
    url='http://www.github.com/Christoph2/k-os',
    packages=['k_os'],
#    package_dir={
#	"k_osek" : "."
#    },
    scripts=[
    "./sysgen/kosgen.py",
    ]
    # py_modules=['foo'],
#    ext_modules=[Extension('foo', ['foo.i','foo.c'])],
)

