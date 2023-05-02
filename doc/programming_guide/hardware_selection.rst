.. _lib_qspi_fast_read_hardware_selection:

Hardware Selection
==================

This library has hardware compatibility requirements for the flash part:

* The flash part MUST support the Quad Read command 0xEB.
* The flash part MUST support 3 byte addresses followed by 6 dummy cycles
* The flash part MUST support a minimum of 50 MHz SCLK is using a 600 MHz XCORE system frequency, or 66 MHz SCLK if using an 800 MHz XCORE system frequency.
* The configuration to use the flash part MUST be compatible with ``lib_quadflash``. Changing nonvolatile flash parameters for use with ``lib_flash_fast_read`` could result in incompatibilities with the bootloader. Refer to the `XTC Tools Documentation <https://www.xmos.ai/documentation/XM-014363-PC-LATEST/html/tools-guide/tools-ref/libraries/libquadflash-included-devices/libquadflash-devices.html>`_ for details on supporting flash parts.
* The flash part MUST be configured to an 8mA IO drive strength to ensure the largest window for successful calibration. The method to do this will vary per flash part and the user must consult the data sheet on how to set this parameter nonvolatile.

It is advised to carefully review the datasheet of any particular flash part as some only support speeds 100+ MHz with additional requirements that may be incompatible with the bootloader. For example some only support higher speeds when a special mode is enabled where the least significant bits of the address are assumed to be 0, and thus all Quad Reads must be word aligned. Others may require additional dummy cycles. If the flash is the same used for the bootloader these values must not be changed, otherwise unexpected power cycling will prevent the application from being able to be loaded.