#include "socketWrappers.hpp"
#include "types.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace poc_autosar;

int SocketWrappers::Socket(int domain, int type, int protocol)
{
	int res = socket(domain, type, protocol);
	if (res == RC_FAIL)
	{
		std::perror("socket failed");
	}
	return res;
}

void SocketWrappers::Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
	int res = bind(sockfd, addr, addrlen);
	if (res == RC_FAIL)
	{
		std::perror("bind failed");
	}
}

void SocketWrappers::Listen(int sockfd, int backlog)
{
	int res = listen(sockfd, backlog);
	if (res == RC_FAIL)
	{
		std::perror("listen failed");
	}
}

int SocketWrappers::Accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen)
{
	int res = accept(sockfd, addr, addrlen);
	if (res == RC_FAIL)
	{
		std::perror("accept failed");
	}
	return res;
}

int SocketWrappers::Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
	int res = connect(sockfd, addr, addrlen);
	if (res == RC_FAIL)
	{
		std::perror("connect failed");
	}
	return res;
}

void SocketWrappers::Inet_pton(int af, const char* src, void* dst)
{
	int res = inet_pton(af, src, dst);
	if (res == 0) // 0 - is not success, inet_pton returns 1 on success
	{
		std::cout << "inet_pton failed: src does not contain a character string representing a valid network address in the specified address family" << std::endl;
	}
	else if (res == RC_FAIL)
	{
		std::perror("inet_pton failed");
	}
}

ssize_t SocketWrappers::Read(int sockfd, char* buf, size_t count)
{
	ssize_t nread = read(sockfd, buf, count);
	if (nread == RC_FAIL)
	{
		std::perror("read failed");
	}
	else if (nread == 0)
	{
		std::cout << "EOF occured" << std::endl;
	}
	
	return nread;
}

ssize_t SocketWrappers::Write(int fd, const void* buf, size_t count)
{
	ssize_t nwrite = write(fd, buf, count);
	if (nwrite == RC_FAIL)
	{
		std::perror("write failed");
	}

	return nwrite;
}