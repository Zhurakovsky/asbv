#include "sensors.hpp"


namespace poc_autosar
{

class SocketSensor : public ISensor
{
public:
    SocketSensor(const SocketSensorConfig& config);
    virtual ~SocketSensor();

    void Init();

    const std::string describe() const override { return "SocketSensor"; }
    err_t read(SensorData& data) override;

private:
	int mHost = 0;
	int mServer = 0;
	int mFileDescriptor = 0;

    float acclX, acclY, acclZ;
    float gyroX, gyroY, gyroZ;
    float veloX, veloY, veloZ;
};

}
