// filepath: d:\Documents\XGIMI_ALEXA_INTEGRATION\src\UtilityFunctions.h
#pragma once
#include "defaults.h"
#include <FastLED.h> 


namespace UtilityFunctions {

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

    void debugLog(String message);
    void debugLog();
    void debugLogf(const char *format, ...);

    
}