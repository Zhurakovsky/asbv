cmake_minimum_required(VERSION 3.10)

project(plotting VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_HEADERS_DIRECTORY ${COMMON_HEADERS_DIRECTORY}  PARENT_SCOPE)
set(OUTPUT_DIR ${OUTPUT_DIR}  PARENT_SCOPE)

find_package(Threads)

add_library(plotting SHARED
            plotting.cpp
            plotting.hpp
            ${COMMON_HEADERS_DIRECTORY}/types.hpp
            ${COMMON_HEADERS_DIRECTORY}/config_mgmt.hpp
            ${COMMON_HEADERS_DIRECTORY}/socketWrappers.cpp
            ${COMMON_HEADERS_DIRECTORY}/socketWrappers.hpp
)

target_link_libraries(plotting Threads::Threads)

target_include_directories(plotting PRIVATE ${COMMON_HEADERS_DIRECTORY})

set_target_properties(plotting PROPERTIES PUBLIC_HEADER plotting.hpp)

install(TARGETS plotting
    LIBRARY DESTINATION ${OUTPUT_DIR}
    PUBLIC_HEADER DESTINATION ${OUTPUT_DIR}
)
