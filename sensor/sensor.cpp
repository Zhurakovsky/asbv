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

#include "log_manager.hpp"

#include "kalman_filter/kalman_filter.h"
//#include <fstream>

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
            LOG(sensor_config.log.name, "SENSOR PARSED: RANDOM");
            sensor.reset(new RandomSensor);
            break;

        case Sensor::I2C_SENSOR:
        #ifdef _WIRINGPI
            LOG(sensor_config.log.name, "SENSOR PARSED: RASPI");
            sensor.reset(new RaspiSensor);
        #else
            LOG(sensor_config.log.name, "SENSOR PARSED: RANDOM (Raspi sensor unavailable)");
            sensor.reset(new RandomSensor);
        #endif //_WIRINGPI
            break;

        case Sensor::STATIC_SENSOR:
            LOG(sensor_config.log.name, "SENSOR PARSED: STATIC");
            sensor.reset(new StaticSensor(sensor_config.static_sensor));
            break;

        case Sensor::CARLA_SENSOR:
            LOG(sensor_config.log.name, "SENSOR PARSED: SOCKET");
            sensor.reset(new SocketSensor(sensor_config.socket_sensor));
            break;
        default:
            LOG(sensor_config.log.name, "SENSOR PARSED: NONE");
            break;
    }

    SensorData data;

    Eigen::VectorXd y(1);
    KalmanFilter kalman_filter;
    kalman_filter.init();

    //std::ofstream data_file("data3.csv");
    //std::ofstream filtered_data_file("filtered_data3.csv");

    while (true)
    {
        if (sensor && sensor->read(data) == RC_SUCCESS)
        {
			std::string logMessage = "SENSOR READ. UNFILTERED ROLL ANGLE: " + std::to_string(data.roll_angle);
            LOG(sensor_config.log.name, logMessage);
			
            //data_file << data.roll_angle << std::endl;
            y << data.roll_angle;
            kalman_filter.predict();
            kalman_filter.update(y);

            data.roll_angle = kalman_filter.state().transpose()[0];
            //filtered_data_file << data.roll_angle << std::endl;
			
			logMessage = "SENSOR READ. FILTERED ROLL ANGLE: " + std::to_string(data.roll_angle);
            LOG(sensor_config.log.name, logMessage);

            if (sender)
            {
                sender->send(data);
            }
            
            std::this_thread::sleep_for(10ms);
		}
    }
    return 0;
}
