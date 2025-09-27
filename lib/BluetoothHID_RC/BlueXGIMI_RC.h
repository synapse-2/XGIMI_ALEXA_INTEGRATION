#pragma once
#include "defaults.h"
#include "UtilityFunctions.h"
#include "BlueRC.h"

#define HID_DEVICE_NAME "XGIMI BH"
#define HID_MANUFACTURER_NAME "Realtek BT"
#define HID_MODEL_NUMBER "Model Nbr 0.9"
#define HID_SERIAL_NUMBER "RTKBeeSerialNum"
#define HID_HARDWARE_REVISION "V1.0.0"
#define HID_FIRMWARE_REVISION "B981C_HG2_CY_V1.0.9"
#define HID_SOFTWARE_REVISION "B981C_HG2_CY_V1.0.9"
static uint8_t HID_SYSTEM_ID[] = {0x00, 0x01, 0x02, 0x00, 0x00, 0x03, 0x04, 0x05};
#define HID_IEE11073_CERT "RTKBeeIEEEDatalist"
#define HID_ADV_STD_ID 0
#define HID_ADV_ONDATA1_ID 1
#define HID_ADV_ONDATA2_ID 1
#define HID_ADV_ONDATA3_ID 1

#define ONBTN_HID_DEVICE_SHORT_NAME "BLuetooth 4.0 RC"

//  Cannot use the esp32 BLE lib as the advertisment takes string and it cannot have nulls in it so we use nimbleESP stack
// Manufacturer Specific (Adv scan response)
//     Length: 12
//     Type: Manufacturer Specific (0xff)
//     Company ID: Texas Instruments Inc. (0x000d)
//     Data: ffff425256312e3030
static std::vector<uint8_t> HID_AD_SCAN_MANUF_DATA = {0x0D, 0x00, 0xff, 0xff, 0x42, 0x52, 0x56, 0x31, 0x2e, 0x30, 0x30};

// Manufacturer Specific (adv std message report)
//     Length: 9
//     Type: Manufacturer Specific (0xff)
//     Company ID: Texas Instruments Inc. (0x000d)
//     Data: 383802000001
static std::vector<uint8_t> HID_AD_MANUF_DATA = {0x0D, 0x00, 0x38, 0x38, 0x02, 0x00, 0x00, 0x01};

// Manufacturer Specific
//     Length: 19
//     Type: Manufacturer Specific (0xff)
//     Company ID: MediaTek, Inc. (0x0046)
//     Data: 15,fb,45,5e,f2,86,80,ff,ff,ff,30,43,52,4b,54,4d
//     data: 01 7e 7d b6 a2 2f 1c ff ff ff 30 43 52 4b 54 4d

//static std::vector<uint8_t> HID_AD2_MANUF_DATA =  {0x46, 0x00, 0x15, 0xfb, 0x45, 0x5e, 0xf2, 0x86, 0x80, 0xff, 0xff, 0xff, 0x30, 0x43, 0x52, 0x4b, 0x54, 0x4d};
static std::vector<uint8_t> HID_AD2_MANUF_DATA =    {0x46, 0x00, 0x01 ,0x7e ,0x7d ,0xb6 ,0xa2 ,0x2f ,0x1c, 0xff, 0xff, 0xff, 0x30, 0x43, 0x52, 0x4b, 0x54, 0x4d};

static uint8_t HID_uuid_0xffd2[] = {0x1c, 0xf3, 0x02, 0xa8, 0x96, 0xdc};
static uint8_t HID_uuid_0xffd3[] = {0x01, 0xa0, 0x10, 0x28};
static uint8_t HID_uuid_0xffd4[] = {0x11, 0x00, 0x00, 0x00};
static uint8_t HID_uuid_0xfff1[] = {0x05, 0x01, 0x00, 0x07, 0x00, 0x08, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00};
static uint8_t HID_uuid_0xffe0[] = {0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0xa0, 0x10, 0x28, 0x11, 0x00, 0x00, 0x00};

/* GATT Characteristic 'PNP ID':

Source: https://www.partsnotincluded.com/understanding-the-xbox-360-wired-controllers-usb-data/

From the XGIMI remote, wireshark shows the following values:


Attribute Data, Handle: 0x0024
Handle: 0x0024 (Device Information: PnP ID)
Vendor ID Source: Bluetooth SIG (0x0001)
Vendor ID: Texas Instruments Inc. (0x000d)
Product ID: 0x3838
Version: 0x0003

PNP(Veendor ID Source, Vendor ID, Product ID, Version)
*/

#define VENDORID_SIG 0x01    // Vendor ID Source: Bluetooth SIG (0x0001)
#define VENDORID_MANUF 0x00d // Vendor ID: Texas Instruments Inc. (0x000d)
#define PRODUCTID 0x3838     // Product ID: 0x3838
#define VERSION 0x0003       // Version: 0x0003

/*

Form Xigimi wire shark snnop
Bluetooth Attribute Protocol
Opcode: Read Response (0x0b)
[Handle: 0x0027 (Human Interface Device: HID Information)]
bcdHID: 0x0000
bCountryCode: Not Supported (0x00)
Flags: 0x01, Remote Wake

*/

