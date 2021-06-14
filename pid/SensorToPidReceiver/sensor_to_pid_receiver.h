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

struct LinuxToPidReceiverConfig
{
	std::string pathname;
	int proj_id;
};

class ISensorToPidReceiver
{
public:
	virtual ~ISensorToPidReceiver() {};
	virtual err_t receive(SensorData* data) = 0;
};

class LinuxToPidReceiver : public ISensorToPidReceiver
{
private:
	key_t key;
	PocMsgTypes expected_msg_type;
	MsgSensorToPid message;
public:
	LinuxToPidReceiver(const LinuxToPidReceiverConfig& config)
	{
		this->key = ftok(config.pathname.c_str(), config.proj_id);
		this->expected_msg_type = PocMsgTypes::SENSOR_TO_PID;
	}
	
	err_t receive(SensorData* data) override
	{
		int msgid = msgget(this->key, 0666 | IPC_CREAT);
		msgrcv(msgid, &message, sizeof(message), this->expected_msg_type, 0);
		*data = this->message.sensor_data;
		return RC_SUCCESS;
	};
};

}

#endif // !SENSOR_TO_PID_RECEIVER

