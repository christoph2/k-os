Installation and Quickstart
===========================

Requirements
~~~~~~~~~~~~
- CMake (3.16+)
- A C compiler and toolchain appropriate for your target platform (MSVC/GCC/Clang)
- Python 3 (for the OIL generator and tooling)

Build (CLion / CMake)
~~~~~~~~~~~~~~~~~~~~~
To build the main executable (Windows example, using the provided Debug profile):

.. code-block:: bash

   cmake --build C:\csprojects\k-os\cmake-build-debug --target k_os

Notes
~~~~~
- The build can optionally include platform-specific ports. Enable targets in CMake as needed.
- If the build fails with missing optional sources (for example ``src/Com_Ext.c``), either
  disable the corresponding feature in the configuration (Os_Cfg.h / CMake options) or
  provide the missing source file for that feature.

Troubleshooting
~~~~~~~~~~~~~~~
- Missing COM sources: disable ``OS_FEATURE_COM`` or provide the missing implementation (``src/Com_Ext.c``).
- Symbol redefinitions (E_OK/E_NOT_OK): ensure headers are included correctly (prefer using ``Os_Types.h`` in OS code).

Sphinx documentation build (optional)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
If you wish to build the HTML documentation locally, install Sphinx and run:

.. code-block:: powershell

   pip install -U sphinx
   sphinx-build -b html docs\ docs\_build\html

