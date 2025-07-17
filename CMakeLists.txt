cmake_minimum_required(VERSION 3.10)
project(http_server C)

# Use C11 standard
set(CMAKE_C_STANDARD 11)

# Source and include directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

# Collect all source files
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.c")

# Print found sources
message(STATUS "Sources: ${SOURCES}")

# Define executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include headers
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

# Add compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

# Link pthread if using threads
target_link_libraries(${PROJECT_NAME} pthread)
