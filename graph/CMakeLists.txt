# Copyright (c) 2016 Andrew Sutton
# All rights reserved

cmake_minimum_required(VERSION 3.0)
project(graph CXX)

enable_testing()

set(CMAKE_CXX_FLAGS "-std=c++1z -fconcepts")

add_library(graph
  utility.cpp
  common.cpp
  graph.cpp
  digraph.cpp
  output.cpp
  dfs.cpp
  queue.cpp
)


macro(add_unit_test target)
  add_executable(${target} ${ARGN})
  target_link_libraries(${target} graph)
  add_test(${target} ${target})
endmacro()


add_subdirectory(graph.test)
add_subdirectory(digraph.test)
add_subdirectory(dfs.test)
add_subdirectory(queue.test)
