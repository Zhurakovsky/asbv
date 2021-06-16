cmake_minimum_required(VERSION 3.10)

project(pidControllerApp VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_HEADERS_DIRECTORY ${COMMON_HEADERS_DIRECTORY}  PARENT_SCOPE)

add_executable(pidControllerApp pid.cpp 
                                pid_controller.hpp 
                                pid_controller.cpp 
                                ${COMMON_HEADERS_DIRECTORY}/msg_types.hpp
                                ${COMMON_HEADERS_DIRECTORY}/types.hpp
                                ${COMMON_HEADERS_DIRECTORY}/config_mgmt.hpp
                                SensorToPidReceiver/sensor_to_pid_receiver.h
                                PidToActuatorSender/pid_to_actuator_sender.h
)

target_include_directories(pidControllerApp PRIVATE ${COMMON_HEADERS_DIRECTORY})

add_custom_command(TARGET pidControllerApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pidControllerApp> ${PROJECT_BINARY_DIR}/../pidControllerApp
    COMMAND ${CMAKE_COMMAND} -E remove_directory $<TARGET_FILE:pidControllerApp> ${PROJECT_BINARY_DIR}
)
