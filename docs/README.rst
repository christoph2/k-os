k_os
=====

k_os — An OSEK/VDX-compliant real-time operating system
------------------------------------------------------

Quick overview
~~~~~~~~~~~~~~
- Deterministic O(1) scheduler with low interrupt latency.
- Small, configurable, and portable implementation suitable for embedded systems.
- Optional OSEK-COM message subsystem and tooling for OIL-based code generation.

What is OSEK/VDX?
~~~~~~~~~~~~~~~~~
OSEK/VDX is a specification developed in the automotive domain for real-time operating
systems and communication stacks. It defines a task model (basic/extended tasks),
events, resources (Priority Ceiling Protocol), alarms/counters, schedule tables and
service hooks (StartupHook, ErrorHook, etc.).

Documentation contents
~~~~~~~~~~~~~~~~~~~~~~
- Installation — `Installation <installation.rst>`_
- Architecture — `Architecture <architecture.rst>`_
- Features — `Features <features.rst>`_
- Tutorial — `Tutorial <tutorial.rst>`_
- Examples — `Examples <examples.rst>`_
- Contributing — `Contributing <contributing.rst>`_
- Changelog — `Changelog <changelog.rst>`_

License and authors
~~~~~~~~~~~~~~~~~~~
See the project top-level files: ``COPYING`` (license) and ``AUTHORS`` (contributors).

Quick links
~~~~~~~~~~~
- Build target: ``k_os`` (CMake)
- OIL tooling and generators: see ``kosek/`` and the ``kosek`` package
- DeepWiki: `k_os on DeepWiki <https://deepwiki.com/christoph2/k-os>`_

For full documentation, follow the table of contents above.
