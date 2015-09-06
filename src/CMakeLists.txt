option(BUILD_SHARED_LIBS "Build the shared library" ON)
set(LIBTYPE STATIC)
if(BUILD_SHARED_LIBS)
add_definitions(-DSPSF_BUILD_SHARED)
set(LIBTYPE SHARED)
endif()

set(PRIVATE_HEADERS "")

FILE(GLOB SRCS ${CURDIR}/*.cpp ${CURDIR}/*.hpp)

add_library(SPSF ${LIBTYPE} ${SRCS})
set_target_properties(SPSF PROPERTIES COMPILE_DEFINITIONS SPSF_BUILD)

set(CURDIR ${TEMP_DIR})
file(GLOB_RECURSE PUBLIC_HEADERS ${CURDIR}/*.hpp)
foreach(Header ${PUBLIC_HEADERS})
	foreach(privHeader ${PRIVATE_HEADERS})
		if(${Header} STREQUAL ${privHeader})
			list(REMOVE_ITEM PUBLIC_HEADERS privHeader)
		endif()
	endforeach(privHeader)
endforeach(Header)

set(PUBLIC_HEADER_OUTPUT_DIR ../public_headers/DevaFramework)

add_custom_target(PublicHeaders SOURCES ${PUBLIC_HEADERS})
foreach(Header ${PUBLIC_HEADERS})
	add_custom_command(TARGET PublicHeaders COMMAND ${CMAKE_COMMAND} -E make_directory ${PUBLIC_HEADER_OUTPUT_DIR} COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${Header} ${PUBLIC_HEADER_OUTPUT_DIR})
endforeach()