#ifndef SENSOR_RASPI_SENSOR_HPP
#define SENSOR_RASPI_SENSOR_HPP

#include <string>

#include "msg_types.hpp"
#include "types.hpp"
#include "sensors.hpp"

namespace poc_autosar
{

class RaspiSensor : public ISensor 
{
public:
    RaspiSensor(/*Config with GPIO data should be here*/)
    {
        initSensor();
    }

    ~RaspiSensor()= default;

    const std::string describe() const override;
    err_t read(SensorData& data) override;
private:
    void initSensor();
    int read_word_2c(int addr);

    int acclX, acclY, acclZ;
    int gyroX, gyroY, gyroZ;
    double acclX_scaled, acclY_scaled, acclZ_scaled;
    double gyroX_scaled, gyroY_scaled, gyroZ_scaled;
};

}

#endif // SENSOR_RASPI_SENSOR_HPP
