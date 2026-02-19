
#include "defaults.h"
#include "BlueRC.h"
#include <freertos/ringbuf.h>

namespace CmdRingBuffer{

    extern RingbufHandle_t ringBufHandle;
    void initCmdRingBuffer();
    void enQueueCmd(ServerDecoder::Remote_Cmd cmd);
    ServerDecoder::Remote_Cmd* peekCmd();
    void dequeueCmd(ServerDecoder::Remote_Cmd*  cmd);

}