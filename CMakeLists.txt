cmake_minimum_required(VERSION 3.5.1)
project(ABAQS_Compiler)

file(GLOB sources src/*)

add_executable(abaqs ${sources})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++17")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wfatal-errors")
