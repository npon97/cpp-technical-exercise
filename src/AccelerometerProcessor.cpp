#include "AccelerometerProcessor.h"

AccelerometerProcessor::AccelerometerProcessor()
{
    // Initialize member variables in the constructor
    validMessagesCount = 0;
    invalidChecksumCount = 0;
    maxX = std::numeric_limits<float>::min();
    maxY = std::numeric_limits<float>::min();
    maxZ = std::numeric_limits<float>::min();
    alertsCount = 0;

    enableZAxisOffset = false; // Default: Z-axis offset is disabled
}

AccelerometerProcessor::~AccelerometerProcessor() {};

void AccelerometerProcessor::enableZOffset(bool enable)
{
    enableZAxisOffset = enable;
}

std::vector<std::string>
AccelerometerProcessor::extractAccelerometerMessages(const std::string &chunk)
{
    std::vector<std::string> messages;
    std::stringstream ss(chunk);

    std::string line;
    while (std::getline(ss, line))
    {
        if (line.find("ACCEL:") != std::string::npos)
        {
            messages.push_back(line);
        }
    }

    return messages;
}

// Method for parsing and processing accelerometer data messages
void AccelerometerProcessor::processAccelerometerData(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        Logger::Error("File could not be opened at: %s", filename);
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        // Ignore lines that don't match the expected format
        if (line.size() != 15 || line[0] != '#' || line[line.size() - 1] != '\n')
            continue;

        // Remove the '\n' and '\r' character at the end of the line
        line.pop_back();
        line.pop_back();
        lines.push_back(line);
    }

    file.close();

    for (const std::string &message : lines)
    {
        std::string characters = message.substr(2, 4); // Extract characters from index 2 to 5 (inclusive)
        int sum = 0;
        for (char c : characters)
        {
            sum += static_cast<int>(c);
        }
        Logger::Trace("sum = %d", sum);
    }

    // Additional processing or actions after processing all accelerometer messages
    // ...
}

// Method for writing statistics to output
void AccelerometerProcessor::writeStatistics()
{
    // Output the collected statistics to console or a separate file
    Logger::Info("Total valid messages: %d", validMessagesCount);
    Logger::Info("Total messages with invalid checksum: %d", invalidChecksumCount);
    Logger::Info("Maximum X-axis value: %2f", maxX);
    Logger::Info("Maximum Y-axis value: %2f", maxY);
    Logger::Info("Maximum Z-axis value: %2f", maxZ);
    Logger::Info("Total alerts encountered: %d", alertsCount);
}