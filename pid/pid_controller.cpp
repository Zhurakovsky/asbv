#include "pid_controller.hpp"

using namespace poc_autosar;

namespace
{
    const float SPEED_THRESHOLD = 0.5; // Will be set experimentally
}

[[maybe_unused]] bool PidController::calculateActuatorValues(const SensorData& sensor_data, ActuatorData& actuator_data)
{
    actuator_data.steer_ange = get_random();
   
    return (actuator_data.request_to_steer = (sensor_data.linear_speed < SPEED_THRESHOLD));
}
