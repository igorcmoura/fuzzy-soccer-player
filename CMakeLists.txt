cmake_minimum_required(VERSION 3.6)
project(FuzzySoccerPlayer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(ENVIRONM_FILES lib/environm/environm.cpp lib/environm/sock.cpp)
set(SOURCE_FILES
        src/main.cpp
        src/fuzzy/fuzzy.cpp
        src/fuzzy/fuzzy_collection.cpp
        src/fuzzy/fuzzy_player.cpp
        src/fuzzy/fuzzy_rule.cpp
        src/fuzzy/fuzzy_set.cpp
        src/fuzzy/fuzzy_system.cpp)
add_executable(FuzzySoccerPlayer ${SOURCE_FILES} ${ENVIRONM_FILES})

add_custom_command(TARGET FuzzySoccerPlayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/cyggcc_s-seh-1.dll"
        $<TARGET_FILE_DIR:FuzzySoccerPlayer>)
add_custom_command(TARGET FuzzySoccerPlayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/cygstdc++-6.dll"
        $<TARGET_FILE_DIR:FuzzySoccerPlayer>)
add_custom_command(TARGET FuzzySoccerPlayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/cygwin1.dll"
        $<TARGET_FILE_DIR:FuzzySoccerPlayer>)

add_custom_command(TARGET FuzzySoccerPlayer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/rules_description.txt"
        $<TARGET_FILE_DIR:FuzzySoccerPlayer>)
