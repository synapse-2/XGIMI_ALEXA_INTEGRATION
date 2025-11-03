// default defines for the project

#pragma once

#include <sdkconfig.h>
#include <vector>
#include <string>

/* defined in sdkconfig.defaults
#ifndef BOARD_HAS_PSRAM
#define BOARD_HAS_PSRAM
#endif

#ifndef ARDUINO_RUNNING_CORE
#define ARDUINO_RUNNING_CORE 1
#endif

#ifndef ARDUINO_EVENT_RUNNING_CORE
#define ARDUINO_EVENT_RUNNING_CORE 1
#endif

/* debugging
    typedef enum {
        WM_DEBUG_SILENT    = 0, // debug OFF but still compiled for runtime
        WM_DEBUG_ERROR     = 1, // error only
        WM_DEBUG_NOTIFY    = 2, // default stable,INFO
        WM_DEBUG_VERBOSE   = 3, // move verbose info
        WM_DEBUG_DEV       = 4, // development useful debugging info
        WM_DEBUG_MAX       = 5  // MAX extra dev auditing, var dumps etc (MAX+1 will print timing,mem and frag info)
    } wm_debuglevel_t;
 */

#define WIFIDEBUG WM_DEBUG_VERBOSE

#define WM_MDNS 1 // enable mdns for the name resolution

/*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
#define ArduinoCloudDebugLevel 4

// esp32 Ble does not support having null cars in mauf data and we have issue with the NimbleBle stack due to
// incompatability with ArduinoIoT so we created our own NewNimBle-esp32 stack

// specific to our ESP32S3 chip
#define RGBCHIP WS2812B
#define LED_BUILTINIO GPIO_NUM_48
#define RGB_DATA_ORDER GRB
#define NUMPIXELS 1
#define LED_MUTEX_WAIT_MS 1000
// #define FASTLED_RMT_MAX_CHANNELS 1
// #define FASTLED_ESP32_I2S

#define I2C_SCLK GPIO_NUM_12
#define I2C_SDA GPIO_NUM_21
#define I2C_FREQ 100000     // 100 kHz
#define I2C_SLAVE_ADDR 0x3f // Slave address for I2C

// set to not start the wifi and aIoT connection, only blutooth wwill be turned on for debugging
// #define XIGIMI_DEBUG_WIFI_OFF
#undef XIGIMI_DEBUG_WIFI_OFF

// use BLE 5.0 adv
#define XGIMI_USE_EXT_ADV

// set to log debug messgaes for led state change
// #define UTILFUNC_DEBUG_LED_ON
#undef UTILFUNC_DEBUG_LED_ON

#define AIOT_POLL_TIME 200 // milis for arduino poll any less(faster) and blutooth is not able to connecet
// #define WEBSERVER_POLL_TIME 3000 //milis for how fast to check for new to web request

#define AP_CONNECT_TIMEOUT 120 // seconds or 2 mins

#define WEB_ESP_RESTART_DELAY 2000 // mills 2 mins
#define WEB_STATUS_LOG_BUFFER 2048  /// buffer for log to be shon in the web page
#define RING_BUFF_SIZE_FOR_CMD 5*11 //each server command message is 11 bytes long

#define NVRAM_PERFS "registry"
#define NVRAM_PERFS_ADMIN_PASS_PROP "adminPass"
#define NVRAM_PERFS_ADMIN_PASS_DEFAULT "admin"
#define NVRAM_PERFS_ADMIN_NAME "admin"
#define NVRAM_PERFS_HOSTNAME_LOCAL_PROP "hostname"
#define NVRAM_PERFS_HOSTNAME_LOCAL_DEFAULT "xigimi-alexa" // only use nuber alphabets and - and dot . NO .local It is added automtically

#define NVRAM_PERFS_BLUETOOTH_NAME_PROP "bluename"
#define NVRAM_PERFS_BLUETOOTH_NAME_DEFAULT "XGIMI BH" // only use nuber alphabets and - and dot . NO .local It is added automtically

#define NVRAM_PERFS_WAKE_PACKET_PROP "WOLnum"
#define NVRAM_PERFS_WAKE_PACKET_DEFAULT 0

#define NVRAM_PERFS_SERVO_IO_PROP "ServoIO"
#define NVRAM_PERFS_SERVO_IO_DEFAULT 2

#define NVRAM_PERFS_SERVO_MAX_ANGLE_PROP "ServoMAXANGLE"
#define NVRAM_PERFS_SERVO_MAX_ANGLE_DEFAULT 180

#define NVRAM_PERFS_SERVO_ACTION_ANGLE_PROP "ServoActionANG"
#define NVRAM_PERFS_SERVO_ACTION_ANGLE_DEFAULT 110

#define NVRAM_PERFS_SERVO_ACTION_HOLD_PROP "ServoActionHOLD"
#define NVRAM_PERFS_SERVO_ACTION_HOLD_DEFAULT 100
#define NVRAM_PERFS_SERVO_ACTION_HOLD_MAX 10000

#define NVRAM_PERFS_SERVO_REST_ANGLE_PROP "ServoRestANGLE"
#define NVRAM_PERFS_SERVO_REST_ANGLE_DEFAULT 90

#define NVRAM_PERFS_SERVO_PWM_MIN_WIDTH_PROP "ServoPWMMinWid"
#define NVRAM_PERFS_SERVO_PWM_MIN_WIDTH_DEFAULT 500

#define NVRAM_PERFS_SERVO_PWM_MAX_WIDTH_PROP "ServoPWMMaxWid"
#define NVRAM_PERFS_SERVO_PWM_MAX_WIDTH_DEFAULT 2500

#define NVRAM_PERFS_SERVO_PWM_FREQ_PROP "ServoPWMFRQ"
#define NVRAM_PERFS_SERVO_PWM_FREQ_DEFAULT 50

#define ResetButton GPIO_NUM_0 // the boot button on the ESP32

// Make your device IoT–ready
// To use this board you will need a Device ID and a Secret Key, please copy and save them or download the PDF.
// Also, keep in mind that this device authentication has a lower security level compared to other Arduino devices.
// Device ID
// dc175b6d-0190-4d72-9445-980a76e7f872
// Secret Key
// #e0num8EJjkWIVZCv5Avp0RmO

#define NVRAM_PERFS_AIoT_DEVICE_ID_PROP "AoITDevID"
#define NVRAM_PERFS_AIoT_DEVICE_ID_DEFAULT  "<add>"
#define NVRAM_PERFS_AIoT_DEVICE_ID_LEN  36
#define NVRAM_PERFS_AIoT_DEVICE_SECRET_PROP "AoITDevSID"
#define NVRAM_PERFS_AIoT_DEVICE_SECRET_LEN 25
#define NVRAM_PERFS_AIoT_DEVICE_SECRET_DEFAULT  "<add>" // Secret device password

struct projectorWAKE_str
{
    std::string projName;
    std::vector<uint8_t> manufData;
};