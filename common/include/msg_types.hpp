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

struct MsgSensorToPid 
{
    PocMsgTypes msg_type;       // Part of linux message queue

    float linear_speed;         // TODO: Add units
    float roll_angle;           // TODO: Add units
    float roll_accelleration;   // TODO: Add units
};

struct MsgPidToActuator 
{
    PocMsgTypes msg_type;       // Part of linux message queue

    float steer_ange;           // TODO: Add units
    bool request_to_steer;      // TODO: Add legend
};

static float get_random()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(0, 1); // range 0 - 1
    return dis(e);
}


}

#endif // POC_EXAMPLE_COMMON_MSG_TYPES_HPP
