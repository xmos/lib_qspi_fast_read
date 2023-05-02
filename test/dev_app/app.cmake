set(DEV_APP_SRC_ROOT ${CMAKE_CURRENT_LIST_DIR})

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
    -Wno-xcore-fptrgroup
    ${CMAKE_CURRENT_LIST_DIR}/src/XK_VOICE_L71.xn
)

set(APP_COMPILE_DEFINITIONS
)

set(APP_LINK_OPTIONS
    -report
    ${CMAKE_CURRENT_LIST_DIR}/src/XK_VOICE_L71.xn
)

set(APP_LINK_LIBRARIES
    lib_qspi_flash_fast_read
)

#**********************
# Targets
#**********************

set(TARGET_NAME dev_fast_flash_app)
add_executable(${TARGET_NAME} EXCLUDE_FROM_ALL)
target_sources(${TARGET_NAME} PUBLIC ${APP_SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC ${APP_INCLUDES})
target_compile_definitions(${TARGET_NAME} PUBLIC ${APP_COMPILE_DEFINITIONS})
target_compile_options(${TARGET_NAME} PRIVATE ${APP_COMPILER_FLAGS})
target_link_libraries(${TARGET_NAME} PUBLIC ${APP_LINK_LIBRARIES})
target_link_options(${TARGET_NAME} PRIVATE ${APP_LINK_OPTIONS})

#**********************
# Helper Targets
#**********************
add_custom_target(run_dev_fast_flash_app
    COMMAND xrun --io dev_fast_flash_app.xe
    DEPENDS dev_fast_flash_app
    COMMENT
    "Run application"
    VERBATIM
)

add_custom_target(debug_dev_fast_flash_app
    COMMAND xgdb dev_fast_flash_app.xe -ex "connect" -ex "connect --xscope" -ex "run"
    DEPENDS dev_fast_flash_app
    COMMENT
    "Debug application"
)

add_custom_target(flash_calibration_dev_fast_flash_app
    COMMAND xflash --write-all ${LIB_QSPI_FAST_READ_ROOT_PATH}/lib_qspi_fast_read/calibration_pattern.bin --target-file=${DEV_APP_SRC_ROOT}/src/XK_VOICE_L71.xn
    COMMENT
    "Flash calibration binary"
)

add_custom_target(xsim_dev_fast_flash_app
    COMMAND ${CMAKE_COMMAND} -E copy ${LIB_QSPI_FAST_READ_ROOT_PATH}/lib_qspi_fast_read/calibration_pattern.bin ./
    COMMAND ${CMAKE_COMMAND} -E copy ${LIB_QSPI_FAST_READ_ROOT_PATH}/test/dev_app/qspi.xml ./
    COMMAND xsim dev_fast_flash_app.xe --plugin QSPI.dll "-xml qspi.xml -core tile[0] XS1_PORT_1B XS1_PORT_1C XS1_PORT_4B"  --vcd-tracing "-o trace.vcd -tile tile[0] -cycles -ports-detailed -clock-blocks -cores -instructions"
    DEPENDS dev_fast_flash_app
    COMMENT
    "Run in xsim"
)
