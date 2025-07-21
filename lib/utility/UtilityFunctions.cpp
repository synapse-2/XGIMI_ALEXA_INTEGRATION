#include "UtilityFunctions.h"
#include "defaults.h"
#include <Wire.h>
#include <FastLED.h>


CRGB UtilityFunctions::leds[NUMPIXELS];
namespace UtilityFunctions
{

    namespace
    {

        // This is a private variable to keep track of the master/slave mode
        // It is not exposed outside this namespace.
        bool masterMode = false; // true for master, false for slave
        struct Button
        {
            const uint8_t PIN;
            uint32_t numberKeyPresses;
            bool pressed;
        };

        // variables to keep track of the timing of recent interrupts
        unsigned long buttonReset_time = 0;
        unsigned long last_buttonReset_time = 0;

        Button buttonReset = {ResetButton, 0, false};


    }

    void ledRed()
    {
        leds[0] = CRGB::Red;
        FastLED.show();
    }

    void ledGreen()
    {
        leds[0] = CRGB::Green;
        FastLED.show();
    }

    void ledYellow()
    {
        leds[0] = CRGB::Yellow;
        FastLED.show();
    }

    void ledBlue()
    {
        leds[0] = CRGB::Blue;
        FastLED.show();
    }


    void ledWhite()
    {
        leds[0] = CRGB::White;
        FastLED.show();
    }

    void ledStop()
    {
        leds[0] = CRGB::Black;
        FastLED.show();
    }

    void IRAM_ATTR isr()
    {
        buttonReset_time = millis();
        if (buttonReset_time - last_buttonReset_time > 250)
        {
            buttonReset.numberKeyPresses++;
            buttonReset.pressed = true;
            last_buttonReset_time = buttonReset_time;
        }

    
       
        
    }

    bool isResetPressed()
    {
        
        return buttonReset.pressed;
    }

    void UtilityFunctionsInit()
    {
        pinMode(buttonReset.PIN, INPUT_PULLUP);
        attachInterrupt(buttonReset.PIN, isr, FALLING);

        FastLED.addLeds<NEOPIXEL, LED_BUILTINIO>(leds, NUMPIXELS);
        uint64_t macID = ESP.getEfuseMac(); // Get the MAC address
        Serial.printf("Device MAC Address: %04X:%04X:%04X:%04X:%04X:%04X\n",
                      (uint16_t)(macID >> 32),
                      (uint16_t)(macID >> 16),
                      (uint16_t)(macID & 0xFFFF),
                      (uint16_t)(macID >> 48),
                      (uint16_t)(macID >> 32),
                      (uint16_t)(macID >> 16));

        // Extract the last two bytes
        uint16_t lastByte = (uint16_t)(macID & 0x00FF);

        Serial.printf("Last Byte: 0x%04X\n", lastByte);

        delay(lastByte);

        Serial.println("Initializing I2C...");
        bool masterSuccess = Wire1.begin(I2C_SDA, I2C_SCLK, I2C_FREQ); // Initialize I2C with specified pins and frequency

        Serial.println(String("were master: ") + (masterSuccess ? "true" : "false"));

        masterMode = masterSuccess; // Set the mode based on the success of Wire1.begin

        if (!masterSuccess)
        {
            // we are in slave mode
            Wire1.begin(I2C_SLAVE_ADDR, I2C_SDA, I2C_SCLK, I2C_FREQ);
        }
        // Initialize the LED array to off (black)
        for (int i = 0; i < NUMPIXELS; ++i)
        {
            leds[i] = CRGB::Black;
        }
        FastLED.show();
    }

    bool MasterOrSlave()
    {
        return masterMode;
    }

    int findI2cOtherAddress()
    {

        byte error, address;
        byte lowestDevADDR;

        Serial.println("Scanning...");

        lowestDevADDR = 0;
        for (address = 1; address < 127; address++)
        {
            // The i2c_scanner uses the return value of
            // the Write.endTransmisstion to see if
            // a device did acknowledge to the address.
            Wire1.beginTransmission(address);
            error = Wire1.endTransmission();

            if (error == 0)
            {
                Serial.print("I2C device found at address 0x");
                if (address < 16)
                    Serial.print("0");
                Serial.print(address, HEX);
                Serial.println("  !");

                if (lowestDevADDR == 0)
                {
                    lowestDevADDR = address; // Store the first found address
                }
                if (address < lowestDevADDR)
                {
                    lowestDevADDR = address; // Store the lowest address found
                    Serial.print("Lowest address found so far: 0x");
                    Serial.println(lowestDevADDR, HEX);
                }
            }
            else if (error == 4)
            {
                Serial.print("Unknown error at address 0x");
                if (address < 16)
                    Serial.print("0");
                Serial.println(address, HEX);
            }
        }

        Serial.println("done\n");
        return lowestDevADDR;
    }
}
