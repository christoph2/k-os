Configuration and OIL (Tutorial)
===============================

OIL-based configuration
------------------------
k_os uses OIL (OSEK Implementation Language) to describe tasks, resources, alarms,
app modes and other configuration items. The ``kosek`` generator converts OIL models
into ``Os_Cfg.h`` and ``Os_Cfg.c`` which are then included in the build.

Typical workflow
-----------------
1. Create or edit an OIL file describing tasks, counters, alarms and resources.
2. Run the generator (``kosgen``) to produce the configuration headers and sources.
3. Build the project with CMake including the generated files.

Example OIL snippet
-------------------
.. code-block:: none

   TASK MyTask {
       TYPE = BASIC;
       PRIORITY = 10;
       ACTIVATION = 1;
       SCHEDULE = FULL;
       AUTOSTART = TRUE {
           APPMODE = DefaultAppMode;
       };
   };

Generators and tooling
----------------------
The repository contains a Python-based generator in the ``kosek/`` directory. Install
it for local use:

.. code-block:: bash

   pip install -e .

Then run the generator on your OIL file:

.. code-block:: bash

   kosgen <input.oil>

