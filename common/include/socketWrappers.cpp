#include "socketWrappers.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace poc_autosar;

int SocketWrappers::Socket(int domain, int type, int protocol)
{
	int res = socket(domain, type, protocol);
	if (res == -1) {
		std::perror("socket failed");
		//exit(EXIT_FAILURE);
	}
	return res;
}

void SocketWrappers::Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
	int res = bind(sockfd, addr, addrlen);
	if (res == -1) {
		std::perror("bind failed");
		//exit(EXIT_FAILURE);
	}
}

void SocketWrappers::Listen(int sockfd, int backlog)
{
	int res = listen(sockfd, backlog);
	if (res == -1) {
		std::perror("listen failed");
		//exit(EXIT_FAILURE);
	}
}

int SocketWrappers::Accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen)
{
	int res = accept(sockfd, addr, addrlen);
	if (res == -1) {
		std::perror("accept failed");
		//exit(EXIT_FAILURE);
	}
	return res;
}

int SocketWrappers::Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
	int res = connect(sockfd, addr, addrlen);
	if (res == -1) {
		std::perror("connect failed");
		//exit(EXIT_FAILURE);
	}
	return res;
}

void SocketWrappers::Inet_pton(int af, const char* src, void* dst)
{
	int res = inet_pton(af, src, dst);
	if (res == 0) {
		std::cout << "inet_pton failed: src does not contain a character string representing a valid network address in the specified address family" << std::endl;
		//exit(EXIT_FAILURE);
	}
	if (res == -1) {
		std::perror("inet_pton failed");
		//exit(EXIT_FAILURE);
	}
}

ssize_t SocketWrappers::Read(int sockfd, char* buf, size_t count)
{
	ssize_t nread = read(sockfd, buf, count);
	
	if (nread == -1)
	{
		std::perror("read failed");
		//exit(EXIT_FAILURE);
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
	
	if (nwrite == -1)
	{
		std::perror("write failed");
		//exit(EXIT_FAILURE);
	}

	return nwrite;
}