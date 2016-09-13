cmake_minimum_required(VERSION 3.6)
project(FuzzySoccerPlayer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(ENVIRONM_FILES lib/environm/environm.cpp lib/environm/sock.cpp)
set(SOURCE_FILES src/main.cpp src/fuzzy/fuzzy.cpp src/fuzzy/fuzzyset.cpp)
add_executable(FuzzySoccerPlayer ${SOURCE_FILES} ${ENVIRONM_FILES})
