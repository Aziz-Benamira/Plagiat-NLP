cmake_minimum_required(VERSION 3.10)

# Project name and version
project(PlagiatNLP VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set build type to Debug if not already set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp" "main.cpp")

# Add executable
add_executable(main ${SOURCES})