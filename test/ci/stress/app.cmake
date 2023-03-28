set(TEST_STRESS_SRC_ROOT ${CMAKE_CURRENT_LIST_DIR})

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
)

set(APP_COMPILE_DEFINITIONS
)

set(APP_LINK_OPTIONS
    -report
    -fcmdline-buffer-bytes=1024
)

set(APP_LINK_LIBRARIES
    lib_qspi_fast_read
    unity
)

#**********************
# Targets
#**********************
set(SYS_FREQS   600 800)
foreach(SYS_FREQ ${SYS_FREQS})
    set(TARGET_NAME test_stress_${SYS_FREQ})
    add_executable(${TARGET_NAME} EXCLUDE_FROM_ALL)
    target_sources(${TARGET_NAME} PUBLIC ${APP_SOURCES})
    target_include_directories(${TARGET_NAME} PUBLIC ${APP_INCLUDES})
    target_compile_definitions(${TARGET_NAME} PUBLIC ${APP_COMPILE_DEFINITIONS} SYS_FREQ=${SYS_FREQ})
    target_compile_options(${TARGET_NAME} PRIVATE ${APP_COMPILER_FLAGS} ${CMAKE_CURRENT_LIST_DIR}/src/XK_VOICE_L71_${SYS_FREQ}.xn)
    target_link_libraries(${TARGET_NAME} PUBLIC ${APP_LINK_LIBRARIES})
    target_link_options(${TARGET_NAME} PRIVATE ${APP_LINK_OPTIONS} ${CMAKE_CURRENT_LIST_DIR}/src/XK_VOICE_L71_${SYS_FREQ}.xn)

    #**********************
    # Helper Targets
    #**********************
    add_custom_target(run_test_stress_${SYS_FREQ}
        COMMAND xrun --io --args test_stress_${SYS_FREQ}.xe -v
        DEPENDS test_stress_${SYS_FREQ}
        COMMENT
        "Run application"
        VERBATIM
    )

    add_custom_target(run_test_stress_${SYS_FREQ}_help
        COMMAND xrun --io --args test_stress_${SYS_FREQ}.xe -h
        DEPENDS test_stress
        COMMENT
        "Run application"
        VERBATIM
    )
endforeach()
