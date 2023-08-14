#include <fstream>
#include <string>
#include "config.h"
#include "Accelerometer.h"

class AccelerometerFile : public Accelerometer {
private:
    double x;
    double y;
    double z;
public:
    void initialize() override {
        // Perform any initialization specific to reading from file
        // For example, open the file
    }

    void readAcceleration() override {
        // Read acceleration values from the file
        std::ifstream inputFile("acceleration_data.txt");
        if (inputFile.is_open()) {
            std::string line;
            if (std::getline(inputFile, line)) {
                // Assuming the file contains comma-separated values for x, y, and z
                size_t pos = 0;
                std::string token;
                int i = 0;
                while ((pos = line.find(',')) != std::string::npos) {
                    token = line.substr(0, pos);
                    line.erase(0, pos + 1);
                    if (i == 0) {
                        x = std::stod(token);
                    } else if (i == 1) {
                        y = std::stod(token);
                    } else if (i == 2) {
                        z = std::stod(token);
                    }
                    i++;
                }
            }
            inputFile.close();
        }
    }

    double getX() const override {
        return x;
    }

    double getY() const override {
        return y;
    }

    double getZ() const override {
        return z;
    }
};
