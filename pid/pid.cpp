#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <cmath>
#include <iostream>

#include "msg_types.hpp"

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
            key = ftok("autosar_poc", 42);
            msgid = msgget(key, 0666 | IPC_CREAT);
            MsgSensorToPid in_message;
            PocMsgTypes expected_msg_type = PocMsgTypes::SENSOR_TO_PID;

            MsgPidToActuator out_message;
            out_message.msg_type = PocMsgTypes::PID_TO_ACTUATOR;

            while (true)
            {
                msgrcv(msgid, &in_message, sizeof(in_message), expected_msg_type, 0);
                
                cout << "PID: message from Sensor : " <<
                in_message.linear_speed << ", " <<
                in_message.roll_angle << ", " <<
                in_message.roll_accelleration << endl;

                /*
                    Here is a main Math logic to calculate steering angle...
                    After that, angle is calculated and go to the actuator process                
                */

                out_message.steer_ange = get_random();

                // Just some dummy fuzzy logic
                out_message.request_to_steer = (fabsf(out_message.steer_ange) < 0.05) ? false : true;

                msgsnd(msgid, &out_message, sizeof(out_message), 0);
                /*
                // TODO: delete message queue on exit
                msgctl(msgid, IPC_RMID, NULL);
                */
               
                sleep(1);
            }
        }
        wait(0);
    }

    return 0;
}