#define HID_COUNTRY 0x00 // bCountryCode: Not Supported (0x00)
#define HID_FLAGS 0x01   // Flags: 0x01, Remote Wake

#define KEYBOARD_REPORT_ID 0x01
#define SECOND_REPORT_ID 0x1E
#define THIRD_REPORT_ID 0x03

/* services esported by xigimi rc via wireshark

Bluetooth Attribute Protocol
    Opcode: Read By Group Type Response (0x11)
        0... .... = Authentication Signature: False
        .0.. .... = Command: False
        ..01 0001 = Method: Read By Group Type Response (0x11)
    Length: 6
    Attribute Data, Handle: 0x0001, Group End Handle: 0x0004, UUID: GATT
        Handle: 0x0001 (GATT)
            [UUID: GATT (0x1801)]
        Group End Handle: 0x0004
        UUID: GATT (0x1801)
    Attribute Data, Handle: 0x0005, Group End Handle: 0x000d, UUID: GAP
        Handle: 0x0005 (GAP)
            [UUID: GAP (0x1800)]
        Group End Handle: 0x000d
        UUID: GAP (0x1800)
    Attribute Data, Handle: 0x000e, Group End Handle: 0x0011, UUID: Battery
        Handle: 0x000e (Battery)
            [UUID: Battery (0x180f)]
        Group End Handle: 0x0011
        UUID: Battery (0x180f)
    Attribute Data, Handle: 0x0012, Group End Handle: 0x0024, UUID: Device Information
        Handle: 0x0012 (Device Information)
            [UUID: Device Information (0x180a)]
        Group End Handle: 0x0024
        UUID: Device Information (0x180a)
    Attribute Data, Handle: 0x0025, Group End Handle: 0x003f, UUID: Human Interface Device
        Handle: 0x0025 (Human Interface Device)
            [UUID: Human Interface Device (0x1812)]
        Group End Handle: 0x003f
        UUID: Human Interface Device (0x1812)
    [UUID: Primary Service (0x2800)]
    [Request in Frame: 816]

*/

/* HID data packet reort structure for xigimi remote


0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x06,        // Usage (Keyboard)
0xA1, 0x01,        // Collection (Application)

// ------ Keyboard

0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
0x09, 0x06,        //   Usage (0x06)
0xA1, 0x01,        //   Collection (Application)
0x85, 0x01,        //     Report ID (1) // this is handle 0x0031 for input report 1 handle 0x0034 for report 1 output in wireshark
0x19, 0xE0,        //     Usage Mium (0xE0)
0x29, 0xE7,        //     Usage Maximum (0xE7)
0x15, 0x00,        //     Logical Mium (0)
0x25, 0x01,        //     Logical Maximum (1)
0x75, 0x01,        //     Report Size (1)
0x95, 0x08,        //     Report Count (8)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x95, 0x01,        //     Report Count (1)
0x75, 0x08,        //     Report Size (8)
0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x95, 0x05,        //     Report Count (5)
0x75, 0x01,        //     Report Size (1)
0x05, 0x08,        //     Usage Page (LEDs)
0x19, 0x01,        //     Usage Mium (Num Lock)
0x29, 0x05,        //     Usage Maximum (Kana)
0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x95, 0x01,        //     Report Count (1)
0x75, 0x03,        //     Report Size (3)
0x91, 0x01,        //     Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x95, 0x06,        //     Report Count (6)
0x75, 0x08,        //     Report Size (8)
0x15, 0x00,        //     Logical Mium (0)
0x25, 0xFF,        //     Logical Maximum (-1)
0x05, 0x07,        //     Usage Page (Kbrd/Keypad)
0x19, 0x00,        //     Usage Mium (0x00)
0x29, 0xFF,        //     Usage Maximum (0xFF)
0x81, 0x00,        //     Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              //   End Collection
0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
0x0A, 0x00, 0xFF,  //   Usage (0xFF00)
0xA1, 0x01,        //   Collection (Application)
0x85, 0x1E,        //     Report ID (30)  // THIS report shows up as handle 0x0038 in the wire snoop log it is inpout report haandle 0x003b is report 30 output report
0x09, 0x01,        //     Usage (0x01)
0x75, 0x08,        //     Report Size (8)
0x95, 0xFF,        //     Report Count (-1)
0x16, 0x00, 0x00,  //     Logical Mium (0)
0x26, 0xFF, 0x00,  //     Logical Maximum (255)
0x19, 0x00,        //     Usage Mium (0x00)
0x29, 0xFF,        //     Usage Maximum (0xFF)
0x81, 0x00,        //     Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x95, 0x08,        //     Report Count (8)
0x75, 0x01,        //     Report Size (1)
0x05, 0x08,        //     Usage Page (LEDs)
0x19, 0x01,        //     Usage Mium (Num Lock)
0x29, 0x08,        //     Usage Maximum (Do Not Disturb)
0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //   End Collection
0x05, 0x0C,        //   Usage Page (Consumer)
0x09, 0x01,        //   Usage (Consumer Control)
0xA1, 0x01,        //   Collection (Application)
0x85, 0x03,        //     Report ID (3)  // this report shows up as handle 00x3f, inout report
0x19, 0x00,        //     Usage Mium (Unassigned)
0x2A, 0x9C, 0x02,  //     Usage Maximum (AC Distribute Vertically)
0x15, 0x00,        //     Logical Mium (0)
0x26, 0x9C, 0x02,  //     Logical Maximum (668)
0x95, 0x02,        //     Report Count (2)
0x75, 0x10,        //     Report Size (16) //this is 8 bytes used for thexgimi  fumcton buttons like on/off settings, , projector config
0x81, 0x00,        //     Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              //   End Collection
0xC0,              // End Collection

//static const uint8_t HID_REPORT_DESCRIPTOR[] = {0x5, 0x1, 0x9, 0x6, 0xa1, 0x1, 0x5, 0x7, 0x9, 0x6, 0xa1, 0x1, 0x85, 0x1, 0x19, 0xe0, 0x29, 0xe7, 0x15, 0x0, 0x25, 0x1, 0x75, 0x1, 0x95, 0x8, 0x81, 0x2, 0x95, 0x1, 0x75, 0x8, 0x81, 0x1, 0x95, 0x5, 0x75, 0x1, 0x5, 0x8, 0x19, 0x1, 0x29, 0x5, 0x91, 0x2, 0x95, 0x1, 0x75, 0x3, 0x91, 0x1, 0x95, 0x6, 0x75, 0x8, 0x15, 0x0, 0x25, 0xff, 0x5, 0x7, 0x19, 0x0, 0x29, 0xff, 0x81, 0x0, 0xc0, 0x6, 0x0, 0xff, 0xa, 0x0, 0xff, 0xa1, 0x1, 0x85, 0x1e, 0x9, 0x1, 0x75, 0x8, 0x95, 0xff, 0x16, 0x0, 0x0, 0x26, 0xff, 0x0, 0x19, 0x0, 0x29, 0xff, 0x81, 0x0, 0x95, 0x8, 0x75, 0x1, 0x5, 0x8, 0x19, 0x1, 0x29, 0x8, 0x91, 0x2, 0xc0, 0x5, 0xc, 0x9, 0x1, 0xa1, 0x1, 0x85, 0x3, 0x19, 0x0, 0x2a, 0x9c, 0x2, 0x15, 0x0, 0x26, 0x9c, 0x2, 0x95, 0x2, 0x75, 0x10, 0x81, 0x0, 0xc0, 0xc0};

*/

