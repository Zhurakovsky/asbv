#ifndef POC_EXAMPLE_COMMON_MSG_TYPES_HPP
#define POC_EXAMPLE_COMMON_MSG_TYPES_HPP

#include <random>

namespace poc_autosar
{

enum PocMsgTypes : long 
{
    SENSOR_TO_PID = 1,
    PID_TO_ACTUATOR
};

struct SensorData
{
    float linear_speed;         // meters per second
    float roll_angle;           // gegree
    float roll_accelleration;   // degree per second
};

struct MsgSensorToPid 
{
    PocMsgTypes msg_type;       // Part of linux message queue
    SensorData sensor_data;
};

struct ActuatorData
{
    float steer_ange;           // degree
    bool request_to_steer;      // for speed around zero should be true
};

struct MsgPidToActuator 
{
    PocMsgTypes msg_type;       // Part of linux message queue
    ActuatorData actuator_data;
};

static float get_random()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(0, 1); // range 0 - 1
    return dis(e);
}


}

#endif // POC_EXAMPLE_COMMON_MSG_TYPES_HPP
