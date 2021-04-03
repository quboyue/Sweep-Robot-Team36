cmake_minimum_required (VERSION 2.8)

add_definitions(-std=c++11)
project (demo)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-pthread")   
SET(CMAKE_BUILD_TYPE Release)
aux_source_directory(. SRC_LIST)

add_executable(main ${SRC_LIST})
