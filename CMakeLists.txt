cmake_minimum_required(VERSION 2.8.11)
project(graph_segment)

add_library(graphseg SHARED
    segment.cpp)

target_include_directories(graphseg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
