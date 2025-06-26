# Set the minimum CMake version required to build the project.
cmake_minimum_required( VERSION 3.10 )

# Silence some warnings on macOS with new CMake versions.
if( NOT ${CMAKE_VERSION} VERSION_LESS 3.9 )
   cmake_policy( SET CMP0068 NEW )
endif()

# Set the project's name and version.
project( RestFrames )

# Set up the "C++ version" to use.
set( CMAKE_CXX_STANDARD_REQUIRED 17 CACHE STRING
   "Minimum C++ standard required for the build" )
set( CMAKE_CXX_STANDARD 17 CACHE STRING
   "C++ standard to use for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL
   "(Dis)allow using compiler extensions" )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify the install locations for libraries and binaries.
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_INSTALL_LIBDIR ${CMAKE_BINARY_DIR}/lib )  # Needed by ROOT_GENERATE_DICTIONARY()

set(RestFrames_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/RestFrames" CACHE STRING "Path to the includes")

# Set the warning flag(s) to use.
set( CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -pedantic -O3 -g" )

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

## Silence boost warnings
#add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)
#add_definitions(-DBOOST_ALLOW_DEPRECATED_HEADERS)
#
## ------------------------------------------------
## Dependencies and sub-libraries
## ------------------------------------------------
#find_package( Boost CONFIG COMPONENTS python REQUIRED )
## hack for homebrew installation and potentially some other platforms
#if( EXISTS "${Boost_INCLUDE_DIR}/root" )
#   set( ROOT_EXTRA_INCLUDE_DIR "${Boost_INCLUDE_DIR}/root" )
#   string( REPLACE "/include/" "/lib/" ROOT_EXTRA_LIB_DIR "${ROOT_EXTRA_INCLUDE_DIR}" )
#   list( APPEND CMAKE_INCLUDE_PATH "${ROOT_EXTRA_INCLUDE_DIR}" )
#   list( APPEND CMAKE_LIBRARY_PATH "${ROOT_EXTRA_LIB_DIR}" )
#endif()
# Add ROOT system directory and require ROOT.
find_package( ROOT 6.28.00 REQUIRED COMPONENTS Core Hist RIO Tree ROOTDataFrame ROOTVecOps)
#find_package( Python3 COMPONENTS Development REQUIRED )

include_directories(${CMAKE_SOURCE_DIR})
# ------------------------------------------------
# The actual RestFrames library
# ------------------------------------------------

file(GLOB SOURCES "src/*.cc" )
file(GLOB LIB_HEADERS "include/*.hh" )

# Build the shared library.
add_library( RestFrames SHARED ${LIB_HEADERS} ${SOURCES} )
set_target_properties(RestFrames PROPERTIES SUFFIX ".so")
target_include_directories( RestFrames PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}> )
target_include_directories(RestFrames SYSTEM PUBLIC )
target_link_libraries( RestFrames PUBLIC ROOT::Core ROOT::Hist ROOT::RIO ROOT::Tree ROOT::ROOTDataFrame ROOT::ROOTVecOps )
set_property( TARGET RestFrames
   PROPERTY PUBLIC_HEADER ${LIB_HEADERS} )

## Build the Python module for C++ logger
#add_library( cppLogger SHARED python_wrapper/utils/Logger.cxx )
#set_target_properties(cppLogger PROPERTIES SUFFIX ".so")
#target_link_libraries( cppLogger PRIVATE Python3::Python )
#set_target_properties( cppLogger PROPERTIES
#   PREFIX ""
#   OUTPUT_NAME "cppLogger" )
#
## Build the Python module for config reader
#add_library( ConfigReaderCpp SHARED python_wrapper/utils/ConfigReaderCpp.cxx ${LIB_HEADERS} )
#set_target_properties(ConfigReaderCpp PROPERTIES SUFFIX ".so")
#target_link_libraries( ConfigReaderCpp PRIVATE Python3::Python ${Boost_LIBRARIES} RestFrames )
#set_target_properties( ConfigReaderCpp PROPERTIES
#   PREFIX ""
#   OUTPUT_NAME "ConfigReaderCpp" )
#

set(SETUP ${CMAKE_CURRENT_BINARY_DIR}/setup.sh)
file(WRITE ${SETUP} "#!/bin/bash\n")
file(APPEND ${SETUP} "# this is an auto-generated setup script\n" )
file(APPEND ${SETUP} "export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/lib\n")
file(APPEND ${SETUP} "export DYLD_LIBRARY_PATH=\${DYLD_LIBRARY_PATH}:${CMAKE_BINARY_DIR}/lib\n")
file(APPEND ${SETUP} "export SHLIB_PATH=\${SHLIB_PATH}:${CMAKE_BINARY_DIR}/lib\n")
file(APPEND ${SETUP} "export LIBPATH=\${LIBPATH}:${CMAKE_BINARY_DIR}/lib\n")
file(APPEND ${SETUP} "export PATH=\${PATH}:${CMAKE_BINARY_DIR}/lib\n")
file(APPEND ${SETUP} "export CPATH=\${CPATH}:${CMAKE_SOURCE_DIR}/RestFrames\n")
file(APPEND ${SETUP} "export RESTFRAMESSYS=\${RESTFRAMESSYS}:${CMAKE_SOURCE_DIR}\n")

# Install the libraries.
install( TARGETS RestFrames 
   EXPORT RestFramesTargets
   ARCHIVE DESTINATION lib
   LIBRARY DESTINATION lib
   PUBLIC_HEADER DESTINATION RestFrames/ )

# Export the targets.
install( EXPORT RestFramesTargets
   FILE RestFramesTargets.cmake
   NAMESPACE RestFrames::
   DESTINATION lib/cmake/RestFrames )

## Create a Config file for the package.
#include(CMakePackageConfigHelpers)
#configure_package_config_file(
#   "${CMAKE_CURRENT_LIST_DIR}/RestFramesConfig.cmake.in"
#   "${CMAKE_CURRENT_BINARY_DIR}/RestFramesConfig.cmake"
#   INSTALL_DESTINATION lib/cmake/RestFrames
#   PATH_VARS CMAKE_INSTALL_PREFIX
#)
#
## Install the Config file.
#install( FILES "${CMAKE_CURRENT_BINARY_DIR}/RestFramesConfig.cmake"
#   DESTINATION lib/cmake/RestFrames )
#
# ------------------------------------------------
# RestFrames executables
# ------------------------------------------------

# Helper macro for building the project's executables.
#macro( RestFrames_add_executable name )
#  add_executable( ${name} ${ARGN} )
#  target_link_libraries( ${name} RestFrames)
#  install( TARGETS ${name}
#    EXPORT RestFramesTargets
#    RUNTIME DESTINATION bin )
#endmacro( RestFrames_add_executable )

#RestFrames_add_executable( fast-frames.exe util/fast-frames.cc )

ROOT_GENERATE_DICTIONARY(RestFrames_dict RestFrames/RestFrame.hh MODULE RestFrames LINKDEF src/LinkDef.h)
# needed as ROOT_GENERATE_DICTIONARY does not support system includes
target_compile_options(RestFrames_dict PUBLIC -Wno-shadow)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib/lib${PROJECT_NAME}_rdict.pcm ${CMAKE_CURRENT_BINARY_DIR}/lib/lib${PROJECT_NAME}.rootmap DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
