#ifndef ACCELEROMETER_PROCESSOR_H_
#define ACCELEROMETER_PROCESSOR_H_

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "logger.h"

class AccelerometerProcessor
{
private:
    int validMessagesCount;   // Total number of valid accelerometer messages parsed
    int invalidChecksumCount; // Total number of accelerometer messages with an invalid checksum
    float maxX;               // Maximum value of the X-axis
    float maxY;               // Maximum value of the Y-axis
    float maxZ;               // Maximum value of the Z-axis
    int alertsCount;          // Total number of alerts encountered

    bool enableZAxisOffset; // Flag to enable Z-axis offset

    std::vector<std::string> extractAccelerometerMessages(const std::string &chunk);

public:
    AccelerometerProcessor();
    ~AccelerometerProcessor();

    void enableZOffset(bool enable);

    // Method for parsing and processing accelerometer data messages
    void processAccelerometerData(const std::string &filename);

    // Method for writing statistics to output
    void writeStatistics();
};

#endif /* ACCELEROMETER_PROCESSOR_H_ */