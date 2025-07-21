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
    bool MasterOrSlave();
    int findI2cOtherAddress();
    bool isResetPressed();

    
}