#include <iostream>
#include <cmath>
#include "raspi_actuator.hpp"

#ifdef _WIRINGPI
#include <softPwm.h> 
#include <wiringPi.h>
#endif //_WIRINGPI

using namespace std;
using namespace poc_autosar;

void RaspiActuator::initActuator()
{
#ifdef _WIRINGPI
    wiringPiSetup();		        /* initialize wiringPi setup */
    pinMode(pwm_pin,PWM_OUTPUT);	/* set GPIO as output */
    softPwmCreate(pwm_pin,0,100);	/* set PWM channel along with range*/
    cout << "Wiring Pi found. Do init actuator" << endl;
#else
    cout << "Wiring Pi not found on this computer" << endl;
#endif //_WIRINGPI

}

const std::string RaspiActuator::describe()
{
    return "Raspberry actuator";
};

err_t RaspiActuator::write(const ActuatorData &data)
{
    float absolute_jaw;
    float desired_jaw = data.steer_angle;
    if (desired_jaw < min_jaw)
    {
        desired_jaw = min_jaw;
        absolute_jaw = std::fabs(desired_jaw - min_jaw);
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

    current_intensity = std::ceil((absolute_jaw / step)) +
    ((max_intensity - min_intensity) / 2); 
#ifdef _WIRINGPI
    softPwmWrite (pwm_pin, current_intensity); /* change the value of PWM */
    return RC_SUCCESS;
#else
    return RC_NOT_IMPLEMENTED;
#endif //_WIRINGPI
    
};