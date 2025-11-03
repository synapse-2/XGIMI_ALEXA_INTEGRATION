# XGIMI ALEXA INTEGRATION over Wifi and Bluetooth

XGIMI Projector with Alexa integration

Objective of the project is to use esp32s3 arduino to enable voice commands from Alexa for the XGIMI projector. 

Project uses Wifi and Arduino cloud IoT to connect to Alexa using the Arduino Alexa skill (you can can also connect to google)
Once the Alexa connection is made the project has a full re-implemenation of the Xgimi bluetooth remote (model: https://us.xgimi.com/products/xgimi-remote-controller-horizon-serieshttps://us.xgimi.com/products/xgimi-remote-controller-horizon-series)
You can pair the ESP23S3 as a remote on the Xgimi projector, alternatively if you want to to be just like the factory remote change the bluetooth device name to "Xgimi RC" in the settting web UI (and it will auto pair with the Xgimi projector)
Once paired you can use Alexa voice command to turn on off, volume change etc.
There is an option to add a servo and have it move on on-off command if you want to have it push another button of some sort
There is also built in remote web gui to send commands from iphone or anotehr web browser etc.

The code was tested with Xgimi horizon-ultra

There you have it.  Alexa integration via cloning of the Bluetooth factory remote. All on the the ESP32S3 single chip supporintg Wifi and BLE5.0

How to set up 
You need 
1. Esp32S3 dev kit for example : https://www.amazon.com/Hosyond-Development-Dual-Mode-Compatible-ESP32-S3-WROOM-1/dp/B0F5QCK6X5/
2. An Aurdino Cloud account at: https://app.arduino.cc/
3. Create a device (name does not matter) in arduino cloud using the "+device button at : https://app.arduino.cc/devices
    a. Select "comptible device" option
    b. Select "ESP32" option
    c. Select "Arduino Nano ESP32" from the drop down
    d. hit continue
    e. give a name
    f. NOTE the DEVICE ID and SECRET KEY. This needs to  be loaded in the settings. 
    g. SECRET KEY is only displpayed once so you will have to re do this step if you forget it
4. Create a thing (name does not matter) in the arduino cloud using the "+thing" button at : https://app.arduino.cc/things 
5. Add a variable to the thing called "projector" (name does matter) if you want it something different change it in the thingProperrtites.h and related reefrences in main.cpp 
6. The "projector" variable should be of 
    a. type "Television"
    b. variable permission : read & write
    c. Variable update policy: On change
7. Associate the thing with the devide you created in step 3 (on the things page)
8. Select the smart home integration as Alexa (on the things page)
9. Go to Alexa app on the phone etc and add the alexa skill for arduino and the device you want refer to this guide: https://docs.arduino.cc/arduino-cloud/guides/alexa/
10. Compile and flash the binary to the ESP32S3
11. Make sure you also flash the file system from the PIO command "Upload filesystem image"
12. On initial start the ESP will be in AP mode, it will create a wifi network called "ESP_XXXXXX" 
13. join to it
14. You can get the ESP32's ip from the serial port else open the browser for host name "Xigimi-alexa.local" (mDNS is enabled by defaut so this should work) if you have conencted to the the ESP32's wifi network
15. Scan to find your wifi
16. Only b and n networks at 2.5GHz ESP32S3 supports 
17. Identify the network, provide the passskey and hit save
18. The device should reboot and connect. The device will go from red led to a long Green blink. The long green series of blinks means it has conencted to the wifi
19. Open the browser to "Xigimi-alexa.local" you should see the Remote HTML UI
20. Go to Settings page. The default user and password both are "admin"
21. In the Settings update the device ID and Secret ID for ATOT things you created
22. Each time you update the device will restart 
23. Once updated the device should connect to the Arduino IOT and you should be able to do commands like "Alexa tun projector off" ; "Alexa set projector volume to 10" etc.
24. Pressing the rest button three times within 3 secs will completey wipe the NV ram and get back to the WIF provisioning step 12


## Project built uisng Visual Stuido Code and PIOArduino extension.

The code is built on PIOArduino (Visual Studio Code Extension - https://marketplace.visualstudio.com/items?itemName=pioarduino.pioarduino-ide)  
It uses the latest 'Arduino Release v3.2.1 based on ESP-IDF v5.4.2' framework with the V14 toolchain-xtensa-esp-elf for gcc++23 version support

The code used hybrid build it builds ## espidf and ## arduino libs using the folling in the platformio.ini (Note it uses PIOarduino packages NOT PlatfomIO arduino packages)

## Clangd 

The code also can have clandg enabled: off by default

you need to run the command "pio run --target compiledb" to generate the inclulde files path for the clangd to work
clangd extention to be loaded from: https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd

1. You have to create a .clangd file in the root of your project folder with the lines, change the abslute path "d:\" to what ever is in your env 

<code>
CompileFlags:
  Remove: [-fno-tree-switch-conversion, -fstrict-volatile-bitfields, -mdisable-hardware-atomics, -mlongcalls, -std=gnu++11, -std=gnu++2b, -std=gnu++2a ]
  Add: [-std=gnu++23, -I D:/Documents/XGIMI_ALEXA_INTEGRATION/managed_components/espressif__mdns/include/, -I D:/Documents/XGIMI_ALEXA_INTEGRATION/managed_components/espressif__servo/include/, -I include ]
</code>

2. Also add the following lines in the settings.jason
<code>
"C_Cpp.intelliSenseEngine": "disabled",
</code>

### platformio.ini

<code>
platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip (version: Arduino Release v3.3.0 based on ESP-IDF v5.5.0)
framework = arduino, espidf

</code>

The code uses one ESP32S3 Arduino chips to simultaneously do Wi-Fi and Bluetooth connections.
Apprently coexistance works GREAT  https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/coexist.html

For coexistance to work:
* Wifi and all apps logic is run on core 1, Bluetooth on core 0
* Arduino is run on core 1 
* Rtos on cores 0 and 1
* We should put all web checks for activity polling functions in one thread, too may tasks and performnce is impacted. Refer to the loop() function

### Threre is a ring buffer between the cores for communication.

## THE LIB MUST BE COMPILED with GNU++23 

You must see "- toolchain-xtensa-esp-elf @ 14.2.0+20241119" or above in the build output
If you want to use the old platfrom code then refactor the code to NOT use "magic_enum" embedded lib (from: https://github.com/Neargye/magic_enum)

If you want to change the sdkconfig options then use the "pio run -t menuconfig" command in the PlatfromCIO Core CLI window. 

CLI for flashing: pio pkg exec -p "tool-esptoolpy" -- esptool.py --help
cli for commands avaliable:pio run --list-targets

### Nifty ESp32 partion table generator: 

https://thelastoutpostworkshop.github.io/microcontroller_devkit/esp32partitionbuilder/

## How Bluetooh works:

https://dronebotworkshop.com/esp32-bluetooth/
https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
https://github.com/espressif/arduino-esp32/tree/master/libraries/BLE

### UID - HID Report decoder/encorder 

https://eleccelerator.com/usbdescreqparser/

### BLuetooth CoD (Class of device) encoder/decoder

https://bluetooth-pentest.narod.ru/software/bluetooth_class_of_device-service_generator.html

Bluetooth COD spec - https://www.ampedrftech.com/guides/cod_definition.pdf 

## Modules used

<table>
<tr> <td>Name</td><td>                       Flash</td><td>           RAM</td><td>             Temp</td><td>        RAM Speed</td> </tr>
<tr> <td>ESP32-S3-WROOM-1-N8</td><td>        8MB(QuadSPI)</td><td>    -  </td><td>             –40~85</td><td> </td></tr>
<tr> <td>ESP32-S3-WROOM-1-N16</td><td>       16MB(QuadSPI)</td><td>   -  </td><td>             –40~85</td><td> </td></tr>
<tr> <td>ESP32-S3-WROOM-1-H4</td><td>        4MB(QuadSPI)</td><td>    -  </td><td>             –40~105</td><td> </td></tr>
<tr> <td>ESP32-S3-WROOM-1-N4R2</td><td>      4MB(QuadSPI)</td><td>    2MB(QuadSPI)</td><td>    –40~85</td><td> </td></tr>
<tr> <td>ESP32-S3-WROOM-1-N8R2</td><td>      8MB(QuadSPI)</td><td>    2MB(QuadSPI)</td><td>    –40~85</td><td> </td></tr>
<tr> <td>ESP32-S3-WROOM-1-N16R2</td><td>     16MB(QuadSPI)</td><td>   2MB(QuadSPI)</td><td>    –40~85</td><td> </td></tr>
<tr> <td>ESP32-S3-WROOM-1-N4R8</td><td>      4MB(QuadSPI)</td><td>    8MB(OctalSPI)</td><td>   –40~65</td><td> </td></tr>
<tr> <td>ESP32-S3-WROOM-1-N8R8</td><td>      8MB(QuadSPI)</td><td>    8MB(OctalSPI)</td><td>   –40~65</td><td> </td></tr>
<tr> <td>ESP32-S3-WROOM-1-N16R8</td><td>     16MB(QuadSPI)</td><td>   8MB(OctalSPI)</td><td>   –40~65</td><td>      80Mhz</td></tr>
<tr> <td>ESP32-S3-WROOM-1-N16R16VA8</td><td> 16MB(QuadSPI)</td><td>   16MB(OctalSPI)</td><td>   –40~65</td><td> </td></tr>
</table>

NOTE: to get the Managed components from espressif to work
1. Add the component you want in the idf_component.yml in the ROOT folder of the project
2. Run menuconfig command that will PULL the component in the project from the web, BUILD commnd does not
3. add the command -I D:/Documents/XGIMI_ALEXA_INTEGRATION/managed_components/espressif__mdns/include/	 to the platformio.ini file updating the location on your hard drive
4. Then compile and it should work
5. If you get error Directory specified in EXTRA_COMPONENT_DIRS doesn't exist: Then create the direcorty in "project root/managed_components" and run the menuconfig command first to get the system to load the managed componet specifed in the idf_component.yml in the main 'src" directory. or comment the conmmands 
FILE(GLOB_RECURSE app_sources2 ${CMAKE_SOURCE_DIR}/../managed_componets/*.h)
list (APPEND app_sources ${app_sources2})
in the cmakelists.txt in the project root/scr folder

# Usable PINS for ESP32S3

## Multiplexing

Pin multiplexing is one of the most important and useful features of the ESP32.

Basically, on the ESP32 we can reassign most of the GPIO functions to act on any pin, with little impact on performance.

Therefore, the values we will see are the default values, but generally, you can change them to others, as it suits you.

## The RTC

The RTC (Real Time Clock) plays a fundamental role during Sleep modes. The RTC consists of the following parts:

RTC controller (including timers and IO peripherals)
RTC memory (fast and slow)
Ultra Low Power (ULP) coprocessor
The ESP32 has 8 kB of SRAM in the RTC part, called fast RTC memory. The data stored here is not erased during deep sleep mode.

In addition, there are another 8kB of SRAM called slow memory, used for the ULP processor.

## Digital Pins

GPIO Pins
The ESP32-S3 has up to 45 GPIO pins that can be assigned to different functions through programming.

Most of these digital GPIOs can be configured with internal pull-up or pull-down resistors.

Configuration (Strapping) Pins of the ESP32-S3
The ESP32 chip has the following configuration (strapping) pins:

* GPIO 0
* GPIO 45
* GPIO 46
These pins are involved in the configuration during startup. So avoid using them in your project.

## Input-Only GPIO Pins

These pins cannot be used as outputs, but can be used as digital or analog inputs, or for other purposes.

GPIO 46
Additionally, unlike the other GPIO pins, they lack internal pull-up and pull-down resistors.

## Interrupt Pins of the ESP32-S3

All GPIO pins can be configured as interrupts.

## RTC GPIO Pins of the ESP32-S3

Some GPIOs are connected to the low-power RTC subsystem and are known as RTC GPIOs

* RTC_GPIO0 - GPIO0
* RTC_GPIO1 - GPIO1
* RTC_GPIO2 - GPIO2
* RTC_GPIO3 - GPIO3
* RTC_GPIO4 - GPIO4
* RTC_GPIO5 - GPIO5
* RTC_GPIO6 - GPIO6
* RTC_GPIO7 - GPIO7
* RTC_GPIO8 - GPIO8
* RTC_GPIO9 - GPIO9
* RTC_GPIO10 - GPIO10
* RTC_GPIO11 - GPIO11
* RTC_GPIO12 - GPIO12
* RTC_GPIO13 - GPIO13
* RTC_GPIO14 - GPIO14
* RTC_GPIO15 - GPIO15
* RTC_GPIO16 - GPIO16
* RTC_GPIO17 - GPIO17
* RTC_GPIO18 - GPIO18
* RTC_GPIO19 - GPIO19
* RTC_GPIO20 - GPIO20
* RTC_GPIO21 - GPIO21

These pins are used to wake the ESP32-S3 from deep low-power mode when the ultra-low power (ULP) coprocessor is running.

## Touch Pins of the ESP32-S3

The ESP32-S3 has 14 capacitive touch GPIO pins.

* TOUCH1 - GPIO 1
* TOUCH2 - GPIO 2
* TOUCH3 - GPIO 3
* TOUCH4 - GPIO 4
* TOUCH5 - GPIO 5
* TOUCH6 - GPIO 6
* TOUCH7 - GPIO 7
* TOUCH8 - GPIO 8
* TOUCH9 - GPIO 9
* TOUCH10 - GPIO 10
* TOUCH11 - GPIO 11
* TOUCH12 - GPIO 12
* TOUCH13 - GPIO 13
* TOUCH14 - GPIO 14

When a capacitive load (such as a human finger) is near the GPIO pin, the ESP32-S3 detects the change in capacitance.

## Enable (EN) Pin

Enable (EN) is the pin that controls the 3V3 regulator. It is configured with a pull-up resistor, so it connects to ground to disable the 3.3V regulator. For example, to reset the ESP32.

## Analog Pins

PWM Pins of the ESP32-S3
The ESP32-S3 board has 8 PWM channels (all GPIO pins except the input-only pins) controlled by a PWM controller.

The PWM output can be used to control motors and digital LEDs.

## ADC Pins of the ESP32-S3

The ESP32 integrates two ADCs and supports measurements on 20 channels

* ADC1_CH0 - GPIO 1
* ADC1_CH1 - GPIO 2
* ADC1_CH2 - GPIO 3
* ADC1_CH3 - GPIO 4
* ADC1_CH4 - GPIO 5
* ADC1_CH5 - GPIO 6
* ADC1_CH6 - GPIO 7
* ADC1_CH7 - GPIO 8
* ADC1_CH8 - GPIO 9
* ADC1_CH9 - GPIO 10
* ADC2_CH0 - GPIO 11
* ADC2_CH1 - GPIO 12
* ADC2_CH2 - GPIO 13
* ADC2_CH3 - GPIO 14
* ADC2_CH4 - GPIO 15
* ADC2_CH5 - GPIO 16
* ADC2_CH6 - GPIO 17
* ADC2_CH7 - GPIO 18
* ADC2_CH8 - GPIO 19
* ADC2_CH9 - GPIO 20
The ADCs are 12 bits, so we have 4096 (2^12) discrete levels, which translates to a precision of 0.8mV.

## DAC Pins of the ESP32-S3

The ESP32-S3 does not include a DAC.

## Communication Pins

UART Pins of the ESP32-S3
The ESP32-S3 development board has three UART interfaces: UART0, UART1, and UART2, supporting asynchronous communication (RS232 and RS485) and IrDA up to 5 Mbps.

The UART0 pins are connected to the USB-to-serial converter and are used for programming and debugging. It is not recommended to use the UART0 pins.

On the other hand, the UART is reserved for the integrated FLASH memory chip. The UART1 pins are reserved for the integrated flash memory chip.

So the best option is to use UART2 to connect UART devices.

## I2C Pins of the ESP32-S3

The ESP32-S3 has a single I2C bus that allows connecting up to 112 sensors and peripherals. The SDA and SCL pins are default assigned to the following pins.

* SDA - GPIO 8
* SCL - GPIO 9
However, it is possible to use any GPIO pin to implement the I2C protocol using the command wire.begin(SDA, SCL).

## SPI Pins of the ESP32-S3

The ESP32 has four interfaces SPI0, SPI1, SPI2, and SPI3. SPI0 is used to connect with the FLASH memory, and SPI1 for PSRAM (if the board has it). So it’s best to focus on SPI2 and SPI3.

<table>
<tr><td>SPI</td><td>	MOSI</td><td>	MISO</td><td>	CLK</td><td>	CS</td></tr>
<tr><td>SPI2</td><td>	GPIO 35</td><td>	GPIO 37</td><td>	GPIO 36</td><td>	GPIO 39</td></tr>
<tr><td>SPI3</td><td>	GPIO 11</td><td>	GPIO 13</td><td>	GPIO 12</td><td>	GPIO 10</td></tr>
</table>

## SPI FLASH

These pins are connected to the integrated SPI flash memory in the ESP32-S3 chip. Do not use these pins in your projects.

* GPIO 26
* GPIO 27
* GPIO 28
* GPIO 29
* GPIO 30
* GPIO 31
* GPIO 32

<table>
<th>GPIO</th><th>FUNCTIONS</th><th>YOU CAN USE IT</th>
<tr><td>0</td><td>RTC_GPIO0, GPIO0</td><td>⚠️ STRAPPING<br>Pulled-up</td></tr>
<tr><td>1</td><td>RTC_GPIO1, GPIO1, TOUCH1, ADC1_CH0</td><td>✔️</td></tr>
<tr><td>2</td><td>RTC_GPIO2, GPIO2, TOUCH2, ADC1_CH1</td><td>✔️</td></tr><tr>
<td>3</td><td>RTC_GPIO3, GPIO3, TOUCH3, ADC1_CH2</td><td>⚠️ STRAPPING<br>Floating</td></tr>
<tr><td>4</td><td>RTC_GPIO4, GPIO4, TOUCH4, ADC1_CH3</td><td>✔️</td></tr>
<tr><td>5</td><td>RTC_GPIO5, GPIO5, TOUCH5, ADC1_CH4</td><td>✔️</td></tr>
<tr><td>6</td><td>RTC_GPIO6, GPIO6, TOUCH6, ADC1_CH5</td><td>✔️</td></tr>
<tr><td>7</td><td>RTC_GPIO7, GPIO7, TOUCH7, ADC1_CH6</td><td>✔️</td></tr>
<tr><td>8</td><td>RTC_GPIO8, GPIO8, TOUCH8, ADC1_CH7</td><td>✔️</td></tr>
<tr><td>9</td><td>RTC_GPIO9, GPIO9, TOUCH9, ADC1_CH8, FSPIHD</td><td>✔️</td></tr>
<tr><td>10</td><td>RTC_GPIO10, GPIO10, TOUCH10, ADC1_CH9, FSPICS0, FSPIIO4</td><td>❌</td></tr>
<tr><td>11</td><td>RTC_GPIO11, GPIO11, TOUCH11, ADC2_CH0, FSPID, FSPIIO5</td><td>❌</td></tr>
<tr><td>12</td><td>RTC_GPIO12, GPIO12, TOUCH12, ADC2_CH1, FSPICLK, FSPIIO6</td><td>❌</td></tr>
<tr><td>13</td><td>RTC_GPIO13, GPIO13, TOUCH13, ADC2_CH2, FSPIQ, FSPIIO7</td><td>❌</td></tr>
<tr><td>14</td><td>RTC_GPIO14, GPIO14, TOUCH14, ADC2_CH3, FSPIWP, FSPIDQS</td><td>❌</td></tr>
<tr><td>15</td><td>RTC_GPIO15, GPIO15, U0RTS, ADC2_CH4, XTAL_32K_P</td><td>✔️</td></tr>
<tr><td>16</td><td>RTC_GPIO16, GPIO16, U0CTS, ADC2_CH5, XTAL_32K_N</td><td>✔️</td></tr>
<tr><td>17</td><td>RTC_GPIO17, GPIO17, U1TXD, ADC2_CH6, DAC_1</td><td>✔️</td></tr>
<tr><td>18</td><td>RTC_GPIO18, GPIO18, U1RXD, ADC2_CH7, DAC_2, CLK_OUT3</td><td>✔️</td></tr>
<tr><td>19</td><td>RTC_GPIO19, GPIO19, U1RTS, ADC2_CH8, CLK_OUT2, USB_D-</td><td>⚠️ USB_D-</td></tr>
<tr><td>20</td><td>RTC_GPIO20, GPIO20, U1CTS, ADC2_CH9, CLK_OUT1, USB_D+</td><td>⚠️ USB_D+</td></tr>
<tr><td>21</td><td>RTC_GPIO21, GPIO21</td><td>✔️</td></tr>
<tr><td>22</td><td>GPIO22</td><td>❌</td></tr>
<tr><td>23</td><td>GPIO23</td><td>❌</td></tr>
<tr><td>24</td><td>GPIO24</td><td>❌</td></tr>
<tr><td>25</td><td>GPIO25</td><td>❌</td></tr>
<tr><td>26</td><td>SPICS1, GPIO26</td><td>❌ FLASH SPI</td></tr>
<tr><td>27</td><td>SPIHD, GPIO27</td><td>❌ FLASH SPI</td></tr>
<tr><td>28</td><td>SPIWP, GPIO28</td><td>❌ FLASH SPI</td></tr>
<tr><td>29</td><td>SPICS0, GPIO29</td><td>❌ FLASH SPI</td></tr>
<tr><td>30</td><td>SPICLK, GPIO30</td><td>❌ FLASH SPI</td></tr>
<tr><td>31</td><td>SPIQ, GPIO31</td><td>❌ FLASH SPI</td></tr>
<tr><td>32</td><td>SPID, GPIO32</td><td>❌ FLASH SPI</td></tr>
<tr><td>33</td><td>SPIIO4, GPIO33, FSPIHD</td><td>🟢 PSRAM</td></tr>
<tr><td>34</td><td>SPIIO5, GPIO34, FSPICS0</td><td>🟢 PSRAM</td></tr>
<tr><td>35</td><td>SPIIO6, GPIO35, FSPID</td><td>🟢 PSRAM</td></tr>
<tr><td>36</td><td>SPIIO7, GPIO36, FSPICLK</td><td>🟢 PSRAM</td></tr>
<tr><td>37</td><td>SPIDQS, GPIO37, FSPIQ</td><td>🟢 PSRAM</td></tr>
<tr><td>38</td><td>GPIO38, FSPIWP</td><td>❌</td></tr>
<tr><td>39</td><td>MTCK, GPIO39, CLK_OUT3</td><td>🟢 DEBUG JTAG</td></tr>
<tr><td>40</td><td>MTDO, GPIO40, CLK_OUT2</td><td>🟢 DEBUG JTAG</td></tr>
<tr><td>41</td><td>MTDI, GPIO41, CLK_OUT1</td><td>🟢 DEBUG JTAG</td></tr>
<tr><td>42</td><td>MTMS, GPIO42</td><td>🟢 DEBUG JTAG</td></tr>
<tr><td>43</td><td>U0TXD, GPIO43, CLK_OUT1</td><td>⚠️ U0TX</td></tr>
<tr><td>44</td><td>U0RXD, GPIO44, CLK_OUT2</td><td>⚠️ U0RX</td></tr>
<tr><td>45</td><td>GPIO45</td><td>❌ STRAPPING<br>Pulled-down</td></tr><
tr><td>46</td><td>GPIO46</td><td>❌ STRAPPING<br>Pulled-down</td></tr>
<tr><td>EN</td><td>CHIP_PU, Reset</td><td>❌</td></tr>
</table>
