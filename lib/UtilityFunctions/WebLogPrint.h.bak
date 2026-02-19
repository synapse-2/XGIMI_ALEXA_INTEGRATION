#ifndef WebLogPrint_H
#define WebLogPrint_H

#include <Arduino.h>
#include <Stream.h> // Include the Arduino Stream base class


// Logger for the debug print so that it does not go to serial io for wifi manager
class WebLogPrint : public Stream
{
public:
    // Constructor (example: takes an ID)
    WebLogPrint();

    // Mandatory Print methods
    size_t write(uint8_t data) override;


     // Mandatory Stream methods
    int available() override;
    int read() override;
    int peek() override;
    void flush() override;
    
protected:
    bool newLineSeen = true;
};

#endif
