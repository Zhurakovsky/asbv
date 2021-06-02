#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <iostream>

#include "msg_types.hpp"

using namespace std;
using namespace poc_autosar;

int main(int argc, char** argv)
{
    key_t key = ftok("autosar_poc", 42);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    MsgPidToActuator message;
    PocMsgTypes expected_msg_type = PocMsgTypes::PID_TO_ACTUATOR;

    while(true)
    {
        msgrcv(msgid, &message, sizeof(message), expected_msg_type, 0);

        cout << "Actuator: message from PID : " <<
        message.steer_ange << ", " <<
        (message.request_to_steer ? "True" : "False") << endl;

        sleep(1);
    }
    return 0;
}
