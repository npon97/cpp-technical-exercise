cmake_minimum_required(VERSION 3.16.3)
project(accelerometer_driver)        # Defines the project name
set(CMAKE_BUILD_TYPE Debug)
set(SAMPLE_DATA_FILENAME "${PROJECT_SOURCE_DIR}/data/sample_data.txt")
set(LOGFILENAME "${PROJECT_SOURCE_DIR}/logs/log1.txt")
configure_file(
    "${PROJECT_SOURCE_DIR}/templates/config.h.in" 
    "${PROJECT_SOURCE_DIR}/includes/config.h")

include_directories(includes)
set(CMAKE_CXX_STANDARD 17)           # Sets the CMAKE_CXX_STANDARD variable to the value of 17
file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_executable(accelerometer_driver ${SOURCES})