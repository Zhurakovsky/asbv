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
    virtual ~IPidToActuatorSender() = default;
    virtual err_t send(const ActuatorData& data) = 0;
};

class PidToLinuxSender : public IPidToActuatorSender
{
public:
    PidToLinuxSender(const LinuxPidToActuatorConfig& config) : key(ftok(config.pathname.c_str(), config.proj_id)), msgid(msgget(key, 0666 | IPC_CREAT))
    {}

    err_t send(const ActuatorData& data)
    {
        message.msg_type = PocMsgTypes::PID_TO_ACTUATOR;
        message.actuator_data.steer_angle = data.steer_angle;
        message.actuator_data.request_to_steer = data.request_to_steer;

        return msgsnd(msgid, &message, sizeof(message), 0) ? RC_FAIL : RC_SUCCESS;
    }
private:
    key_t key;
    int msgid;
    MsgPidToActuator message;
};

}



#endif // !PID_TO_ACTUATOR_SENDER

