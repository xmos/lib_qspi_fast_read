set(TEST_GENERAL_SRC_ROOT ${CMAKE_CURRENT_LIST_DIR})

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
    -fcmdline-buffer-bytes=1024
    ${CMAKE_CURRENT_LIST_DIR}/src/XK_VOICE_L71.xn
)

set(APP_COMPILE_DEFINITIONS
    CLK_DIVIDE=3
)

set(APP_LINK_OPTIONS
    -report
    -fcmdline-buffer-bytes=1024
    ${CMAKE_CURRENT_LIST_DIR}/src/XK_VOICE_L71.xn
)

set(APP_LINK_LIBRARIES
    lib_qspi_fast_read
    unity
)

#**********************
# Targets
#**********************

set(TARGET_NAME test_general)
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
add_custom_target(run_test_general
    COMMAND xrun --io --args test_general.xe -v
    DEPENDS test_general
    COMMENT
    "Run application"
    VERBATIM
)

add_custom_target(run_test_general_help
    COMMAND xrun --io --args test_general.xe -h
    DEPENDS test_general
    COMMENT
    "Run application"
    VERBATIM
)