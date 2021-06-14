#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <iostream>

#include "msg_types.hpp"
#include "sensors.hpp"
#include "sensortopidsenders.hpp"

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
            SensorData data;
            LinuxToPidSenderConfig sender_config =  {.pathname = "autosar_poc", .proj_id = 42 };
            ISensorToPidSender *sender = new LinuxToPidSender(sender_config);
            ISensor *sensor = new RandomSensor();

            while (true)
            {
                if (sensor->read(&data) != RC_SUCCESS) {
                    continue;
                }
                sender->send(&data);

                sleep(1);
            }
            wait(0);
        }
    }

    return 0;
}
