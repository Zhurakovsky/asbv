#ifndef ACTUATOR_RASPI_ACTUATOR_HPP
#define ACTUATOR_RASPI_ACTUATOR_HPP

#include "actuators.hpp"

namespace poc_autosar
{

class RaspiActuator : public IActuator
{
public:
    RaspiActuator(/*Config with GPIO data should be here*/)
    {
        initActuator();
    }

    const std::string describe() override;
    err_t write(const ActuatorData &data) override;

private:
    void initActuator();

    int pwm_pin{1};
    int intensity;
    int current_intensity = 15;
    int min_intensity = 9;
    int max_intensity = 22;
    float min_jaw = -55.0;
    float current_jaw = 0;
    float max_jaw = 55.0;
    float step = 9.16;

};


}

#endif // ACTUATOR_RASPI_ACTUATOR_HPP
