project(PotreeConverter)

set(RAPIDJSON_INCLUDE_DIR lib/rapidjson/include)

file(GLOB_RECURSE files src/*.cpp include/*.h include/*.hpp )
file(GLOB_RECURSE lib_rapidjson_files ${RAPIDJSON_INCLUDE_DIR}/* )

include_directories(include ${LASZIP_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
include_directories(include ${RAPIDJSON_INCLUDE_DIR})

SOURCE_GROUP("Header Files\\io" FILES 
	include/BINPointReader.hpp 
	include/BINPointWriter.hpp
	include/LASPointReader.h
	include/LASPointWriter.hpp
	include/PlyPointReader.h
	include/PointReader.h
	include/PointWriter.hpp
	include/PTXPointReader.h
	include/XYZPointReader.hpp
)

SOURCE_GROUP("Source Files\\io" FILES 
	src/BINPointReader.cpp
	src/LASPointReader.cpp
	src/LASPointWriter.cpp
	src/PTXPointReader.cpp
)

SOURCE_GROUP("lib\\rapidjson" FILES ${lib_rapidjson_files})

add_executable(PotreeConverter ${files} ${lib_rapidjson_files})
add_dependencies(PotreeConverter LAStools)

target_link_libraries(PotreeConverter ${LASZIP_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})

install(TARGETS PotreeConverter RUNTIME DESTINATION bin/ )
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION bin/)

if(UNIX)
    set(CPACK_GENERATOR DEB)

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    else()
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
    endif()

    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
    set(CPACK_DEBIAN_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Tool for converting pointclouds to Potrees.")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Will Lucas")
    set(CPACK_DEBIAN_PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

    include(CPack)
endif()