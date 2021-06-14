#ifndef PID_TO_ACTUATOR_SENDER
#define PID_TO_ACTUATOR_SENDER
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msg_types.hpp"
#include "types.hpp"
#include <string>
#include <iostream>

namespace poc_autosar
{

struct PidToLinuxSenderConfig
{
    std::string pathname;
    int proj_id;
};

class IPidToActuatorSender
{
public:
    virtual ~IPidToActuatorSender() {};
    virtual err_t send(ActuatorData* data) = 0;
};

class PidToLinuxSender : public IPidToActuatorSender
{
private:
    key_t key;
    int msgid;
    MsgPidToActuator message;
public:
    PidToLinuxSender(const PidToLinuxSenderConfig& config)
    {
        key= ftok(config.pathname.c_str(), config.proj_id);
        msgid = msgget(key, 0666 | IPC_CREAT);
    }

    err_t send(ActuatorData* data)
    {
        message.msg_type = PocMsgTypes::PID_TO_ACTUATOR;
        message.actuator_data.steer_ange = data->steer_ange;
        message.actuator_data.request_to_steer = data->request_to_steer;

        return msgsnd(msgid, &message, sizeof(message), 0) ? RC_FAIL : RC_SUCCESS;
    }
};

}



#endif // !PID_TO_ACTUATOR_SENDER

