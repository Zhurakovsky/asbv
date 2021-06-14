#include <unistd.h>
#include <sys/types.h>

#include <iostream>

#include "msg_types.hpp"
#include "types.hpp"
#include "actuators.hpp"
#include "pidtoactuatorreceivers.hpp"

using namespace std;
using namespace poc_autosar;

int main(int argc, char** argv)
{
    struct LinuxToActuatorReceiverConfig config = {.pathname = "autosar_poc", .proj_id = 42 };
    IPidToActuatorReceiver *receiver = new LinuxToActuatorReceiver(config);
    IActuator *actuator = new StdoutActuator();
    ActuatorData message;

    while(true)
    {
        receiver->receive(&message);
        actuator->write(&message);
        sleep(1);
    }
    return 0;
}
