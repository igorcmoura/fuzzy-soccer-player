cmake_minimum_required(VERSION 3.6)
project(FuzzySoccerPlayer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/fuzzylib.h src/fuzzylib.cpp)
add_executable(FuzzySoccerPlayer ${SOURCE_FILES})