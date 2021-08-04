cmake_minimum_required(VERSION 3.10)
project(AutosarPocApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_HEADERS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/common/include)
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(OUTPUT_DIR ${PROJECT_BINARY_DIR})
set(CMAKE_INSTALL_RPATH "${OUTPUT_DIR}")

message("MY PROCESSOR *******" ${CMAKE_SYSTEM_PROCESSOR})
message("MY SYSTEM *******" ${CMAKE_SYSTEM_NAME})

find_package(Threads)

add_subdirectory(plotting)
add_subdirectory(quill)
add_subdirectory(sensor)
add_subdirectory(pid)
add_subdirectory(actuator)
add_subdirectory(webui)

add_executable(AutosarPocApp
    main.cpp
    config_parser/config_parser.cpp
    config_parser/config_parser.hpp
    ${COMMON_HEADERS_DIRECTORY}/types.hpp
    ${COMMON_HEADERS_DIRECTORY}/log_manager.hpp
    ${COMMON_HEADERS_DIRECTORY}/log_manager.cpp
)

target_include_directories(AutosarPocApp PUBLIC ${COMMON_HEADERS_DIRECTORY})

target_link_libraries(AutosarPocApp
    PRIVATE quill::quill
    ${CMAKE_THREAD_LIBS_INIT}
)