static const uint8_t HID_REPORT_DESCRIPTOR[] = {
    // HID report descriptor for the XGIMI remote
    USAGE_PAGE(1), 0x01, // USAGE_PAGE (Generic Desktop Ctrls)
    USAGE(1), 0x06,      // USAGE (Keyboard)
    COLLECTION(1), 0x01, // COLLECTION (Application)
                         // ------------------------------------------------- Keyboard

    USAGE_PAGE(1), 0x07,              //   USAGE_PAGE (Kbrd/Keypad)
    USAGE(1), 0x06,                   // USAGE (Keyboard)
    COLLECTION(1), 0x01,              // COLLECTION (Application)
    REPORT_ID(1), KEYBOARD_REPORT_ID, //   REPORT_ID (1)
    USAGE_MINIMUM(1), 0xE0,           //   USAGE_MIUM (0xE0)
    USAGE_MAXIMUM(1), 0xE7,           //   USAGE_MAXIMUM (0xE7)
    LOGICAL_MINIMUM(1), 0x00,         //   LOGICAL_MIUM (0)
    LOGICAL_MAXIMUM(1), 0x01,         //   Logical Maximum (1)
    REPORT_SIZE(1), 0x01,             //   REPORT_SIZE (1) one bit
    REPORT_COUNT(1), 0x08,            //   REPORT_COUNT (8) one byte representing key pressed
    HIDINPUT(1), 0x02,                //   INPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position) - one byte input
    REPORT_COUNT(1), 0x01,            //   REPORT_COUNT (1) ; 1 bit (Reserved)
    REPORT_SIZE(1), 0x08,             //   REPORT_SIZE (8) ; One BYTE for reserved cost
    HIDINPUT(1), 0x01,                //   INPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position) - one byte const
    REPORT_COUNT(1), 0x05,            //   REPORT_COUNT (5) ; 5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
    REPORT_SIZE(1), 0x01,             //   REPORT_SIZE (1)
    USAGE_PAGE(1), 0x08,              //   USAGE_PAGE (LEDs)
    USAGE_MINIMUM(1), 0x01,           //   USAGE_MIUM (0x01) ; Num Lock
    USAGE_MAXIMUM(1), 0x05,           //   USAGE_MAXIMUM (0x05) ; Kana
    HIDOUTPUT(1), 0x02,               //   OUTPUT (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)- 5 bits for LEDs output
    REPORT_COUNT(1), 0x01,            //   REPORT_COUNT (1) ; 3 bits (Padding)
    REPORT_SIZE(1), 0x03,             //   REPORT_SIZE (3); //   Padding bits
    HIDOUTPUT(1), 0x01,               //   OUTPUT (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile) - 3 buts const for outout padding
    REPORT_COUNT(1), 0x06,            //   REPORT_COUNT (6) ; 6 bits (Keys)
    REPORT_SIZE(1), 0x08,             //   REPORT_SIZE(8); // 6 buts padding to be ignored
    LOGICAL_MINIMUM(1), 0x00,         //   LOGICAL_MIUM(0)
    LOGICAL_MAXIMUM(1), 0xff,         //   LOGICAL_MAXIMUM(0xFF) ; Usage Maximum (0xFF)
    USAGE_PAGE(1), 0x07,              //   USAGE_PAGE (Kbrd/Keypad)
    USAGE_MINIMUM(1), 0x00,           //   USAGE_MIUM (0)
    USAGE_MAXIMUM(1), 0xFF,           //   USAGE_MAXIMUM (0xFF) ; Usage Maximum (0xFF)
    HIDINPUT(1), 0x00,                //   INPUT (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position) - 48 bits for keys input
    END_COLLECTION(0),                // END_COLLECTION

    USAGE_PAGE(2), 0x00, 0xFF,      //   USAGE_PAGE (Vendor Defined 0xFF00)
    USAGE(2), 0x00, 0xFF,           //   USAGE (0xFF00) // vendor defined usage
    COLLECTION(1), 0x01,            //   COLLECTION (Application)
    REPORT_ID(1), SECOND_REPORT_ID, //     REPORT_ID (30)
    USAGE(1), 0x01,                 //     USAGE (0x01)
    REPORT_SIZE(1), 0x08,           //     REPORT_SIZE (8)
    REPORT_COUNT(1), 0xFF,          //     REPORT_COUNT (-1)
    LOGICAL_MINIMUM(2), 0x00, 0x00, //     //     LOGICAL_MIUM (0)
    LOGICAL_MAXIMUM(2), 0xFF, 0x00, //     //     LOGICAL_MAXIMUM (255)
    USAGE_MINIMUM(1), 0x00,         //     USAGE_MIUM (0x00)
    USAGE_MAXIMUM(1), 0xFF,         //     USAGE_MAXIMUM (0xFF)
    HIDINPUT(1), 0x00,              //     INPUT (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position) 255 byye array
    REPORT_COUNT(1), 0x08,          //     REPORT_COUNT (8)
    REPORT_SIZE(1), 0x01,           //     REPORT_SIZE (1)
    USAGE_PAGE(1), 0x08,            //     USAGE_PAGE (LEDs)
    USAGE_MINIMUM(1), 0x01,         //     USAGE_MIUM (Num Lock)
    USAGE_MAXIMUM(1), 0x08,         //     USAGE_MAXIMUM (Do Not Disturb)
    HIDOUTPUT(1), 0x02,             //     OUTPUT (Data,Var, Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile) - one byte out
    END_COLLECTION(0),              //   END_COLLECTION

    USAGE_PAGE(1), 0x0C,            //   USAGE_PAGE (Consumer)
    USAGE(1), 0x01,                 //   USAGE (Consumer Control)
    COLLECTION(1), 0x01,            //   COLLECTION (Application)
    REPORT_ID(1), THIRD_REPORT_ID,  //     REPORT_ID (3)
    USAGE_MINIMUM(1), 0x00,         //     USAGE_MIUM (Unassigned)
    USAGE_MAXIMUM(2), 0x9C, 0x02,   //     USAGE_MAXIMUM (AC Distribute Vertically)
    LOGICAL_MINIMUM(1), 0x00,       //     LOGICAL_MIUM (0)
    LOGICAL_MAXIMUM(2), 0x9C, 0x02, //     LOGICAL_MAXIMUM (668)
    REPORT_COUNT(1), 0x02,          //     REPORT_COUNT (2)
    REPORT_SIZE(1), 0x10,           //     REPORT_SIZE (16)
    HIDINPUT(1), 0x00,              //     INPUT (Data,Array, Abs,No Wrap,Linear,Preferred State,No Null Position) 32 bits for input
    END_COLLECTION(0),              //   END_COLLECTION
    END_COLLECTION(0)               // End Collection

};

