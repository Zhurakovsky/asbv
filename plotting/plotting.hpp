#ifndef POC_EXAMPLE_PLOTTING_HPP
#define POC_EXAMPLE_PLOTTING_HPP

#include <thread>
#include <string>
#include <map>
#include <mutex>
#include <memory>
#include <arpa/inet.h>
#include "types.hpp"

class Plotting;
class Plot;

class Plot
{
private:
    std::map<std::string, double> vars;
    std::string plot_name;
    std::mutex data_lock;
public:
    Plot(const std::string &name);
    void AddPlotVar(const std::string &name);
    void AddPlotPoint(const std::string &name, float point);
    float getLastValue(const std::string &name);

    inline std::map<std::string, double>::iterator begin() { return vars.begin(); }
    inline std::map<std::string, double>::const_iterator begin() const { return vars.cbegin(); }
    inline std::map<std::string, double>::iterator end() { return vars.end(); }
    inline std::map<std::string, double>::const_iterator end() const { return vars.cend(); }
};

class Plotting
{
private:
    static std::unique_ptr<Plotting> instance;
    std::thread socket_conn_thread;
    bool socket_thread_enabled;
    std::map<std::string, std::unique_ptr<Plot>> plots;
    std::string message_to_send;
    static void socket_conn_thread_fn(Plotting* p, const std::string &addr, const uint32_t port);
    void updateData();

public:
    static Plotting& GetInstance();
    void Configure(const std::string &addr, const uint32_t port);
    Plot& AddPlot(const std::string &name);
    ~Plotting();
};

#endif
