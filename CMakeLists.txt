cmake_minimum_required(VERSION 3.16.3)
project(accelerometer_driver)        # Defines the project name

set(CMAKE_BUILD_TYPE Debug)
set(SAMPLE_DATA_FILENAME "${PROJECT_SOURCE_DIR}/data/sample_data.txt")
set(LOGFILENAME "${PROJECT_SOURCE_DIR}/logs/log1.txt")
set(OUTPUTFILENAME "${PROJECT_SOURCE_DIR}/data/output.txt")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
configure_file(
    "${PROJECT_SOURCE_DIR}/templates/config.h.in" 
    "${PROJECT_SOURCE_DIR}/includes/config.h")

file(GLOB SOURCES "src/*.cpp" "src/*.c")

add_executable(accelerometer_driver ${SOURCES})

# Include the headers from the "includes" directory
target_include_directories(accelerometer_driver
    PRIVATE
    ${PROJECT_SOURCE_DIR}/includes
)