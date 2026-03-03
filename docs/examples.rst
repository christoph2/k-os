Examples
========

Sample applications
-------------------
The ``samples/`` directory contains example projects and configurations. Notable
examples include:

- ``samples/first/`` — a basic example demonstrating core OS features
- ``samples/OSEKBumser/`` — larger sample showcasing features and integrations
- ``samples/Slick/`` — additional example configurations

How to run samples
------------------
1. Choose the sample and the target port (``port/<arch>``).
2. Ensure generated configuration (``Os_Cfg.h``/``.c``) is present for the sample.
3. Build using CMake and run/flash the resulting binary per your platform instructions.

For platform-specific notes, consult the sample directories for example CMake configurations.
