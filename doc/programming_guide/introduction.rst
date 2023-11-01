.. _lib_qspi_fast_read_introduction:

Introduction
============

``lib_qspi_fast_read`` is a library for interfacing with a QSPI flash device on an XMOS device. This library achieves higher data transfer bandwidth than the default supplied flash libraries at the cost of flexiblity and write/erase functions.

The SCLK is derived from the system clock, so maximum data rates can only be achieved on the XMOS XS3 architecture.

See :ref:`lib_qspi_fast_read_getting_started` to begin with an XMOS development board.

See :ref:`lib_qspi_fast_read_hardware_selection` for flash part parameters to check for designing your own board.

Find the latest version of ``lib_qspi_fast_read`` on `GitHub
<https://github.com/xmos/lib_qspi_fast_read>`_.

.. only:: html

    Copyright & Disclaimer
    ======================

    |XMOS copyright|

    |XMOS disclaimer|

    |XMOS trademarks|
