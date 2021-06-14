#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <cmath>
#include <memory>

#include "msg_types.hpp"
#include "pid_controller.hpp"
#include "sensor_to_pid_receiver.h"
#include "pid_to_actuator_sender.h"

using namespace std;
using namespace poc_autosar;

int main(int argc, char** argv)
{
    int status;
    char* args[2];
    string child_app = "./actuatorApp";
    args[0] = (char*)child_app.c_str();
    args[1] = NULL;

    key_t key;
    int msgid;

    pid_t pid = fork();
    
    switch(pid)
    {
        case -1:
        perror("PID fork");
        break;

        case 0:
        if ((execvp (args[0], args)) == -1)
        {
            perror("execvp");
        }
        break;

        default:
        {            
            std::unique_ptr<PidController> pid_controller;

            LinuxToPidReceiverConfig receiver_config = { "autosar_poc", 42 };
            std::unique_ptr<ISensorToPidReceiver> sensor_to_pid_receiver = std::make_unique<LinuxToPidReceiver>(receiver_config);    

            PidToLinuxSenderConfig sender_config = { "autosar_poc", 42 };
            std::unique_ptr<IPidToActuatorSender> pid_to_actuator_sender = std::make_unique<PidToLinuxSender>(sender_config);
            
            SensorData sensor_data;
            ActuatorData actuator_data;            
            
            
            while (true)
            {
                sensor_to_pid_receiver->receive(sensor_data);

                pid_controller->calculateActuatorValues(sensor_data, actuator_data);
               
                pid_to_actuator_sender->send(actuator_data);
                
               
                sleep(1);
            }
        }
        wait(0);
    }

    return 0;
}
