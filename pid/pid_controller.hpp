#ifndef PID_PID_CONTROLLER_HPP
#define PID_PID_CONTROLLER_HPP

#include "msg_types.hpp"

namespace poc_autosar
{

class PidController
{
    public:
        PidController() = default;
        ~PidController() = default;

        PidController(const PidController&) = delete;
        PidController(PidController&&) = delete;
        PidController& operator = (const PidController&) = delete;
        PidController& operator = (PidController&&) = delete;
        
        bool calculateActuatorValues(const SensorData& sensor_data, ActuatorData& actuator_data);

    private:
        // TODO
};

}

#endif // PID_PID_CONTROLLER_HPP