/*
A) Battery Service UUID 0x180F
1) uuid 0x2a19 - battery level, read, notify,
    descriptor (typ) for above, uuid 0x2902 - client characteristics config, readonly

B) Device information  service UUID 0x180A - Primary has
1) uuid 0x2a29 - manufacturer string , read only, value "Realteck BT" (simple one byte text )
2) uuid 0x2a24 - Model number string, read only, value "Model nbr 0.9" (simple one byte text )
3) uuid 0x2a25 - Serial number string, read only, value "RTKBeeSerialNum" (simple one byte text )
4) uuid 0x2a27 - hardware revision string, read only, value  "V1.0.0" (simple one byte text )
5) uuid 0x2a26 - firmware revision string, read only, value "B981C_HG2_CY_V1.0.9" (simple one byte text )
6) uuid 0x2a28 - software revision string, read only, value "B981C_HG2_CY_V1.0.9" (simple one byte text )
7) uuid 0x2a23 - system ID, readonly, value "0x0001020000030405"
8) uuid 0x2a2a - regulatory cretificate, read only, value "RTKBeeIEEDatalist"
9) uuid 0x2a50 - pnp id, red only,  value "0x010D0038380300" (note big endian representation)

C) Customm service uuid 0000D1FF-3C17-D293-8E48-14FE2E4DA212 primary has
1) uuid 0xa001, custom characteristic, write, no response, notify,
 descriptor (typ) for above, uuid 0x2902 - client characteristics config, readonly
2) uuid 0xa002, custom charasteristic, write no response

D) Customm service uuid 0000D0FF-3C17-D293-8E48-14FE2E4DA212 primary has
1) uuid 0xffd1, custom characteristic, write, no response,
2) uuid 0xffd2, custom characteristic, read (value 0x1cf302a896dc)
3) uuid 0xffd3, custom characteristic, read (value 0x01a01028)
4) uuid 0xffd4, custom characteristic, read (value 0x11000000)
5) uuid 0xffd5, custom characteristic, read
6) uuid 0xffd8, custom characteristic, write no response
7) uuid 0xffF1, custom characteristic, read (value 0501 0007 0008 0000 f00 f00 00)
8) uuid 0xffF2, custom characteristic, write
9) uuid 0xffE0, custom characteristic, read (value 0x00,00,00,01,01,00,00,00,00,01,a0,10,28,11,00,00,00)


E) Customm service uuid 00006287-3C17-D293-8E48-14FE2E4DA212 primary has
1) uuid 00006387-3C17-D293-8E48-14FE2E4DA212, write no response
2) uuid 00006487-3C17-D293-8E48-14FE2E4DA212, write, notify
descriptor (typ) for above, uuid 0x2902 - client characteristics config, readonly

*/

