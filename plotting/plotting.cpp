#include "plotting.hpp"
#include "socketWrappers.hpp"
#include <sys/socket.h>
#include <sys/time.h>
#include <string.h>
#include <sstream>
#include <chrono>
#include <unistd.h>

#include <iostream>

using namespace poc_autosar;
using namespace std;
using namespace std::chrono_literals;

Plot::Plot(const std::string &name):
    plot_name(name)
{}

void Plot::AddPlotVar(const std::string &name)
{
    data_lock.lock();
    vars.insert({name, 0});
    data_lock.unlock();
}


void Plot::AddPlotPoint(const std::string &name, float point)
{
    data_lock.lock();
    vars[name] = point;
    data_lock.unlock();
}

float Plot::getLastValue(const std::string &name)
{
    data_lock.lock();
    return vars[name];
    data_lock.unlock();
}

std::unique_ptr<Plotting> Plotting::instance;

Plotting& Plotting::GetInstance()
{
    if (instance == nullptr) {
        instance.reset(new Plotting());
    }
    return *instance;
}

void Plotting::Configure(const std::string &addr, const uint32_t port)
{
    socket_thread_enabled = true;
    socket_conn_thread = std::thread(Plotting::socket_conn_thread_fn, this,  addr, port);
    updateData();
}

void Plotting::updateData()
{
    std::string message;
    std::ostringstream s;

    s << "{'plots': {";
    for (const auto& plot: plots)
    {
        s << '"' << plot.first << "\":{";
        for (const auto& plot_var: *(plot.second))
        {
            s << '"' << plot_var.first << "\":" << plot_var.second << ",";
        }
        s << "},";
    }
    s << "}}";

    message_to_send = s.str();
}

Plot& Plotting::AddPlot(const std::string &name)
{
    auto inserted_pair = plots.insert(std::make_pair(name, std::unique_ptr<Plot>(new Plot(name))));
    return *(inserted_pair.first->second);
}

Plotting::~Plotting()
{
    socket_thread_enabled = false;
    plots.clear();
}


void Plotting::socket_conn_thread_fn(Plotting* p, const std::string &addr, const uint32_t port)
{
    int sockfd;
    struct sockaddr_in serv_addr = {.sin_family = AF_INET, .sin_port = htons(port)};
    struct timeval timeout = {.tv_sec = 3, .tv_usec = 0};
    bool is_connected = false;

    SocketWrappers::Inet_pton(AF_INET, addr.c_str(), &serv_addr.sin_addr);

    while (p->socket_thread_enabled)
    {
        if (!is_connected)
        {
            sockfd = SocketWrappers::Socket(AF_INET, SOCK_STREAM, 0);
            if (connect(sockfd, (struct sockaddr *)&(serv_addr), sizeof(serv_addr)))
            {
                close(sockfd);
            }
            else
            {
                is_connected = true;
                setsockopt (sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
                setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
            }
        }
        if (is_connected)
        {
            int error_code = 0;
            socklen_t error_code_size = sizeof(error_code);
            p->updateData();
            SocketWrappers::Write(sockfd, (void*)(p->message_to_send.c_str()), p->message_to_send.length());
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
            if (error_code)
            {
                is_connected = false;
                int ret = close(sockfd);
            }
        }
        std::this_thread::sleep_for(50ms);
    }
}
