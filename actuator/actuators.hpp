#ifndef ACTUATOR_ACTUATORS_HPP
#define ACTUATOR_ACTUATORS_HPP

#include "msg_types.hpp"
#include "types.hpp"
#include <string>
#include <iostream>

namespace poc_autosar
{

class IActuator
{
public:
    virtual ~IActuator() {};
    virtual const std::string describe() = 0;
    virtual err_t write(const ActuatorData *data) = 0;
};


class StdoutActuator : public IActuator
{
public:
    virtual const std::string describe() override { return "Bypass"; };
    err_t write(const ActuatorData *data) override
    {
        if (data) {
            std::cout << "Request to steer: " << (data->request_to_steer ? "Yes" : "No") << std::endl;
            std::cout << "Steer angle: " << data->steer_ange << std::endl;
            return RC_SUCCESS;
        }
        return RC_FAIL;
    };
};


class CarlaActuator : public IActuator
{
    virtual const std::string describe() override { return "Carla"; };
    err_t write(const ActuatorData *data) override { return RC_NOT_IMPLEMENTED; };
};


class RaspberryActuator : public IActuator
{
    virtual const std::string describe() override { return "Raspberry"; };
    err_t write(const ActuatorData *data) override { return RC_NOT_IMPLEMENTED; };
};


class AutosarActuator : public IActuator
{
    virtual const std::string describe() override { return "Autosar"; };
    err_t write(const ActuatorData *data) override { return RC_NOT_IMPLEMENTED; };
};

}

#endif /* ACTUATOR_ACTUATORS_HPP */
