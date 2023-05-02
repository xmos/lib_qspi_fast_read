set(THRUPUT_EVAL_SRC_ROOT ${CMAKE_CURRENT_LIST_DIR})

#**********************
# Gather Sources
#**********************
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.c)
set(APP_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/src)

#**********************
# Flags
#**********************
set(APP_COMPILER_FLAGS
    -Os
    -g
    -report
    -fxscope
    -mcmodel=large
    ${CMAKE_CURRENT_LIST_DIR}/src/config.xscope
)

set(APP_COMPILE_DEFINITIONS
)

set(APP_LINK_OPTIONS
    -report
    ${CMAKE_CURRENT_LIST_DIR}/src/config.xscope
    
)

set(APP_LINK_LIBRARIES
    lib_qspi_fast_read
)

set(SUPPORTED_BOARDS
    XK_VOICE_L71
    XCORE_AI_EXPLORER
)

foreach(BOARD ${SUPPORTED_BOARDS})
    #**********************
    # Targets
    #**********************
    set(TARGET_NAME example_ff_thruput_eval_${BOARD})
    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} PUBLIC ${APP_SOURCES})
    target_include_directories(${TARGET_NAME} PUBLIC ${APP_INCLUDES})
    target_compile_definitions(${TARGET_NAME} 
        PUBLIC
        ${APP_COMPILE_DEFINITIONS}
        ${BOARD}=1
    )
    target_compile_options(${TARGET_NAME}
        PRIVATE
            ${APP_COMPILER_FLAGS}
            ${CMAKE_CURRENT_LIST_DIR}/src/${BOARD}.xn
    )
    target_link_libraries(${TARGET_NAME} PUBLIC ${APP_LINK_LIBRARIES})
    target_link_options(${TARGET_NAME} 
        PRIVATE 
            ${APP_LINK_OPTIONS}
            ${CMAKE_CURRENT_LIST_DIR}/src/${BOARD}.xn
    )

    #**********************
    # Helper Targets
    #**********************
    add_custom_target(run_${TARGET_NAME}
        COMMAND xrun --xscope ${TARGET_NAME}.xe
        DEPENDS ${TARGET_NAME}
        COMMENT
            "Run application"
        VERBATIM
    )

    add_custom_target(debug_${TARGET_NAME}
        COMMAND xgdb ${TARGET_NAME}.xe -ex "connect" -ex "connect --xscope" -ex "run"
        DEPENDS ${TARGET_NAME}
        COMMENT
            "Debug application"
    )

    add_custom_target(flash_calibration_${TARGET_NAME}
        COMMAND xflash --write-all ${LIB_QSPI_FAST_READ_ROOT_PATH}/lib_qspi_fast_read/calibration_pattern.bin --target-file=${THRUPUT_EVAL_SRC_ROOT}/src/${BOARD}.xn
        COMMENT
            "Flash calibration binary"
    )
endforeach()

#**********************
# Helper Targets
#**********************
add_custom_target(flash_config_drive_str_50_XK_VOICE_L71
    COMMAND xflash --spi-command 0x06 --target-file=${THRUPUT_EVAL_SRC_ROOT}/src/XK_VOICE_L71.xn
    COMMAND xflash --spi-command 0x11 0 0x40 --target-file=${THRUPUT_EVAL_SRC_ROOT}/src/XK_VOICE_L71.xn
    COMMENT
        "Set W25Q64JW Status Register 3, drive strength to 50%"
)

add_custom_target(flash_config_drive_str_25_XK_VOICE_L71
    COMMAND xflash --spi-command 0x06 --target-file=${THRUPUT_EVAL_SRC_ROOT}/src/XK_VOICE_L71.xn
    COMMAND xflash --spi-command 0x11 0 0x60 --target-file=${THRUPUT_EVAL_SRC_ROOT}/src/XK_VOICE_L71.xn
    COMMENT
        "Set W25Q64JW Status Register 3, drive strength to 25%"
)

add_custom_target(flash_config_read_drive_str_XK_VOICE_L71
    COMMAND xflash --spi-command 0x15 1 --target-file=${THRUPUT_EVAL_SRC_ROOT}/src/XK_VOICE_L71.xn
    COMMENT
        "Read W25Q64JW Status Register 3"
)