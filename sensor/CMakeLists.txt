cmake_minimum_required(VERSION 3.10)

project(sensorApp VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_HEADERS_DIRECTORY ${COMMON_HEADERS_DIRECTORY}  PARENT_SCOPE)

add_executable(sensorApp sensor.cpp ${COMMON_HEADERS_DIRECTORY}/msg_types.hpp)

target_include_directories(sensorApp PRIVATE ${COMMON_HEADERS_DIRECTORY})

add_custom_command(TARGET sensorApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sensorApp> ${PROJECT_BINARY_DIR}/../sensorApp
    COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE:sensorApp> ${PROJECT_BINARY_DIR}
)
