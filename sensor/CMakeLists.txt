cmake_minimum_required(VERSION 3.10)

project(sensorApp VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_HEADERS_DIRECTORY ${COMMON_HEADERS_DIRECTORY}  PARENT_SCOPE)

find_package(Threads)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "armv7l")
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
if(NOT WIRINGPI_LIBRARIES)
  message("WiringPi not found")
endif()
endif()

add_executable(sensorApp
    sensor.cpp
    sensors.hpp
    sensortopidsenders.hpp
    socketSensor.cpp
    socketSensor.hpp
    raspi_sensor.hpp
    raspi_sensor.cpp
    ${COMMON_HEADERS_DIRECTORY}/msg_types.hpp
    ${COMMON_HEADERS_DIRECTORY}/types.hpp
    ${COMMON_HEADERS_DIRECTORY}/config_mgmt.hpp
    ${COMMON_HEADERS_DIRECTORY}/socketWrappers.cpp
    ${COMMON_HEADERS_DIRECTORY}/socketWrappers.hpp
    ${COMMON_HEADERS_DIRECTORY}/log_manager.cpp
    ${COMMON_HEADERS_DIRECTORY}/log_manager.hpp
)

target_include_directories(sensorApp PUBLIC ${COMMON_HEADERS_DIRECTORY})

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "armv7l")
if(WIRINGPI_LIBRARIES)
    target_link_libraries(${PROJECT_NAME}  ${WIRINGPI_LIBRARIES})
    ADD_DEFINITIONS( -D_WIRINGPI=\"1\")
endif()
endif()

target_link_libraries(sensorApp 
    PRIVATE quill::quill
    ${CMAKE_THREAD_LIBS_INIT}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sensorApp> ${PROJECT_BINARY_DIR}/../sensorApp
    COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE:sensorApp> ${PROJECT_BINARY_DIR}
)
