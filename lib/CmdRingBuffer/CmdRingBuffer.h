
#include "defaults.h"
#include "BlueRC.h"
#include <freertos/ringbuf.h>

namespace CmdRingBuffer{

    extern RingbufHandle_t ringBufHandle;
    void initCmdRingBuffer();
    void enQueueCmd(ServerDecoder::Remote_Cmd cmd);

    // remember pperk dequeues the item but we sill need to dispose it
    ServerDecoder::Remote_Cmd* peekCmd();

    // de que frees the item we got from peep in teh heap
    void dequeueCmd(ServerDecoder::Remote_Cmd*  cmd);
    bool isEmpty();

}