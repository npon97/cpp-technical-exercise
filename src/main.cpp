#include <stdio.h>
#include "string.h"
#include "config.h"
#include "logger.h"

int main() {
    char buffer[16];
    Logger::EnableFileOutput(LOGFILENAME);

    Logger::Info("Hello");
}

