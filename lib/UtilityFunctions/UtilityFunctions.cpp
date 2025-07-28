#include <UtilityFunctions.h>
#include <defaults.h>
#include <Wire.h>
#include <Arduino.h>
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
        bool initPerformed = false;
    }

    // waits till init is completed; the execution is blocked
    void waitTillInitComplete()
    {

        for (; !initPerformed;)
        {
            UtilityFunctions::delay(30);
        }
    }
    void delay(long waitMills)
    {
        // non blocking delay i.e busy wait
        long cuurentMillis = millis();
        for (; millis() - cuurentMillis <= waitMills;)
        {

            //__asm__("nop"); // saver battery/power
            //yield();        // tell watchdog timer do other level 0 idle tasks
            vTaskDelay(waitMills * portTICK_PERIOD_MS);
        }
    }
    void ledRed()
    {
        debugLog("Turning LED red");
        leds[0] = CRGB::Red;
        FastLED.show();
    }

    void ledGreen()
    {
        debugLog("Turning LED green");
        leds[0] = CRGB::Green;
        FastLED.show();
    }

    void ledYellow()
    {
        debugLog("Turning LED yellow");
        leds[0] = CRGB::Yellow;
        FastLED.show();
    }

    void ledBlue()
    {
        debugLog("Turning LED blue");
        leds[0] = CRGB::Blue;
        FastLED.show();
    }

    void ledWhite()
    {
        debugLog("Turning LED white");
        leds[0] = CRGB::White;
        FastLED.show();
    }

    void ledStop()
    {
        debugLog("Turning LED off");
        leds[0] = CRGB::Black;
        FastLED.show();
    }

    void IRAM_ATTR isr()
    {
        // only work for master
        if (!isMaster())
        {
            // we are slave and this ISR is not valid for us
            return;
        }
        buttonReset_time = millis();
        if (buttonReset_time - last_buttonReset_time > 250)
        {
            buttonReset.numberKeyPresses++;
            buttonReset.pressed = true;
            last_buttonReset_time = buttonReset_time;
        }
        if (buttonReset_time - last_buttonReset_time > 5000)
        {
            buttonReset.numberKeyPresses = 1; // Reset the count if over 5 secs
            buttonReset.pressed = true;       // Unpress the button
            last_buttonReset_time = buttonReset_time;
        }
    }

    bool isResetPressed()
    {

        return buttonReset.pressed;
    }

    int numTimesResetPressed()
    {
        return buttonReset.numberKeyPresses;
    }

    void unpressRest()
    {
        buttonReset.pressed = false;
    }

    void UtilityFunctionsInit()
    {
        // only init once
        if (initPerformed)
        {
            return;
        }
        Serial.begin(115200);
        while (!Serial)
            ; // wait for serial attach
        debugLog("Initializing...");

        FastLED.addLeds<RGBCHIP, LED_BUILTINIO, RGB_DATA_ORDER>(leds, NUMPIXELS);
        // Initialize the LED array to off (black)
        for (int i = 0; i < NUMPIXELS; ++i)
        {
            leds[i] = CRGB::Black;
        }
        FastLED.show();

        debugLog();
        /* Print chip information */
        esp_chip_info_t chip_info;
        uint32_t flash_size;
        esp_chip_info(&chip_info);
        unsigned major_rev = chip_info.revision / 100;
        unsigned minor_rev = chip_info.revision % 100;
        if (esp_flash_get_size(NULL, &flash_size) != ESP_OK)
        {
            debugLog("Get flash size failed");
        }

        debugLogf("This is %s chip with %d CPU core(s), %s%s%s%s, silicon revision v%d.%d, %" PRIu32 "MB %s flash\n",
                  CONFIG_IDF_TARGET,
                  chip_info.cores,
                  (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
                  (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
                  (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
                  (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "", major_rev, minor_rev, flash_size / (uint32_t)(1024 * 1024),
                  (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

        printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size);
        printf("Total heap: %u\n", ESP.getHeapSize());
        printf("Free heap: %u\n", ESP.getFreeHeap());
        printf("Total PSRAM: %u\n", ESP.getPsramSize());
        printf("Free PSRAM: %d\n", ESP.getFreePsram());
        

        // FastLED.addLeds<NEOPIXEL, LED_BUILTINIO>(leds, NUMPIXELS);
        uint64_t macID = ESP.getEfuseMac(); // Get the MAC address
        debugLogf("Device MAC Address: %04X:%04X:%04X:%04X:%04X:%04X\n",
                  (uint16_t)(macID >> 32),
                  (uint16_t)(macID >> 16),
                  (uint16_t)(macID & 0xFFFF),
                  (uint16_t)(macID >> 48),
                  (uint16_t)(macID >> 32),
                  (uint16_t)(macID >> 16));

        // Extract the last two bytes
        uint16_t lastByte = (uint16_t)(macID & 0x00FF);

        debugLogf("Last Byte: 0x%04X\n", lastByte);

        UtilityFunctions::delay(lastByte);

        debugLog("Initializing I2C...");
        bool masterSuccess = Wire1.begin(I2C_SDA, I2C_SCLK, I2C_FREQ); // Initialize I2C with specified pins and frequency

        debugLog(String("We are master: ") + (masterSuccess ? "true" : "false"));

        masterMode = masterSuccess; // Set the mode based on the success of Wire1.begin

        if (masterSuccess)
        {
            // if we are mastwer we need the boot button to reset the saved WIFI info
            pinMode(buttonReset.PIN, INPUT_PULLUP);
            attachInterrupt(buttonReset.PIN, isr, FALLING);
        }
        else
        {
            // we are in slave mode
            Wire1.begin(I2C_SLAVE_ADDR, I2C_SDA, I2C_SCLK, I2C_FREQ);
            debugLog(String("were slave: ") + (masterSuccess ? "false" : "true")); // opposite of master
        }

        // set init was cmpleted ok
        initPerformed = true;
    }

    bool isMaster()
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

    void debugLog()
    {
        Serial.println();
    }

    void debugLog(String message)
    {
        Serial.print("CORE:");
        Serial.print(xPortGetCoreID());
        Serial.print(":");
        Serial.println(message);
    }
    void debugLogf(const char *format, ...)
    {
        char loc_buf[64];
        char *temp = loc_buf;
        va_list arg;
        va_list copy;
        va_start(arg, format);
        va_copy(copy, arg);
        int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
        va_end(copy);
        if (len < 0)
        {
            va_end(arg);
            return;
        }
        if (len >= (int)sizeof(loc_buf))
        { // comparation of same sign type for the compiler
            temp = (char *)malloc(len + 1);
            if (temp == NULL)
            {
                va_end(arg);
                return;
            }
            len = vsnprintf(temp, len + 1, format, arg);
        }
        va_end(arg);
        Serial.print("CORE:");
        Serial.print(xPortGetCoreID());
        Serial.print(":");
        len = Serial.write((uint8_t *)temp, len);
        if (temp != loc_buf)
        {
            free(temp);
        }
        return;
    }
}
