.. _lib_qspi_fast_read_calibration:

Calibration
===========

This library achieves SCLK speeds higher than 75MHz by utilizing a calibration routine to ensure that the sampling port time has sufficient head and tail room in the SIO data valid window. This is achieved by reading a known pattern from flash with various timing parameters which gradually move the sampling time within the bounds of XCORE port behavour. From these tests, if a sufficient window is found, the timing parameters are saved and the ideal configuration can be set by the library API. This pattern is handcrafted to have the worst case EMI transitions on the wire.

Two calibration pattern binary files intended to be stored in flash. Both contain the same data, but one is nibble swapped, allowing the application programmer to choose the flash read mode that best fits their use case.