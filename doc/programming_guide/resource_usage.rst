.. _lib_qspi_fast_read_resource_usage:

QSPI Fast Read Resource Usage
#############################

Each qspi_flash_fast_read peripheral requires ports, clock blocks, compute time, and memory.

This page attempts to capture the requirements for each hardware type with 
relevant configurations.

.. warning::
  Compute resource usage depends heavily on size of read operations performed.

Discrete Resources
==================

+-------------------+--------------------+
| Resource          | Count              |
+===================+====================+
| 1b port           | 2                  |
+-------------------+--------------------+
| 4b port           | 1                  |
+-------------------+--------------------+
| clock block       | 1                  |
+-------------------+--------------------+
| thread            | <= 1               |
+-------------------+--------------------+

.. note::

  All operation are performed in place, so thread count usage depends on the application's use case.

Ports
-----

Each ``qspi_flash_fast_read`` peripheral requires a 1-bit port SCLK, a 1-bit port CS, and a 4-bit port SIO.

Clock Blocks
------------

Each ``qspi_flash_fast_read peripheral`` requires 1 clock block. This clock block is used to generate SCLK clock from the system frequency clock.

Threads
-------

This library performs all operations in place and does not explicitly require a dedicated thread. The MIPS usage in a given thread varies heavily based on the SCLK frequency, frequency of flash reads, and size of flash reads.

Compute Requirements
====================

This library has been evaluated to function at a minimum of 75 MIPs, the worst case scenario with a 600 MHz system frequency and all 8 cores scheduling operations.

Memory
======

The ``qspi_flash_fast_read`` library requires approximately 1.8 kB, split between <200 bytes data and <1600 bytes prog.

Each qspi_flash_fast_read peripheral context requires 36 bytes of data.
