include(ExternalProject)

set(LASTOOLS_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/LAStools)

ExternalProject_Add(
    LAStools
    PREFIX ${LASTOOLS_PREFIX}
    GIT_REPOSITORY "https://github.com/mevatron/LAStools.git"
    GIT_TAG "master"
    INSTALL_DIR ${LASTOOLS_PREFIX}/install
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${LASTOOLS_PREFIX}/install "${LASTOOLS_PREFIX}/src/LAStools/LASzip"
)

set(LASZIP_INCLUDE_DIRS ${LASTOOLS_PREFIX}/install/include)
set(LASZIP_LIBRARY ${LASTOOLS_PREFIX}/install/lib/liblaszip.a)