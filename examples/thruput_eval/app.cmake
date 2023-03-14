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
    ${CMAKE_CURRENT_LIST_DIR}/src/XK_VOICE_L71.xn
)

set(APP_COMPILE_DEFINITIONS
)

set(APP_LINK_OPTIONS
    -report
    ${CMAKE_CURRENT_LIST_DIR}/src/config.xscope
    ${CMAKE_CURRENT_LIST_DIR}/src/XK_VOICE_L71.xn
)

set(APP_LINK_LIBRARIES
    lib_qspi_flash_fast_read
)

#**********************
# Targets
#**********************

set(TARGET_NAME example_ff_thruput_eval)
add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME} PUBLIC ${APP_SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC ${APP_INCLUDES})
target_compile_definitions(${TARGET_NAME} PUBLIC ${APP_COMPILE_DEFINITIONS})
target_compile_options(${TARGET_NAME} PRIVATE ${APP_COMPILER_FLAGS})
target_link_libraries(${TARGET_NAME} PUBLIC ${APP_LINK_LIBRARIES})
target_link_options(${TARGET_NAME} PRIVATE ${APP_LINK_OPTIONS})

#**********************
# Helper Targets
#**********************
add_custom_target(run_example_ff_thruput_eval
    COMMAND xrun --xscope example_ff_thruput_eval.xe
    DEPENDS example_ff_thruput_eval
    COMMENT
    "Run application"
    VERBATIM
)

add_custom_target(debug_example_ff_thruput_eval
    COMMAND xgdb example_ff_thruput_eval.xe -ex "connect" -ex "connect --xscope" -ex "run"
    DEPENDS example_ff_thruput_eval
    COMMENT
    "Debug application"
)

add_custom_target(flash_calibration_example_ff_thruput_eval
    COMMAND xflash --write-all ${LIB_QSPI_FAST_READ_ROOT_PATH}/lib_qspi_fast_read/calibration_pattern.bin --target-file=${THRUPUT_EVAL_SRC_ROOT}/src/XK_VOICE_L71.xn
    COMMENT
    "Flash calibration binary"
)
