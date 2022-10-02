cmake_minimum_required(VERSION 3.8)

#Global cmake features
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#set project name
project(conFPS)

# Set compiler options
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# MP build for MSVC
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc")
endif()

# Set project constants
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}) # Project dir
set(BINARIES_DIR ${PROJECT_ROOT}/bin) # Binaries dir

# Project declaration
file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.hpp")
add_executable(conFPS ${SOURCES})
target_include_directories(conFPS PUBLIC "include/")

# Cpp17
target_compile_features(conFPS PUBLIC cxx_std_17)

# Set output directories
set_target_properties(${SFML_TARGETS} PROPERTIES 
	ARCHIVE_OUTPUT_DIRECTORY "${BINARIES_DIR}/Lib"
	RUNTIME_OUTPUT_DIRECTORY "${BINARIES_DIR}/Bin"
	LIBRARY_OUTPUT_DIRECTORY "${BINARIES_DIR}/Bin"
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CACHEFILE_DIR}")