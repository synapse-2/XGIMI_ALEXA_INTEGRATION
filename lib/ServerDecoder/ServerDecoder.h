#pragma once
#include "magicEnum/magic_enum.hpp"

namespace ServerDecoder
{
    typedef enum : uint8_t
    {
        FastForward = 0,
        Next = 1,
        Pause = 2,
        Play = 3,
        Previous = 4,
        Rewind = 5,
        StartOver = 6,
        Stop = 7,

        Raw_8Byte = 232,
        Raw_2Byte = 233,
        Menu_Btn = 234,
        Channel_Up_Btn = 235,
        Channel_Dn_Btn = 236,
        Ok_Btn = 237,
        Up_Btn = 238,
        Right_Btn = 239,
        Left_Btn = 240,
        Down_Btn = 241,
        Vol_Up_Btn = 242,
        Vol_Dn_Btn = 243,
        Projector_Setting_Btn = 244,
        Settings_Btn = 245,
        Home_Btn = 246,
        Options_Btn = 247,
        Pairing_On = 248,
        Pairing_Off = 249,
        On_OFF_Btn = 250,
        Volume = 251,
        Channel = 252,
        Mute = 253,
        ChangeInput = 254,
        None = 255
    } RC_Cmd_Action;

    typedef union
    {
        struct
        {
            uint8_t cmd;
            uint8_t fromVal;
            uint8_t toVal;
            uint8_t codeData[8];
        } __attribute__((packed)) cmds;
        struct
        {
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
            uint8_t codeData[8]; //used to send the additional codes from raw blutooth.
        } __attribute__((packed)) bytes3;

    } __attribute__((packed)) Remote_Cmd;

}

class ServerDecoderBase
{
public:
    virtual void sendButtonPress(ServerDecoder::Remote_Cmd command) = 0;
    virtual bool canHandleButtonPress(ServerDecoder::Remote_Cmd command) = 0;
    ~ServerDecoderBase() {};
};

class ServerDecoderHandler
{
public:
    virtual void doCmd(ServerDecoder::Remote_Cmd *cmd) = 0;
    virtual void start() = 0;
    ~ServerDecoderHandler() {};
};

template <>
struct magic_enum::customize::enum_range<ServerDecoder::RC_Cmd_Action>
{
    static constexpr int min = 0;
    static constexpr int max = 255;
    // (max - min) must be less than UINT16_MAX.
};
