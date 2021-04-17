cmake_minimum_required (VERSION 2.8)

add_definitions(-std=c++11)
project (demo)

find_package(OpenCV REQUIRED)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-pthread")   

SET(CMAKE_BUILD_TYPE Release)
aux_source_directory(. SRC_LIST) 
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)

add_executable(main ${SRC_LIST})
target_link_libraries(main ${OpenCV_LIBS} ${WIRINGPI_LIBRARIES})

add_executable(GPIO ./GPIO/GPIO.cpp)
target_link_libraries(GPIO ${OpenCV_LIBS} ${WIRINGPI_LIBRARIES})

