#pragma once
#define LED_BUILTINIO GPIO_NUM_48
#define NUMPIXELS 1

#define I2C_SCLK GPIO_NUM_12
#define I2C_SDA GPIO_NUM_21
#define I2C_FREQ 100000 // 100 kHz
#define I2C_SLAVE_ADDR 0x3f // Slave address for I2C

#define HOHSTNAME_Local "ALEXA_ESP32.local"   
#define AP_CONNECT_TIMEOUT 600 // 10 minutes

#define ResetButton GPIO_NUM_0 // the boot button on the ESP32

// Make your device IoT–ready
// To use this board you will need a Device ID and a Secret Key, please copy and save them or download the PDF. 
// Also, keep in mind that this device authentication has a lower security level compared to other Arduino devices.
// Device ID
// dc175b6d-0190-4d72-9445-980a76e7f872
// Secret Key
// #e0num8EJjkWIVZCv5Avp0RmO