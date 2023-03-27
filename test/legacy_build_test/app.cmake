add_custom_target(legacy_lib_qspi_fast_read_build_test
    COMMAND xmake
    BYPRODUCTS
        bin/legacy_lib_qspi_fast_read_build.xe
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_LIST_DIR}
)
