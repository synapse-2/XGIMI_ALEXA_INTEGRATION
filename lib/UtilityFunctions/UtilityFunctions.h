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


    //save 
    void saveLocalHostname(String newHostname);

    // Load wol packet num  from NVRAM
    int loadWakePacketNum();

    // Save  wol packet num  from NVRAM
    void saveWakePacketNum(int newWolNum);


    // Load BlueTooth Name from NVRAM
    String loadBlueToothName();
    
    // Save BlueTooth Name to NVRAM
    void saveBlueToothName(String newBlueName);

    ///get the actual ad bytes
    std::vector<uint8_t> getHID_AD2_MANUF_DATA();

}