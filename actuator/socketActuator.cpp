#include "socketActuator.hpp"

#include "socketWrappers.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

using namespace poc_autosar;


SocketActuator::SocketActuator(const SocketActuatorConfig& config)
{
    mHost = config.port;
    mNetworkAddress = config.addr;
    mReconnectionDelayMs = config.reconnection_delay_ms;
    Init();
}

SocketActuator::~SocketActuator()
{
    close(mFileDescriptor);
}

void SocketActuator::Init()
{
    mFileDescriptor = SocketWrappers::Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(mHost);
    
    SocketWrappers::Inet_pton(AF_INET, mNetworkAddress.c_str(), &adr.sin_addr);

    while (SocketWrappers::Connect(mFileDescriptor, (struct sockaddr*) &adr, sizeof adr) == RC_FAIL)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(mReconnectionDelayMs));
    }
}

err_t SocketActuator::write(const ActuatorData& data)
{
    std::string msg = "{\"angle\":" + std::to_string(data.steer_angle) + "}";

    err_t result = RC_FAIL;
    if (SocketWrappers::Write(mFileDescriptor, msg.c_str(), msg.length()) != RC_FAIL)
    {
        result = RC_SUCCESS;
    }

    return result;
}
