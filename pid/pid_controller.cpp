#include "pid_controller.hpp"

using namespace std;
using namespace poc_autosar;
using namespace std::chrono_literals;


PidController::PidController(const PidControllerConfig& pid_config) : kp(pid_config.kp), ki(pid_config.ki), kd(pid_config.kd)
{
    init_pid_controller();
}


void PidController::init_pid_controller()
{
    prev_time = std::chrono::high_resolution_clock::now();
}

[[maybe_unused]] bool PidController::calculateActuatorValues(const SensorData& sensor_data, ActuatorData& actuator_data)
{
    auto call_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = call_time-prev_time;
    if (elapsed.count() < 10)
    {
        actuator_data.steer_angle = jaw_output;
        actuator_data.request_to_steer = true;
        return true;
    }
    prev_time = call_time;

    float e = roll_reference - sensor_data.roll_angle;
    float p_out = e * kp;
    
    integral += e * (elapsed.count() / 1000.0);
    if (integral > windup)
    {
        integral = windup;
    }
    else if (integral < (-1 * windup))
    {
        integral = (-1 * windup);
    }

    float i_out = integral * ki;
    float d_out;

    if ( !std::isnan(prev_error))
    {
        d_out = (e - prev_error) / ((elapsed.count()/1000.0) * kd);
    }
    else
    {
        d_out = 0.0;
        prev_error = 0.0;
    }

    roll_output = p_out + i_out + d_out;
    if (roll_output > roll_output_max)
    {
        roll_output = roll_output_max;
    }
    else if (roll_output < roll_output_min)
    {
        roll_output = roll_output_min;
    }


    if (roll_output < 0)
    {
        jaw_output = (roll_output / roll_output_min) * jaw_output_max;
    }
    else
    {
        jaw_output = (roll_output / roll_output_max) * jaw_output_min;
    }

    actuator_data.steer_angle = jaw_output;
    actuator_data.request_to_steer = true;
    return true;
}
