
cmake_minimum_required(VERSION 2.8)

project(sunlib CXX)

set(SRC_DIR "${CMAKE_SOURCE_DIR}")
set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(LIB_DIR "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SRC_DIR}/cmake/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${LIB_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${LIB_DIR}")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

add_definitions(-DENABLE_ZLOG_TO)

find_package(Boost REQUIRED
    COMPONENTS filesystem system
)

include_directories(
    ${Boost_INCLUDE_DIRS}
    include
)

add_library(zlog STATIC src/zlog.cpp)

add_library(zpath STATIC src/zpath.cpp)
target_link_libraries(zpath ${Boost_LIBRARIES})

add_subdirectory(tests)
