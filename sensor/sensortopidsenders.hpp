#ifndef SENDER_SENDERTOPIDSENDER_HPP
#define SENDER_SENDERTOPIDSENDER_HPP

#include <sys/ipc.h>
#include <sys/msg.h>

#include "msg_types.hpp"
#include "types.hpp"
#include <string>
#include <iostream>

namespace poc_autosar
{

class ISensorToPidSender
{
public:
    virtual ~ISensorToPidSender() = default;
    virtual const std::string describe() = 0;
    virtual err_t send(const SensorData& data) = 0;
};

class LinuxToPidSender : public ISensorToPidSender
{
public:
    LinuxToPidSender(const LinuxSensorToPidConfig& config)
    {
        this->key = ftok(config.pathname.c_str(), config.proj_id);
        this->msgid = msgget(key, 0666 | IPC_CREAT);
        this->expected_msg_type = PocMsgTypes::SENSOR_TO_PID;
    }

    const std::string describe() override { return "MsgQueue"; };

    err_t send(const SensorData& data) override
    {
        message.msg_type                        = PocMsgTypes::SENSOR_TO_PID;
        message.sensor_data.linear_speed        = data.linear_speed;
        message.sensor_data.roll_accelleration  = data.roll_accelleration;
        message.sensor_data.roll_angle          = data.roll_angle;

        return msgsnd(this->msgid, &message, sizeof(message), 0) ? RC_FAIL : RC_SUCCESS;
    };

private:
    key_t key;
    int msgid;
    PocMsgTypes expected_msg_type;
    MsgSensorToPid message;
};

}

#endif /* SENDER_SENDERTOPIDSENDER_HPP */
