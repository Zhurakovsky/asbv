#ifndef POC_EXAMPLE_COMMON_TYPES_HPP
#define POC_EXAMPLE_COMMON_TYPES_HPP

namespace poc_autosar
{

enum err_t {
    RC_FAIL = -1,
    RC_SUCCESS,
    RC_TIMEOUT,
    RC_NOT_IMPLEMENTED
};

enum class Sensor : uint8_t
{
    UNDEFINED_SENSOR,
    RANDOM_SENSOR,
    CARLA_SENSOR,
    I2C_SENSOR,
    CAN_SENSOR,
    STATIC_SENSOR
};

enum class SensorToPid : uint8_t
{
    UNDEFINED_SENSOR_TO_PID,
    LINUX_SENSOR_TO_PID,
    CAN_SENSOR_TO_PID
};

enum class PidToActuator : uint8_t
{
    UNDEFINED_PID_TO_ACTUATOR,
    LINUX_PID_TO_ACTUATOR,
    CAN_PID_TO_ACTUATOR
};

enum class Actuator : uint8_t
{
    UNDEFINED_ACTUATOR,
    STDOUT_ACTUATOR,
    CARLA_ACTUATOR,
    PWM_ACTUATOR,
    CAN_ACTUATOR
};

struct LinuxMsgQueueConfig
{
    std::string pathname;
    int proj_id;
};

typedef LinuxMsgQueueConfig LinuxSensorToPidConfig;
typedef LinuxMsgQueueConfig LinuxPidToActuatorConfig;

struct SocketSensorConfig
{
    uint32_t port;
};

struct StaticSensorConfig
{
    float roll;
    float acc;
    float speed;
};

struct SensorSwcConfigType
{
    Sensor sensor{Sensor::UNDEFINED_SENSOR};
    SocketSensorConfig socket_sensor;
    StaticSensorConfig static_sensor;
    SensorToPid sensor_to_pid_sender{SensorToPid::UNDEFINED_SENSOR_TO_PID};
    LinuxSensorToPidConfig linux_sender_to_pid;
};

struct PidSwcConfigType
{
    SensorToPid sensor_to_pid_receiver{SensorToPid::UNDEFINED_SENSOR_TO_PID};
    PidToActuator pid_to_actuator_sender{PidToActuator::UNDEFINED_PID_TO_ACTUATOR};
    LinuxSensorToPidConfig linux_sender_to_pid;
    LinuxPidToActuatorConfig linux_pid_to_actuator;
};

struct SocketActuatorConfig
{
    std::string addr;
    uint32_t port;
};

struct ActuatorSwcConfigType
{
    PidToActuator pid_to_actuator_receiver{PidToActuator::UNDEFINED_PID_TO_ACTUATOR};
    Actuator actuator{Actuator::UNDEFINED_ACTUATOR};
    SocketActuatorConfig socket_actuator;
    LinuxPidToActuatorConfig linux_pid_to_actuator;
};

struct CommonSwcConfigType
{
    SensorSwcConfigType sensor;
    PidSwcConfigType pid;
    ActuatorSwcConfigType actuator;
};

}

#endif /* POC_EXAMPLE_COMMON_TYPES_HPP */
