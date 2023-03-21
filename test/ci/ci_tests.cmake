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