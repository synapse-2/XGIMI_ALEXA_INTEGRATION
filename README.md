# XGIMI_ALEXA_INTEGRATION
XGIMI Projector with Alexa integration

Objective of the project is to use esp32s3 arduino to enable voice commands from Alexa for the XGIMI projector. 

# Project built uisng Visual Stuido Code and PlatformIO extension.

The code is built on PlatformIO  (Visual Studio Code Extension - https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)  
It uses the latest 'Arduino Release v3.2.1 based on ESP-IDF v5.4.2' framework with the V14 toolchain-xtensa-esp-elf for gcc++23 version support

The code used hybrid build it builds ## espidf and ## arduino libs using the folling in the platformio.ini (Note it uses PIOarduino packages NOT PlatfomIO arduino packages)

### platformio.ini
<code>
platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip (version: Arduino Release v3.3.0 based on ESP-IDF v5.5.0)
framework = arduino, espidf


</code>


### PIOArduino 
You can also build this using the native new fork of platoformio called # PIOArduino (another Visual Studio Code extension - https://marketplace.visualstudio.com/items?itemName=pioarduino.pioarduino-ide)


The code uses one ESP32S3 Arduino chips to simultaneously do Wi-Fi and Bluetooth connections.
Apprently coexistance works GREAT  https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/coexist.html

For coexistance to work wifi is run on core 1, Bluetooh on code 0. Arduino is run on code 1 and Rtos oncores 0 and 1
We should put all web checks for activity polling functions in one thread, too may tasks and performnce is impacted. Refer to the loop() function

Threre is a ring buffer between the cores for communication.

# THE LIB MUST BE COMPILED with GNU++23 
You must see "- toolchain-xtensa-esp-elf @ 14.2.0+20241119" or above in the build output
If you want to use the old platfromm code then refactor the code to NOT use "magic_enum" embedded lib (from: https://github.com/Neargye/magic_enum)

If you want to change the sdkconfig options then use the "pio run -t menuconfig" command in the PlatfromCIO Core CLI window. 


CLI for flashing: pio pkg exec -p "tool-esptoolpy" -- esptool.py --help
cli for commands avaliable:pio run --list-targets

Nifty ESp32 partion table generator: https://thelastoutpostworkshop.github.io/microcontroller_devkit/esp32partitionbuilder/

# How Bluetooh works:

https://dronebotworkshop.com/esp32-bluetooth/
https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
https://github.com/espressif/arduino-esp32/tree/master/libraries/BLE

# UID - HID Report decoder/encorder 
https://eleccelerator.com/usbdescreqparser/

# BLuetooth CoD (Class of device) encoder/decoder
https://bluetooth-pentest.narod.ru/software/bluetooth_class_of_device-service_generator.html

Bluetooth COD spec - https://www.ampedrftech.com/guides/cod_definition.pdf 


Modules used

<code>
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
</code>
NOTE: to get the Manged componets to work
1) add the componet you want in the idf_component.yml in the ROOT folder of the project
2) Run menuconfig command that will PULL the componet in the project from the web, BUILD commnd does not
3) add the command -I D:/Documents/XGIMI_ALEXA_INTEGRATION/managed_components/espressif__mdns/include/	 to the platformio.ini file updating the location on your hard drive
4) then compile and it should work
5) if you get error Directory specified in EXTRA_COMPONENT_DIRS doesn't exist: Then create the direcorty in "project root/managed_components" and run the menuconfig command first to get the system to load the managed componet specifed in the idf_component.yml in the main 'src" directory. or comment the conmmands 
FILE(GLOB_RECURSE app_sources2 ${CMAKE_SOURCE_DIR}/../managed_componets/*.h)
list (APPEND app_sources ${app_sources2})
in the cmakelists.txt in the project root/scr folder