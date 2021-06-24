#include "socketActuator.hpp"

#include "socketWrappers.hpp"
#include <arpa/inet.h>
#include <unistd.h>

using namespace poc_autosar;


SocketActuator::SocketActuator(const SocketActuatorConfig& config)
{
    mHost = config.port;
    mNetworkAddress = config.addr;
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

    SocketWrappers::Connect(mFileDescriptor, (struct sockaddr*) &adr, sizeof adr);
}

err_t SocketActuator::write(const ActuatorData& data)
{
    const char format[] = "{\"angle\":%s}";
    const size_t formatLength = sizeof(format) - 3; // %s\0
    
    char angleStr[16];
    int angleLength = sprintf(angleStr, "%f", data.steer_angle);

    size_t msgLen = formatLength + angleLength + 1;
    char* msg = static_cast<char*>(malloc(msgLen));
    msgLen = snprintf(msg, msgLen, format, angleStr);
    
    std::cout << "ACTUATOR SOCKET SEND: " << msg << std::endl;

    err_t result = RC_FAIL;
    if (SocketWrappers::Write(mFileDescriptor, msg, msgLen) != RC_FAIL)
    {
        result = RC_SUCCESS;
    }
    
    free(msg);

    return result;
}
