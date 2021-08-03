cmake_minimum_required(VERSION 3.10)

project(actuatorApp VERSION 0.0.1 LANGUAGES C CXX)

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

add_executable(actuatorApp
    actuator.cpp
    actuators.hpp
    pidtoactuatorreceivers.hpp
    socketActuator.cpp
    socketActuator.hpp
    raspi_actuator.hpp
    raspi_actuator.cpp
    ${COMMON_HEADERS_DIRECTORY}/msg_types.hpp
    ${COMMON_HEADERS_DIRECTORY}/types.hpp
    ${COMMON_HEADERS_DIRECTORY}/socketWrappers.cpp
    ${COMMON_HEADERS_DIRECTORY}/socketWrappers.hpp
    ${COMMON_HEADERS_DIRECTORY}/config_mgmt.hpp)

target_include_directories(actuatorApp PRIVATE ${COMMON_HEADERS_DIRECTORY})

target_link_libraries(actuatorApp Threads::Threads)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "armv7l")
if(WIRINGPI_LIBRARIES)
    target_link_libraries(${PROJECT_NAME}  ${WIRINGPI_LIBRARIES})
    ADD_DEFINITIONS( -D_WIRINGPI=\"1\")
endif()
endif()

add_custom_command(TARGET actuatorApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:actuatorApp> ${PROJECT_BINARY_DIR}/../actuatorApp
    COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE:actuatorApp> ${PROJECT_BINARY_DIR}
)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries (actuatorApp Eigen3::Eigen)
