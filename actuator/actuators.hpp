#ifndef ACTUATOR_ACTUATORS_HPP
#define ACTUATOR_ACTUATORS_HPP

#include <string>
#include <iostream>
#include <iomanip>

#include "msg_types.hpp"
#include "types.hpp"
#include <iomanip>

namespace poc_autosar
{

class IActuator
{
public:
    virtual ~IActuator() = default;
    virtual const std::string describe() = 0;
    virtual err_t write(const ActuatorData &data) = 0;
};


class StdoutActuator : public IActuator
{
public:
    virtual const std::string describe() override { return "Bypass"; }
    err_t write(const ActuatorData &data) override
    {
        static const float min_jaw = -55.0;
        static float max_jaw = 55.0;
        static float step = 9.16;
        static int half_range = 6;

        float current_intensity;
        float absolute_jaw;
        float desired_jaw = data.steer_angle;

        if (desired_jaw < (min_jaw))
        {
            desired_jaw = (min_jaw);
            absolute_jaw = std::fabs(desired_jaw - (min_jaw));
        }
        else if (desired_jaw < 0.0)
        {
            absolute_jaw = std::fabs(min_jaw) - std::fabs(desired_jaw);
        }
        else if (desired_jaw > max_jaw)
        {
            desired_jaw = max_jaw;
            absolute_jaw = desired_jaw + max_jaw;
        }
        else
        {
            absolute_jaw = desired_jaw + max_jaw;
        }

        current_intensity = std::ceil((absolute_jaw / step)) + half_range;

        std::cout << std::setw(10) << "Desired jaw: " << desired_jaw << ", ABS angle: " << absolute_jaw << ", PWM Pulses: " << current_intensity << std::endl; 

        return RC_SUCCESS;
    }
};


class AutosarActuator : public IActuator
{
    virtual const std::string describe() override { return "Autosar"; }
    err_t write(const ActuatorData &data) override { return RC_NOT_IMPLEMENTED; }
};

}

#endif /* ACTUATOR_ACTUATORS_HPP */
