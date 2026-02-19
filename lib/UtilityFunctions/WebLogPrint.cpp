/**
 * @file WebLogPrint.cpp
 * @brief Brief summary of this file.
 *
 * Detailed description of the file and its purpose.
 */

#include <Print.h>
#include "UtilityFunctions.h"
#include "WebLogPrint.h"


WebLogPrint::WebLogPrint()
{
}

size_t WebLogPrint::write(uint8_t data)
{

    if (data == '\n')
    {
        newLineSeen = true;

        // no timestamp log
/**
 * @brief Brief description of finalLog.
 *
 * @param param Describe this parameter.
 * @param false Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
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
        UtilityFunctions::finalLog((char)data,false);
        return 1;
    }

    if (newLineSeen)
    {
        newLineSeen = false;

        // this will generate a time stamp log
/**
 * @brief Brief description of finalLog.
 *
 * @param param Describe this parameter.
 * @param true Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
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
        UtilityFunctions::finalLog((char)data,true);
        return 1;
    }
    else
    {

        // no time stamp log
/**
 * @brief Brief description of finalLog.
 *
 * @param param Describe this parameter.
 * @param false Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
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
        UtilityFunctions::finalLog((char)data  ,false);
        return 1;
    }
}

int WebLogPrint::available()
{
    return 0;
}

int WebLogPrint::read()
{

    return -1;
}

int WebLogPrint::peek()
{
    return -1;
}

void WebLogPrint::flush()
{
    // Clear outgoing buffers if implementing hardware communication
}
