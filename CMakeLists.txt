cmake_minimum_required(VERSION 3.10)
project(MyCLILibrary C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra")

# Include directories
include_directories(include)

# Source files
set(SOURCES
    app/main.c
    src/cli.c
    src/log.c
)

# Create an executable
add_executable(my_cli_app ${SOURCES})