// button commands
// Key delay
static uint8_t HID_KEY_DELAY = 100; // 50 mills

// rport 30
// Back button pressed
static uint8_t HID_REP03_BACK_CMD[] = {0x24, 0x02};
// OK_btn = 237
static uint8_t HID_REP03_OK_CMD[] = {0x41, 0x00};
// Up_Btn = 238
static uint8_t HID_REP03_UP_CMD[] = {0x42, 0x0};
// Right_Btn = 239,
static uint8_t HID_REP03_RIGHT_CMD[] = {0x45, 0x0};
// Left_Btn = 240,
static uint8_t HID_REP03_LEFT_CMD[] = {0x44, 0x0};
// Down_Btn = 241,
static uint8_t HID_REP03_DOWN_CMD[] = {0x43, 0x0};
// Vol_Up_Btn = 242,
static uint8_t HID_REP03_VOL_UP_CMD[] = {0xe9, 0x0};
// Vol_Dn_Btn = 243,
static uint8_t HID_REP03_VOL_DOWN_CMD[] = {0xea, 0x0};

// Home_Btn = 246,
static uint8_t HID_REP03_HOME_CMD[] = {0x23, 0x02};
// Options_Btn = 247,
static uint8_t HID_REP03_OPTIONS_CMD[] = {0x50, 0x0};

// NUll cmd
static uint8_t HID_REP03_NULL_CMD[] = {0x00, 0x00};
// report 01
// OFF commmand 00,00,66,00,00,00,00,00
static uint8_t HID_REP01_OFF_CMD[] = {0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00};
// Projector_Setting_Btn = 244,
static uint8_t HID_REP01_PROJ_SET_CMD[] = {0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00};
// Settings_Btn = 245,
static uint8_t HID_REP01_SETTING_CMD[] = {0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00};

