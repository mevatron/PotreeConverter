# Find LASZIP package
# This module defines:
# LASZIP_FOUND - If false, don't try to use LASzip
# LASZIP_INCLUDE_DIR - Where to find laszip_dll.h
# LASZIP_LIBRARY - The LASzip library

find_path(LASZIP_INCLUDE_DIRS laszip_dll.h
        HINTS
        /usr/local/include
        /usr/include
        ${LASZIP_ROOT}/dll
        $ENV{LASZIP_ROOT}/dll
)

find_library(LASZIP_LIBRARY src/liblaszip.so
        PATHS
        ${LASZIP_ROOT}
        $ENV{LASZIP_ROOT}
        PATH_SUFFIXES build
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    LASZIP
    DEFAULT_MSG
    LASZIP_INCLUDE_DIRS
    LASZIP_LIBRARY
)

mark_as_advanced(LASZIP_INCLUDE_DIRS LASZIP_LIBRARY)
