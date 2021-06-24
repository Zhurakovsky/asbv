#ifndef PID_PID_CONTROLLER_HPP
#define PID_PID_CONTROLLER_HPP

#include "msg_types.hpp"
#include "types.hpp"
#include <chrono>
#include <limits>

namespace poc_autosar
{

class PidController
{
    public:
        PidController(const PidControllerConfig& pid_config);
        ~PidController() = default;
        PidController(const PidController&) = delete;
        PidController(PidController&&) = delete;
        PidController& operator = (const PidController&) = delete;
        PidController& operator = (PidController&&) = delete;
        
        bool calculateActuatorValues(const SensorData& sensor_data, ActuatorData& actuator_data);

    private:
        void init_pid_controller();
        
        float roll_reference = 0.0; // Expected roll angle. Negative roll means roll left, positive means roll right
        float prev_error = std::numeric_limits<float>::quiet_NaN();
        float roll_output = 0.0;
        float roll_output_max = 10.0;
        float roll_output_min = -10.0;
        float jaw_output = 0.0;
        float jaw_output_max = 55.0;
        float jaw_output_min = -55.0;
        float integral = 0.0;
        float windup = 5.0;
        std::chrono::high_resolution_clock::time_point prev_time;

        const float kp{0.0}; // = 1.5;   // Proportional coef.
        const float ki{0.0}; // = 0.25;  // Integral coef.
        const float kd{0.0}; // = 2.5;   // Derivative coef.
};

}

#endif // PID_PID_CONTROLLER_HPP