// NUll command
static uint8_t HID_REP01_NULL_CMD[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

class BlueXGIMI_RC : public BlueRC::BluetoothHID_RC
{
public:
    BlueXGIMI_RC(BLEServer *server);
    void sendButtonPress(BlueRC::Remote_Cmd command) override;
    bool canHandleButtonPress(BlueRC::Remote_Cmd command) override;
    void initStandardAdvData() override;
    void startServices();
    void doCMD_ON_OFF();
    void doButtons(BlueRC::Remote_Cmd command);
    virtual ~BlueXGIMI_RC();

    friend class Callback_handler_Rep_Inp_01;
    friend class Callback_handler_Rep_Out_01;
    friend class Callback_handler_Rep_Inp_30;
    friend class Callback_handler_Rep_Out_30;
    friend class Callback_handler_Rep_Inp_03;

protected:
    bool connected = false;
#ifdef XGIMI_USE_EXT_ADV

    NimBLEExtAdvertisement advertisingOnButtonDataType1;
    NimBLEExtAdvertisement advertisingOnButtonDataScanDataType1;
    NimBLEExtAdvertisement advertisingOnButtonDataType2;
    NimBLEExtAdvertisement advertisingOnButtonDataScanDataType2;
    NimBLEExtAdvertisement advertisingOnButtonDataType3;
    NimBLEExtAdvertisement advertisingOnButtonDataScanDataType3;
#else
    NimBLEAdvertisementData advertisingOnButtonDataType1;
    NimBLEAdvertisementData advertisingOnButtonDataType2;
    NimBLEAdvertisementData advertisingOnButtonDataScanDataType1And2;
#endif

    BLECharacteristic *keyboardInput_01;      // one byte input, one byte const; 48 bits for keys input for num pad
    BLECharacteristic *keyboardOutput_01;     // onbyte output, 5 bits for LEDs, 3 bits for padding
    BLECharacteristic *secondDeviceInput_30;  // 255 byye array
    BLECharacteristic *secondDeviceOutput_30; // one byte out
    BLECharacteristic *thirdDeviceInput_03;   // 32 bits for input

    /*
    B) Device information  service UUID 0x180A - Primary has
    1) uuid 0x2a29 - manufacturer string , read only, value "Realteck BT" (simple one byte text ) (ALREADY Done by BLEHIDevice class)
    2) uuid 0x2a24 - Model number string, read only, value "Model nbr 0.9" (simple one byte text )
    3) uuid 0x2a25 - Serial number string, read only, value "RTKBeeSerialNum" (simple one byte text )
    4) uuid 0x2a27 - hardware revision string, read only, value  "V1.0.0" (simple one byte text )
    5) uuid 0x2a26 - firmware revision string, read only, value "B981C_HG2_CY_V1.0.9" (simple one byte text )
    6) uuid 0x2a28 - software revision string, read only, value "B981C_HG2_CY_V1.0.9" (simple one byte text )
    7) uuid 0x2a23 - system ID, readonly, value "0x0001020000030405"
    8) uuid 0x2a2a - regulatory cretificate, read only, value "RTKBeeIEEDatalist"
    9) uuid 0x2a50 - pnp id, red only,  value "0x010D0038380300" (note big endian representation) (ALREADY Done by BLEHIDevice class)
    */
    BLECharacteristic *m_modelNumberCharacteristic;         // uuid 0x2a24
    BLECharacteristic *m_SerialNumberCharacteristic;        // uuid 0x2a25
    BLECharacteristic *m_hardwarRevisionStrCharacteristic;  // uuid 0x2a27
    BLECharacteristic *m_firmwareRevisionStrCharacteristic; // uuid 0x2a26
    BLECharacteristic *m_softwareRevisionCharacteristic;    // uuid 0x2a28
    BLECharacteristic *m_systemIDCharacteristic;            // uuid 0x2a23
    BLECharacteristic *m_IEE11073CertCharacteristic;        // uuid 0x2a2a

    /* C) Customm service uuid 0000D1FF-3C17-D293-8E48-14FE2E4DA212 primary has
    1) uuid 0xa001, custom characteristic, write, no response, notify,
     descriptor (typ) for above, uuid 0x2902 - client characteristics config, readonly
    2) uuid 0xa002, custom charasteristic, write no response
    */
    BLEService *m_CustomService1;              // uuid 0000D1FF-3C17-D293-8E48-14FE2E4DA212
    BLECharacteristic *m_0xA001Characteristic; // uuid 0xa001,
    BLECharacteristic *m_0xA002Characteristic; // uuid 0xa002

    /* D) Customm service uuid 0000D0FF-3C17-D293-8E48-14FE2E4DA212 primary has
    1) uuid 0xffd1, custom characteristic, write, no response,
    2) uuid 0xffd2, custom characteristic, read (value 0x1cf302a896dc)
    3) uuid 0xffd3, custom characteristic, read (value 0x1a01028)
    4) uuid 0xffd4, custom characteristic, read (value 0x11000000)
    5) uuid 0xffd5, custom characteristic, read
    6) uuid 0xffd8, custom characteristic, write no response
    7) uuid 0xffF1, custom characteristic, read (value 0501 0007 0008 0000 f00 f00 000)
    8) uuid 0xffF2, custom characteristic, write
    9) uuid 0xffE0, custom characteristic, read (value 0x000000 0101 0000000 01a00102811000000)
    */
    BLEService *m_CustomService2;              // uuid 0000D0FF-3C17-D293-8E48-14FE2E4DA212
    BLECharacteristic *m_0xFFD1Characteristic; // uuid 0xFFD1
    BLECharacteristic *m_0xFFD2Characteristic; // uuid 0xFFD2
    BLECharacteristic *m_0xFFD3Characteristic; // uuid 0xFFD3
    BLECharacteristic *m_0xFFD4Characteristic; // uuid 0xFFD4
    BLECharacteristic *m_0xFFD5Characteristic; // uuid 0xFFD5
    BLECharacteristic *m_0xFFD8Characteristic; // uuid 0xFFD8
    BLECharacteristic *m_0xFFF1Characteristic; // uuid 0xFFF1
    BLECharacteristic *m_0xFFF2Characteristic; // uuid 0xFFF2
    BLECharacteristic *m_0xFFE0Characteristic; // uuid 0xFFE0

    /* E) Customm service uuid 00006287-3C17-D293-8E48-14FE2E4DA212 primary has
    1) uuid 00006387-3C17-D293-8E48-14FE2E4DA212, write no response
    2) uuid 00006487-3C17-D293-8E48-14FE2E4DA212, write, nitify
        descriptor (typ) for above, uuid 0x2902 - client characteristics config, readonly
    */
    BLEService *m_CustomService3;              // uuid 00006287-3C17-D293-8E48-14FE2E4DA212
    BLECharacteristic *m_0x6287Characteristic; // uuid 00006387-3C17-D293-8E48-14FE2E4DA212
    BLECharacteristic *m_0x6387Characteristic; // uuid 00006487-3C17-D293-8E48-14FE2E4DA212

    void initOnButtonAdvData();
    void startStandardAdv();

    // BLEServerCallbacks
    virtual void onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo) override;
    virtual void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason) override;
    // BLECharacteristicCallbacks
    virtual void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override;
    virtual void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override;
    virtual void onStatus(NimBLECharacteristic *pCharacteristic, int code) override;
    virtual void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue) override;

    // report id call backs for Report#01 Input i.e bytes sent from esp32 to the projector/computer
    //  BLECharacteristicCallbacks
    virtual void ReportInput_01_onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo);
    virtual void ReportInput_01_onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo);
    virtual void ReportInput_01_onStatus(NimBLECharacteristic *pCharacteristic, int code);
    virtual void ReportInput_01_onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue);

    // report id call backs for Report#01 Output i.e bytes sent from projector/computer to theesp32
    //  BLECharacteristicCallbacks
    virtual void ReportOutput_01_onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo);
    virtual void ReportOutput_01_onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo);
    virtual void ReportOutput_01_onStatus(NimBLECharacteristic *pCharacteristic, int code);
    virtual void ReportOutput_01_onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue);

    // report id call backs for Report#30 Input i.e bytes sent from esp32 to the projector/computer
    // BLECharacteristicCallbacks
    virtual void ReportInput_30_onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo);
    virtual void ReportInput_30_onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo);
    virtual void ReportInput_30_onStatus(NimBLECharacteristic *pCharacteristic, int code);
    virtual void ReportInput_30_onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue);

    // report id call backs for Report#30 Output i.e bytes sent from projector/computer to theesp32
    //  BLECharacteristicCallbacks
    virtual void ReportOutput_30_onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo);
    virtual void ReportOutput_30_onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo);
    virtual void ReportOutput_30_onStatus(NimBLECharacteristic *pCharacteristic, int code);
    virtual void ReportOutput_30_onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue);

    // report id call backs for Report#03 Input i.e bytes sent from esp32 to the projector/computer
    // BLECharacteristicCallbacks
    virtual void ReportInput_03_onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo);
    virtual void ReportInput_03_onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo);
    virtual void ReportInput_03_onStatus(NimBLECharacteristic *pCharacteristic, int code);
    virtual void ReportInput_03_onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue);
};

