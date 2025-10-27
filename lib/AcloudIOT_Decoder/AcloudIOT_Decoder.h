

#include "defaults.h"
#include "BlueRC.h"
#include <WiFiManager.h>
#include <ArduinoIoTCloud.h>
#include "ServerDecoder.h"

class AcloudIOT_Decoder
{
public:
    AcloudIOT_Decoder();
    void setup(String ssid, String psk);
    void start();
    static void checkResetPressed();
    bool hasFirstCloudSyncHasHappened();
    void onNetworkConnect();
    void onNetworkDisconnect();
    void onNetworkError();
    void onProjectorChange(CloudTelevision newPrj);
    void enQueueCmd(ServerDecoder::Remote_Cmd cmd);

protected:
    WiFiConnectionHandler * iot_connector;
    bool firstCloudSyncHasHappened = false; // used to ignore the first cloud update after connection as it syncs the variable
    CloudTelevision oldProjector_value;     // used to compare the value to derermine what to do
};