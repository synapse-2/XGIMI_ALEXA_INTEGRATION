#include "defaults.h"
// #include <Wire.h>
#include <Arduino.h>
#include <FastLED.h>
#include <esp_chip_info.h>
#include <magicEnum/magic_enum.hpp>
#include <magicEnum/magic_enum_iostream.hpp>
#include "UtilityFunctions.h"
#include <Preferences.h>
#include <format>
#include <string>
#include <WiFiManager.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

extern std::vector<projectorWAKE_str> projectorWakeList;

template <typename E>
auto to_integer(magic_enum::Enum<E> value) -> int
{
    // magic_enum::Enum<E> - C++17 Concept for enum type.
    return static_cast<magic_enum::underlying_type_t<E>>(value);
}

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
            // yield();        // tell watchdog timer do other level 0 idle tasks
            vTaskDelay(waitMills * portTICK_PERIOD_MS);
        }
    }
    void ledRed()
    {
#ifdef UTILFUNC_DEBUG_LED_ON
        debugLog("Turning LED red");
#endif
        leds[0] = CRGB::Red;
        FastLED.show();
    }

    void ledGreen()
    {
#ifdef UTILFUNC_DEBUG_LED_ON
        debugLog("Turning LED green");
#endif
        leds[0] = CRGB::Green;
        FastLED.show();
    }

    void ledYellow()
    {
#ifdef UTILFUNC_DEBUG_LED_ON
        debugLog("Turning LED yellow");
#endif
        leds[0] = CRGB::Yellow3;
        FastLED.show();
    }

    void ledBlue()
    {
#ifdef UTILFUNC_DEBUG_LED_ON
        debugLog("Turning LED blue");
#endif
        leds[0] = CRGB::Blue;
        FastLED.show();
    }

    void ledWhite()
    {
#ifdef UTILFUNC_DEBUG_LED_ON
        debugLog("Turning LED white");
#endif
        leds[0] = CRGB::White;
        FastLED.show();
    }

    void ledStop()
    {

#ifdef UTILFUNC_DEBUG_LED_ON
        debugLog("Turning LED off");
#endif
        leds[0] = CRGB::Black;
        FastLED.show();
    }

    void ledBlinkGreen()
    {
        CRGB currColor = leds[0];

        UtilityFunctions::ledGreen(); // Turn on the LED to indicate a change has been received
        UtilityFunctions::delay(30);
        UtilityFunctions::ledStop(); // Turn off the LED after processing the change

        leds[0] = currColor;
        FastLED.show();
    }

    void ledBlinkRed()
    {
        CRGB currColor = leds[0];

        UtilityFunctions::ledRed(); // Turn on the LED to indicate a change has been received
        UtilityFunctions::delay(30);
        UtilityFunctions::ledStop(); // Turn off the LED after processing the change

        leds[0] = currColor;
        FastLED.show();
    }

    void ledBlinkYellow()
    {
        CRGB currColor = leds[0];

        UtilityFunctions::ledYellow(); // Turn on the LED to indicate a change has been received
        UtilityFunctions::delay(30);
        UtilityFunctions::ledStop(); // Turn off the LED after processing the change

        leds[0] = currColor;
        FastLED.show();
    }

    void ledBlinkBlue()
    {
        CRGB currColor = leds[0];

        UtilityFunctions::ledBlue(); // Turn on the LED to indicate a change has been received
        UtilityFunctions::delay(30);
        UtilityFunctions::ledStop(); // Turn off the LED after processing the change

        leds[0] = currColor;
        FastLED.show();
    }

    void ledBlinkGreenLong()
    {
        CRGB currColor = leds[0];

        for (int i = 0; i < 10; i++)
        {
            UtilityFunctions::ledGreen();
            UtilityFunctions::delay(30);
            UtilityFunctions::ledStop();
            UtilityFunctions::delay(30);
        }
        UtilityFunctions::ledStop(); // Turn off the LED after processing the change

        leds[0] = currColor;
        FastLED.show();
    }

    void ledBlinkRedLong()
    {
        CRGB currColor = leds[0];

        for (int i = 0; i < 10; i++)
        {
            UtilityFunctions::ledRed();
            UtilityFunctions::delay(30);
            UtilityFunctions::ledStop();
            UtilityFunctions::delay(30);
        }
        UtilityFunctions::ledStop(); // Turn off the LED after processing the change

        leds[0] = currColor;
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
        unsigned long buttonReset_time = millis();

        if (buttonReset_time < last_buttonReset_time)
        {
            // we have overflowed the mills re set the lasst button time
            last_buttonReset_time = buttonReset_time;
        }
        unsigned long diff = buttonReset_time - last_buttonReset_time;
        if ((diff > 250) && (diff < 3000))
        {
            buttonReset.numberKeyPresses++;
            buttonReset.pressed = true;
            last_buttonReset_time = buttonReset_time;
        }
        if (diff > 3000)
        {
            buttonReset.numberKeyPresses = 0; // Reset the count if over 3 secs
            buttonReset.pressed = false;      // Unpress the button
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
    unsigned long resetMills()
    {
        return last_buttonReset_time;
    }

    void unpressRest()
    {
        buttonReset.pressed = false;
    }

    String taskInfo()
    {

        std::string str;
#ifdef configUSE_TRACE_FACILITY
        str = "Free RTOS TASKS :\n";
        // Get the total number of tasks
        UBaseType_t numberOfTasks = uxTaskGetNumberOfTasks();

        // Allocate memory for the TaskStatus_t array
        TaskStatus_t *taskStatusArray = new TaskStatus_t[numberOfTasks];

        // Get the system state and fill the array

#ifdef configGENERATE_RUN_TIME_STATS

        configRUN_TIME_COUNTER_TYPE ulTotalRunTime, ulStatsAsPercentage;

        /* For percentage calculations. */
        ulTotalRunTime = 100;
        if (uxTaskGetSystemState(taskStatusArray, numberOfTasks, &ulTotalRunTime) > 0)
#else
        if (uxTaskGetSystemState(taskStatusArray, numberOfTasks, NULL) > 0)
#endif
        {

            str = str + std::format("{: <" STRINGIFY(CONFIG_FREERTOS_MAX_TASK_NAME_LEN) "}{: <10}{: <10}{: <10}{: <10}\n", "Task Name", "State", "Prio", "Core ID", "% CPU");

            for (int pri = ESP_TASK_PRIO_MAX - 1; pri >= ESP_TASK_PRIO_MIN; pri = pri - 1)
            {
                for (int i = 0; i < numberOfTasks; i++)
                {

                    if (taskStatusArray[i].uxCurrentPriority == pri)
                    {
#ifdef configGENERATE_RUN_TIME_STATS
                        /* What percentage of the total run time has the task used?
                            This will always be rounded down to the nearest integer.
                        ulTotalRunTimeDiv100 has already been divided by 100. */
                        if (ulTotalRunTime != 0)
                        {
                            ulStatsAsPercentage = (taskStatusArray[i].ulRunTimeCounter / ulTotalRunTime);
                        }
                        else
                        {
                            ulStatsAsPercentage = taskStatusArray[i].ulRunTimeCounter;
                        }
#else
                        ulStatsAsPercentage = 0;
#endif

                        const char *taskName = taskStatusArray[i].pcTaskName;

                        // Get task state as a readable string
                        const char *state;
                        switch (taskStatusArray[i].eCurrentState)
                        {
                        case eRunning:
                            state = "Running";
                            break;
                        case eReady:
                            state = "Ready";
                            break;
                        case eBlocked:
                            state = "Blocked";
                            break;
                        case eSuspended:
                            state = "Suspended";
                            break;
                        case eDeleted:
                            state = "Deleted";
                            break;
                        default:
                            state = "Unknown";
                            break;
                        }

                        UBaseType_t priority = taskStatusArray[i].uxCurrentPriority;

                        UBaseType_t coreID = 0;
#ifdef configTASKLIST_INCLUDE_COREID
                        coreID = taskStatusArray[i].xCoreID; // Specific to ESP-IDF
#endif

                        str = str + std::format("{: <" STRINGIFY(CONFIG_FREERTOS_MAX_TASK_NAME_LEN) "}{: <10}{: <10}{: <10}{: <10}\n", taskName, state, priority, coreID, ulStatsAsPercentage);
                    }
                }
            }

            // Free the dynamically allocated memory
            delete[] taskStatusArray;
#else
        str = "Free RTOS TASKS NOT CONFIGURED :\n";
#endif
            return String(str.c_str());
        }
    }
    String chipInfo()
    {
        /* Print chip information */

        unsigned major_rev = ESP.getChipRevision() / 100;
        unsigned minor_rev = ESP.getChipRevision() % 100;
        uint32_t flash_size = ESP.getFlashChipSize();
        esp_chip_info_t chip_info;
        esp_chip_info(&chip_info);

        std::string str = std::format("System {} chip with {} CPU core(s) Clock Feq {} MHz, {}{}{}{}, silicon revision v{}.{}, {} MB {} flash \n",
                                      ESP.getChipModel(),
                                      ESP.getChipCores(),
                                      ESP.getCpuFreqMHz(),
                                      (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
                                      (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
                                      (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
                                      (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "", major_rev, minor_rev, flash_size / (uint32_t)(1024 * 1024),
                                      (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

        str = str + std::format("Minimum free heap size: {} KB \n", esp_get_minimum_free_heap_size() / 1024);
        str = str + std::format("Total heap: {} KB \n", ESP.getHeapSize() / 1024);
        str = str + std::format("Free heap: {} KB \n", ESP.getFreeHeap() / 1024);
        str = str + std::format("Total Flash Chip Mode: {} \n", magic_enum::enum_name(ESP.getFlashChipMode()));
        str = str + std::format("Fash Chip Speed {} MHz \n", ESP.getFlashChipSpeed() / (1000 * 1000));
        str = str + std::format("Total PSRAM: {} KB \n", ESP.getPsramSize() / 1024);
        str = str + std::format("Free PSRAM: {} KB\n", ESP.getFreePsram() / 1024);

        str = str + std::format("SDK version: {} \n", ESP.getSdkVersion());
        str = str + std::format("Core version: {} \n", ESP.getCoreVersion());

        str = str + std::format("Sketch Size: {} KB \n", ESP.getSketchSize() / 1024);
        str = str + std::format("Sketch Free Space: {} KB\n", ESP.getFreeSketchSpace() / 1024);

        uint64_t macID = ESP.getEfuseMac(); // Get the MAC address
        str = str + std::format("Device MAC Address: {:02X}:{:02X}:{:02X}:{:02X}:{:02X}:{:02X}\n",
                                (uint16_t)(macID & 0x0000000000FF),
                                (uint16_t)(macID >> 8) & 0x0000000000FF,
                                (uint16_t)(macID >> 16) & 0x0000000000FF,
                                (uint16_t)(macID >> 24) & 0x0000000000FF,
                                (uint16_t)(macID >> 32) & 0x0000000000FF,
                                (uint16_t)(macID >> 40) & 0x0000000000FF);

        WiFiManager wm;
        // can contain gargbage on esp32 if wifi is not ready yet
        str = str + "[WIFI] WIFI_INFO DEBUG \n";
        str = str + std::format("[WIFI] MODE: {} \n", wm.getModeString(WiFi.getMode()).c_str());
        str = str + std::format("[WIFI] SAVED: {} \n", (wm.getWiFiIsSaved() ? "YES" : "NO"));
        str = str + std::format("[WIFI] SSID: {} \n", wm.getWiFiSSID().c_str());
        str = str + std::format("[WIFI] CHANNEL: {} \n", WiFi.channel());
        str = str + std::format("[WIFI] RSSI: {} \n", WiFi.RSSI());
        str = str + std::format("[WIFI] PASS: {} \n", wm.getWiFiPass().c_str());
        str = str + std::format("[WIFI] HOSTNAME: {} \n", WiFi.getHostname());

        str = str + "In order to RESET and erase NVRAM press boot key 3 times within 3 seconds";

        return String(str.c_str());
    }

    String ledCInfo()
    {

        std::string str = std::format("LEDC Channel Status:\n");

        // Iterate through high-speed mode channels
#if SOC_LEDC_SUPPORT_HS_MODE
        str = str + std::format("High-Speed Mode Channels:\n");
        for (int channel = 0; channel < LEDC_CHANNEL_MAX; channel++)
        {
            uint32_t duty = ledc_get_duty(ledc_mode_t::LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel);

            str = str + std::format("Channel {}: Duty={}\n", channel, duty);
        }
        for (int timer = 0; timer < LEDC_TIMER_MAX; timer++)
        {
            uint32_t freq = ledc_get_freq(ledc_mode_t::LEDC_LOW_SPEED_MODE, (ledc_timer_t)timer);
            str = str + std::format("Timer {}: Freq={} Hz\n", timer, freq);
        }
#else
    str = str + std::format("High-Speed Mode Channels: NA \n");
#endif

        // Iterate through low-speed mode channels
        str = str + std::format("Low-Speed Mode Channels:\n");
        for (int channel = 0; channel < LEDC_CHANNEL_MAX; channel++)
        {
            uint32_t duty = ledc_get_duty(ledc_mode_t::LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel);

            str = str + std::format("Channel {}: Duty={}\n", channel, duty);
        }
        for (int timer = 0; timer < LEDC_TIMER_MAX; timer++)
        {
            uint32_t freq = ledc_get_freq(ledc_mode_t::LEDC_LOW_SPEED_MODE, (ledc_timer_t)timer);
            str = str + std::format("Timer {}: Freq={} Hz\n", timer, freq);
        }

        return String(str.c_str());
    }

    void UtilityFunctionsInit()
    {
        // only init once
        if (initPerformed)
        {
            return;
        }

        FastLED.addLeds<RGBCHIP, LED_BUILTINIO, RGB_DATA_ORDER>(leds, NUMPIXELS);
        // Initialize the LED array to off (black)
        for (int i = 0; i < NUMPIXELS; ++i)
        {
            leds[i] = CRGB::Black;
        }
        FastLED.show();

        // start debug log
        debugLog();

        debugLog(chipInfo());

        // Extract the last two bytes
        // uint16_t lastByte = (uint16_t)(macID & 0x00FF);

        // debugLogf("Last Byte: 0x%04X\n", lastByte);

        // UtilityFunctions::delay(lastByte);

        // debugLog("Initializing I2C...");
        // bool masterSuccess = Wire1.begin(I2C_SDA, I2C_SCLK, I2C_FREQ); // Initialize I2C with specified pins and frequency

        // debugLog(String("We are master: ") + (masterSuccess ? "true" : "false"));

        masterMode = true; // Set the mode based on the success of Wire1.begin

        if (masterMode)
        {
            // if we are mastwer we need the boot button to reset the saved WIFI info
            pinMode(buttonReset.PIN, INPUT_PULLUP);
            attachInterrupt(buttonReset.PIN, isr, FALLING);
        }
        else
        {
            // we are in slave mode
            // Wire1.begin(I2C_SLAVE_ADDR, I2C_SDA, I2C_SCLK, I2C_FREQ);
            debugLog(String("were slave: ") + (masterMode ? "false" : "true")); // opposite of master
        }

        // set init was cmpleted ok
        initPerformed = true;
    }

    bool isMaster()
    {
        return masterMode;
    }

    // int findI2cOtherAddress()
    // {

    //     byte error, address;
    //     byte lowestDevADDR;

    //     Serial.println("Scanning...");

    //     lowestDevADDR = 0;
    //     for (address = 1; address < 127; address++)
    //     {
    //         // The i2c_scanner uses the return value of
    //         // the Write.endTransmisstion to see if
    //         // a device did acknowledge to the address.
    //         Wire1.beginTransmission(address);
    //         error = Wire1.endTransmission();

    //         if (error == 0)
    //         {
    //             Serial.print("I2C device found at address 0x");
    //             if (address < 16)
    //                 Serial.print("0");
    //             Serial.print(address, HEX);
    //             Serial.println("  !");

    //             if (lowestDevADDR == 0)
    //             {
    //                 lowestDevADDR = address; // Store the first found address
    //             }
    //             if (address < lowestDevADDR)
    //             {
    //                 lowestDevADDR = address; // Store the lowest address found
    //                 Serial.print("Lowest address found so far: 0x");
    //                 Serial.println(lowestDevADDR, HEX);
    //             }
    //         }
    //         else if (error == 4)
    //         {
    //             Serial.print("Unknown error at address 0x");
    //             if (address < 16)
    //                 Serial.print("0");
    //             Serial.println(address, HEX);
    //         }
    //     }

    //     Serial.println("done\n");
    //     return lowestDevADDR;
    // }

    // Load hostname from NVRAM
    String loadLocalHostname()
    {
        Preferences _preferences;
        _preferences.begin(NVRAM_PERFS, false);
        String _localHostname = _preferences.getString(NVRAM_PERFS_HOSTNAME_LOCAL_PROP, NVRAM_PERFS_HOSTNAME_LOCAL_DEFAULT);
        _preferences.end();
        UtilityFunctions::debugLogf("loaded local hostname from NVRAM. %s\n", _localHostname.c_str());

        return _localHostname;
    }

    // Save hostname to NVRAM
    void saveLocalHostname(String newHostname)
    {
        if (!newHostname.isEmpty() && (newHostname.length() < 32) && (!newHostname.endsWith(".local")))
        {
            Preferences _preferences;
            _preferences.begin(NVRAM_PERFS, false);
            _preferences.putString(NVRAM_PERFS_HOSTNAME_LOCAL_PROP, newHostname);
            _preferences.end();
            UtilityFunctions::debugLog("hostname updated and saved to NVRAM.");
            ESP.restart();
        }
        else
        {
            UtilityFunctions::debugLog("Invalid hostnname, must be less than 32 chars;not empty; and not .local in the end");
        }
    }

    // Load BlueTooth Name from NVRAM
    String loadBlueToothName()
    {
        Preferences _preferences;
        _preferences.begin(NVRAM_PERFS, false);
        String newBlueName = _preferences.getString(NVRAM_PERFS_BLUETOOTH_NAME_PROP, NVRAM_PERFS_BLUETOOTH_NAME_DEFAULT);
        _preferences.end();
        UtilityFunctions::debugLogf("loaded local hostname from NVRAM. %s\n", newBlueName.c_str());

        return newBlueName;
    }

    // Save BlueTooth Name to NVRAM
    void saveBlueToothName(String newBlueName)
    {
        if (!newBlueName.isEmpty() && (newBlueName.length() < 32))
        {
            Preferences _preferences;
            _preferences.begin(NVRAM_PERFS, false);
            _preferences.putString(NVRAM_PERFS_BLUETOOTH_NAME_PROP, newBlueName);
            _preferences.end();
            UtilityFunctions::debugLog("hostname updated and saved to NVRAM.");
            ESP.restart();
        }
        else
        {
            UtilityFunctions::debugLog("Invalid BlueTooth Name , must be less than 32 chars;not empty");
        }
    }

    // Load wol packet num  from NVRAM
    int loadWakePacketNum()
    {
        Preferences _preferences;
        _preferences.begin(NVRAM_PERFS, false);
        int wolNum = _preferences.getInt(NVRAM_PERFS_WAKE_PACKET_PROP, NVRAM_PERFS_WAKE_PACKET_DEFAULT);
        _preferences.end();
        UtilityFunctions::debugLogf("loaded WOL Packet num from NVRAM. %i\n", wolNum);

        return wolNum;
    }

    // Save  wol packet num  from NVRAM
    void saveWakePacketNum(int newWolNum)
    {
        if ((newWolNum >= 0) && (newWolNum < projectorWakeList.size()))
        {
            Preferences _preferences;
            _preferences.begin(NVRAM_PERFS, false);
            _preferences.putInt(NVRAM_PERFS_WAKE_PACKET_PROP, newWolNum);
            _preferences.end();
            UtilityFunctions::debugLog("WOl packet num updated and saved to NVRAM.");
        }
        else
        {
            UtilityFunctions::debugLogf("Number need to be bterween 0 and  less than %i got %i\n", projectorWakeList.size(), newWolNum);
        }
    }

    std::vector<uint8_t> getHID_AD2_MANUF_DATA()
    {
        return projectorWakeList[loadWakePacketNum()].manufData;
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

    // Load servoIO pin  num  from NVRAM
    int loadServoIOPin()
    {
        Preferences _preferences;
        _preferences.begin(NVRAM_PERFS, false);
        int servoIOPin = _preferences.getInt(NVRAM_PERFS_SERVO_IO_PROP, NVRAM_PERFS_SERVO_IO_DEFAULT);
        _preferences.end();
        UtilityFunctions::debugLogf("loaded servoIO pin num from NVRAM. %i\n", servoIOPin);

        return servoIOPin;
    }

    // Save ServoIO pin num from NVRAM
    void saveServoIOPin(int newPinIO)
    {

        // only use gpio.1, gpio.2, gpio.5, gpio.6, gpio.7, gpio.8, gpio.9
        // gpio.15 ,gpio.16 ,gpio.17 ,gpio.18
        // gpio.21
        if (((newPinIO >= 1) && (newPinIO < 2)) || ((newPinIO >= 5) && (newPinIO < 9)) || ((newPinIO >= 15) && (newPinIO < 18)) || (newPinIO == 21))
        {
            Preferences _preferences;
            _preferences.begin(NVRAM_PERFS, false);
            _preferences.putInt(NVRAM_PERFS_SERVO_IO_PROP, newPinIO);
            _preferences.end();
            UtilityFunctions::debugLog("servoIO pin num updated and saved to NVRAM.");
        }
        else
        {
            UtilityFunctions::debugLogf("Only use on ESP32S3 gpio.1, gpio.2, gpio.5, gpio.6, gpio.7, gpio.8, gpio.9, gpio.15 ,gpio.16 ,gpio.17 ,gpio.18, gpio.21 got %i\n", newPinIO);
        }
    }

    // Load servo PWM MIN Width from NVRAM
    uint16_t loadServoPWMMinWidth()
    {
        Preferences _preferences;
        _preferences.begin(NVRAM_PERFS, false);
        uint16_t minWidth = _preferences.getUInt(NVRAM_PERFS_SERVO_PWM_MIN_WIDTH_PROP, NVRAM_PERFS_SERVO_PWM_MIN_WIDTH_DEFAULT);
        _preferences.end();
        UtilityFunctions::debugLogf("loaded servo Min Width in us 500 = 0.5 us from NVRAM. %i\n", minWidth);

        return minWidth;
    }

    // Save servo PWN MIN Width from NVRAM
    void saveServoPWMMinWidth(uint16_t newMinWidth)
    {
        if (((newMinWidth >= 0) && (newMinWidth <= 5000)))
        {
            Preferences _preferences;
            _preferences.begin(NVRAM_PERFS, false);
            _preferences.putUInt(NVRAM_PERFS_SERVO_PWM_MIN_WIDTH_PROP, newMinWidth);
            _preferences.end();
            UtilityFunctions::debugLog("servo Min Width updated and saved to NVRAM.");
        }
        else
        {
            UtilityFunctions::debugLogf("Must be between 0 and 5000; 500 = 0.5 us got %i\n", newMinWidth);
        }
    }

    // Load servo PWM MAX Width from NVRAM
    uint16_t loadServoPWMMaxWidth()
    {
        Preferences _preferences;
        _preferences.begin(NVRAM_PERFS, false);
        uint16_t maxWidth = _preferences.getUInt(NVRAM_PERFS_SERVO_PWM_MAX_WIDTH_PROP, NVRAM_PERFS_SERVO_PWM_MAX_WIDTH_DEFAULT);
        _preferences.end();
        UtilityFunctions::debugLogf("loaded servo max Width in us 500 = 0.5 us from NVRAM. %i\n", maxWidth);

        return maxWidth;
    }

    // Save servo PWN MIN Width from NVRAM
    void saveServoPWMMaxWidth(uint16_t newMaxWidth)
    {
        if (((newMaxWidth >= 0) && (newMaxWidth <= 5000)))
        {
            Preferences _preferences;
            _preferences.begin(NVRAM_PERFS, false);
            _preferences.putUInt(NVRAM_PERFS_SERVO_PWM_MAX_WIDTH_PROP, newMaxWidth);
            _preferences.end();
            UtilityFunctions::debugLog("servo Max Width updated and saved to NVRAM.");
        }
        else
        {
            UtilityFunctions::debugLogf("Must be between 0 and 5000; 2500 = 2.5 us got %i\n", newMaxWidth);
        }
    }

    // Load servo PWM Freq from NVRAM
    uint32_t loadServoPWMFreq()
    {
        Preferences _preferences;
        _preferences.begin(NVRAM_PERFS, false);
        uint32_t freq = _preferences.getUInt(NVRAM_PERFS_SERVO_PWM_FREQ_PROP, NVRAM_PERFS_SERVO_PWM_FREQ_DEFAULT);
        _preferences.end();
        UtilityFunctions::debugLogf("loaded servo freq from NVRAM. %i\n", freq);

        return freq;
    }

    // Save servo PWN MIN Width from NVRAM
    void saveServoPWMFreq(uint32_t newFreq)
    {
        if (((newFreq >= 50) && (newFreq <= 400)))
        {
            Preferences _preferences;
            _preferences.begin(NVRAM_PERFS, false);
            _preferences.putUInt(NVRAM_PERFS_SERVO_PWM_FREQ_PROP, newFreq);
            _preferences.end();
            UtilityFunctions::debugLog("Servo Freq updated and saved to NVRAM.");
        }
        else
        {
            UtilityFunctions::debugLogf("Must be between 50 and 400; type 50 hz got %i\n", newFreq);
        }
    }

    // Load servo max angle from NVRAM
    uint16_t loadServoMaxAngle()
    {
        Preferences _preferences;
        _preferences.begin(NVRAM_PERFS, false);
        uint16_t angle = _preferences.getUInt(NVRAM_PERFS_SERVO_MAX_ANGLE_PROP, NVRAM_PERFS_SERVO_MAX_ANGLE_DEFAULT);
        _preferences.end();
        UtilityFunctions::debugLogf("loaded servo max angle from NVRAM. %i\n", angle);

        return angle;
    }

    // Save servo max angle  from NVRAM
    void saveServoMaxAngle(uint16_t newAngle)
    {
        if (((newAngle >= 0) && (newAngle <= 200)))
        {
            Preferences _preferences;
            _preferences.begin(NVRAM_PERFS, false);
            _preferences.putUInt(NVRAM_PERFS_SERVO_MAX_ANGLE_PROP, newAngle);
            _preferences.end();
            UtilityFunctions::debugLog("Servo max angle updated and saved to NVRAM.");
        }
        else
        {
            UtilityFunctions::debugLogf("Max angle must be between 0 and 200; got %i\n", newAngle);
        }
    }

    // Load servo action angle from NVRAM
    uint16_t loadServoActionAngle()
    {
        Preferences _preferences;
        _preferences.begin(NVRAM_PERFS, false);
        uint16_t angle = _preferences.getUInt(NVRAM_PERFS_SERVO_ACTION_ANGLE_PROP, NVRAM_PERFS_SERVO_ACTION_ANGLE_DEFAULT);
        _preferences.end();
        UtilityFunctions::debugLogf("loaded servo action angle from NVRAM. %i\n", angle);

        return angle;
    }

    // Save servo action angle from NVRAM
    void saveServoActionAngle(uint16_t newAngle)
    {
        if (((newAngle >= 0) && (newAngle <= 200)))
        {
            Preferences _preferences;
            _preferences.begin(NVRAM_PERFS, false);
            _preferences.putUInt(NVRAM_PERFS_SERVO_ACTION_ANGLE_PROP, newAngle);
            _preferences.end();
            UtilityFunctions::debugLog("Servo action angle updated and saved to NVRAM.");
        }
        else
        {
            UtilityFunctions::debugLogf("Action angle must be between 0 and 200; got %i\n", newAngle);
        }
    }

    // Load servo rest angle from NVRAM
    uint16_t loadServoRestAngle()
    {
        Preferences _preferences;
        _preferences.begin(NVRAM_PERFS, false);
        uint16_t angle = _preferences.getUInt(NVRAM_PERFS_SERVO_REST_ANGLE_PROP, NVRAM_PERFS_SERVO_REST_ANGLE_DEFAULT);
        _preferences.end();
        UtilityFunctions::debugLogf("loaded servo rest angle from NVRAM. %i\n", angle);

        return angle;
    }

    // Save servo rest angle from NVRAM
    void saveServoRestAngle(uint16_t newAngle)
    {
        if (((newAngle >= 0) && (newAngle <= 200)))
        {
            Preferences _preferences;
            _preferences.begin(NVRAM_PERFS, false);
            _preferences.putUInt(NVRAM_PERFS_SERVO_REST_ANGLE_PROP, newAngle);
            _preferences.end();
            UtilityFunctions::debugLog("Servo rest angle updated and saved to NVRAM.");
        }
        else
        {
            UtilityFunctions::debugLogf("Rest angle Must be between 0 and 200; got %i\n", newAngle);
        }
    }

    // Load servo action hold from NVRAM
    uint16_t loadServoActionHold()
    {
        Preferences _preferences;
        _preferences.begin(NVRAM_PERFS, false);
        uint16_t hold = _preferences.getUInt(NVRAM_PERFS_SERVO_ACTION_HOLD_PROP, NVRAM_PERFS_SERVO_ACTION_HOLD_DEFAULT);
        _preferences.end();
        UtilityFunctions::debugLogf("loaded servo action hold time in ms from NVRAM. %i\n", hold);

        return hold;
    }

    // Save servo action hold from NVRAM
    void saveServoActionHold(uint16_t newHold)
    {
        if (((newHold >= 1) && (newHold <= 1000)))
        {
            Preferences _preferences;
            _preferences.begin(NVRAM_PERFS, false);
            _preferences.putUInt(NVRAM_PERFS_SERVO_ACTION_HOLD_PROP, newHold);
            _preferences.end();
            UtilityFunctions::debugLog("Servo action HOLD updated and saved to NVRAM.");
        }
        else
        {
            UtilityFunctions::debugLogf("Action HOLD must be between 1 and 1000 ms 1000 ms = 1 sec; got %i\n", newHold);
        }
    }
}
