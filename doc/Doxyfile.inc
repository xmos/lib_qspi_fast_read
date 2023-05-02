# This file provides overrides to the Doxyfile configuration

PROJECT_NAME = lib_qspi_fast_read
PROJECT_BRIEF = "QSPI Flash Fast Read library User Guide for the XMOS XS3 architecture"

PREDEFINED = __DOXYGEN__=1 
PREDEFINED += DWORD_ALIGNED=
PREDEFINED += __attribute__((weak))=
PREDEFINED += C_API= MA_C_API= C_API_START= C_API_END= EXTERN_C=

INPUT += ../lib_qspi_fast_read/api

ALIASES += "beginrst=^^\verbatim embed:rst^^"
ALIASES += "endrst=\endverbatim"
