cmake_minimum_required(VERSION 2.8)
project (ical2crontab)

# Require library:
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Lib ical:
find_package(LIBICAL REQUIRED)
include_directories(${LIBICAL_INCLUDE_DIRS})

# The version number.
set (ical2crontab_VERSION_MAJOR 0)
set (ical2crontab_VERSION_MINOR 2)

# Core application:
add_subdirectory (src)

# Testing application:
enable_testing()
add_subdirectory (tests)

