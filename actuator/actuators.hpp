#ifndef ACTUATOR_ACTUATORS_HPP
#define ACTUATOR_ACTUATORS_HPP

#include <string>
#include <iostream>

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
    virtual const std::string describe() override { return "Bypass"; };
    err_t write(const ActuatorData &data) override
    {
        std::cout <<  "ACTUATOR: RTS: " << (data.request_to_steer ? "Yes" : "No") ;
        std::cout << " Steer angle: " << data.steer_ange << std::endl;
        return RC_SUCCESS;
    };
};


class CarlaActuator : public IActuator
{
    virtual const std::string describe() override { return "Carla"; };
    err_t write(const ActuatorData &data) override { return RC_NOT_IMPLEMENTED; };
};


class RaspberryActuator : public IActuator
{
    virtual const std::string describe() override { return "Raspberry"; };
    err_t write(const ActuatorData &data) override { return RC_NOT_IMPLEMENTED; };
};


class AutosarActuator : public IActuator
{
    virtual const std::string describe() override { return "Autosar"; };
    err_t write(const ActuatorData &data) override { return RC_NOT_IMPLEMENTED; };
};

}

#endif /* ACTUATOR_ACTUATORS_HPP */
