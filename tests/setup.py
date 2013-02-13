#!/usr/bin/env/ python
# -*- coding: utf-8 -*-

from distutils.core import setup, Extension

def makeExtension(name, additionalSources = []):
        name = '_%s' % name # .lower()
        nt = name[ 1 : ].title()
        swig_opts = [
                #'-c++',
                #'-features "autodoc"',
                '-MD',
                '-O',
                #'-modern',
                #'-modernargs',
                '-keyword'
        ]

        sources = [
                '%s.i' % nt,
                '../src/%s.c' % nt,
                '../../k-dk/common/src/Utl.c',
		'Os_Cfg.c',
                'CommonMocks.c',
        ]

	if additionalSources:
	     sources.extend(additionalSources)

        include_dirs = ['./', '../inc', '../', '../../k-dk/']
	define_macros = [('SWIG', '1'), ('KOS_MEMORY_MAPPING', 'STD_OFF')]
        # define_macros : [(name : string, value : string|None)]
        # library_dirs : [string]
        # libraries : [string]
        # extra_compile_args : [string]
        # extra_link_args : [string

        return Extension(name = name, sources = sources, swig_opts = swig_opts,
            include_dirs = include_dirs, define_macros = define_macros
        )

setup(
  name = "kos-tests",
  version = "0.1",
  author = "Christoph Schueler",
  description = "K-OS Testsuite",
  ext_modules = [
      makeExtension('Os_Alm', ['OsAlm_Mocks.c']),
      makeExtension('Os_Ctr', ['OsCtr_Mocks.c']),
      makeExtension('Os_Evt', ['OsEvt_Mocks.c']),

      makeExtension('Os_Res', []),
      #makeExtension('Os_Task', []),
  ],
)

