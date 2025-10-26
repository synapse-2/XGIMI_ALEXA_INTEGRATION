
#include "defaults.h"
#include <freertos/ringbuf.h>
#include "BlueXGIMI_RC.h"

class BLE_Remote_Decoder
{
public:
    BLE_Remote_Decoder();
    void dequeueCmd();
    void start();
    uint8_t getConnectedCount();

protected:
    BlueXGIMI_RC *rcHID;
};

