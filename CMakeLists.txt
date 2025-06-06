# This CMakeLists.txt is adapted and minimized from the following:
#    https://github.com/mortinger91/cpp-cmake-template/blob/master/projectFolder/CMakeLists.txt
# 

cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(
    "tg_20250606"
    DESCRIPTION "tg_20250606"
    HOMEPAGE_URL "https://github.com/kinchungwong/tg_20250606"
)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
endif()

# Dependencies
find_package(OpenCV REQUIRED PATHS "${CMAKE_SOURCE_DIR}/opencv_build" NO_DEFAULT_PATH)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/int")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")

add_executable(${PROJECT_NAME} src/main.cpp)
add_subdirectory(src)
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        ${OpenCV_LIBS}
    PRIVATE
        ${PROJECT_NAME}_LIB
)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
