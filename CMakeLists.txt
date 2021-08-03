cmake_minimum_required(VERSION 3.10)
project(AutosarPocApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_HEADERS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/common/include)

message("MY PROCESSOR *******" ${CMAKE_SYSTEM_PROCESSOR})
message("MY SYSTEM *******" ${CMAKE_SYSTEM_NAME})

find_package(Threads)

add_subdirectory(sensor)
add_subdirectory(pid)
add_subdirectory(actuator)

add_executable(AutosarPocApp
    main.cpp
    config_parser/config_parser.cpp
    config_parser/config_parser.hpp
    ${COMMON_HEADERS_DIRECTORY}/types.hpp
)

target_include_directories(AutosarPocApp PUBLIC ${COMMON_HEADERS_DIRECTORY})

target_link_libraries(AutosarPocApp ${CMAKE_THREAD_LIBS_INIT})

