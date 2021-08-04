cmake_minimum_required(VERSION 3.10)

project(pidControllerApp VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_HEADERS_DIRECTORY ${COMMON_HEADERS_DIRECTORY}  PARENT_SCOPE)
set(OUTPUT_DIR ${OUTPUT_DIR}  PARENT_SCOPE)

find_package(Threads)

add_executable(pidControllerApp pid.cpp 
                                pid_controller.hpp 
                                pid_controller.cpp 
                                ${COMMON_HEADERS_DIRECTORY}/msg_types.hpp
                                ${COMMON_HEADERS_DIRECTORY}/types.hpp
                                ${COMMON_HEADERS_DIRECTORY}/config_mgmt.hpp
                                SensorToPidReceiver/sensor_to_pid_receiver.h
                                PidToActuatorSender/pid_to_actuator_sender.h
)

target_link_libraries(pidControllerApp Threads::Threads)

target_include_directories(pidControllerApp PRIVATE ${COMMON_HEADERS_DIRECTORY})

install(TARGETS pidControllerApp
    RUNTIME DESTINATION ${OUTPUT_DIR}
)
