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
        UtilityFunctions::finalLog((char)data);
        return 1;
    }

    if (newLineSeen)
    {
        newLineSeen = false;

        // this will generate a time stamp log
        UtilityFunctions::debugLogf("%c", data);
        return 1;
    }
    else
    {

        // no time stamp log
        UtilityFunctions::finalLog((char)data);
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