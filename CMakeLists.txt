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

# Set the locations of source and header files 
set ( CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
set ( HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RestFrames )

# Specify the install locations for libraries and binaries.
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_INSTALL_LIBDIR ${CMAKE_BINARY_DIR}/lib )  # Needed by ROOT_GENERATE_DICTIONARY()

# Set the warning flag(s) to use.
set( CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -pedantic -O3 -g" )

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Add ROOT system directory and require ROOT.
find_package( ROOT 6.28.00 REQUIRED COMPONENTS Core Hist RIO Tree ROOTDataFrame ROOTVecOps)
include_directories( ${CMAKE_SOURCE_DIR} )
message(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")
# ------------------------------------------------
# The actual RestFrames library
# ------------------------------------------------

file(GLOB_RECURSE SOURCES "${CPP_DIR}/*.cc" )
file(GLOB_RECURSE LIB_HEADERS "${HEADER_DIR}/*.hh" )
message(STATUS "RestFrames sources: ${SOURCES}")
message(STATUS "RestFrames headers: ${LIB_HEADERS}")
# Build the shared library.
add_library( RestFrames SHARED ${LIB_HEADERS} ${SOURCES} )
set_target_properties(RestFrames PROPERTIES SUFFIX ".so")
target_include_directories( RestFrames PUBLIC $<BUILD_INTERFACE:${CPP_DIR}> )
target_include_directories( RestFrames SYSTEM PUBLIC )
target_include_directories( RestFrames PUBLIC ${HEADER_DIR} )
target_link_libraries( RestFrames PUBLIC ROOT::Core ROOT::Hist ROOT::RIO ROOT::Tree ROOT::ROOTDataFrame ROOT::ROOTVecOps )
set_property( TARGET RestFrames
   PROPERTY PUBLIC_HEADER ${LIB_HEADERS} )

ROOT_GENERATE_DICTIONARY(RestFrames_dict RestFrames/RestFrames.hh MODULE RestFrames LINKDEF RestFrames/RestFrames_LinkDef.h)
# needed as ROOT_GENERATE_DICTIONARY does not support system includes
target_compile_options(RestFrames_dict PUBLIC -Wno-shadow)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib/lib${PROJECT_NAME}_rdict.pcm ${CMAKE_CURRENT_BINARY_DIR}/lib/lib${PROJECT_NAME}.rootmap DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

# Add a setup script to the build directory.
set(SETUP ${CMAKE_CURRENT_BINARY_DIR}/setup.sh)
file(WRITE ${SETUP} "#!/bin/bash\n")

file(APPEND ${SETUP} "drop_from_path()\n{\n  # Assert that we got enough arguments\n  if test $# -ne 2 ; then\n    echo \"drop_from_path: needs 2 arguments\"\n    return 1\n  fi\n\n")
file(APPEND ${SETUP} "  p=$1\n  drop=$2\n\n")
file(APPEND ${SETUP} "  newpath=`echo $p | sed -e \"s;:\${drop}:;:;g\" \\
                          -e \"s;:\${drop};;g\"   \\
                          -e \"s;\${drop}:;;g\"   \\
                          -e \"s;\${drop};;g\"`\n}\n\n")
file(APPEND ${SETUP} "if [ -n \"\${RESTFRAMESSYS}\" ] ; then\n  echo \"Getting rid of the old path\"\n  old_restframessys=\${RESTFRAMESSYS}\nfi\n")
file(APPEND ${SETUP} "RESTFRAMESSYS=${CMAKE_SOURCE_DIR}; export RESTFRAMESSYS\n\n")

file(APPEND ${SETUP} "if [ -n \"\${old_restframessys}\" ] ; then\n")
file(APPEND ${SETUP} "  echo \"RestFrames environment variable already set to \${old_restframessys}\"\n")
file(APPEND ${SETUP} "  if [ -n \"\${PATH}\" ]; then\n")
file(APPEND ${SETUP} "    drop_from_path \"\${PATH}\" \${old_restframessys}/build/lib\n")
file(APPEND ${SETUP} "    PATH=$newpath\n")
file(APPEND ${SETUP} "  fi\n")
file(APPEND ${SETUP} "  if [ -n \"\${LD_LIBRARY_PATH}\" ]; then\n")
file(APPEND ${SETUP} "     drop_from_path \$LD_LIBRARY_PATH \${old_restframessys}/build/lib\n")
file(APPEND ${SETUP} "     LD_LIBRARY_PATH=$newpath\n")
file(APPEND ${SETUP} "  fi\n")
file(APPEND ${SETUP} "  if [ -n \"\${DYLD_LIBRARY_PATH}\" ]; then\n")
file(APPEND ${SETUP} "     drop_from_path \$DYLD_LIBRARY_PATH \${old_restframessys}/build/lib\n")
file(APPEND ${SETUP} "     DYLD_LIBRARY_PATH=$newpath\n")
file(APPEND ${SETUP} "  fi\n")
file(APPEND ${SETUP} "  if [ -n \"\${SHLIB_PATH}\" ]; then\n")
file(APPEND ${SETUP} "     drop_from_path \$SHLIB_PATH \${old_restframessys}/build/lib\n")
file(APPEND ${SETUP} "     SHLIB_PATH=$newpath\n")
file(APPEND ${SETUP} "  fi\n")
file(APPEND ${SETUP} "  if [ -n \"${LIBPATH}\" ]; then\n")
file(APPEND ${SETUP} "     drop_from_path \$LIBPATH \${old_restframessys}/build/lib\n")
file(APPEND ${SETUP} "     LIBPATH=$newpath\n")
file(APPEND ${SETUP} "  fi\n")
file(APPEND ${SETUP} "  if [ -n \"\${PYTHONPATH}\" ]; then\n")
file(APPEND ${SETUP} "     drop_from_path \$PYTHONPATH \${old_restframessys}/build/lib\n")
file(APPEND ${SETUP} "     PYTHONPATH=$newpath\n")
file(APPEND ${SETUP} "  fi\n")
file(APPEND ${SETUP} "  if [ -n \"\${CPLUS_INCLUDE_PATH}\" ]; then\n")
file(APPEND ${SETUP} "     drop_from_path \$CPLUS_INCLUDE_PATH \${old_restframessys}/RestFrames\n")
file(APPEND ${SETUP} "     CPLUS_INCLUDE_PATH=$newpath\n")
file(APPEND ${SETUP} "  fi\n")
file(APPEND ${SETUP} "  if [ -n \"\${CPATH}\" ]; then\n")
file(APPEND ${SETUP} "     drop_from_path \$CPATH \${old_restframessys}/RestFrames\n")
file(APPEND ${SETUP} "     CPATH=$newpath\n")
file(APPEND ${SETUP} "  fi\n")
file(APPEND ${SETUP} "fi\n")
file(APPEND ${SETUP} "\n")
file(APPEND ${SETUP} "if [ -z \"\${PATH}\" ]; then\n")
file(APPEND ${SETUP} "   PATH=${CMAKE_SOURCE_DIR}/build/lib; export PATH\n")
file(APPEND ${SETUP} "else\n")
file(APPEND ${SETUP} "   PATH=${CMAKE_SOURCE_DIR}/build/lib:$PATH; export PATH\n")
file(APPEND ${SETUP} "fi\n")
file(APPEND ${SETUP} "\n")
file(APPEND ${SETUP} "if [ -z \"\${LD_LIBRARY_PATH}\" ]; then\n")
file(APPEND ${SETUP} "   LD_LIBRARY_PATH=${CMAKE_SOURCE_DIR}/build/lib; export LD_LIBRARY_PATH       # Linux, ELF HP-UX\n")
file(APPEND ${SETUP} "else\n")
file(APPEND ${SETUP} "   LD_LIBRARY_PATH=${CMAKE_SOURCE_DIR}/build/lib:$LD_LIBRARY_PATH; export LD_LIBRARY_PATH\n")
file(APPEND ${SETUP} "fi\n")
file(APPEND ${SETUP} "\n")
file(APPEND ${SETUP} "if [ -z \"\${DYLD_LIBRARY_PATH}\" ]; then\n")
file(APPEND ${SETUP} "   DYLD_LIBRARY_PATH=${CMAKE_SOURCE_DIR}/build/lib; export DYLD_LIBRARY_PATH   # Mac OS X\n")
file(APPEND ${SETUP} "else\n")
file(APPEND ${SETUP} "   DYLD_LIBRARY_PATH=${CMAKE_SOURCE_DIR}/build/lib:$DYLD_LIBRARY_PATH; export DYLD_LIBRARY_PATH\n")
file(APPEND ${SETUP} "fi\n")
file(APPEND ${SETUP} "\n")
file(APPEND ${SETUP} "if [ -z \"\${SHLIB_PATH}\" ]; then\n")
file(APPEND ${SETUP} "   SHLIB_PATH=${CMAKE_SOURCE_DIR}/build/lib; export SHLIB_PATH                 # legacy HP-UX\n")
file(APPEND ${SETUP} "else\n")
file(APPEND ${SETUP} "   SHLIB_PATH=${CMAKE_SOURCE_DIR}/build/lib:$SHLIB_PATH; export SHLIB_PATH\n")
file(APPEND ${SETUP} "fi\n")
file(APPEND ${SETUP} "\n")
file(APPEND ${SETUP} "if [ -z \"\${LIBPATH}\" ]; then\n")
file(APPEND ${SETUP} "   LIBPATH=${CMAKE_SOURCE_DIR}/build/lib; export LIBPATH                       # AIX\n")
file(APPEND ${SETUP} "else\n")
file(APPEND ${SETUP} "   LIBPATH=${CMAKE_SOURCE_DIR}/build/lib:$LIBPATH; export LIBPATH\n")
file(APPEND ${SETUP} "fi\n")
file(APPEND ${SETUP} "\n")
file(APPEND ${SETUP} "if [ -z \"\${PYTHONPATH}\" ]; then\n")
file(APPEND ${SETUP} "   PYTHONPATH=${CMAKE_SOURCE_DIR}/build/lib; export PYTHONPATH\n")
file(APPEND ${SETUP} "else\n")
file(APPEND ${SETUP} "   PYTHONPATH=${CMAKE_SOURCE_DIR}/build/lib:$PYTHONPATH; export PYTHONPATH\n")
file(APPEND ${SETUP} "fi\n")
file(APPEND ${SETUP} "\n")
file(APPEND ${SETUP} "if [ -z \"\${CPLUS_INCLUDE_PATH}\" ]; then\n")
file(APPEND ${SETUP} "   CPLUS_INCLUDE_PATH=${CMAKE_SOURCE_DIR}/RestFrames; export CPLUS_INCLUDE_PATH\n")
file(APPEND ${SETUP} "else\n")
file(APPEND ${SETUP} "   CPLUS_INCLUDE_PATH=${CMAKE_SOURCE_DIR}/RestFrames:$CPLUS_INCLUDE_PATH; export CPLUS_INCLUDE_PATH\n")
file(APPEND ${SETUP} "fi\n")
file(APPEND ${SETUP} "\n")
file(APPEND ${SETUP} "if [ -z \"\${CPATH}\" ]; then\n")
file(APPEND ${SETUP} "   CPATH=${CMAKE_SOURCE_DIR}/RestFrames; export CPATH\n")
file(APPEND ${SETUP} "else\n")
file(APPEND ${SETUP} "   CPATH=${CMAKE_SOURCE_DIR}/RestFrames:$CPATH; export CPATH\n")
file(APPEND ${SETUP} "fi\n")
file(APPEND ${SETUP} "\n")
file(APPEND ${SETUP} "unset old_restframessys")
