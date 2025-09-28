
#include "defaults.h"
#include "BlueRC.h"
#include <freertos/ringbuf.h>

namespace CmdRingBuffer{

    extern RingbufHandle_t ringBufHandle;
    void initCmdRingBuffer();
    void enQueueCmd(BlueRC::Remote_Cmd cmd);
    BlueRC::Remote_Cmd* peekCmd();
    void dequeueCmd(BlueRC::Remote_Cmd*  cmd);

}