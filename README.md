# XGIMI_ALEXA_INTEGRATION
XGIMI Projector with Alexa integration

Objective of the project is to use esp32s3 arduino to enable voice commands from alex for teh XGIMI projector. 

The code uses two esp32s3 arduinio chips to do a simultaneously do wi fi and bt connect. It may be done with one (not sure) but i wanted to keep it simple

The ardunio talk to each other via i2c bus one being a master and another slave

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
