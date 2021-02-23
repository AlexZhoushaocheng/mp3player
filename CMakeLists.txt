cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

add_definitions(-std=c++11)

project(mp3Player)

add_library(mp3Player SHARED mp3Player.cpp)

target_link_libraries(mp3Player pthread portaudio)

install(TARGETS mp3Player  DESTINATION ${CMAKE_INSTALL_PREFIX}/mp3Player/lib)
install(FILES mp3Player.h  DESTINATION ${CMAKE_INSTALL_PREFIX}/mp3Player/include)
