cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(mp3Player)

add_library(mp3Player mp3Player.cpp)

target_link_libraries(mp3Player pthread)