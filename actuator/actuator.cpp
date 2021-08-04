#include <unistd.h>
#include <sys/types.h>

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include "types.hpp"
#include "actuators.hpp"
#include "raspi_actuator.hpp"
#include "pidtoactuatorreceivers.hpp"
#include "config_mgmt.hpp"
#include "socketActuator.hpp"
#include "plotting.hpp"

#include "log_manager.hpp"

using namespace std;
using namespace poc_autosar;
using namespace std::chrono_literals;

err_t parse_cmdline(int argc, char** argv, ActuatorSwcConfigType &config)
{
    CmdLineArgsParser parser(argc, argv);

    if (parser.config_get<bool>("TYPE_STDOUT"s))
    {
        config.actuator = Actuator::STDOUT_ACTUATOR;
    }
    else if (parser.config_get<bool>("TYPE_CARLA"s))
    {
        config.actuator = Actuator::CARLA_ACTUATOR;
    }
    else if (parser.config_get<bool>("TYPE_PWM"s))
    {
        config.actuator = Actuator::PWM_ACTUATOR;
    }
    else if (parser.config_get<bool>("TYPE_CAN"s))
    {
        config.actuator = Actuator::CAN_ACTUATOR;
    }

    if (parser.config_get<bool>("PID_TO_ACTUATOR_LINUX"s))
    {
        config.pid_to_actuator_receiver = PidToActuator::LINUX_PID_TO_ACTUATOR;
    }
    if (parser.config_get<bool>("PID_TO_ACTUATOR_CAN"s))
    {
        config.pid_to_actuator_receiver = PidToActuator::CAN_PID_TO_ACTUATOR;
    }

    config.linux_pid_to_actuator.pathname = parser.config_get<string>("PID_TO_ACTUATOR_LINUX_PATHNAME"s);
    config.linux_pid_to_actuator.proj_id = parser.config_get<int>("PID_TO_ACTUATOR_LINUX_PROJID"s);

    config.socket_actuator.port = parser.config_get<int>("SOCKET_PORT"s);
    config.socket_actuator.addr = parser.config_get<string>("SOCKET_ADDR"s);

    config.plotting_config.webui_server_ip = parser.config_get<string>("PLOTTING_SERVER_IP"s);
    config.plotting_config.webui_server_port = parser.config_get<int>("PLOTTING_SERVER_PORT"s);

    config.log.name = parser.config_get<string>("LOG_NAME"s);
    config.log.console_colour_foreground = parser.config_get<string>("LOG_CONSOLE_COLOUR_FOREGROUND"s);
    config.log.use_console = parser.config_get<bool>("LOG_USE_CONSOLE"s);
    config.log.use_file = parser.config_get<bool>("LOG_USE_FILE"s);
    config.log.custom_filename = parser.config_get<string>("LOG_CUSTOM_FILENAME"s);

    return RC_SUCCESS;
}

int main(int argc, char** argv)
{
    ActuatorSwcConfigType actuator_config;
    parse_cmdline(argc, argv, actuator_config);

    LogManager::Start();
    LogManager::Create(actuator_config.log);

    std::unique_ptr<IPidToActuatorReceiver> receiver(nullptr);
    std::unique_ptr<IActuator> actuator(nullptr);

    Plotting::GetInstance().Configure(actuator_config.plotting_config.webui_server_ip,
                                      actuator_config.plotting_config.webui_server_port);

    if (actuator_config.pid_to_actuator_receiver == PidToActuator::LINUX_PID_TO_ACTUATOR)
    {
        receiver.reset(new LinuxToActuatorReceiver(actuator_config.linux_pid_to_actuator));
    }
    
    switch(actuator_config.actuator)
    {
        case Actuator::STDOUT_ACTUATOR:
            LogManager::Log(actuator_config.log.name, "ACTUATOR PARSED: STDOUT");
            actuator.reset(new StdoutActuator());
            break;
        case Actuator::CARLA_ACTUATOR:
            LogManager::Log(actuator_config.log.name, "ACTUATOR PARSED: SOCKET");
            actuator.reset(new SocketActuator(actuator_config.socket_actuator));
            break;
        case Actuator::PWM_ACTUATOR:
            LogManager::Log(actuator_config.log.name, "ACTUATOR PARSED: PWM");
            actuator.reset(new RaspiActuator());
            break;
        default:
            LogManager::Log(actuator_config.log.name, "ACTUATOR PARSED: NONE");
            break;
    }

    ActuatorData data = {0};

    while(true)
    {
        if (receiver)
        {
            if (receiver->receive(data) != RC_SUCCESS)
            {
                continue;
            }
        }

        if (actuator)
        {
            if (actuator->write(data) == RC_SUCCESS)
            {
                std::string logMessage = "ACTUATOR SEND. NEXT STEER ANGLE: " + std::to_string(data.steer_angle);
                LogManager::Log(actuator_config.log.name, logMessage);
            }
        }
        std::this_thread::sleep_for(10ms);
    }
    return 0;
}
