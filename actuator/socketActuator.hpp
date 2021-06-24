#include "actuators.hpp"


namespace poc_autosar
{

class SocketActuator : public IActuator
{
public:
    SocketActuator(const SocketActuatorConfig& config);
    virtual ~SocketActuator();

    void Init();

    virtual const std::string describe() override { return "SocketActuator"; }
    err_t write(const ActuatorData& data) override;

private:
	int mHost = 0;
	std::string mNetworkAddress;
	int mFileDescriptor = 0;
};

}
