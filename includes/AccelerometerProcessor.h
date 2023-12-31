#ifndef ACCELEROMETER_PROCESSOR_H_
#define ACCELEROMETER_PROCESSOR_H_

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "logger.h"
#include "config.h"
#include "custom_time.h"

class AccelerometerProcessor
{
private:
    const float Y_THRESHOLD = 0.2; 
    
    unsigned int validMessagesCount;   // Total number of valid accelerometer messages parsed
    unsigned int invalidChecksumCount; // Total number of accelerometer messages with an invalid checksum
    float maxX;               // Maximum value of the X-axis
    float maxY;               // Maximum value of the Y-axis
    float maxZ;               // Maximum value of the Z-axis
    unsigned int alertsCount;          // Total number of alerts encountered
    bool enableZAxisOffset;   // Flag to enable Z-axis offset

    int convertToInt(const std::string& str);
    float convertToGs(int value);
    bool isNumeric(const std::string& str);
    int convertToDecimal(int value);

public:
    AccelerometerProcessor();
    ~AccelerometerProcessor();

    void enableZOffset(bool enable);

    // Method for parsing and processing accelerometer data messages
    void processAccelerometerData(const std::string &filename);

    uint8_t calculateChecksum(const std::string& message);

    // Method for writing statistics to output
    void writeStatistics();
};

#endif /* ACCELEROMETER_PROCESSOR_H_ */