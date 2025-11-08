#include "CmdRingBuffer.h"
#include "defaults.h"
#include "BlueRC.h"
#include "UtilityFunctions.h"
#include <freertos/ringbuf.h>


namespace CmdRingBuffer
{
    RingbufHandle_t ringBufHandle;
    void initCmdRingBuffer()
    {
        // Create a ring buffer of bytes with no-split type
        ringBufHandle = xRingbufferCreate(RING_BUFF_SIZE_FOR_CMD, RINGBUF_TYPE_NOSPLIT);

        if (ringBufHandle == NULL)
        {
            UtilityFunctions::debugLog("Failed to create ring buffer\n");
            ESP.restart();
        }
    }
    void enQueueCmd(ServerDecoder::Remote_Cmd cmd)
    {
        if (ringBufHandle == NULL)
        {
            UtilityFunctions::debugLog("Failed to create ring buffer CANNOT ENQUE\n");
            return;
        }
        size_t sizefree = xRingbufferGetCurFreeSize(ringBufHandle);
        UBaseType_t res = xRingbufferSend(ringBufHandle, (void *)&cmd, sizeof(cmd), 100);
        if (res != pdTRUE)
        {
            UtilityFunctions::debugLogf("Failed to enqueue RC cmd SKIPPING free size in ring buffer %i \n", sizefree);
        }
    }
    ServerDecoder::Remote_Cmd *peekCmd()
    {

        ServerDecoder::Remote_Cmd *cmd;
        size_t received_len;

        if (ringBufHandle == NULL)
        {
            UtilityFunctions::debugLog("Failed to create ring buffer CANNOT PEEK\n");
            return NULL;
        }

        cmd = (ServerDecoder::Remote_Cmd *)xRingbufferReceive(ringBufHandle, &received_len, 50);
        if (cmd != NULL)
        {
            std::string s_cmd = std::string((magic_enum::enum_name((ServerDecoder::RC_Cmd_Action)cmd->cmds.cmd)));
            UtilityFunctions::debugLogf("Remote command DEQUEUEED Str:%s INt:%i \n", s_cmd.c_str(), cmd->cmds.cmd);
        }

        return (cmd);
    }

    void dequeueCmd(ServerDecoder::Remote_Cmd *cmd)
    {
        if (ringBufHandle == NULL)
        {
            UtilityFunctions::debugLog("Failed to create ring buffer CANNOT DEQUE\n");
            return;
        }
        vRingbufferReturnItem(ringBufHandle, (void *)cmd);
    }
}
