cmake_minimum_required(VERSION 3.10)
project(http_server C)

# Set C standard and warnings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# Source and header directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# Collect all .c files from src/
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.c")

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include header files from include/
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
