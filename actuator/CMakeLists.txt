cmake_minimum_required(VERSION 3.10)

project(actuatorApp VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(actuatorApp actuator.cpp)

add_custom_command(TARGET actuatorApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:actuatorApp> ${PROJECT_BINARY_DIR}/../actuatorApp
    COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE:actuatorApp> ${PROJECT_BINARY_DIR}
)
