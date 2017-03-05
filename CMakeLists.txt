
cmake_minimum_required(VERSION 2.8)

project(sunshine CXX)
enable_testing()

set(SRC_DIR "${CMAKE_SOURCE_DIR}")
set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(LIB_DIR "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${LIB_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${LIB_DIR}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SRC_DIR}/cmake/")
include(Platform)
include(Utils)

set(CMAKE_CXX_STANDARD 14)
if(CLANG OR GCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")
endif()

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED
    COMPONENTS filesystem system
)
set(zpath_LIBRARIES ${Boost_LIBRARIES})

include_directories(
    ${Boost_INCLUDE_DIRS}
    include
)

add_definitions(-DENABLE_ZLOG_TO)

add_library(zlog STATIC src/zlog.cpp)

add_subdirectory(tests)

