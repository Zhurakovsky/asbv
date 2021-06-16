#ifndef ACTUATOR_PIDTOACTUATORRECEIVER_HPP
#define ACTUATOR_PIDTOACTUATORRECEIVER_HPP

#include <sys/ipc.h>
#include <sys/msg.h>

#include "msg_types.hpp"
#include "types.hpp"
#include <string>
#include <iostream>

namespace poc_autosar
{

class IPidToActuatorReceiver
{
public:
    virtual ~IPidToActuatorReceiver() = default;
    virtual const std::string describe() = 0;
    virtual err_t receive(ActuatorData &data) = 0;
};


class LinuxToActuatorReceiver : public IPidToActuatorReceiver
{
public:
    LinuxToActuatorReceiver(const LinuxPidToActuatorConfig config)
    {
        this->key = ftok(config.pathname.c_str(), config.proj_id);
        this->expected_msg_type = PocMsgTypes::PID_TO_ACTUATOR;
    }

    const std::string describe() override { return "MsgQueue"; };

    err_t receive(ActuatorData &data) override
    {
        int msgid = msgget(this->key, 0666 | IPC_CREAT);
        msgrcv(msgid, &message, sizeof(message), this->expected_msg_type, 0);
        data = this->message.actuator_data;

        return RC_SUCCESS;
    };
private:
    key_t key;
    PocMsgTypes expected_msg_type;
    MsgPidToActuator message;
};

}

#endif /* ACTUATOR_PIDTOACTUATORRECEIVER_HPP */
