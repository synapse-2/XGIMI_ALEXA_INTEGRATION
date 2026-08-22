# Changelog

All notable changes to this project are documented here.

## [Unreleased]

- Continue maintenance of the ESP32-S3 build and dependency configuration.
- Keep dependency ownership in PlatformIO and the project library layout so builds do not depend on local git submodule state.

## [2026-08-21]

- Moved `magic_enum` out of the old `include/magicEnum` header directory and into the PlatformIO-managed `ESP32_magic_enum` library dependency.
- Updated all affected includes in `src/main.cpp`, `lib/AcloudIOT_Decoder`, `lib/BluetoothHID_RC`, `lib/ServerDecoder`, and `lib/UtilityFunctions` from `magicEnum/...` to `magic_enum/...`.
- Fixed the build include path after the `magic_enum` module move.

## [2026-08-20]

- Moved `Time` from the checked-in `lib/Time` git submodule to the PlatformIO-managed Git dependency declared in `platformio.ini`.
- Moved `esp-nimble-cpp` from the checked-in `lib/esp-nimble-cpp` git submodule to the PlatformIO-managed Git dependency declared in `platformio.ini`.
- Removed the obsolete submodule entries from `.gitmodules` and the repository working tree.
- Fixed dependency setup so a fresh checkout can resolve these libraries through PlatformIO instead of requiring manually initialized submodules.

## Release 2.0

- Fixed BLE-dependent servo and relay actions by adding wait-on-BLE enable flags, preventing actions from running before a projector connection is available.
- Added servo TogglePush behavior for devices that need a press-and-release action instead of a fixed servo position.
- Fixed the settings page to load and save the servo wait and TogglePush options and to disable dependent controls when the servo is disabled.
- Fixed dynamic web-interface state refresh by generating reloadable `var` values in `globals.js` instead of redeclaring `const` values.
- Added OTA updates for firmware and the filesystem.
- Added synchronization between the Arduino IoT Cloud projector state and the projector BLE state.
- Added servo and web-interface toggle support.

## Release 1.0

- Added Alexa integration through Arduino IoT Cloud.
- Implemented the XGIMI Bluetooth remote on the ESP32-S3.
- Added Wi-Fi provisioning and a web-based remote control interface.
- Added optional servo and 5V relay support.
