cmake_minimum_required(VERSION 3.0)

project(argparser)

set(CMAKE_CXX_STANDARD 20)
find_package(Boost 1.74.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(cli cli/main.cpp)

target_link_libraries(cli ${Boost_LIBRARIES})

add_library(
        argparse
        source/argparse/argparse.cpp
)

target_include_directories(
        argparse PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(cli argparse)