class Callback_handler_Rep_Inp_01 : public BLECharacteristicCallbacks
{

private:
    BlueXGIMI_RC *parent;

public:
    Callback_handler_Rep_Inp_01(BlueXGIMI_RC *p) { parent = p; }

    // BLECharacteristicCallbacks
    void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
    {
        if (parent != NULL)
        {
            parent->ReportInput_01_onRead(pCharacteristic, connInfo);
        }
        else
        {
            UtilityFunctions::debugLogf("Input report 01 handler called onRead but parent is null %s", pCharacteristic->toString().c_str());
        }
    }
    void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
    {
        if (parent != NULL)
        {
            parent->ReportInput_01_onWrite(pCharacteristic, connInfo);
        }
        else
        {
            UtilityFunctions::debugLogf("Input report 01 handler called onWrite but parent is null %s", pCharacteristic->toString().c_str());
        }
    }
    void onStatus(NimBLECharacteristic *pCharacteristic, int code)
    {
        if (parent != NULL)
        {
            parent->ReportInput_01_onStatus(pCharacteristic, code);
        }
        else
        {
            UtilityFunctions::debugLogf("Input report 01 handler called onStaus but parent is null %s and codevalue %i", pCharacteristic->toString().c_str(), code);
        }
    }
    void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
    {
        if (parent != NULL)
        {
            parent->ReportInput_01_onSubscribe(pCharacteristic, connInfo, subValue);
        }
        else
        {
            UtilityFunctions::debugLogf("Input report 01 handler called onSubscrive but parent is null %s and subvalue %i", pCharacteristic->toString().c_str(), subValue);
        }
    }
};

class Callback_handler_Rep_Inp_30 : public BLECharacteristicCallbacks
{

private:
    BlueXGIMI_RC *parent;

public:
    Callback_handler_Rep_Inp_30(BlueXGIMI_RC *p) { parent = p; }

    // BLECharacteristicCallbacks
    void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
    {
        if (parent != NULL)
        {
            parent->ReportInput_30_onRead(pCharacteristic, connInfo);
        }
        else
        {
            UtilityFunctions::debugLogf("Input report 30 handler called onRead but parent is null %s", pCharacteristic->toString().c_str());
        }
    }
    void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
    {
        if (parent != NULL)
        {
            parent->ReportInput_30_onWrite(pCharacteristic, connInfo);
        }
        else
        {
            UtilityFunctions::debugLogf("Input report 30 handler called onWrite but parent is null %s", pCharacteristic->toString().c_str());
        }
    }
    void onStatus(NimBLECharacteristic *pCharacteristic, int code)
    {
        if (parent != NULL)
        {
            parent->ReportInput_30_onStatus(pCharacteristic, code);
        }
        else
        {
            UtilityFunctions::debugLogf("Input report 30 handler called onStaus but parent is null %s and codevalue %i", pCharacteristic->toString().c_str(), code);
        }
    }
    void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
    {
        if (parent != NULL)
        {
            parent->ReportInput_30_onSubscribe(pCharacteristic, connInfo, subValue);
        }
        else
        {
            UtilityFunctions::debugLogf("Input report 30 handler called onSubscrive but parent is null %s and subvalue %i", pCharacteristic->toString().c_str(), subValue);
        }
    }
};

