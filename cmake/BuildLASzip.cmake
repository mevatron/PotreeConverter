include(ExternalProject)

set(LASTOOLS_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/LAStools)

ExternalProject_Add(
    LAStools
    PREFIX ${LASTOOLS_PREFIX}
    GIT_REPOSITORY "https://github.com/mevatron/LAStools.git"
    GIT_TAG "master"
    INSTALL_DIR ""
    INSTALL_COMMAND ""
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${LASTOOLS_PREFIX}/install "${LASTOOLS_PREFIX}/src/LAStools/LASzip"
)

set(LASZIP_INCLUDE_DIRS ${LASTOOLS_PREFIX}/src/LAStools/LASzip/dll)
set(LASZIP_LIBRARY ${LASTOOLS_PREFIX}/src/LAStools-build/src/liblaszip.a)