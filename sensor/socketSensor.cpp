#include "socketSensor.hpp"

#include "types.hpp"
#include "socketWrappers.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>
#include <string>

using namespace poc_autosar;
using namespace std;

namespace
{
    struct sockaddr_in serv_addr;
    struct hostent *server;

    void error(const string& error_data)
    {
        cout << error_data << endl;
    }

    int read_data(int& sock_fd, char* buffer, size_t len)
    {
        int res = read(sock_fd, buffer, len);
        return res;
    }
}

SocketSensor::SocketSensor(const SocketSensorConfig& config)
{
    
    portno = config.port;
    
    mNetworkAddress = config.addr;
    std::cout << "Requested address: " << mNetworkAddress << std::endl;

    mReconnectionDelayMs = config.reconnection_delay_ms;
    std::cout << "Requested connectiondelay: " << mReconnectionDelayMs << std::endl;
    Init();
}

void SocketSensor::Init()
{

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
    {
        error("ERROR opening socket");
    }

    server = gethostbyname(mNetworkAddress.c_str());

    if (!server)
    {
        error("ERROR, no such host");
        return;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr, 
          (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    while (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "Failed try connect to socket ..." << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(mReconnectionDelayMs));
    }

}


SocketSensor::~SocketSensor()
{
    close(sockfd);
}

err_t SocketSensor::read(SensorData& data)
{
    err_t result = RC_FAIL;

    bzero(buffer,256);
    
    n = read_data(sockfd, buffer, 255);
    
    if (n < 0) 
    {
        error("ERROR reading from socket");
        return RC_FAIL;
    }

    sscanf(buffer, R"({"accel": {"x": %f, "y": %f, "z": %f}, 
                "gyro": {"x": %f, "y": %f, "z": %f}, 
                "velocity": {"x": %f, "y": %f, "z": %f}, 
                "timestamp": %f})", 
                &acclX, &acclY, &acclZ, 
                &gyroX, &gyroY, &gyroZ, 
                &veloX, &veloY, &veloZ, 
                &timestamp);

    data.roll_accelleration = acclX;
    data.linear_speed = 0.0;
        
    double radians = gyroX * (timestamp - timestampPrev);
    double degrees = (radians * (180.0 / M_PI));
    mAngleCur += degrees;
    data.roll_angle = mAngleCur;
        
    timestampPrev = timestamp;

    return RC_SUCCESS;


    return result; 
}
