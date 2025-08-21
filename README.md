# XGIMI_ALEXA_INTEGRATION
XGIMI Projector with Alexa integration

Objective of the project is to use esp32s3 arduino to enable voice commands from Alexa for the XGIMI projector. 

Project built uisng Visual Stuido Code and PlatformIO extension.

The code is built on PlatformIO  (Visual Studio Code Extension - https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)  
It uses the latest 'Arduino Release v3.2.1 based on ESP-IDF v5.4.2' framework with the V14 toolchain-xtensa-esp-elf for gcc++23 version support

The code used hybrid build it builds exprisif and arduino libs using the folling in the platformio.ini (Note it uses PIOarduino packages NOT PlatfomIO arduino packages)
platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip (version: Arduino Release v3.3.0 based on ESP-IDF v5.5.0)
framework = arduino, espidf


You can also build this using the native new fork of platoformio called PIOArduino (another Visual Studio Code extension - https://marketplace.visualstudio.com/items?itemName=pioarduino.pioarduino-ide)


The code uses one ESP32S3 Arduino chips to simultaneously do Wi-Fi and Bluetooth connections.
Apprently coextance work but barely in my testing read more at https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/coexist.html

For coexisstance to work wifi is run on core 1, Bluetooh on code 0. Arduino is run on code 1 and Rtos oncores 0 and 1
The tas
k loops at most can wait for 3000 millis when the radios lose conenction or ESP32S3 core panic's

Threre is a ring buffer between the cores for communication.

THE LIB MUST BE COMPILED with GNU++23 
You must see "- toolchain-xtensa-esp-elf @ 14.2.0+20241119" or above in the build output
If you want to use the old platfromm code then refactor the code to NOT use "magic_enum" embedded lib (from: https://github.com/Neargye/magic_enum)

If you want to change the sdkconfig options then use the "pio run -t menuconfig" command in the PlatfromCIO Core CLI window. 

You can also usue two Esp32 chips with I2C bus on two pins
The Arduinos communicate with each other via the I2C bus, with one acting as the master and the other as the slave.
Pins for I2C defined by in the defaults.h (NOT TESTED)
#define I2C_SCLK GPIO_NUM_12
#define I2C_SDA GPIO_NUM_21

you connect each pin 12 on together on ESP32 and pin 21 on each esp32 together. 
You fash the same code on the ESP32. They will figure out who the master and slave is determistically every time.

CLI for flashing: pio pkg exec -p "tool-esptoolpy" -- esptool.py --help
cli for commands avaliable:pio run --list-targets

Nifty ESp32 partion table generator: https://thelastoutpostworkshop.github.io/microcontroller_devkit/esp32partitionbuilder/

How Blutooh works:

https://dronebotworkshop.com/esp32-bluetooth/
https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
https://github.com/espressif/arduino-esp32/tree/master/libraries/BLE




Modules used


Name                        Flash           RAM             Temp        RAM Speed
 ESP32-S3-WROOM-1-N8        8MB(QuadSPI)    -               –40~85
 ESP32-S3-WROOM-1-N16       16MB(QuadSPI)   -               –40~85
 ESP32-S3-WROOM-1-H4        4MB(QuadSPI)    -               –40~105
 ESP32-S3-WROOM-1-N4R2      4MB(QuadSPI)    2MB(QuadSPI)    –40~85
 ESP32-S3-WROOM-1-N8R2      8MB(QuadSPI)    2MB(QuadSPI)    –40~85
 ESP32-S3-WROOM-1-N16R2     16MB(QuadSPI)   2MB(QuadSPI)    –40~85
 ESP32-S3-WROOM-1-N4R8      4MB(QuadSPI)    8MB(OctalSPI)   –40~65
 ESP32-S3-WROOM-1-N8R8      8MB(QuadSPI)    8MB(OctalSPI)   –40~65
 ESP32-S3-WROOM-1-N16R8     16MB(QuadSPI)   8MB(OctalSPI)   –40~65      80Mhz
 ESP32-S3-WROOM-1-N16R16VA8 16MB(QuadSPI)   16MB(OctalSPI)   –40~65
