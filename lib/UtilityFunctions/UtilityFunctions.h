// filepath: d:\Documents\XGIMI_ALEXA_INTEGRATION\src\UtilityFunctions.h
#pragma once
#include "defaults.h"
#include <FastLED.h>

namespace UtilityFunctions
{

    extern CRGB leds[NUMPIXELS];

    void ledRed();
    void ledGreen();
    void ledYellow();
    void ledBlue();
    void ledWhite();
    void ledStop();
    void UtilityFunctionsInit();
    bool isMaster();
    int findI2cOtherAddress();
    bool isResetPressed();
    int numTimesResetPressed();
    void unpressRest();
    unsigned long resetMills();
    void delay(long waitMills);
    void waitTillInitComplete();

    void ledBlinkGreen();
    void ledBlinkBlue();
    void ledBlinkRed();
    void ledBlinkYellow();
    void ledBlinkGreenLong();
    void ledBlinkRedLong();

    void debugLog(String message);
    void debugLog();
    void debugLogf(const char *format, ...);

    String chipInfo();
    String taskInfo();

    // Load the mDNS hostname from NVRAM
    String loadLocalHostname();

    // save
    void saveLocalHostname(String newHostname);

    // Load wol packet num  from NVRAM
    int loadWakePacketNum();

    // Save  wol packet num  from NVRAM
    void saveWakePacketNum(int newWolNum);

    // Load BlueTooth Name from NVRAM
    String loadBlueToothName();

    // Save BlueTooth Name to NVRAM
    void saveBlueToothName(String newBlueName);

    /// get the actual ad bytes
    std::vector<uint8_t> getHID_AD2_MANUF_DATA();

    // Load servoIO pin  num  from NVRAM
    int loadServoIOPin();

    // Save  ServoIO pin  num   from NVRAM
    void saveServoIOPin(int newPinIO);

    // Load servo PWM MIN Width from NVRAM
    uint16_t loadServoPWMMinWidth();

    // Save servo PWN MIN Width from NVRAM
    void saveServoPWMMinWidth(uint16_t newMinWidth);

    // Load servo PWM MAX Width from NVRAM
    uint16_t loadServoPWMMaxWidth();

    // Save servo PWN MIN Width from NVRAM
    void saveServoPWMMaxWidth(uint16_t newMaxWidth);

    // Load servo PWM Freq from NVRAM
    uint32_t loadServoPWMFreq();

    // Save servo PWN MIN Width from NVRAM
    void saveServoPWMFreq(uint32_t newFreq);

    // Load servo max angle from NVRAM
    uint16_t loadServoMaxAngle();

    // Save servo max angle  from NVRAM
    void saveServoMaxAngle(uint16_t newAngle);

    // Load servo action angle from NVRAM
    uint16_t loadServoActionAngle();

    // Save servo action angle from NVRAM
    void saveServoActionAngle(uint16_t newAngle);

    // Load servo rest angle from NVRAM
    uint16_t loadServoRestAngle();

    // Save servo rest angle from NVRAM
    void saveServoRestAngle(uint16_t newAngle);


      // Load servo action hold from NVRAM
    uint16_t loadServoActionHold();

    // Save servo action hold from NVRAM
    void saveServoActionHold(uint16_t newhold);

}