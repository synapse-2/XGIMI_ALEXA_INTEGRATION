#pragma once

#include "defaults.h"
#include <freertos/ringbuf.h>
#include "BlueXGIMI_RC.h"


class BLE_Remote_Decoder {
public:
    BLE_Remote_Decoder(); 
    BLE_Remote_Decoder(RingbufHandle_t buf);
    void dequeueCmd();
    void start();


protected:
     BlueXGIMI_RC* rcHID;
     static RingbufHandle_t ringBufHandle;
};