cmake_minimum_required(VERSION 3.10)
project(AutosarPocApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_HEADERS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/common/include)

add_subdirectory(sensor)
add_subdirectory(pid)
add_subdirectory(actuator)

add_executable(AutosarPocApp main.cpp)

target_include_directories(AutosarPocApp PRIVATE ${COMMON_HEADERS_DIRECTORY})
