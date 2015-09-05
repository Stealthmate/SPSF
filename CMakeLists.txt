#Sprite Sheet Font

project(SPSF)
cmake_minimum_required(VERSION 2.8.7)

set(BINDIR ../bin)
set(LIBDIR ../bin/lib)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINDIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBDIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBDIR} )

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BINDIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBDIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBDIR} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

#Version
set(SPSF_VERSION_MAJOR 1)
set(SPSF_VERSION_MINOR 0)

#Set include path
include_directories("include/")
link_directories("lib/" "bin/lib")

set(REQUIRED_COMPILER_VERSION_GCC "4.7")
set(REQUIRED_COMPILER_VERSION_MSVC "19.0")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.7.0")
		set(CMAKE_CXX_FLAGS "-std=c++0x")
	else()
		SET( CMAKE_CXX_FLAGS  "-Wall -std=c++14" )
	endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${REQUIRED_COMPILER_VERSION_MSVC})
		message( SEND_ERROR "Compiler version is lower than required (${REQUIRED_COMPILER_VERSION_MSVC})! Build may fail!")
	endif()
	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /wd4251" )
endif()

#System dependencies
#if (WIN32)
#set(DEVA_FRAMEWORK_SYSTEM_DEP_OGL opengl32 glu32)
#else()
#set(DEVA_FRAMEWORK_SYSTEM_DEP_OGL GLU glbinding GL X11 Xxf86vm Xrandr pthread Xi Xinerama Xcursor)
#endif()

#Provided dependencies
#set(DEVA_FRAMEWORK_PROV_DEP_GLBINDING glbinding)
#set(DEVA_FRAMEWORK_PROV_DEP_GLFW glfw3)
#set(DEVA_FRAMEWORK_PROV_DEP_LUA lua)
#set(DEVA_FRAMEWORK_PROV_DEP_PNG libpng16)

#All dependencies
#set(DEVA_FRAMEWORK_DEP ${DEVA_SYSTEM_DEP_OGL} ${DEVA_PROV_DEP_PNG} ${DEVA_PROV_DEP_LUA})

#set(SHADERS_OUTPUT_DIR bin/shaders)

#Framework Sources
set(CURDIR src)
include(${CURDIR}/CMakeLists.txt)
#Tests
set(CURDIR tests)
include(${CURDIR}/CMakeLists.txt)
#Docs
set(DOXYFILE_NAME "SPSF.doxy")
add_custom_target(Docs COMMAND cd .. && doxygen SPSF.doxy)