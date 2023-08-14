#include <stdio.h>
#include "string.h"

#include "config.h"
#include "logger.h"
#include "AccelerometerProcessor.h"

int main() {
    Logger::EnableFileOutput(LOGFILENAME);
    Logger::SetPriority(LogPriority::TracePriority);
    Logger::Info("Starting accelerometer reading...");

    // Create an instance of the AccelerometerProcessor class
    AccelerometerProcessor processor;
    processor.processAccelerometerData(SAMPLE_DATA_FILENAME);


    return 0;
}
