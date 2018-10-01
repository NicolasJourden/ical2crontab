cmake_minimum_required(VERSION 2.8)
project (ical2crontab)
enable_testing()
option(BUILD_TESTING "Build tests" ON)
set (ical2crontab_VERSION_MAJOR 0)
set (ical2crontab_VERSION_MINOR 2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(LIBICAL REQUIRED)
include_directories(${LIBICAL_INCLUDE_DIRS})

# Core application:
add_subdirectory (src)

# Testing application:
if (BUILD_TESTING)
  add_subdirectory (tests)
endif (BUILD_TESTING)

