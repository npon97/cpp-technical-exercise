#include "custom_time.h"

std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* timeInfo = std::gmtime(&now);
    
    std::stringstream oss;
    oss << std::put_time(timeInfo, "%Y-%m-%dT%H:%M:%SZ");
    
    return oss.str();
}
