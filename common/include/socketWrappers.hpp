#ifndef POC_EXAMPLE_COMMON_SOCKET_WRAPPERS_HPP
#define POC_EXAMPLE_COMMON_SOCKET_WRAPPERS_HPP

#include <sys/types.h>
#include <sys/socket.h>

namespace poc_autosar
{

class SocketWrappers
{	
public:
    static int Socket(int domain, int type, int protocol);
    static void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
    static void Listen(int sockfd, int backlog);
    static int Accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
    static int Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
    static void Inet_pton(int af, const char* src, void* dst);
    static ssize_t Read(int sockfd, char* buf, size_t count);
    static ssize_t Write(int fd, const void* buf, size_t count);
};

}

#endif