#include <stdio.h>
#include <unistd.h>
#include "string.h"
#include "custom_time.h"
#include "config.h"
#include "logger.h"
#include "AccelerometerProcessor.h"

int main(int argc, char** argv)
{
    Logger::EnableFileOutput(LOGFILENAME);
    Logger::SetPriority(LogPriority::TracePriority);
    Logger::Info("Starting accelerometer reading...");

    // Create an instance of the AccelerometerProcessor class
    AccelerometerProcessor *processor = new AccelerometerProcessor();

    int opt;
    while ((opt = getopt(argc, argv, "z")) != -1)
    {
        switch (opt)
        {
        case 'z':
            processor->enableZOffset(true);
            break;
        default:
            // Handle invalid options
            break;
        }
    }

    processor->processAccelerometerData(SAMPLE_DATA_FILENAME);

    return 0;
}
