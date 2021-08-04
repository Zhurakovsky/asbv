#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <cmath>
#include <string>

#include "types.hpp"
#include "sensors.hpp"
#include "sensortopidsenders.hpp"
#include "config_mgmt.hpp"
#include "raspi_sensor.hpp"
#include "socketSensor.hpp"
#include "plotting.hpp"

#include "log_manager.hpp"

using namespace std;
using namespace poc_autosar;
using namespace std::chrono_literals;

err_t parse_cmdline(int argc, char** argv, SensorSwcConfigType &config)
{
    CmdLineArgsParser parser(argc, argv);

    if (parser.config_get<bool>("TYPE_RANDOM"s))
    {
        config.sensor = Sensor::RANDOM_SENSOR;
    }
    else if (parser.config_get<bool>("TYPE_CARLA"s))
    {
        config.sensor = Sensor::CARLA_SENSOR;
    }
    else if (parser.config_get<bool>("TYPE_I2C"s))
    {
        config.sensor = Sensor::I2C_SENSOR;
    }
    else if (parser.config_get<bool>("TYPE_CAN"s))
    {
        config.sensor = Sensor::CAN_SENSOR;
    }
    else if (parser.config_get<bool>("TYPE_STATIC"s))
    {
        config.sensor = Sensor::STATIC_SENSOR;
    }
    config.socket_sensor.port = parser.config_get<int>("SOCKET_PORT"s);
    config.socket_sensor.addr = parser.config_get<string>("SOCKET_ADDR"s);

    config.static_sensor.roll = parser.config_get<float>("STATIC_ROLL_VALUE"s);
    config.static_sensor.acc = parser.config_get<float>("STATIC_ACC_VALUE"s);
    config.static_sensor.speed = parser.config_get<float>("STATIC_SPEED_VALUE"s);

    if (parser.config_get<bool>("SENSOR_TO_PID_LINUX"s))
    {
        config.sensor_to_pid_sender = SensorToPid::LINUX_SENSOR_TO_PID;
    }
    
    if (parser.config_get<bool>("SENSOR_TO_PID_CAN"s))
    {
        config.sensor_to_pid_sender = SensorToPid::CAN_SENSOR_TO_PID;
    }

    config.linux_sender_to_pid.pathname = parser.config_get<string>("SENSOR_TO_PID_LINUX_PATHNAME"s);
    config.linux_sender_to_pid.proj_id = parser.config_get<int>("SENSOR_TO_PID_LINUX_PROJID"s);

    config.log.name = parser.config_get<string>("LOG_NAME"s);
    config.log.console_colour_foreground = parser.config_get<string>("LOG_CONSOLE_COLOUR_FOREGROUND"s);
    config.log.use_console = parser.config_get<bool>("LOG_USE_CONSOLE"s);
    config.log.use_file = parser.config_get<bool>("LOG_USE_FILE"s);
    config.log.custom_filename = parser.config_get<string>("LOG_CUSTOM_FILENAME"s);
    config.plotting_config.webui_server_ip = parser.config_get<string>("PLOTTING_SERVER_IP"s);
    config.plotting_config.webui_server_port = parser.config_get<int>("PLOTTING_SERVER_PORT"s);

    config.log.use_file = parser.config_get<bool>("LOG_USE_FILE"s);
    config.log.custom_filename = parser.config_get<string>("LOG_CUSTOM_FILENAME"s);

    return RC_SUCCESS;
}

int main(int argc, char** argv)
{
    SensorSwcConfigType sensor_config;
    parse_cmdline(argc, argv, sensor_config);

    LogManager::Start();
    LogManager::Create(sensor_config.log);

    std::unique_ptr<ISensorToPidSender> sender(nullptr);
    std::unique_ptr<ISensor> sensor(nullptr);

    Plotting::GetInstance().Configure(sensor_config.plotting_config.webui_server_ip,
                                      sensor_config.plotting_config.webui_server_port);
    Plot& sensor_plot = Plotting::GetInstance().AddPlot("Sensor");
    sensor_plot.AddPlotVar("linear_speed");
    sensor_plot.AddPlotVar("roll_accelleration");
    sensor_plot.AddPlotVar("roll_angle");

    switch (sensor_config.sensor_to_pid_sender)
    {
        case SensorToPid::LINUX_SENSOR_TO_PID:
            sender.reset(new LinuxToPidSender(sensor_config.linux_sender_to_pid));
            break;
    
        default:
            break;
    }

    switch (sensor_config.sensor)
    {
        case Sensor::RANDOM_SENSOR:
            LogManager::Log(sensor_config.log.name, "SENSOR PARSED: RANDOM");
            sensor.reset(new RandomSensor);
            break;

        case Sensor::I2C_SENSOR:
        #ifdef _WIRINGPI
            LogManager::Log(sensor_config.log.name, "SENSOR PARSED: RASPI");
            sensor.reset(new RaspiSensor);
        #else
            LogManager::Log(sensor_config.log.name, "SENSOR PARSED: RANDOM (Raspi sensor unavailable)");
            sensor.reset(new RandomSensor);
        #endif //_WIRINGPI
            break;

        case Sensor::STATIC_SENSOR:
            LogManager::Log(sensor_config.log.name, "SENSOR PARSED: STATIC");
            sensor.reset(new StaticSensor(sensor_config.static_sensor));
            break;

        case Sensor::CARLA_SENSOR:
            LogManager::Log(sensor_config.log.name, "SENSOR PARSED: SOCKET");
            sensor.reset(new SocketSensor(sensor_config.socket_sensor));
            break;
        default:
            LogManager::Log(sensor_config.log.name, "SENSOR PARSED: NONE");
            break;
    }

    SensorData data;

    while (true)
    {
        if (sensor)
        {
            if (sensor->read(data) != RC_SUCCESS)
            {
                continue;
            }
            else
            {
                std::string logMessage = "SENSOR READ. CURRENT ROLL ANGLE: " + std::to_string(data.roll_angle);
                LogManager::Log(sensor_config.log.name, logMessage);
            }
        }

        if (sender)
        {
            sender->send(data);
            sensor_plot.AddPlotPoint("linear_speed", data.linear_speed);
            sensor_plot.AddPlotPoint("roll_accelleration", data.roll_accelleration);
            sensor_plot.AddPlotPoint("roll_angle", data.roll_angle);
        }
        
        std::this_thread::sleep_for(10ms);
    }
    return 0;
}
