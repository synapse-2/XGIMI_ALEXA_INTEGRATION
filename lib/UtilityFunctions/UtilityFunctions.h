/**
 * @file UtilityFunctions.h
 * @brief Brief summary of this file.
 *
 * Detailed description of the file and its purpose.
 */

// filepath: d:\Documents\XGIMI_ALEXA_INTEGRATION\src\UtilityFunctions.h
#ifndef UtilityFunctions_H
#define UtilityFunctions_H

#include "defaults.h"
#include <FastLED.h>
#include <Print.h>

namespace UtilityFunctions
{


/**
 * @brief Brief description of delay.
 *
 * @param waitMills Describe this parameter.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void delay(long waitMills);
/**
 * @brief Brief description of waitTillInitComplete.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void waitTillInitComplete();

/**
 * @brief Brief description of UtilityFunctionsInit.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void UtilityFunctionsInit();
/**
 * @brief Brief description of isMaster.
 *
 * @return bool Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  bool isMaster();

/**
 * @brief Brief description of findI2cOtherAddress.
 *
 * @return int Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  int findI2cOtherAddress();
/**
 * @brief Brief description of isResetPressed.
 *
 * @return bool Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  bool isResetPressed();
/**
 * @brief Brief description of numTimesResetPressed.
 *
 * @return int Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  int numTimesResetPressed();
/**
 * @brief Brief description of unpressRest.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void unpressRest();
/**
 * @brief Brief description of resetMills.
 *
 * @return unsigned long Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  unsigned long resetMills();

/**
 * @brief Brief description of ledRed.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledRed();
/**
 * @brief Brief description of ledGreen.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledGreen();
/**
 * @brief Brief description of ledYellow.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledYellow();
/**
 * @brief Brief description of ledBlue.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledBlue();
/**
 * @brief Brief description of ledWhite.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledWhite();
/**
 * @brief Brief description of ledBrown.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledBrown();
/**
 * @brief Brief description of ledStop.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledStop();

/**
 * @brief Brief description of ledBlinkGreen.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledBlinkGreen();
/**
 * @brief Brief description of ledBlinkBlue.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledBlinkBlue();
/**
 * @brief Brief description of ledBlinkRed.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledBlinkRed();
/**
 * @brief Brief description of ledBlinkYellow.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledBlinkYellow();
/**
 * @brief Brief description of ledBlinkGreenLong.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledBlinkGreenLong();
/**
 * @brief Brief description of ledBlinkRedLong.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ledBlinkRedLong();

/**
 * @brief Brief description of debugLog.
 *
 * @param message Describe this parameter.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void debugLog(String message);
/**
 * @brief Brief description of debugLog.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void debugLog();
/**
 * @brief Brief description of debugLogf.
 *
 * @param param Describe this parameter.
 * @param ... Describe this parameter.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void debugLogf(const char *format, ...);
/**
 * @brief Brief description of finalLog.
 *
 * @param param Describe this parameter.
 * @param true Describe this parameter.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void finalLog(char *temp, bool timestamp = true);
/**
 * @brief Brief description of finalLog.
 *
 * @param temp Describe this parameter.
 * @param true Describe this parameter.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void finalLog(char temp, bool timestamp = true);

  // used for arduino cloud and wifi manager log
/**
 * @brief Brief description of webLogPrintf.
 *
 * @param param Describe this parameter.
 * @param args Describe this parameter.
 * @return int Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  int webLogPrintf(const char *format, va_list args);

/**
 * @brief Brief description of chipInfo.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String chipInfo();
/**
 * @brief Brief description of taskInfo.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String taskInfo();
/**
 * @brief Brief description of ledCInfo.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String ledCInfo();
/**
 * @brief Brief description of partitionInfo.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String partitionInfo();
/**
 * @brief Brief description of webLog.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String webLog();


  //get RTC time 
/**
 * @brief Brief description of getDateTimeUTC.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String getDateTimeUTC();

  // Load the mDNS hostname from NVRAM
/**
 * @brief Brief description of loadLocalHostname.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String loadLocalHostname();

  // save
/**
 * @brief Brief description of saveLocalHostname.
 *
 * @param newHostname Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveLocalHostname(String newHostname);

  // Load wol packet num  from NVRAM
/**
 * @brief Brief description of loadWakePacketNum.
 *
 * @return int Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  int loadWakePacketNum();

  // Save  wol packet num  from NVRAM
/**
 * @brief Brief description of saveWakePacketNum.
 *
 * @param newWolNum Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveWakePacketNum(int newWolNum);

  // Load BlueTooth Name from NVRAM
/**
 * @brief Brief description of loadBlueToothName.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String loadBlueToothName();

  // Save BlueTooth Name to NVRAM
/**
 * @brief Brief description of saveBlueToothName.
 *
 * @param newBlueName Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveBlueToothName(String newBlueName);

  /// get the actual ad bytes
/**
 * @brief Brief description of getHID_AD2_MANUF_DATA.
 *
 * @return std::vector<uint8_t> Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  std::vector<uint8_t> getHID_AD2_MANUF_DATA();

  // Load servoIO pin  num  from NVRAM
/**
 * @brief Brief description of loadServoIOPin.
 *
 * @return int Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  int loadServoIOPin();

  // Save  ServoIO pin  num   from NVRAM
/**
 * @brief Brief description of saveServoIOPin.
 *
 * @param newPinIO Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveServoIOPin(int newPinIO);

  // Load servo PWM MIN Width from NVRAM
/**
 * @brief Brief description of loadServoPWMMinWidth.
 *
 * @return uint16_t Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  uint16_t loadServoPWMMinWidth();

  // Save servo PWN MIN Width from NVRAM
/**
 * @brief Brief description of saveServoPWMMinWidth.
 *
 * @param newMinWidth Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveServoPWMMinWidth(uint16_t newMinWidth);

  // Load servo PWM MAX Width from NVRAM
/**
 * @brief Brief description of loadServoPWMMaxWidth.
 *
 * @return uint16_t Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  uint16_t loadServoPWMMaxWidth();

  // Save servo PWN MIN Width from NVRAM
/**
 * @brief Brief description of saveServoPWMMaxWidth.
 *
 * @param newMaxWidth Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveServoPWMMaxWidth(uint16_t newMaxWidth);

  // Load servo PWM Freq from NVRAM
/**
 * @brief Brief description of loadServoPWMFreq.
 *
 * @return uint32_t Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  uint32_t loadServoPWMFreq();

  // Save servo PWN MIN Width from NVRAM
/**
 * @brief Brief description of saveServoPWMFreq.
 *
 * @param newFreq Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveServoPWMFreq(uint32_t newFreq);

  // Load servo max angle from NVRAM
/**
 * @brief Brief description of loadServoMaxAngle.
 *
 * @return uint16_t Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  uint16_t loadServoMaxAngle();

  // Save servo max angle  from NVRAM
/**
 * @brief Brief description of saveServoMaxAngle.
 *
 * @param newAngle Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveServoMaxAngle(uint16_t newAngle);

  // Load servo action angle from NVRAM
/**
 * @brief Brief description of loadServoActionAngle.
 *
 * @return uint16_t Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  uint16_t loadServoActionAngle();

  // Save servo action angle from NVRAM
/**
 * @brief Brief description of saveServoActionAngle.
 *
 * @param newAngle Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveServoActionAngle(uint16_t newAngle);

  // Load servo rest angle from NVRAM
/**
 * @brief Brief description of loadServoRestAngle.
 *
 * @return uint16_t Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  uint16_t loadServoRestAngle();

  // Save servo rest angle from NVRAM
/**
 * @brief Brief description of saveServoRestAngle.
 *
 * @param newAngle Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveServoRestAngle(uint16_t newAngle);

  // Load servo action hold from NVRAM
/**
 * @brief Brief description of loadServoActionHold.
 *
 * @return uint16_t Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  uint16_t loadServoActionHold();

  // Save servo action hold from NVRAM
/**
 * @brief Brief description of saveServoActionHold.
 *
 * @param newhold Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveServoActionHold(uint16_t newhold);

/**
 * @brief Brief description of loadAIoTDeviceID.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String loadAIoTDeviceID();
/**
 * @brief Brief description of saveAIoTDeviceID.
 *
 * @param newDeviceID Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveAIoTDeviceID(String newDeviceID);

/**
 * @brief Brief description of loadAIoTDeviceSECRET.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String loadAIoTDeviceSECRET();
/**
 * @brief Brief description of saveAIoTDeviceSECRET.
 *
 * @param newSecret Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveAIoTDeviceSECRET(String newSecret);

  // Load relayIO pin  num  from NVRAM
/**
 * @brief Brief description of loadRelayIOPin.
 *
 * @return int Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  int loadRelayIOPin();

  // Save RelayIO pin  num   from NVRAM
/**
 * @brief Brief description of saveRelayIOPin.
 *
 * @param newPinIO Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveRelayIOPin(int newPinIO);

  // Load relay action hold from NVRAM
/**
 * @brief Brief description of loadRelayActionHold.
 *
 * @return uint16_t Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  uint16_t loadRelayActionHold();

  // Save servo action hold from NVRAM
/**
 * @brief Brief description of saveRelayActionHold.
 *
 * @param newhold Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveRelayActionHold(uint16_t newhold);

  // Load servo enable flag from NVRAM
/**
 * @brief Brief description of loadServoEnableFlag.
 *
 * @return bool Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  bool loadServoEnableFlag();

  // Save servo action hold from NVRAM
/**
 * @brief Brief description of saveServoEnableFlag.
 *
 * @param flag Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveServoEnableFlag(bool flag);

  // Load relay enable flag from NVRAM
/**
 * @brief Brief description of loadRelayEnableFlag.
 *
 * @return bool Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  bool loadRelayEnableFlag();

  // Save relay action hold from NVRAM
/**
 * @brief Brief description of saveRelayEnableFlag.
 *
 * @param flag Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveRelayEnableFlag(bool flag);


  // Load sync AaIOt with BLE device pair flag
/**
 * @brief Brief description of loadSyncAIoTWithBLEDevice.
 *
 * @return bool Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  bool loadSyncAIoTWithBLEDevice();

  // Save sync AaIOt with BLE device pair flag
/**
 * @brief Brief description of saveSyncAIoTWithBLEDevice.
 *
 * @param flag Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String saveSyncAIoTWithBLEDevice(bool flag);

/**
 * @brief Brief description of getBuildTimeVersion.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String getBuildTimeVersion();

  // get the current cloud projector value
/**
 * @brief Brief description of getAIoTProjectorVarValue.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String getAIoTProjectorVarValue();

  // save the old log in nvram and restart
/**
 * @brief Brief description of ESP32Restart.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  void ESP32Restart();

  // get the log from last boot
/**
 * @brief Brief description of getPreBootWebLog.
 *
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  String getPreBootWebLog();

}

#endif
