cmake_minimum_required(VERSION 3.10)
project(BiliBili_DownloadFileProcessor VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Executable output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Generate executable
add_executable(BiliBili_DownloadFileProcessor ${SOURCE_FILES})