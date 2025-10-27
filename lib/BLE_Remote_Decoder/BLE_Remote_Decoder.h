
#include "defaults.h"
#include <freertos/ringbuf.h>
#include "BlueXGIMI_RC.h"
#include "ServerDecoder.h"

class BLE_Remote_Decoder: public ServerDecoderHandler
{
public:
    BLE_Remote_Decoder();
    void doCmd(ServerDecoder::Remote_Cmd *cmd);
    void start();
    uint8_t getConnectedCount();

protected:
    BlueXGIMI_RC *rcHID;
};

