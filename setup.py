#!/bin/env/python

from distutils.core import setup,Extension
from setuptools import find_packages
from glob import glob


setup(
    name = 'kosek',	# pySART.kosek
    version = '0.9.2rc-1',
    description = "'Konnex-Operationg System based on the OSEK/VDX-Standard'-Distribution",
    author = 'Christoph Schueler',
    author_email = 'cpu12.gems@googlemail.com',
    url = 'https://www.github.com/Christoph2/k-os',
    packages = ['kosek', 'kosek.ApplicationDefinition', 'kosek.ImplementationDefinition'],
    #namespace_packages = ['pySART'],
    data_files = [
	    #('kosek/config/oil', glob('config/oil/*.oil')),	
	    ('kosek/config/templates', glob('config/templates/*.tmpl')),
    ],
    install_requires = ['mako', ],
    entry_points = {
	'console_scripts': [
		'kosgen = kosek.kosgen:main',
        ],
#	'gui_scripts': [
#	    'baz = my_package_gui.start_func',
#	]
    }
)

