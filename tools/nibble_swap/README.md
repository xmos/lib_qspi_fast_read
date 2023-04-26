# Nibble Swapping Utility

To achieve maximum flash reading throughput when using `lib_qspi_fast_read`, the data stored in flash should be read in the raw format. The XTC Tools program `xflash` will nibble swap data written to flash.  
This utility will nibble swap each byte in the input file. This nibble swapped file can be written with `xflash` which will effectively unswap the nibbles allowing the data to be read raw with `lib_qspi_fast_read`.

## Building the program

To build the program, run the following commands in the root of the repository:

    cmake -B build_host
    cd build_host
    make nibble_swap -j

The output executable will be in the `tools/nibble_swap` directory.  You may wish to copy this to a directory in your `PATH`.

## Running the program

The run the program, run the following command in the output executable directory or any directory if `nibble_swap` in in your `PATH`:

    nibble_swap <input-file> <output-file>