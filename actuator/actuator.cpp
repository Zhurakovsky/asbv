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

    return RC_SUCCESS;
}

int main(int argc, char** argv)
{
    ActuatorSwcConfigType actuator_config;
    parse_cmdline(argc, argv, actuator_config);

    std::unique_ptr<IPidToActuatorReceiver> receiver(nullptr);
    std::unique_ptr<IActuator> actuator(nullptr);

    if (actuator_config.pid_to_actuator_receiver == PidToActuator::LINUX_PID_TO_ACTUATOR)
    {
        receiver.reset(new LinuxToActuatorReceiver(actuator_config.linux_pid_to_actuator));
    }
    
    switch(actuator_config.actuator)
    {
        case Actuator::STDOUT_ACTUATOR:
            actuator.reset(new StdoutActuator());
            break;
        case Actuator::PWM_ACTUATOR:
            actuator.reset(new RaspiActuator());
            break;
        default:
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
            actuator->write(data);
        }
        std::this_thread::sleep_for(50ms);
    }
    return 0;
}
