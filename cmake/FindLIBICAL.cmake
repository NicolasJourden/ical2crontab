# Find Libical
#
#  LIBICAL_FOUND - system has Libical with the minimum version needed
#  LIBICAL_INCLUDE_DIRS - the Libical include directories
#  LIBICAL_LIBRARIES - The libraries needed to use Libical
#  LIBICAL_VERSION = The value of ICAL_VERSION defined in ical.h

# we cannot search for ical.h as we might find the wrong one without the libical suffix
FIND_PATH(LIBICAL_INCLUDE_DIR icalcomponent.h
  PATH_SUFFIXES include/libical
  PATHS
  /usr/local
  /usr
)

FIND_LIBRARY(LIBICAL_LIBRARY_RELEASE
  NAMES ical libical libical-static
)
if (LIBICAL_LIBRARY_RELEASE)
	list(APPEND LIBICAL_LIBRARIES optimized ${LIBICAL_LIBRARY_RELEASE})
endif()

FIND_LIBRARY(LIBICAL_LIBRARY_DEBUG
  NAMES ical_d libical_d libical-static_d
)
if (LIBICAL_LIBRARY_DEBUG)
	list(APPEND LIBICAL_LIBRARIES debug ${LIBICAL_LIBRARY_DEBUG})
else()
	if (UNIX)
		list(APPEND LIBICAL_LIBRARIES debug ${LIBICAL_LIBRARY_RELEASE})
	endif()
endif()

FIND_LIBRARY(LIBICALSS_LIBRARY_RELEASE
  NAMES icalss libicalss libicalss-static
)
if (LIBICALSS_LIBRARY_RELEASE)
	list(APPEND LIBICAL_LIBRARIES optimized ${LIBICALSS_LIBRARY_RELEASE})
endif()

FIND_LIBRARY(LIBICALSS_LIBRARY_DEBUG
  NAMES icalss_d libicalss_d libicalss-static_d
)
if (LIBICALSS_LIBRARY_DEBUG)
	list(APPEND LIBICAL_LIBRARIES debug ${LIBICALSS_LIBRARY_DEBUG})
else()
	if (UNIX)
		list(APPEND LIBICAL_LIBRARIES debug ${LIBICALSS_LIBRARY_RELEASE})
	endif()
endif()


INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBICAL DEFAULT_MSG LIBICAL_LIBRARIES LIBICAL_INCLUDE_DIR)
MARK_AS_ADVANCED(LIBICAL_LIBRARIES LIBICAL_INCLUDE_DIR)

