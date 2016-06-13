cmake_minimum_required (VERSION 2.6)
project (ical2crontab)

# The version number.
set (ical2crontab_VERSION_MAJOR 1)
set (ical2crontab_VERSION_MINOR 0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

#configure_file ("${PROJECT_SOURCE_DIR}/config.h.in" "${PROJECT_BINARY_DIR}/config.h")

# Need CURL:
#find_package(CURL REQUIRED)
#include_directories(${CURL_INCLUDE_DIRS})

# Need libical:
find_package(LIBICAL REQUIRED)
include_directories(${LIBICAL_INCLUDE_DIRS})
 
# libcurl + google to get the calendar:
#add_executable(getGoogle google.c)
#target_link_libraries(getGoogle ${CURL_LIBRARIES})

# libical to parse the calendar:
add_executable(ical2crontab ical.c)
target_link_libraries(ical2crontab ${LIBICAL_LIBRARIES})
