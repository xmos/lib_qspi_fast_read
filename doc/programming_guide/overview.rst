.. _lib_qspi_fast_read_overview:

Overview
========

``lib_qspi_fast_read`` is a library for interfacing with a QSPI flash device on an XMOS device to perform quad read operations.

Capabilities
------------

* Supports SCLK up to 133 MHz versus default lib_quadflash limit of 25 MHz
* Allows support for multiple flash parts, one per IO context
* Quad read 0xEB command only
* Provides startup and shutdown, enabling ports to be reused in some situations
* Supports raw reads and nibble swapped reads to support various flash storage layouts

.. raw:: latex

  \newpage
