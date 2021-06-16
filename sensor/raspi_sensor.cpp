#include <iostream>
#include "raspi_sensor.hpp"

#ifdef _WIRINGPI
#include <wiringPiI2C.h>
#include <wiringPi.h>
#endif //_WIRINGPI

namespace
{

static int fd = -1;

double dist(double a, double b)
{
    return sqrt((a*a) + (b*b));
}

double get_y_rotation(double x, double y, double z)
{
    double radians;
    radians = atan2(x, dist(y, z));
    return -(radians * (180.0 / M_PI));
}

double get_x_rotation(double x, double y, double z)
{
    double radians;
    radians = atan2(y, dist(x, z));
    return (radians * (180.0 / M_PI));
}

double get_z_rotation(double x, double y, double z)
{
    double radians;
    radians = atan2(z, dist(y, x));
    return (radians * (180.0 / M_PI));
}

}

using namespace std;
using namespace poc_autosar;

const std::string RaspiSensor::describe() const
{
    return "Raspberry Sensor";
};

int RaspiSensor::read_word_2c(int addr)
{
    int val = -1;
    #ifdef _WIRINGPI
        val = wiringPiI2CReadReg8(fd, addr);
        val = val << 8;
        val += wiringPiI2CReadReg8(fd, addr+1);
        if (val >= 0x8000)
        val = -(65536 - val);
    #endif //_WIRINGPI
    return val;
}

err_t RaspiSensor::read(SensorData& data)
{
#ifdef _WIRINGPI
    acclX = read_word_2c(0x3B);
    acclY = read_word_2c(0x3D);
    acclZ = read_word_2c(0x3F);
#else
    cout << "Wiring Pi not found on this computer" << endl;
    return RC_FAIL;
#endif //_WIRINGPI
    acclX_scaled = acclX / 16384.0;
    acclY_scaled = acclY / 16384.0;
    acclZ_scaled = acclZ / 16384.0;

    data.roll_accelleration = acclX_scaled;
    data.roll_angle = get_x_rotation(acclX_scaled, acclY_scaled, acclZ_scaled);
    //cout << "Roll Acceleration = " << acclX_scaled << ", Roll Angle = " << data.roll_angle << endl;
    data.linear_speed = 0.0;

    acclX = 0.0;
    acclY = 0.0;
    acclZ = 0.0;
    return RC_SUCCESS;
};

void RaspiSensor::initSensor()
{
#ifdef _WIRINGPI
    cout << "Wiring Pi found. Do init." << endl;
    fd = wiringPiI2CSetup (0x68);
    wiringPiI2CWriteReg8 (fd,0x6B,0x00);//disable sleep mode
    cout << "set 0x6B=" << std::hex << (wiringPiI2CReadReg8 (fd,0x6B)) << endl;

#else
    cout << "Wiring Pi not found on this computer" << endl;
#endif //_WIRINGPI
}