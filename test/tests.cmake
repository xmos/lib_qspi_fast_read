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

set(UNITY_EXTENSION_FIXTURE ON)
set(UNITY_EXTENSION_MEMORY ON)

add_subdirectory(${CMAKE_BINARY_DIR}/dependencies/unity)

target_compile_options(unity PRIVATE -Wno-xcore-fptrgroup)
target_compile_definitions(unity PUBLIC UNITY_INCLUDE_CONFIG_H=1)
target_include_directories(unity PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/ci/unity_conf>)

include(${CMAKE_CURRENT_LIST_DIR}/ci/api/app.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/ci/resource_reuse/app.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/ci/stress/app.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/dev_app/app.cmake)
