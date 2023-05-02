// Copyright (c) 2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1

#include <stdio.h>
#include <stdlib.h>

#define SWAP_NIBBLES(x)      ((x & 0x0F) << 4 | (x & 0xF0) >> 4)

int main(int argc, char *argv[])
{
    FILE *in_file;
    FILE *out_file;
    unsigned char byte;

    if (argc != 3) {
        printf("ERROR missing argument: Usage example %s input.bin output.bin\n", argv[0]);
        exit(-1);
    }

    in_file=fopen(argv[1], "rb");
    if (!in_file)
    {
        printf("Unable to open input file %s", argv[1]);
        return 1;
    }

    out_file=fopen(argv[2], "wb");
    if (!out_file)
    {
        printf("Unable to open output file %s", argv[2]);
        return 1;
    }

    while(fread(&byte, 1, 1, in_file) == 1)
    {
        byte = SWAP_NIBBLES(byte);
        fwrite(&byte, 1, 1, out_file);
    }

    fclose(in_file);
    fclose(out_file);

    return 0;
}