class Callback_handler_Rep_Inp_03 : public BLECharacteristicCallbacks
{

private:
    BlueXGIMI_RC *parent;

public:
    Callback_handler_Rep_Inp_03(BlueXGIMI_RC *p) { parent = p; }

    // BLECharacteristicCallbacks
    void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
    {
        if (parent != NULL)
        {
            parent->ReportInput_03_onRead(pCharacteristic, connInfo);
        }
        else
        {
            UtilityFunctions::debugLogf("Input report 03 handler called onRead but parent is null %s", pCharacteristic->toString().c_str());
        }
    }
    void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
    {
        if (parent != NULL)
        {
            parent->ReportInput_03_onWrite(pCharacteristic, connInfo);
        }
        else
        {
            UtilityFunctions::debugLogf("Input report 03 handler called onWrite but parent is null %s", pCharacteristic->toString().c_str());
        }
    }
    void onStatus(NimBLECharacteristic *pCharacteristic, int code)
    {
        if (parent != NULL)
        {
            parent->ReportInput_03_onStatus(pCharacteristic, code);
        }
        else
        {
            UtilityFunctions::debugLogf("Input report 03 handler called onStaus but parent is null %s and codevalue %i", pCharacteristic->toString().c_str(), code);
        }
    }
    void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
    {
        if (parent != NULL)
        {
            parent->ReportInput_03_onSubscribe(pCharacteristic, connInfo, subValue);
        }
        else
        {
            UtilityFunctions::debugLogf("Input report 03 handler called onSubscrive but parent is null %s and subvalue %i", pCharacteristic->toString().c_str(), subValue);
        }
    }
};

class Callback_handler_Rep_Out_01 : public BLECharacteristicCallbacks
{

private:
    BlueXGIMI_RC *parent;

public:
    Callback_handler_Rep_Out_01(BlueXGIMI_RC *p) { parent = p; }

    // BLECharacteristicCallbacks
    void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
    {
        if (parent != NULL)
        {
            parent->ReportOutput_01_onRead(pCharacteristic, connInfo);
        }
        else
        {
            UtilityFunctions::debugLogf("Output report 01 handler called onRead but parent is null %s", pCharacteristic->toString().c_str());
        }
    }
    void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
    {
        if (parent != NULL)
        {
            parent->ReportOutput_01_onWrite(pCharacteristic, connInfo);
        }
        else
        {
            UtilityFunctions::debugLogf("Output report 01 handler called onWrite but parent is null %s", pCharacteristic->toString().c_str());
        }
    }
    void onStatus(NimBLECharacteristic *pCharacteristic, int code)
    {
        if (parent != NULL)
        {
            parent->ReportOutput_01_onStatus(pCharacteristic, code);
        }
        else
        {
            UtilityFunctions::debugLogf("output report 01 handler called onStaus but parent is null %s and codevalue %i", pCharacteristic->toString().c_str(), code);
        }
    }
    void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
    {
        if (parent != NULL)
        {
            parent->ReportOutput_01_onSubscribe(pCharacteristic, connInfo, subValue);
        }
        else
        {
            UtilityFunctions::debugLogf("Output report 01 handler called onSubscrive but parent is null %s and subvalue %i", pCharacteristic->toString().c_str(), subValue);
        }
    }
};

class Callback_handler_Rep_Out_30 : public BLECharacteristicCallbacks
{

private:
    BlueXGIMI_RC *parent;

public:
    Callback_handler_Rep_Out_30(BlueXGIMI_RC *p) { parent = p; }

    // BLECharacteristicCallbacks
    void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
    {
        if (parent != NULL)
        {
            parent->ReportOutput_30_onRead(pCharacteristic, connInfo);
        }
        else
        {
            UtilityFunctions::debugLogf("Output report 30 handler called onRead but parent is null %s", pCharacteristic->toString().c_str());
        }
    }
    void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
    {
        if (parent != NULL)
        {
            parent->ReportOutput_30_onWrite(pCharacteristic, connInfo);
        }
        else
        {
            UtilityFunctions::debugLogf("Output report 30 handler called onWrite but parent is null %s", pCharacteristic->toString().c_str());
        }
    }
    void onStatus(NimBLECharacteristic *pCharacteristic, int code)
    {
        if (parent != NULL)
        {
            parent->ReportOutput_30_onStatus(pCharacteristic, code);
        }
        else
        {
            UtilityFunctions::debugLogf("output report 30 handler called onStaus but parent is null %s and codevalue %i", pCharacteristic->toString().c_str(), code);
        }
    }
    void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
    {
        if (parent != NULL)
        {
            parent->ReportOutput_30_onSubscribe(pCharacteristic, connInfo, subValue);
        }
        else
        {
            UtilityFunctions::debugLogf("Output report 30 handler called onSubscrive but parent is null %s and subvalue %i", pCharacteristic->toString().c_str(), subValue);
        }
    }
};