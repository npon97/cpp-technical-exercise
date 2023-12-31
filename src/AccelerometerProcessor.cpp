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
    Logger::Warn("Z offset enabled");
}

bool AccelerometerProcessor::isNumeric(const std::string &str)
{
    for (char c : str)
    {
        if (!isdigit(c) && !(c >= 'A' && c <= 'F') && !(c >= 'a' && c <= 'f'))
        {
            return false;
        }
    }
    return true;
}

int AccelerometerProcessor::convertToInt(const std::string &str)
{
    return std::stoi(str, nullptr, 16);
}

float AccelerometerProcessor::convertToGs(int value)
{
    // Convert the 12-bit integer representation to Gs
    float gsValue = static_cast<float>(value) / 0x40;
    return gsValue;
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
    outputFile << getCurrentTimestamp() << std::endl;
    int yThresholdCount = 0;
    for (const std::string &message : lines)
    {
        uint8_t sum = calculateChecksum(message);
        std::string str = message.substr(12, 2);
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
            invalidChecksumCount++;
            continue;
        }

        // At this point the message is a valid accelerometer message
        validMessagesCount++;

        std::string x = message.substr(3, 3);
        std::string y = message.substr(6, 3);
        std::string z = message.substr(9, 3);

        // First parse the string, then cast to int, then interpret 2s complement in
        // convert to decimal then convert to gravity units (g).
        float xAxisValue, yAxisValue, zAxisValue;
        xAxisValue = convertToGs(convertToDecimal(convertToInt(x)));
        yAxisValue = convertToGs(convertToDecimal(convertToInt(y)));
        zAxisValue = convertToGs(convertToDecimal(convertToInt(z)));

        if (yAxisValue > Y_THRESHOLD || yAxisValue < -1 * Y_THRESHOLD) // Checking absolute
            yThresholdCount++;
        else
            yThresholdCount = 0;

        if (enableZAxisOffset)
            zAxisValue += 1;

        // Check maximum values
        if (xAxisValue > maxX)
            maxX = xAxisValue;
        if (yAxisValue > maxY)
            maxY = yAxisValue;
        if (zAxisValue > maxZ)
            maxZ = zAxisValue;

        // Print to output file
        if (outputFile.is_open())
        {
            outputFile << "X=" << std::fixed << std::setprecision(2) << xAxisValue << ", ";
            outputFile << "Y=" << std::fixed << std::setprecision(2) << yAxisValue << ", ";
            outputFile << "Z=" << std::fixed << std::setprecision(2) << zAxisValue;
            if (yThresholdCount == 3){
                outputFile << "[ALERT]";
                alertsCount++;
            }
            outputFile << std::endl;
        }
        else
        {
            Logger::Critical("Unable to open output file.");
        }

        continue;
    }

    writeStatistics();
    outputFile.close();

    return;
}

int AccelerometerProcessor::convertToDecimal(int value)
{
    // Check if the value is negative
    bool isNegative = (value & 0x800) != 0;

    // If negative, perform 2's complement
    if (isNegative)
    {
        value = value - 1;
        value = ~value;
        value = value & 0xFFF;
        value = -value;
    }

    return value;
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