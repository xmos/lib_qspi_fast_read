include(${CMAKE_CURRENT_LIST_DIR}/api/app.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/resource_reuse/app.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/stress/app.cmake)

add_custom_target(flash_erase_ci_tests
    COMMAND xflash --erase-all --target-file=${CMAKE_CURRENT_LIST_DIR}/XK_VOICE_L71.xn
    COMMENT
    "Erase flash"
)

add_custom_target(flash_data_ci_tests
    COMMAND xflash --write-all ${CMAKE_CURRENT_LIST_DIR}/ci_test_flash_contents.bin --target-file=${CMAKE_CURRENT_LIST_DIR}/XK_VOICE_L71.xn
    COMMENT
    "Flash calibration and test data binary for lib_qspi_fast_read tests"
)

add_custom_target(flash_config_drive_str_50_ci_test
    COMMAND xflash --spi-command 0x06 --target-file=${CMAKE_CURRENT_LIST_DIR}/XK_VOICE_L71.xn
    COMMAND xflash --spi-command 0x11 0 0x40 --target-file=${CMAKE_CURRENT_LIST_DIR}/XK_VOICE_L71.xn
    COMMENT
        "Set W25Q64JW Status Register 3, drive strength to 50%"
)

add_custom_target(flash_config_drive_str_25_ci_test
    COMMAND xflash --spi-command 0x06 --target-file=${CMAKE_CURRENT_LIST_DIR}/XK_VOICE_L71.xn
    COMMAND xflash --spi-command 0x11 0 0x60 --target-file=${CMAKE_CURRENT_LIST_DIR}/XK_VOICE_L71.xn
    COMMENT
        "Set W25Q64JW Status Register 3, drive strength to 25%"
)

add_custom_target(flash_config_read_drive_str_ci_test
    COMMAND xflash --spi-command 0x15 1 --target-file=${THRUPUT_EVAL_SRC_ROOT}/src/XK_VOICE_L71.xn
    COMMENT
        "Read W25Q64JW Status Register 3"
)
