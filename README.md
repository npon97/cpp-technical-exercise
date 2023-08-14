# Camera Matics C++ Programming Test

## Set up

1. Install CMake and Make: Ensure you have CMake and Make installed on your local system
 and any other build essentials. CMake is an open-source, cross-platform tool that uses
 compiler and platform-independent configuration files to generate native build tool
 files specific to your compiler and platform code.visualstudio.com.
2. Create a directories: `mdkir build logs data`. You may have to delete the cached build
 if it already exists.
3. Change into the build directory.
4. Run `cmake .. && make -j4` to configure and build using g++.
5. The executable is stored in the `build` directory.
6. Run that executable with `./accelerometer_driver`

## File info
* `data/output.txt` stores the X, Y and Z accelerometer values.
* `logs/log1.txt` stores the last output of the standard output stream.
* `includes/logger.h` is a thread safe logger with multiple log levels if this project
 needs to be scaled.
