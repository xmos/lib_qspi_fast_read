cmake_minimum_required(VERSION 3.21)

project(nibble_swap)

add_executable(nibble_swap 
    nibble_swap.c
)

set_property(TARGET nibble_swap PROPERTY C_STANDARD 99)

# Determine OS, set up output dirs
if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    set(NIBBLE_SWAP_INSTALL_DIR "/opt/xmos/bin")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
    set(NIBBLE_SWAP_INSTALL_DIR "/opt/xmos/bin")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    set(NIBBLE_SWAP_INSTALL_DIR "$ENV{USERPROFILE}\\.xmos\\bin")
endif()

install(TARGETS nibble_swap DESTINATION ${NIBBLE_SWAP_INSTALL_DIR})
