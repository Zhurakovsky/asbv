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
    std::string mNetworkAddress;
	int mFileDescriptor = 0;
    float mAngleCur = 0.f;
    uint32_t mReconnectionDelayMs = 0;

    float acclX, acclY, acclZ;
    float gyroX, gyroY, gyroZ;
    float veloX, veloY, veloZ;
    float timestampPrev = 0.f;
    float timestamp = 0.f;

    int sockfd;
    int portno;
    int n;

    // struct sockaddr_in serv_addr;
    // struct hostent *server;

    char buffer[256];
};

}
