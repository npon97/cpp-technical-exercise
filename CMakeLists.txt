cmake_minimum_required(VERSION 3.16.3)
project(accelerometer_driver)        # Defines the project name

set(FILENAME "data/sample_data.txt")
configure_file(templates/config.h.in config.h)

include_directories(includes)
set(CMAKE_CXX_STANDARD 17)           # Sets the CMAKE_CXX_STANDARD variable to the value of 17
file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_executable(accelerometer_driver ${SOURCES})