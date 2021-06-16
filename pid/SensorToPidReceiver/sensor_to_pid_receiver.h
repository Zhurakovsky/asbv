#ifndef SENSOR_TO_PID_RECEIVER
#define SENSOR_TO_PID_RECEIVER

#include <sys/ipc.h>
#include <sys/msg.h>

#include "msg_types.hpp"
#include "types.hpp"
#include <string>
#include <iostream>

namespace poc_autosar
{

class ISensorToPidReceiver
{
public:
	virtual ~ISensorToPidReceiver() = default;
	virtual err_t receive(SensorData& data) = 0;
};

class LinuxToPidReceiver : public ISensorToPidReceiver
{
public:
	LinuxToPidReceiver(const LinuxSensorToPidConfig& config) : key(ftok(config.pathname.c_str(), config.proj_id)), expected_msg_type(PocMsgTypes::SENSOR_TO_PID)
	{}
	
	err_t receive(SensorData& data) override
	{
		int msgid = msgget(key, 0666 | IPC_CREAT);
		msgrcv(msgid, &message, sizeof(message), expected_msg_type, 0);
		data = message.sensor_data;
		return RC_SUCCESS;
	}
private:
	key_t key;
	PocMsgTypes expected_msg_type;
	MsgSensorToPid message;
};

}

#endif // !SENSOR_TO_PID_RECEIVER

