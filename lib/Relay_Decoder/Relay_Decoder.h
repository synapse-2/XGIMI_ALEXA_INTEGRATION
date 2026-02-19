
#include "defaults.h"
#include <freertos/ringbuf.h>
#include "ServerDecoder.h"

class Relay_Decoder : public ServerDecoderHandler
{
public:
    Relay_Decoder();
    void doCmd(ServerDecoder::Remote_Cmd *cmd);
    void start();
};
