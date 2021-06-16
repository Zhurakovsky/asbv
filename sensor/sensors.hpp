#ifndef SENSOR_SENSORS_HPP
#define SENSOR_SENSORS_HPP

#include "msg_types.hpp"
#include "types.hpp"
#include <string>

namespace poc_autosar
{

class ISensor {
public:
    virtual ~ISensor() = default;
    virtual const std::string describe() = 0;
    virtual err_t read(SensorData &data) = 0;
};


class RandomSensor : public ISensor {
public:
    virtual const std::string describe() { return "Randomizer"; };
    err_t read(SensorData &data)
    {
        data.linear_speed = get_random();
        data.roll_angle = get_random();
        data.roll_accelleration = get_random();
        return RC_SUCCESS;
    };
};


class CarlaSensor : public ISensor {
    virtual const std::string describe() { return "Carla"; };
    err_t read(SensorData &data) {return RC_NOT_IMPLEMENTED; };
};


class RaspiSensor : public ISensor {
    virtual const std::string describe() { return "Raspberry"; };
    err_t read(SensorData &data) {return RC_NOT_IMPLEMENTED; };
};


class AutosarSensor : public ISensor {
    virtual const std::string describe() { return "Autosar"; };
    err_t read(SensorData &data) { return RC_NOT_IMPLEMENTED; };
};

}
#endif /* SENSOR_SENSORS_HPP */
