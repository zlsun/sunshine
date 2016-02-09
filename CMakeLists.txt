
cmake_minimum_required(VERSION 2.8)

project(sunlib CXX)

set(SRC_DIR "${CMAKE_SOURCE_DIR}")
set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(LIB_DIR "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SRC_DIR}/cmake/")
include(Utils)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED
    COMPONENTS filesystem system
)

include_directories(
    ${Boost_INCLUDE_DIRS}
    include
)

add_library(zpath STATIC src/zpath.cpp)
target_link_libraries(zpath ${Boost_LIBRARIES})
set_target_properties(zpath PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${LIB_DIR}"
)

add_subdirectory(tests)

