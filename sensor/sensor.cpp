#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <memory>

#include "types.hpp"
#include "sensors.hpp"
#include "sensortopidsenders.hpp"
#include "config_mgmt.hpp"

using namespace std;
using namespace poc_autosar;

err_t parse_cmdline(int argc, char** argv, SensorSwcConfigType &config)
{
    CmdLineArgsParser parser(argc, argv);

    if (parser.config_get<bool>("TYPE_RANDOM"s))
        config.sensor = Sensor::RANDOM_SENSOR;
    else if (parser.config_get<bool>("TYPE_CARLA"s))
        config.sensor = Sensor::CARLA_SENSOR;
    else if (parser.config_get<bool>("TYPE_I2C"s))
        config.sensor = Sensor::I2C_SENSOR;
    else if (parser.config_get<bool>("TYPE_CAN"s))
        config.sensor = Sensor::CAN_SENSOR;

    config.socket_sensor.port = parser.config_get<int>("SOCKET_PORT"s);
    config.socket_sensor.addr = parser.config_get<string>("SOCKET_ADDR"s);

    if (parser.config_get<bool>("SENSOR_TO_PID_LINUX"s))
        config.sensor_to_pid_sender = SensorToPid::LINUX_SENSOR_TO_PID;
    if (parser.config_get<bool>("SENSOR_TO_PID_CAN"s))
        config.sensor_to_pid_sender = SensorToPid::CAN_SENSOR_TO_PID;

    config.linux_sender_to_pid.pathname = parser.config_get<string>("SENSOR_TO_PID_LINUX_PATHNAME"s);
    config.linux_sender_to_pid.proj_id = parser.config_get<int>("SENSOR_TO_PID_LINUX_PROJID"s);

    return RC_SUCCESS;
}

int main(int argc, char** argv)
{
    SensorSwcConfigType sensor_config;
    parse_cmdline(argc, argv, sensor_config);

    std::unique_ptr<ISensorToPidSender> sender(nullptr);
    std::unique_ptr<ISensor> sensor(nullptr);

    if (sensor_config.sensor_to_pid_sender == SensorToPid::LINUX_SENSOR_TO_PID)
        sender.reset(new LinuxToPidSender(sensor_config.linux_sender_to_pid));

    if (sensor_config.sensor == Sensor::RANDOM_SENSOR)
        sensor.reset(new RandomSensor());

    SensorData data;

    while (true)
    {
        if (sensor) 
            if (sensor->read(data) != RC_SUCCESS)
                continue;

        if (sender)
            sender->send(data);

        sleep(1);
    }
    return 0;
}
