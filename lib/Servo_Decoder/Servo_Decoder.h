
#include "defaults.h"
#include <freertos/ringbuf.h>
#include "ServerDecoder.h"

class Servo_Decoder : public ServerDecoderHandler
{
public:
    Servo_Decoder();
    void doCmd(ServerDecoder::Remote_Cmd *cmd);
    void start();
};
