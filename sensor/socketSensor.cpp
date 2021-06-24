#include "socketSensor.hpp"

#include "types.hpp"
#include "socketWrappers.hpp"
#include <arpa/inet.h>
#include <unistd.h>

using namespace poc_autosar;

namespace
{

double dist(double a, double b)
{
    return sqrt((a*a) + (b*b));
}

double get_x_rotation(double x, double y, double z)
{
    double radians;
    radians = atan2(y, dist(x, z));
    return (radians * (180.0 / M_PI));
}

}

SocketSensor::SocketSensor(const SocketSensorConfig& config)
{
    mHost = config.port;
    Init();
}

void SocketSensor::Init()
{
    mServer = SocketWrappers::Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(mHost);

    SocketWrappers::Bind(mServer, (struct sockaddr*) &adr, sizeof adr);
    SocketWrappers::Listen(mServer, 1);
    
    socklen_t adrlen = sizeof adr;
    mFileDescriptor = SocketWrappers::Accept(mServer, (struct sockaddr*) &adr, &adrlen);
}


SocketSensor::~SocketSensor()
{
    close(mFileDescriptor);
    close(mServer);
}

err_t SocketSensor::read(SensorData& data)
{
    const size_t msgLen = 512;
    char buf[msgLen];
    
    if (SocketWrappers::Read(mFileDescriptor, buf, msgLen))
    {
        std::cout << "SENSOR SOCKET READ: " << buf << std::endl;
        
        sscanf(buf, R"({"accel": {"x": %f, "y": %f, "z": %f}, "gyro": {"x": %f, "y": %f, "z": %f}, "velocity": {"x": %f, "y": %f, "z": %f})", &acclX, &acclY, &acclZ, &gyroX, &gyroY, &gyroZ, &veloX, &veloY, &veloZ);
        
        data.roll_accelleration = acclX;
        float tmp_roll_angle = get_x_rotation(acclX, acclY, acclZ);
        data.roll_angle = round( tmp_roll_angle * 1000.0 ) / 1000.0;
        data.linear_speed = 0.0;

        return RC_SUCCESS;
    }

    return RC_FAIL; 
}
