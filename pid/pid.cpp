#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <cmath>
#include <memory>
#include <chrono>
#include <thread>

#include "msg_types.hpp"
#include "pid_controller.hpp"
#include "SensorToPidReceiver/sensor_to_pid_receiver.h"
#include "PidToActuatorSender/pid_to_actuator_sender.h"
#include "config_mgmt.hpp"

using namespace std;
using namespace poc_autosar;
using namespace std::chrono_literals;


err_t parse_cmdline(int argc, char** argv, PidSwcConfigType &config)
{
    CmdLineArgsParser parser(argc, argv);

    if (parser.config_get<bool>("PID_TO_ACTUATOR_LINUX"s))
    {
        config.pid_to_actuator_sender = PidToActuator::LINUX_PID_TO_ACTUATOR;
    }
    if (parser.config_get<bool>("PID_TO_ACTUATOR_CAN"s))
    {
        config.pid_to_actuator_sender = PidToActuator::CAN_PID_TO_ACTUATOR;
    }

    if (parser.config_get<bool>("SENSOR_TO_PID_LINUX"s))
    {
        config.sensor_to_pid_receiver = SensorToPid::LINUX_SENSOR_TO_PID;
    }
    if (parser.config_get<bool>("SENSOR_TO_PID_CAN"s))
    {
        config.sensor_to_pid_receiver = SensorToPid::CAN_SENSOR_TO_PID;
    }

    config.linux_pid_to_actuator.pathname = parser.config_get<string>("SENSOR_TO_PID_LINUX_PATHNAME"s);
    config.linux_pid_to_actuator.proj_id = parser.config_get<int>("SENSOR_TO_PID_LINUX_PROJID"s);

    config.linux_pid_to_actuator.pathname = parser.config_get<string>("PID_TO_ACTUATOR_LINUX_PATHNAME"s);
    config.linux_pid_to_actuator.proj_id = parser.config_get<int>("PID_TO_ACTUATOR_LINUX_PROJID"s);

    config.pid_config.kp = parser.config_get<float>("PID_PROPORTIONAL_COEF"s);
    config.pid_config.kp = parser.config_get<float>("PID_INTEGRAL_COEF"s);
    config.pid_config.kp = parser.config_get<float>("PID_DERIVATIVE_COEF"s);

    return RC_SUCCESS;
}

int main(int argc, char** argv)
{
    PidSwcConfigType pid_config;
    parse_cmdline(argc, argv, pid_config);

    std::unique_ptr<PidController> pid_controller(new PidController(pid_config.pid_config));
    std::unique_ptr<ISensorToPidReceiver> sensor_to_pid_receiver(nullptr);
    std::unique_ptr<IPidToActuatorSender> pid_to_actuator_sender(nullptr);

    if (pid_config.sensor_to_pid_receiver == SensorToPid::LINUX_SENSOR_TO_PID)
    {
        sensor_to_pid_receiver.reset(new LinuxToPidReceiver(pid_config.linux_sender_to_pid));
    }
    
    if (pid_config.pid_to_actuator_sender == PidToActuator::LINUX_PID_TO_ACTUATOR)
    {
        pid_to_actuator_sender.reset(new PidToLinuxSender(pid_config.linux_pid_to_actuator));
    }

    SensorData sensor_data;
    ActuatorData actuator_data;            
    
    while (true)
    {
        if (sensor_to_pid_receiver)
        {
            sensor_to_pid_receiver->receive(sensor_data);
        }
        pid_controller->calculateActuatorValues(sensor_data, actuator_data);

        if (pid_to_actuator_sender)
        {
            pid_to_actuator_sender->send(actuator_data);
        }

        std::this_thread::sleep_for(10ms);
    }

    return 0;
}
