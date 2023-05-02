## Import dependencies
include(FetchContent)

IF(EXISTS ${CMAKE_BINARY_DIR}/dependencies/unity)
    message(STATUS "Skipped: dependencies/unity")
else()
    message(STATUS "Fetching: dependencies/unity")
    FetchContent_Declare(
        unity
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG        v2.5.2
        GIT_SHALLOW    TRUE
        SOURCE_DIR     ${CMAKE_BINARY_DIR}/dependencies/unity
    )
    FetchContent_Populate(unity)
endif()

add_library(unity STATIC)

target_sources(unity
    PUBLIC
        ${CMAKE_BINARY_DIR}/dependencies/unity/src/unity.c
        ${CMAKE_BINARY_DIR}/dependencies/unity/extras/fixture/src/unity_fixture.c
        ${CMAKE_BINARY_DIR}/dependencies/unity/extras/memory/src/unity_memory.c
)
target_include_directories(unity
    PUBLIC
        ${CMAKE_BINARY_DIR}/dependencies/unity/src
        ${CMAKE_BINARY_DIR}/dependencies/unity/extras/fixture/src
        ${CMAKE_BINARY_DIR}/dependencies/unity/extras/memory/src
        ${CMAKE_CURRENT_LIST_DIR}/ci/unity_conf
)
target_compile_options(unity PRIVATE -Wno-xcore-fptrgroup)
target_compile_definitions(unity PUBLIC UNITY_INCLUDE_CONFIG_H=1)

include(${CMAKE_CURRENT_LIST_DIR}/ci/ci_tests.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/dev_app/app.cmake)

## While this should be inside the ci folder, it has to be here instead
## because of the directory structure required for xmake to find modules
include(${CMAKE_CURRENT_LIST_DIR}/legacy_build_test/app.cmake)