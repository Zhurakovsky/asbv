cmake_minimum_required(VERSION 3.10)
project(AutosarPocApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(sensor)
add_subdirectory(pid)
add_subdirectory(actuator)

add_executable(AutosarPocApp main.cpp)
