//start of file 
const headingTxt = 'XGIMI BH';
const wifiStrength = "wifi-none";
const bluetoothStatus = "bt-connected";
const WifiRssi = -75;
const projectorList = [
    ["XGIMI Horizon Ultra", 0],
    ["XGIMI Elfin", 1],
    ["XGIMI Z6X", 2],
    ["XGIMI Horizon Pro 4K", 3],
    ["XGIMI H3", 4],
    ["XGIMI H3S-a", 5],
    ["XGIMI H3S-b", 6],
    ["XGIMI H5", 7],
    ["XGIMI H1S", 8],
    ["XGIMI Z4Air", 9],
    ["XGIMI Horizon", 10]
];
const projSelected = 0;
const devHostname = "xigimi-alexa";
var statusTxt = 'This is ESP32-S3 chip with 2 CPU core(s) Clok Feq 240 MHz, WiFi/BLE, silicon revision v0.2, 16 MB external flash ' /
    'Minimum free heap size: 8144 KB ' /
    'Total heap: 265 KB ' /
    'Free heap: 84 KB ' /
    'Total Flash Chip Mode: FM_QIO ' /
    'Fash Chip Speed 80 MHz ' /
    'Total PSRAM: 8192 KB ' /
    'Free PSRAM: 8110 ' /
    'SDK version: 5.5.0 ' /
    'Core version: 3.3.0 ' /
    'Sketch Size: 1900992 ' /
    'Sketch Free Space: 7405568 ' /
    'Device MAC Address: E0B2:157D:7850:0000:E0B2:157D ' /
    '[WIFI] WIFI_INFO DEBUG ' /
    '[WIFI] MODE: STA ' /
    '[WIFI] SAVED: YES ' /
    '[WIFI] SSID: deepspace-xib ' /
    '[WIFI] CHANNEL: 9 ' /
    '[WIFI] RSSI: -72 ' /
    '[WIFI] PASS: qwertyuiop ' /
    '[WIFI] HOSTNAME: xigimi-alexa ' /
    'to RESET to defaults and rease NVRAM press boot key 3 times within 3 seconds';