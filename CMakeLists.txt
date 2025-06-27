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

# Set the warning flag(s) to use.
set( CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -pedantic -O3 -g" )

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Add ROOT system directory and require ROOT.
find_package( ROOT 6.28.00 REQUIRED COMPONENTS Core Hist RIO Tree ROOTDataFrame ROOTVecOps)

include_directories(${CMAKE_SOURCE_DIR})
# ------------------------------------------------
# The actual RestFrames library
# ------------------------------------------------

file(GLOB SOURCES "src/*.cc" )
file(GLOB LIB_HEADERS "RestFrames/*.hh" )

# Build the shared library.
add_library( RestFrames SHARED ${LIB_HEADERS} ${SOURCES} )
set_target_properties(RestFrames PROPERTIES SUFFIX ".so")
target_include_directories( RestFrames PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}> )
target_include_directories(RestFrames SYSTEM PUBLIC )
target_link_libraries( RestFrames PUBLIC ROOT::Core ROOT::Hist ROOT::RIO ROOT::Tree ROOT::ROOTDataFrame ROOT::ROOTVecOps )
set_property( TARGET RestFrames
   PROPERTY PUBLIC_HEADER ${LIB_HEADERS} )

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

ROOT_GENERATE_DICTIONARY(RestFrames_dict RestFrames/RestFrame.hh MODULE RestFrames LINKDEF RestFrames/RestFrames_LinkDef.h)
# needed as ROOT_GENERATE_DICTIONARY does not support system includes
target_compile_options(RestFrames_dict PUBLIC -Wno-shadow)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib/lib${PROJECT_NAME}_rdict.pcm ${CMAKE_CURRENT_BINARY_DIR}/lib/lib${PROJECT_NAME}.rootmap DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
