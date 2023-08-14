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

AccelerometerProcessor::~AccelerometerProcessor(){};

void AccelerometerProcessor::enableZOffset(bool enable)
{
    enableZAxisOffset = enable;
}

bool AccelerometerProcessor::isNumeric(const std::string& str) {
    for (char c : str) {
        if (!isdigit(c) && !(c >= 'A' && c <= 'F') && !(c >= 'a' && c <= 'f')) {
            return false;
        }
    }
    return true;
}


int AccelerometerProcessor::convertToInt(const std::string &str)
{
    return std::stoi(str, nullptr, 16);
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
        if (line.size() != 15 ||
            line[0] != '#' || line[1] != 'G' || line[2] != '4' ||
            line[line.size() - 1] != '\r')
            continue;

        // Remove the '\r' character at the end of the line
        line.pop_back();
        lines.push_back(line);
    }

    file.close();

    std::ofstream outputFile(OUTPUTFILENAME);
    for (const std::string &message : lines)
    {
        uint8_t sum = calculateChecksum(message);
        std::string str = message.substr(12, 12);
        int convertedInt;
        if (str.length() == 2 && isNumeric(str))
        {
            convertedInt = convertToInt(str);
        }
        else
        {
            Logger::Debug("Invalid input: %s", str);
            continue;
        }

        Logger::Trace("Message: %s", message.c_str());
        Logger::Trace("sum = %X, expected checksum = %X", sum, convertedInt);
        if (sum != convertedInt)
        {
            Logger::Error("Checksum unmatched");
            continue;
        }

        std::string x = message.substr(3, 4);
        std::string y = message.substr(5, 6);
        std::string z = message.substr(7, 8);

        // if (outputFile.is_open()) {
        //     outputFile << getCurrentTimestamp() << std::endl;
        //     outputFile << "X=" << std::fixed << std::setprecision(2) << xAxisValue << ", ";
        //     outputFile << "Y=" << std::fixed << std::setprecision(2) << yAxisValue << ", ";
        //     outputFile << "Z=" << std::fixed << std::setprecision(2) << zAxisValue << std::endl;
        //     outputFile.close();
        // } else {
        //     Logger::Critical("Unable to open output file.");
        // }

        continue;
    }

    // Additional processing or actions after processing all accelerometer messages
    // ...

    return;
}

uint8_t AccelerometerProcessor::calculateChecksum(const std::string &message)
{
    std::string characters = message.substr(0, 12); // Extract characters from index 2 to 5 (inclusive)

    uint8_t sum = 0;
    for (char c : characters)
    {
        sum += static_cast<uint8_t>(c);
    }

    return sum;
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