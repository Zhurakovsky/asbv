#include "socketSensor.hpp"

#include "types.hpp"
#include "socketWrappers.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace poc_autosar;

SocketSensor::SocketSensor(const SocketSensorConfig& config)
{
    mHost = config.port;
    mNetworkAddress = config.addr;
    Init();
}

void SocketSensor::Init()
{
    mFileDescriptor = SocketWrappers::Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(mHost);

    SocketWrappers::Inet_pton(AF_INET, mNetworkAddress.c_str(), &adr.sin_addr);

    SocketWrappers::Connect(mFileDescriptor, (struct sockaddr*) &adr, sizeof adr);
}


SocketSensor::~SocketSensor()
{
    close(mFileDescriptor);
}

err_t SocketSensor::read(SensorData& data)
{
    const size_t msgLen = 512;
    char buf[msgLen];
    memset(buf, 0, msgLen);

    if (SocketWrappers::Read(mFileDescriptor, buf, msgLen) != RC_FAIL)
    {
        sscanf(buf, R"({"accel": {"x": %f, "y": %f, "z": %f}, "gyro": {"x": %f, "y": %f, "z": %f}, "velocity": {"x": %f, "y": %f, "z": %f}, "timestamp": %f})", &acclX, &acclY, &acclZ, &gyroX, &gyroY, &gyroZ, &veloX, &veloY, &veloZ, &timestamp);
        
        data.roll_accelleration = acclX;
        data.linear_speed = 0.0;
        
        double radians = gyroX * (timestamp - timestampPrev);
        double degrees = (radians * (180.0 / M_PI));
        mAngleCur += degrees;
        data.roll_angle = mAngleCur;
        
        timestampPrev = timestamp;

        return RC_SUCCESS;
    }

    return RC_FAIL; 
}
