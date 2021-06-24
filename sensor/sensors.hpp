#ifndef SENSOR_SENSORS_HPP
#define SENSOR_SENSORS_HPP

#include "msg_types.hpp"
#include "types.hpp"
#include <string>
#include <cmath>
#include <iostream>

namespace poc_autosar
{

class ISensor {
public:
    virtual ~ISensor() {};
    virtual const std::string describe() const = 0;
    virtual err_t read(SensorData& data) = 0;
};


class RandomSensor : public ISensor {
public:
    virtual const std::string describe() const { return "Randomizer"; }
    err_t read(SensorData& data)
    {
        data.linear_speed =         get_random();
        data.roll_angle =           get_random();
        data.roll_accelleration =   get_random();
        
        return RC_SUCCESS;
    }
};

class AutosarSensor : public ISensor {
public:
    virtual const std::string describe() const { return "Autosar"; }
    err_t read(SensorData& data) override { return RC_NOT_IMPLEMENTED; }
};

class StaticSensor : public ISensor {
public:
    StaticSensor(const StaticSensorConfig& config)
    : roll_angle(config.roll), accelleration(config.acc), speed(config.speed)
    {
        std::cout << "Static Sensor Constructor. roll = " << config.roll << std::endl;
     }
    virtual const std::string describe() const { return "Static Sensor"; }
    err_t read(SensorData& data) override 
    {
        std::cout << "SCAN. angle == " << roll_angle << std::endl;
        data.roll_angle = roll_angle;
        data.roll_accelleration = accelleration;
        data.linear_speed = speed;

        return RC_SUCCESS;
    }

private:
    float roll_angle;
    float accelleration;
    float speed;

};

//TYPE_STATIC//

}
#endif /* SENSOR_SENSORS_HPP */
