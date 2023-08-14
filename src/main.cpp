#include <stdio.h>
#include <fstream>
#include "string.h"

int main() {
    std::ifstream file("sample_data.txt");
    char buffer[16];
    while (file.read(buffer, 16)) {
        // Process buffer
    }
}

