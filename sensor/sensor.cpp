#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <iostream>

#include "msg_types.hpp"

using namespace std;
using namespace poc_autosar;

int main(int argc, char** argv)
{
    int status;
    char* args[2];
    string child_app = "./pidControllerApp";
    args[0] = (char*)child_app.c_str();
    
    args[1] = NULL;

    key_t key;
    int msgid;

    pid_t pid = fork();
    
    switch(pid)
    {
        case -1:
        perror("Sensor fork");
        break;

        case 0:
        if ((execvp (args[0], args)) == -1 )
        {
            perror("execvp");
        }
        break;

        default:
        {
            // ConcreteSensorInstance sensor

            key = ftok("autosar_poc", 42);
            msgid = msgget(key, 0666 | IPC_CREAT);
            MsgSensorToPid message;
            message.msg_type = PocMsgTypes::SENSOR_TO_PID;
            
            while (true)
            {
                // sensor.read(&message);
                message.sensor_data.linear_speed = get_random();
                message.sensor_data.roll_angle = get_random();
                message.sensor_data.roll_accelleration = get_random();

                // RTE_SendMessage(&message);
                msgsnd(msgid, &message, sizeof(message), 0);
                
                sleep(1);
            }
            wait(0);
        }
    }

    return 0;
}
