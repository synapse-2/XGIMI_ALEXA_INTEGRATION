# XGIMI_ALEXA_INTEGRATION
XGIMI Projector with Alexa integration

Objective of the project is to use esp32s3 arduino to enable voice commands from Alexa for the XGIMI projector. 

The code uses two ESP32-S3 Arduino chips to simultaneously do Wi-Fi and Bluetooth connections. It may be possible with one (not sure), but I wanted to keep it simple.

The Arduinos communicate with each other via the I2C bus, with one acting as the master and the other as the slave.

Modules used


Name                        Flash           RAM             Temp
 ESP32-S3-WROOM-1-N8        8MB(QuadSPI)    -               –40~85
 ESP32-S3-WROOM-1-N16       16MB(QuadSPI)   -               –40~85
 ESP32-S3-WROOM-1-H4        4MB(QuadSPI)    -               –40~105
 ESP32-S3-WROOM-1-N4R2      4MB(QuadSPI)    2MB(QuadSPI)    –40~85
 ESP32-S3-WROOM-1-N8R2      8MB(QuadSPI)    2MB(QuadSPI)    –40~85
 ESP32-S3-WROOM-1-N16R2     16MB(QuadSPI)   2MB(QuadSPI)    –40~85
 ESP32-S3-WROOM-1-N4R8      4MB(QuadSPI)    8MB(OctalSPI)   –40~65
 ESP32-S3-WROOM-1-N8R8      8MB(QuadSPI)    8MB(OctalSPI)   –40~65
 ESP32-S3-WROOM-1-N16R8     16MB(QuadSPI)   8MB(OctalSPI)   –40~65
 ESP32-S3-WROOM-1-N16R16VA8 16MB(QuadSPI)   16MB(OctalSPI)   –40~65
