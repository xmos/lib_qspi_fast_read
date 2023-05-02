cmake_minimum_required(VERSION 3.21)

project(nibble_swap)

add_executable(nibble_swap 
    nibble_swap.c
)

set_property(TARGET nibble_swap PROPERTY C_STANDARD 99)
