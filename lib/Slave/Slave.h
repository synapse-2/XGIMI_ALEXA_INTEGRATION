#pragma once

#include "defaults.h"
#include <freertos/ringbuf.h>
#include "BlueXGIMI_RC.h"


class Slave {
public:
    Slave(); 
    Slave(RingbufHandle_t buf);
    void start();


protected:
     BlueXGIMI_RC* rcHID;
     static RingbufHandle_t ringBufHandle;
};